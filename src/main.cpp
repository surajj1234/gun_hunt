/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#include "Simulator.h"
#include <cstddef>

int main(void)
{
    Simulator sim;
    sim.InitGraphics();

    sim.Run(NULL);
    return 0;
}

