/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#ifndef FAKE_SERIAL_H
#define FAKE_SERIAL_H

#include "Serial.h"
#include <string>
#include <vector>
#include <mutex>

class FakeSerial : public Serial
{
    public:
        FakeSerial();
        ~FakeSerial();
        uint8_t ReadByte();
        int BytesAvailable();
        void AddData(std::string data);
    private:
        std::vector<uint8_t> rx_queue;
        std::mutex my_lock;
};

#endif
