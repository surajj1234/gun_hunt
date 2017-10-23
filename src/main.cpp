/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#include "Simulator.h"
#include "AudioDetector.h"
#include "Executor.h"
#include "CommClient.h"
#include "FakeSerial.h"
#include "GpsDriver.h"
#include <cstddef>
#include <thread>
#include <iostream>


int main()
{
    CommClient comms("127.0.0.1", 4444);
    AudioDetector detector;
    FakeSerial serial;
    GpsDriver gps(serial);
    Executor executor(detector, gps, comms);
    Simulator sim(detector, comms, serial, gps);

    sim.InitGraphics();

    std::thread comms_thread(&CommClient::Run, std::ref(comms));
    std::thread detect_thread(&AudioDetector::Run, std::ref(detector));
    std::thread gps_thread(&GpsDriver::Run, std::ref(gps));
    std::thread executor_thread(&Executor::Run, std::ref(executor));
    std::thread sim_thread(&Simulator::Run, std::ref(sim));

    sim_thread.join();

    detector.Quit();
    detect_thread.join();

    executor.Quit();
    executor_thread.join();

    comms.Quit();
    comms_thread.join();

    gps.Quit();
    gps_thread.join();

    return 0;
}

