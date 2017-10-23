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
    clock_gettime(CLOCK_REALTIME, &last_data_update);
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
    //TODO: Critical section
    return data;
}

void GpsDriver::PulseInterruptHandler(struct timespec time)
{
    // This method should be called from a hardware interrupt handler which 
    // triggers on receiving a synchronizing pulse from the gps device
    // TODO: Implement this
}

#include <iostream>
void GpsDriver::processPacket(std::string& packet)
{
    std::vector<std::string> items = splitPacket(packet);
    if (validPacket(items))
    {
        //std::cout << packet;
        for (int i = 0; i < (int)items.size(); i++)
        {
            //std::cout << items[i] << "\n";
        }
        // TODO: Critical section
        data.latitude = std::stoi(items[1]);
        data.lat_direction = items[2][0];
        data.longitude = std::stoi(items[3]);
        data.long_direction = items[4][0];
        data.time = std::stod(items[5]);
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
    // TODO: Implement this
    return true;
}

bool GpsDriver::rxTimeout()
{
    // TODO: Implement this
    return false;
}
