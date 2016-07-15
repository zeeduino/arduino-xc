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

/*
 * Digital I/O is highly platform specific
 * and these functions do very low-level kind of
 * stuff and so we end up with pretty much calling
 * board level API for each of these.
 */


extern void pinMode( uint32_t ulPin, uint32_t ulMode )
{
    Board_Digital_PinMode(ulPin, ulMode);
}

extern void digitalWrite( uint32_t ulPin, uint32_t ulVal )
{
    Board_Digital_Write(ulPin, ulVal);
}

extern int digitalRead( uint32_t ulPin )
{
    bool result = Board_Digital_Read(ulPin);

    return result ? HIGH : LOW;
}


