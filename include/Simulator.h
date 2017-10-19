/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#ifndef SIMULATOR_H
#define SIMULATOR_H

class Simulator
{
    public:
        Simulator();
        ~Simulator();
        void Run(void * param);
        void InitGraphics(void);

    private:
        int rows, cols;
        int start_y, start_x;
        int spacing_y, spacing_x;
        int status_y, status_x;
        int box_y1, box_x1, box_y2, box_x2, box_y3, box_x3;
        void drawScreen(void);
        void handleMouseEvent(void);
        bool pointInBox(int x, int y);
        void updatePositionData(int mouse_y, int mouse_x);
        void checkForScreenResize(void);
};

#endif

