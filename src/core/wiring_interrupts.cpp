/*
  Copyright (c) 2011-2012 Arduino.  All right reserved.
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

void attachInterrupt(uint32_t intNo, void (*callback)(void), uint32_t mode)
{
    uint32_t pinNumber;

    if(!isInterruptNumber(intNo))
        return;

    pinNumber = interruptNumberToPin(intNo);

    Board_Attach_Interrupt(pinNumber, callback, mode);
}

void detachInterrupt(uint32_t intNo)
{
//    if(intNo >= EXTERNAL_NUM_INTERRUPTS)
//        return;

    Board_Detach_Interrupt(intNo);
}

