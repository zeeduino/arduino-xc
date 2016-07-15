/*
  board_spi.h - Board HAL API for SPI peripheral
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

#ifndef BOARD_BOARD_SPI_H_
#define BOARD_BOARD_SPI_H_

#include <stdint.h>

typedef struct
{
	void *handle;
	uint32_t cs:8;
	uint32_t clk:8;
	uint32_t miso:8;
	uint32_t mosi:8;
	uint32_t clock;
	uint8_t bitOrder;
	uint8_t dataMode;
} BoardSPIPort;

void Board_SPI_Init(BoardSPIPort *port);
void Board_SPI_End(BoardSPIPort *port);
uint8_t Board_SPI_Transfer(BoardSPIPort *port, uint8_t data);
void Board_SPI_CS_High(BoardSPIPort *port);
void Board_SPI_CS_Low(BoardSPIPort *port);


#endif /* BOARD_BOARD_SPI_H_ */
