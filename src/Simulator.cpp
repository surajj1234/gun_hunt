/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#include "Simulator.h"
#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

Simulator::Simulator(AudioDetector& ad, CommClient& comms, FakeSerial& ser) :
    audio_detector(ad), communications(comms), serial(ser)
{
}

void Simulator::InitGraphics()
{
    // Refer to ncurses docs for initialization details
    initscr();
    timeout(0);
    raw();
    curs_set(0);
    keypad(stdscr, true);
    noecho();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(75);      // In ms

    if (has_colors())
    {
        start_color();
        init_pair(RED_ON_WHITE, COLOR_RED, COLOR_WHITE);
        init_pair(YELLOW_ON_BLACK, COLOR_YELLOW, COLOR_BLACK);
        init_pair(RED_ON_BLACK, COLOR_RED, COLOR_BLACK);
        init_pair(GREEN_ON_BLACK, COLOR_GREEN, COLOR_BLACK);
    }

    // Ugly hack to get terminal emulators to report mouse movement
    // Bypassing the terminfo database via the 1003 escape sequence
    printf("\033[?1003h\n");

    drawScreen();
}

void Simulator::drawScreen()
{
    clear();

    turnOnColor(RED_ON_WHITE);
    mvprintw(0, 0, "Move mouse inside battlefield to change Lat/Long");
    mvprintw(2, 0, "Mouse Left Click inside battlefield to fire a gunshot");
    mvprintw(4, 0, "Press q to exit\n");
    turnOffColor(RED_ON_WHITE);

    // Figure out scaled co-ordinates and draw box for battlefield, with
    // dynamic spacing from the window borders
    getmaxyx(stdscr, rows, cols);
    getyx(stdscr, start_y, start_x);

    spacing_y = (10 * (rows - start_y)) / 100;
    spacing_x = (10 * (cols - start_x)) / 100;

    // (y1, x1)..............(y3, x3)
    // .                        .
    // .                        .
    // .                        .        
    // (y2, x2).................
    box_y1 = start_y + spacing_y;
    box_x1 = start_x + spacing_x;
    box_y2 = rows - spacing_y;
    box_x2 = box_x1;
    box_y3 = box_y1;
    box_x3 = cols - spacing_x;

    turnOnColor(YELLOW_ON_BLACK);
    mvhline(box_y1, box_x1, '*', cols - (2 * spacing_x));
    mvhline(box_y2, box_x2, '*', cols - (2 * spacing_x));
    mvvline(box_y3, box_x3, '*', (rows - start_y) - (2 * spacing_y));
    mvvline(box_y1, box_x1, '*', (rows - start_y) - (2 * spacing_y));
    turnOffColor(YELLOW_ON_BLACK);

    // Set window positions for status fields
    latitude_y = box_y1 - 1;
    latitude_x = box_x1;

    longitude_y = latitude_y;
    longitude_x = box_x1 + 15;

    timestamp_y = latitude_y;
    timestamp_x = box_x1 + 30;

    server_y = latitude_y;
    server_x = box_x1 + 50;

    updatePositionData(box_y1, box_x1);

    refresh();
}

Simulator::~Simulator()
{
    printf("\033[?1003l\n"); // Disable mouse movement events
    endwin();
}

void Simulator::Run()
{
    int ch;

    while ((ch = getch()) != 'q')
    {
        if (ch == KEY_MOUSE)
        {
            handleMouseEvent();
        }
        updateEvents_1Hz();
        checkForScreenResize();
    }
}

void Simulator::handleMouseEvent()
{
    MEVENT event;
    if(getmouse(&event) == OK)
    {
        int mouse_y = (uint8_t)event.y;
        int mouse_x = (uint8_t)event.x;
        if (pointInBox(mouse_y, mouse_x))
        {
            if (event.bstate & REPORT_MOUSE_POSITION)
            {
                updatePositionData(mouse_y, mouse_x);
            }
            else if (event.bstate & BUTTON1_PRESSED)
            {
                mvprintw(mouse_y, mouse_x, "BOOM!");
                audio_detector.setDetected(true);
            }
        }
    }
}

bool Simulator::pointInBox(int y, int x)
{
    return  ((y > box_y1) && (y < box_y2)) &&
            ((x > box_x1) && (x < box_x3));
}

void Simulator::updatePositionData(int mouse_y, int mouse_x)
{
    gps_latitude = mouse_y - box_y1;
    gps_longitude = mouse_x - box_x1;
    mvprintw(latitude_y, latitude_x, "Lat: %03dS", gps_latitude);
    mvprintw(longitude_y, longitude_x, "Long: %03dE", gps_longitude);
}

void Simulator::updateEvents_1Hz()
{
    static struct timespec prev = {0,0};;
    if (timeSinceMs(prev) > 1000)           // 1 Hz
    {
        // Update timestamp
        clock_gettime(CLOCK_REALTIME, &prev);
        mvprintw(timestamp_y, timestamp_x, "Time: %ld", prev.tv_sec);

        // Update server status
        mvprintw(server_y, server_x, "Server: ");
        bool connected = communications.IsConnected();
        if (connected)
        {
            turnOnColor(GREEN_ON_BLACK);
            printw("CONNECTED   ");
            turnOffColor(GREEN_ON_BLACK);
        }
        else
        {
            turnOnColor(RED_ON_BLACK);
            printw("DISCONNECTED");
            turnOffColor(RED_ON_BLACK);
        }

        // TODO: Call gps pulse interrupt handler
        // TODO: Send gps info to serial port
        double gps_time = prev.tv_sec + (double)prev.tv_nsec / 1.0e9;
        // TODO: Refactor packet construction
        std::string gps_output = std::string("$GPXXX")          +
                                 std::string(",")               +
                                 std::to_string(gps_latitude)   +
                                 std::string(",")               +
                                 std::string("S")               +
                                 std::string(",")               +
                                 std::to_string(gps_longitude)  +
                                 std::string(",")               +
                                 std::string("E")               +
                                 std::string(",")               +
                                 std::to_string(gps_time)       +
                                 "\n";
        serial.AddData(gps_output);
    }
}

double Simulator::timeSinceMs(struct timespec start)
{
    static struct timespec end;
    clock_gettime(CLOCK_REALTIME, &end);
    double diff = (double)(end.tv_sec - start.tv_sec) * 1.0e9 +
              (double)(end.tv_nsec - start.tv_nsec);
    return diff / 1.0e6;
}

void Simulator::checkForScreenResize()
{
    int current_rows, current_cols;
    getmaxyx(stdscr, current_rows, current_cols);
    if ((current_rows != rows) || (current_cols != cols))
    {
        drawScreen();
    }
}

void Simulator::turnOnColor(int color_pair_index)
{
    if (has_colors())
    {
        attron(COLOR_PAIR(color_pair_index));
    }
}

void Simulator::turnOffColor(int color_pair_index)
{
    if (has_colors())
    {
        attroff(COLOR_PAIR(color_pair_index));
    }
}
