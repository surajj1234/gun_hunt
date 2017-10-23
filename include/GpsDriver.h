/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#ifndef GPS_DRIVER_H
#define GPS_DRIVER_H

#include "Serial.h"
#include <time.h>
#include <string>
#include <vector>

typedef struct GpsData
{
    int    latitude;
    char   lat_direction;
    int    longitude;
    char   long_direction;
    double time;            // Time since UNIX epoch, in seconds (ns precision)
} GpsData;

class GpsDriver
{
    public:
        GpsDriver(Serial& serial);
        ~GpsDriver();
        void Run();
        void Quit();
        GpsData GetData();
        void PulseInterruptHandler(struct timespec time);
    private:
        Serial& serial;
        GpsData data;
        bool terminate;
        enum RxState {RX_START_OF_FRAME, RX_DATA};
        RxState rx_state;
        uint8_t rx_length;
        std::string rx_buffer;
        struct timespec last_data_update;
        struct timespec frame_rx_start;

        void updateStateMachine(uint8_t byte);
        void processPacket(std::string& packet);
        bool validPacket(std::vector<std::string>& items);
        std::vector<std::string> splitPacket(std::string& packet);
        bool rxTimeout();
};

#endif
