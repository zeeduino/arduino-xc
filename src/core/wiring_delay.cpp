/*
 Copyright (c) 2011 Arduino.  All right reserved.
 Copyright (c) 2016 Ravendyne Inc. All rights reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Arduino.h"


uint32_t millis( void )
{
    return Board_Delay_Millis();
}

uint32_t micros( void )
{
    return Board_Delay_Micros();
}

void delay( uint32_t dwMs )
{
    uint32_t maxMillisCount = Board_Delay_MillisMax();
    uint32_t start = Board_Delay_Millis();
    uint32_t now;
    uint32_t delta = 0;

    while(delta < dwMs)
    {
        yield();
        now = Board_Delay_Millis();
        if(now < start)
        {
            // this handles timer counter overflow
            // when counter overflows, 'now' becomes < 'start'
            delta = maxMillisCount - start + now;
        }
        else
        {
            // regular situation is when 'start' comes before 'now'
            delta = now - start;
        }
    }
}

/* Delay for the given number of microseconds. */
void delayMicroseconds( uint32_t dwUs )
{
    uint32_t maxMicrosCount = Board_Delay_MicrosMax();
    uint32_t start = Board_Delay_Micros();
    uint32_t now;
    uint32_t delta = 0;

    while(delta < dwUs)
    {
//        yield();
        now = Board_Delay_Micros();
        if(now < start)
        {
            // this handles timer counter overflow
            // when counter overflows, 'now' becomes < 'start'
            delta = maxMicrosCount - start + now;
        }
        else
        {
            // regular situation is when 'start' comes before 'now'
            delta = now - start;
        }
    }
}

