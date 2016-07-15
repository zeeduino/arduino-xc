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

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.
 * timeout is in milliseconds and counts from moment the call to pulseIn() is
 * made to the end of the measured pulse. This means that, once you make a call
 * to pulseIn(), waiting for pulse to start, pulse start and pulse end, all has
 * to happen within timeout period of time.
 * I.e. if your timeout is 1000ms that means you won't be able to measure pulses
 * longer than 1000ms. Even if pulse is 100ms long, but it starts 950ms after
 * you called pulseIn(), it won't be measured because (950ms + 100ms) > 1000ms.
 */
extern uint32_t pulseIn( uint32_t ulPin, uint32_t state, uint32_t timeout )
{
    uint32_t pulseMeasureStart;
    uint32_t pulseStart;
    uint32_t pulseWidth;
    bool pulseActiveState = state == HIGH ? true : false;

    if(!isPin(ulPin))
        return 0;

    // Convert timeout to microseconds
    timeout *= 1000;

    pulseMeasureStart = micros();
    // Wait for pin to go out of previous active state (if any)
    while(digitalRead(ulPin) == pulseActiveState)
    {
        if((micros() - pulseMeasureStart) > timeout)
            return 0;
    }
    // Wait for pin to go to active state
    while(digitalRead(ulPin) != pulseActiveState)
    {
        if((micros() - pulseMeasureStart) > timeout)
            return 0;
    }

    pulseStart = micros();

    // Wait for pin to go out of active state
    while(digitalRead(ulPin) == pulseActiveState)
    {
        if((micros() - pulseMeasureStart) > timeout)
            return 0;
    }

    pulseWidth = micros() - pulseStart;

    return pulseWidth;
}
