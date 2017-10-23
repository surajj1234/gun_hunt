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
            // TODO: Refactor packet construction
            std::string packet = std::string("Latitude: ")            +
                                 std::to_string(data.latitude)        +
                                 std::string(" ")                     +
                                 std::to_string(data.lat_direction)      +
                                 std::string("\t")                    +
                                 std::string("Longitude: ")           +
                                 std::to_string(data.longitude)       +
                                 std::string(" ")                     +
                                 std::to_string(data.long_direction)  +
                                 std::string("\t")                    +
                                 std::string("Time: ")                +
                                 std::to_string(data.longitude)       +
                                 std::to_string(data.time)            +
                                 std::string("\n");
            comms.SendData(packet);
            audio_detector.Reset();
        }
    }
}

void Executor::Quit()
{
    terminate = true;
}
