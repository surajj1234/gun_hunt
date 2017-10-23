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
#include <mutex>

typedef struct GpsData
{
    int           latitude;
    std::string   lat_direction;
    int           longitude;
    std::string   long_direction;
    double time;  // Time since UNIX epoch, in seconds (with ns precision)
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
        std::mutex my_lock;
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
        bool isAValidNumber(std::string number);
        bool isAValidDirection(std::string direction);
};

#endif
