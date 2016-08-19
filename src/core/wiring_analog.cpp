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

static int _readResolution = 10;
static int _writeResolution = 8;

void analogOutputInit( void )
{

}

void analogReadResolution(int res)
{
    _readResolution = res;
}

void analogWriteResolution(int res)
{
    _writeResolution = res;
}

uint32_t analog_reference = DEFAULT;

void analogReference(uint32_t ulMode)
{
    analog_reference = ulMode;
}

uint32_t analogRead(uint32_t ulInput)
{
    uint16_t dataADC;

    dataADC = Board_Analog_Read(ulInput);

    // LPC1347 has 12-bit ADC, scale to 10-bit for Arduino
    return mapBits((uint32_t) dataADC, 12, _readResolution);
}

extern const uint32_t arduinoPinsArrayLength;

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins we do nothing
void analogWrite(uint32_t ulPin, uint32_t ulValue)
{
    if ( !isPin(ulPin) )
        return;

    // Check if selected pin has PWM functionality
    if ( !isPinPwm(ulPin) )
    {
        // Defaults to digital write
//        pinMode(ulPin, OUTPUT);
//        ulValue = mapBits(ulValue, _writeResolution, 8);
//        if (ulValue < 128)
//            digitalWrite(ulPin, LOW);
//        else
//            digitalWrite(ulPin, HIGH);
        return;
    }

    // For this implementation, PWM duty cycle is internally fixed to 10-bit resolution
    ulValue = mapBits(ulValue, _writeResolution, 10);
    Board_Analog_Write(ulPin, ulValue);
}
