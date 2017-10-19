#include <ncurses.h>
#include <assert.h>

int main(void)
{
    int ch;
    MEVENT event;

    initscr();
    timeout(0);
    raw();
    curs_set(0);
    keypad(stdscr, true);
    noecho();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(100);      // In ms
    printf("\033[?1003h\n"); // Makes the terminal report mouse movement events

    if (has_colors())
    {
        start_color();
        init_pair(1, COLOR_RED, COLOR_WHITE);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        attron(COLOR_PAIR(1));
    }

    mvprintw(0, 0, "Move mouse inside battlefield to change Lat/Long");
    mvprintw(2, 0, "Mouse Left Click inside battlefied to fire a gunshot");
    mvprintw(4, 0, "Press q to exit\n");

    int rows, cols, startx, starty;
    getmaxyx(stdscr, rows, cols);
    getyx(stdscr, starty, startx);

    // Figure out scaled co-ordinates and draw box for battlefield
    int spacing_y = (10 * (rows - starty)) / 100;
    int spacing_x = (10 * (cols - startx)) / 100;
    int box_y1 = starty + spacing_y;
    int box_x1 = startx + spacing_x;
    int box_y2 = rows - spacing_y;
    int box_x2 = box_x1;
    int box_y3 = box_y1;
    int box_x3 = cols - spacing_x;

    if (has_colors())
    {
        attron(COLOR_PAIR(2));
    }
    mvhline(box_y1, box_x1, '*', cols - (2 * spacing_x));
    mvhline(box_y2, box_x2, '*', cols - (2 * spacing_x));
    mvvline(box_y3, box_x3, '*', (rows - starty) - (2 * spacing_y));
    mvvline(box_y1, box_x1, '*', (rows - starty) - (2 * spacing_y));

    if (has_colors())
    {
        attroff(COLOR_PAIR(2));
    }

    int data_y = box_y1 - 1;
    int data_x = box_x1 + 1;
    mvprintw(data_y, data_x, "Lat: %03d S\tLong: %03d E\tTime: %d", 0, 0, 0);

    while ((ch = getch()) != 'q')
    {
        if (ch == KEY_MOUSE)
        {
            if(getmouse(&event) == OK)
            {
                if ((event.y > box_y1 && event.y < box_y2) &&
                    (event.x > box_x1 && event.x < box_x3))
                {
                    if (event.bstate & REPORT_MOUSE_POSITION)
                    {
                        mvprintw(data_y, data_x, "Lat: %03d S\tLong: %03d E\tTime: %d", event.y, event.x, 0);
                    }
                    else if (event.bstate & BUTTON1_PRESSED)
                    {
                        mvprintw(event.y, event.x, "BOOM!");
                    }
                }
            }
        }
    }

/*
    //while (c != 'q' && c != 0x03)
    while ((c = getch()) != 'q')
    {
        //c = getch();
        switch(c)
        {
            case KEY_MOUSE:
                if (getmouse(&event) == OK)
                {
//                    if (event.bstate & BUTTON1_PRESSED)
                    {
//                        mvprintw(box_y1 - 1, box_x1 + 2, "A");
                    }
//                    else if (event.bstate & REPORT_MOUSE_POSITION)
                    {
                        mvprintw(0,0, "B");
                    }
                }
                else
                {
                    mvprintw(0,0, "C");
                }
                break;
            default:
                break;
        }
    }
*/
    printf("\033[?1003l\n"); // Disable mouse movement events
    endwin();

    return 0;
}

