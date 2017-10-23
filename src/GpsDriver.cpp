/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

// Using a simplified version of the NMEA protocol. 

// For the purpose of this demo, a typical GPS packet is specified to be in
// the following format
//
//      $GPXXX,lat,lat_direction,long,long_direction,time\n
//
//  For example
//      $GPXXX,100,S,25,E,152.123456789\n

#include "GpsDriver.h"
#include <string>
#include <cstring>

static const std::string FRAME_HEADER      = "GPXXX";
static const uint8_t     FRAME_START_BYTE  = '$';
static const uint8_t     FRAME_END_BYTE    = '\n';
static const uint8_t     FRAME_SEPARATOR   = ',';
static const uint8_t     FRAME_MAX_LENGTH  = 50;
static const uint32_t    FRAME_TIMEOUT_MS  = 100;

GpsDriver::GpsDriver(Serial& ser) : serial(ser)
{
    rx_state = RX_START_OF_FRAME;
    rx_length = 0;
    std::memset(&data, 0x0, sizeof(data));
    terminate = false;
}

GpsDriver::~GpsDriver()
{
}

void GpsDriver::Run()
{
    while(terminate == false)
    {
        if (serial.BytesAvailable() > 0)
        {
            updateStateMachine(serial.ReadByte());
        }
    }
}

void GpsDriver::updateStateMachine(uint8_t byte)
{
    switch(rx_state)
    {
        case RX_START_OF_FRAME:
            if (byte == FRAME_START_BYTE)
            {
                rx_length = 0;
                rx_buffer = "";
                clock_gettime(CLOCK_REALTIME, &frame_rx_start);
                rx_state = RX_DATA;
            }
            break;
        case RX_DATA:
            if (byte == FRAME_END_BYTE)
            {
                rx_buffer += byte;
                processPacket(rx_buffer);
                rx_state = RX_START_OF_FRAME;
            }
            else if (rx_length >= FRAME_MAX_LENGTH)
            {
                // Should have seen the terminator by now, abort current frame
                rx_state = RX_START_OF_FRAME;
            }
            else if (rxTimeout())
            {
                rx_state = RX_START_OF_FRAME;
            }
            else
            {
                rx_buffer += byte;
                rx_length++;
            }
            break;
        default:
            rx_state = RX_START_OF_FRAME;
            break;
    }
}

void GpsDriver::Quit()
{
    terminate = true;
}

GpsData GpsDriver::GetData()
{
    // Note: Since there is a fixed update rate from the sensor, there
    // will a worst case latency in the position estimate equal to the
    // update interval. A possible mitigation could be to track velocity
    // and predict current position based on the current timestamp.

    // Critical section
    std::lock_guard<std::mutex> lk(my_lock);
    return data;
}

void GpsDriver::PulseInterruptHandler(struct timespec time)
{
    // This method should be called from a hardware interrupt handler which 
    // triggers on receiving a synchronizing pulse from the gps device
    // TODO: Implement this

    // Critical section
    std::lock_guard<std::mutex> lk(my_lock);
}

void GpsDriver::processPacket(std::string& packet)
{
    std::vector<std::string> items = splitPacket(packet);
    if (validPacket(items))
    {
        // Critical section
        std::lock_guard<std::mutex> lk(my_lock);

        data.latitude = std::stoi(items[1]);
        data.lat_direction = items[2];
        data.longitude = std::stoi(items[3]);
        data.long_direction = items[4];
        data.time = std::stod(items[5]);
    }
    else
    {
        // TODO: Not sure how to handle invalid packets. The cached data will
        // become stale if we miss a legitimate packet. Perhaps implement a
        // timeout and set an error flag if we see too many packet drops, or
        // haven't seem a good packet in a while
    }
}

std::vector<std::string> GpsDriver::splitPacket(std::string& packet)
{
    std::vector<std::string> items;
    std::string field = "";
    for (uint16_t i  = 0; i < packet.size(); i++)
    {
        if ((packet[i] == FRAME_SEPARATOR) || (packet[i] == FRAME_END_BYTE))
        {
            items.push_back(field);
            field = "";
        }
        else
        {
            field += packet[i];
        }
    }
    return items;
}

bool GpsDriver::validPacket(std::vector<std::string>& items)
{
    if (items.size() != 6)
        return false;

    if (items[0] != std::string(FRAME_HEADER))
        return false;

    if (!isAValidNumber(items[1]))      // Latitude should be a valid number
        return false;

    if (!isAValidDirection(items[2]))
        return false;

    if (!isAValidNumber(items[3]))      // Longitude should be a valid number
        return false;

    if (!isAValidDirection(items[4]))
        return false;

    if (!isAValidNumber(items[5]))      // Time should be a valid number
        return false;

    return true;
}

bool GpsDriver::rxTimeout()
{
    // TODO: Implement this
    return false;
}

bool GpsDriver::isAValidNumber(std::string number)
{
    // TODO: Implement a method that returns true if the input is a valid
    // integer or float, and false otherwise
    return true;
}

bool GpsDriver::isAValidDirection(std::string direction)
{
    // TODO: Implement a method that returns true if the input is equal to
    // one of the following: "N", "S", "E", "W", and false otherwise
    return true;
}
