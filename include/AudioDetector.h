/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#ifndef AUDIO_DETECTOR_H
#define AUDIO_DETECTOR_H

class AudioDetector
{
    public:
        AudioDetector();
        ~AudioDetector();
        void Run();
        bool GunshotDetected();
        void Reset();
        void Quit();

    private:
       void setDetected(bool state);
       bool detected;
       bool terminate;
       friend class Simulator;  // TODO: Is there a better way to do this? 
};

#endif
