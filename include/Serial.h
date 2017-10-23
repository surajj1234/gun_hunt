/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

class Serial
{
    public:
        virtual uint8_t ReadByte() = 0;
        virtual int BytesAvailable() = 0;
};


#endif
