/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#include "Executor.h"
#include <string>

Executor::Executor(AudioDetector& ad, GpsDriver& gd, CommClient& cc) :
    audio_detector(ad), gps(gd), comms(cc)
{
    terminate = false;
}

Executor::~Executor()
{
}

void Executor::Run()
{
    while(terminate == false)
    {
        if (audio_detector.GunshotDetected())
        {
            GpsData data = gps.GetData();
            std::string packet = makePacket(data);
            comms.SendData(packet);
            audio_detector.Reset();
        }
    }
}

void Executor::Quit()
{
    terminate = true;
}

std::string Executor::makePacket(GpsData& data)
{
    std::string packet = std::string("Latitude: ")            +
                         std::to_string(data.latitude)        +
                         data.lat_direction                   +
                         std::string("\t")                    +
                         std::string("Longitude: ")           +
                         std::to_string(data.longitude)       +
                         data.long_direction                  +
                         std::string("\t")                    +
                         std::string("Time: ")                +
                         std::to_string(data.time)            +
                         std::string("\n");
    return packet;
}
