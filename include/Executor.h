/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "AudioDetector.h"
#include "GpsDriver.h"
#include "CommClient.h"

class Executor
{
    public:
        Executor(AudioDetector& ad, GpsDriver& gps, CommClient& comms);
        ~Executor();
        void Run();
        void Quit();

    private:
        AudioDetector& audio_detector;
        GpsDriver& gps;
        CommClient& comms;
        bool terminate;
        std::string makePacket(GpsData& data);
};

#endif
