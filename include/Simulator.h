/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "AudioDetector.h"
#include "CommClient.h"

class Simulator
{
    public:
        Simulator(AudioDetector& ad, CommClient& comms);
        ~Simulator();
        void Run();
        void InitGraphics();

    private:
        enum ColorPair {RED_ON_WHITE = 1, YELLOW_ON_BLACK = 2,
                        RED_ON_BLACK = 3, GREEN_ON_BLACK = 4};
        AudioDetector& audio_detector;
        CommClient& communications;
        int rows, cols;
        int start_y, start_x;
        int spacing_y, spacing_x;
        int latitude_y, latitude_x, longitude_y, longitude_x;
        int box_y1, box_x1, box_y2, box_x2, box_y3, box_x3;
        int timestamp_y, timestamp_x;
        int server_y, server_x;
        void drawScreen();
        void handleMouseEvent();
        bool pointInBox(int x, int y);
        void updatePositionData(int mouse_y, int mouse_x);
        void updateEvents_1Hz();
        void checkForScreenResize();
        double timeSinceMs(struct timespec start);
        void turnOnColor(int color_pair_index);
        void turnOffColor(int color_pair_index);
};

#endif

