/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#include "Simulator.h"
#include "AudioDetector.h"
#include <cstddef>
#include <thread>
#include <iostream>

int main()
{
    AudioDetector detector;
    Simulator sim(detector);
    sim.InitGraphics();


    std::thread detect_thread(&AudioDetector::Run, std::ref(detector));
    std::thread sim_thread(&Simulator::Run, std::ref(sim));

    sim_thread.join();

    detector.Quit();
    detect_thread.join();

    return 0;
}

