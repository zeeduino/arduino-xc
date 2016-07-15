/*
  SPI.cpp - SPI class implementation for Arduino-XC using board HAL API
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

#include "lib/SPI.h"
#include "board/board_spi.h"

SPIClass::SPIClass(uint8_t cs, uint8_t clk, uint8_t miso, uint8_t mosi, uint32_t _id, void (*_initCb)(void)) :
    id(_id), initCb(_initCb), initialized(false)
{
    BoardSPIPort *SPICustomPort = new BoardSPIPort();
    SPICustomPort->handle = (void*)NULL;
    SPICustomPort->cs = cs;
    SPICustomPort->clk = clk;
    SPICustomPort->miso = miso;
    SPICustomPort->mosi = mosi;

    pSpi = (void*)SPICustomPort;

}

SPIClass::SPIClass(void *_spi, uint32_t _id, void(*_initCb)(void)) :
	pSpi(_spi), id(_id), initCb(_initCb), initialized(false)
{
	// Empty
}

void SPIClass::begin()
{
	init();
}

void SPIClass::begin(uint8_t _pin)
{
	init();
}

void SPIClass::init()
{
	if (initialized)
		return;

	Board_SPI_Init((BoardSPIPort *)pSpi);
    Board_SPI_CS_Low((BoardSPIPort *)pSpi);
    Board_SPI_CS_High((BoardSPIPort *)pSpi);

	if(initCb != NULL)
	{
	    initCb();
	}
	initialized = true;
}

void SPISettings::init(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
{
	border = bitOrder;
}

void SPIClass::usingInterrupt(uint8_t interruptNumber)
{
}

void SPIClass::beginTransaction(uint8_t pin, SPISettings settings)
{
}

void SPIClass::endTransaction(void)
{
}

void SPIClass::end(uint8_t _pin)
{
}

void SPIClass::end()
{
	Board_SPI_End((BoardSPIPort *)pSpi);
	initialized = false;
}

void SPIClass::setBitOrder(uint8_t _pin, uint8_t _bitOrder)
{
}

void SPIClass::setDataMode(uint8_t _pin, uint8_t _mode)
{
}

void SPIClass::setClockDivider(uint8_t _pin, uint8_t _divider)
{
}

uint8_t SPIClass::transfer(uint8_t _pin, uint8_t _data, SPITransferMode _mode)
{
    uint8_t r;
    Board_SPI_CS_Low((BoardSPIPort *)pSpi);
    r = Board_SPI_Transfer((BoardSPIPort *)pSpi, _data);
    Board_SPI_CS_High((BoardSPIPort *)pSpi);
	return r;
}

void SPIClass::transfer(uint8_t _pin, void *_buf, size_t _count, SPITransferMode _mode)
{
	uint32_t d;
	uint32_t r;

	if (_count == 0)
		return;

	uint8_t *buffer = (uint8_t *)_buf;
	if (_count == 1)
	{
		*buffer = transfer(_pin, *buffer, _mode);
		return;
	}

    Board_SPI_CS_Low((BoardSPIPort *)pSpi);
	while (_count != 0)
	{
		// Prepare next byte
		d = *(buffer);

		// Read transferred byte and send next one straight away
		r = Board_SPI_Transfer((BoardSPIPort *)pSpi, d);

		*buffer = r;
		buffer++;
		_count--;
	}

    // Receive the last transferred byte
	d = *(buffer);
	r = Board_SPI_Transfer((BoardSPIPort *)pSpi, d);
	*buffer = r;
    Board_SPI_CS_High((BoardSPIPort *)pSpi);
}

void SPIClass::attachInterrupt(void)
{
	// Should be enableInterrupt()
}

void SPIClass::detachInterrupt(void)
{
	// Should be disableInterrupt()
}

