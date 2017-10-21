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
        int latitude_y, latitude_x, longitude_y, longitude_x;
        int box_y1, box_x1, box_y2, box_x2, box_y3, box_x3;
        int timestamp_y, timestamp_x;
        void drawScreen(void);
        void handleMouseEvent(void);
        bool pointInBox(int x, int y);
        void updatePositionData(int mouse_y, int mouse_x);
        void updateTimestamp(void);
        void checkForScreenResize(void);
        double timeSinceMs(struct timespec start);
};

#endif

