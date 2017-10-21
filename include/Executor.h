/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "AudioDetector.h"

class Executor
{
    public:
        Executor(AudioDetector& ad);
        ~Executor();
        void Run();
        void Quit();

    private:
        AudioDetector& audio_detector;
        bool terminate;
};

#endif
