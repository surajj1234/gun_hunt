/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#include "Simulator.h"
#include "AudioDetector.h"
#include "Executor.h"
#include <cstddef>
#include <thread>
#include <iostream>


int main()
{
    AudioDetector detector;
    Executor executor(detector);
    Simulator sim(detector);
    sim.InitGraphics();

    std::thread detect_thread(&AudioDetector::Run, std::ref(detector));
    std::thread executor_thread(&Executor::Run, std::ref(executor));
    std::thread sim_thread(&Simulator::Run, std::ref(sim));

    sim_thread.join();

    detector.Quit();
    detect_thread.join();

    executor.Quit();
    executor_thread.join();

    return 0;
}

