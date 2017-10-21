/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#include "Simulator.h"
#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

Simulator::Simulator()
{
}

void Simulator::InitGraphics(void)
{
    // Refer to ncurses docs for initialization details
    initscr();
    timeout(0);
    raw();
    curs_set(0);
    keypad(stdscr, true);
    noecho();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(100);      // In ms

    if (has_colors())
    {
        start_color();
        init_pair(1, COLOR_RED, COLOR_WHITE);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    }

    // Ugly hack to get terminal emulators to report mouse movement
    // Bypassing the terminfo database via the 1003 escape sequence
    printf("\033[?1003h\n");

    drawScreen();
}

void Simulator::drawScreen(void)
{
    clear();

    if (has_colors())
    {
        attron(COLOR_PAIR(1));
    }

    mvprintw(0, 0, "Move mouse inside battlefield to change Lat/Long");
    mvprintw(2, 0, "Mouse Left Click inside battlefield to fire a gunshot");
    mvprintw(4, 0, "Press q to exit\n");

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

    if (has_colors())
    {
        attron(COLOR_PAIR(2));
    }
    mvhline(box_y1, box_x1, '*', cols - (2 * spacing_x));
    mvhline(box_y2, box_x2, '*', cols - (2 * spacing_x));
    mvvline(box_y3, box_x3, '*', (rows - start_y) - (2 * spacing_y));
    mvvline(box_y1, box_x1, '*', (rows - start_y) - (2 * spacing_y));

    if (has_colors())
    {
        attroff(COLOR_PAIR(2));
    }

    // Set window positions for status fields
    latitude_y = box_y1 - 1;
    latitude_x = box_x1;

    longitude_y = latitude_y;
    longitude_x = box_x1 + 15;

    timestamp_y = latitude_y;
    timestamp_x = box_x1 + 30;

    updatePositionData(box_y1, box_x1);

    refresh();
}

Simulator::~Simulator()
{
    printf("\033[?1003l\n"); // Disable mouse movement events
    endwin();
}

void Simulator::Run(void * param)
{
    int ch;

    while ((ch = getch()) != 'q')
    {
        if (ch == KEY_MOUSE)
        {
            handleMouseEvent();
        }
        updateTimestamp();
        checkForScreenResize();
    }
}

void Simulator::handleMouseEvent(void)
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
    mvprintw(latitude_y, latitude_x, "Lat: %03dS", mouse_y - box_y1);
    mvprintw(longitude_y, longitude_x, "Long: %03dE", mouse_x - box_x1);
}

void Simulator::updateTimestamp(void)
{
    static struct timespec prev = {0,0};;
    if (timeSinceMs(prev) > 1000)           // 1 Hz
    {
        clock_gettime(CLOCK_REALTIME, &prev);
        mvprintw(timestamp_y, timestamp_x, "Time: %ld", prev.tv_sec);
        // TODO: Send gps info to serial port
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

void Simulator::checkForScreenResize(void)
{
    int current_rows, current_cols;
    getmaxyx(stdscr, current_rows, current_cols);
    if ((current_rows != rows) || (current_cols != cols))
    {
        drawScreen();
    }
}
