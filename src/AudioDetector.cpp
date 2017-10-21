/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#include "AudioDetector.h"

AudioDetector::AudioDetector()
{
    detected = false;
    terminate = false;
}

AudioDetector::~AudioDetector()
{
}

void AudioDetector::Run()
{
    // Run some funky algorithm to detect a gunshot from the incoming audio
    // Maybe a FFT -> frequency analysis -> pattern matching -> etc...?
    // If all else fails, throw some youtube videos into neural nets and
    // hope it works !
    while(terminate == false);
}

bool AudioDetector::GunshotDetected()
{
    return detected;
}

void AudioDetector::Reset()
{
    detected = false;
}

void AudioDetector::Quit()
{
    terminate = true;
}
void AudioDetector::setDetected(bool state)
{
    detected = state;
}

