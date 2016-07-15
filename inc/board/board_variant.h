/*
  board_variant.h - Board HAL API for Arduino core functions: digital, analog, pulse, delay, timer, interrupt
  Copyright (c) 2016 Ravendyne Inc.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef BOARD_VARIANT_H_
#define BOARD_VARIANT_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "utilities.h"

void Board_Digital_PinMode( uint32_t ulPin, uint32_t ulMode );
void Board_Digital_Write( uint32_t ulPin, uint32_t ulVal );
bool Board_Digital_Read( uint32_t ulPin );

uint32_t Board_Analog_Read(uint32_t adcChannel);
void Board_Analog_Write(uint32_t ulPin, uint32_t ulValue);

void Board_Attach_Interrupt(uint32_t ulPin, void (*callback)(void), uint32_t mode);
void Board_Detach_Interrupt(uint32_t intNo);

bool Board_Digital_PulseIn( uint32_t ulPin, uint32_t state, uint32_t timeout );

void Board_Delay_InitTimer( void );
uint32_t Board_Delay_Millis( void );
uint32_t Board_Delay_MillisMax( void );
uint32_t Board_Delay_Micros( void );
uint32_t Board_Delay_MicrosMax( void );

void Board_Init(void);

#endif /* BOARD_VARIANT_H_ */
