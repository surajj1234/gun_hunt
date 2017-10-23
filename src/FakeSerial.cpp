/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#include "FakeSerial.h"

FakeSerial::FakeSerial()
{
}

FakeSerial::~FakeSerial()
{
}

uint8_t FakeSerial::ReadByte()
{
    uint8_t byte = 0;

    // Critical section
    std::lock_guard<std::mutex> lk(my_lock);
    if (rx_queue.size() > 0)
    {
        byte = rx_queue[0];
        rx_queue.erase(rx_queue.begin());
    }
    return byte;
}

int FakeSerial::BytesAvailable()
{
    // Critical section
    std::lock_guard<std::mutex> lk(my_lock);
    return rx_queue.size();
}

void FakeSerial::AddData(std::string data)
{
    // Critical section
    std::lock_guard<std::mutex> lk(my_lock);
    for (uint32_t i = 0; i < data.length(); i++)
    {
        rx_queue.push_back(data[i]);
    }
}
