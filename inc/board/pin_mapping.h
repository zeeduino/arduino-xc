/*
  pin_mapping.h - Board HAL API for pin mapping and management
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

#ifndef PIN_MAPPING_H_
#define PIN_MAPPING_H_

#include <stdint.h>
#include <stdbool.h>

#define NOT_A_PIN (0xFFFFFFFF)

#define SPI_BUILTIN_SSEL    (20)
#define SPI_BUILTIN_SCK     (21)
#define SPI_BUILTIN_MISO    (22)
#define SPI_BUILTIN_MOSI    (23)

#define WIRE_BUILTIN_SCL     (24)
#define WIRE_BUILTIN_SDA     (25)

bool isPinPwm(uint32_t ulPin);
bool isPwmChannel(uint32_t ulPwmCahnnel);
uint32_t pinToPwmChannel(uint32_t ulPin);

bool isAdcChannel(uint32_t ulAdcCahnnel);
uint32_t adcChannelToPin(uint32_t adcChannel);
uint32_t pinToAdcChannel(uint32_t ulPin);

bool isInterruptNumber(uint32_t ulIntNumber);
uint32_t interruptNumberToPin(uint32_t intNo);

uint32_t pinCount(void);
bool isPin(uint32_t ulPin);

#endif /* PIN_MAPPING_H_ */
