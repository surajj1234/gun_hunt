/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#include "Simulator.h"
#include <cstddef>
#include <thread>
#include <iostream>

int main(void)
{
    Simulator sim;
    sim.InitGraphics();

    std::thread sim_thread(&Simulator::Run, std::ref(sim));

    sim_thread.join();
    return 0;
}

