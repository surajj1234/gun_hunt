/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#include "Executor.h"
#include "stdio.h"

Executor::Executor(AudioDetector& ad) : audio_detector(ad)
{
    terminate = false;
}

Executor::~Executor()
{
}

void Executor::Run()
{
    while(false == terminate)
    {
        if (audio_detector.GunshotDetected())
        {
            // Read gps data
            // Send comms packet to server
        }
    }
}

void Executor::Quit()
{
    terminate = true;
}
