/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include "wiring_constants.h"

// SPI_HAS_TRANSACTION means SPI has
//   - beginTransaction()
//   - endTransaction()
//   - usingInterrupt()
//   - SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 0

// SPI_HAS_EXTENDED_CS_PIN_HANDLING means SPI has automatic 
// CS pin handling and provides the following methods:
//   - begin(pin)
//   - end(pin)
//   - setBitOrder(pin, bitorder)
//   - setDataMode(pin, datamode)
//   - setClockDivider(pin, clockdiv)
//   - transfer(pin, data, SPI_LAST/SPI_CONTINUE)
//   - beginTransaction(pin, SPISettings settings) (if transactions are available)
#define SPI_HAS_EXTENDED_CS_PIN_HANDLING 0

#define SPI_MODE0 0x02
#define SPI_MODE1 0x00
#define SPI_MODE2 0x03
#define SPI_MODE3 0x01

// TODO
#define BOARD_SPI_DEFAULT_SS (0)

enum SPITransferMode
{
	SPI_CONTINUE, SPI_LAST
};

class SPISettings
{
public:

	SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
	{
			init(clock, bitOrder, dataMode);
	}
	SPISettings()
	{
		init(4000000, MSBFIRST, SPI_MODE0);
	}
private:
	void init(uint32_t clock, uint8_t bitOrder, uint8_t dataMode);

	uint32_t config;
	uint8_t border;
	friend class SPIClass;
};

class SPIClass
{
public:
    SPIClass(uint8_t cs, uint8_t clk, uint8_t miso, uint8_t mosi, uint32_t _id, void (*_initCb)(void) = NULL);
    SPIClass(void *_spi, uint32_t _id, void (*_initCb)(void));

	// Transfer functions
	uint8_t transfer(uint8_t _pin, uint8_t _data, SPITransferMode _mode = SPI_LAST);
	void transfer(uint8_t _pin, void *_buf, size_t _count, SPITransferMode _mode = SPI_LAST);
	// Transfer functions on default pin BOARD_SPI_DEFAULT_SS
	uint8_t transfer(uint8_t _data, SPITransferMode _mode = SPI_LAST)
	{
		return transfer(BOARD_SPI_DEFAULT_SS, _data, _mode);
	}
	void transfer(void *_buf, size_t _count, SPITransferMode _mode = SPI_LAST)
	{
		transfer(BOARD_SPI_DEFAULT_SS, _buf, _count, _mode);
	}

	// Transaction Functions
	void usingInterrupt(uint8_t interruptNumber);
	void beginTransaction(SPISettings settings)
	{
		beginTransaction(BOARD_SPI_DEFAULT_SS, settings);
	}
	void beginTransaction(uint8_t pin, SPISettings settings);
	void endTransaction(void);

	// SPI Configuration methods
	void attachInterrupt(void);
	void detachInterrupt(void);

	void begin(void);
	void end(void);

	// Attach/Detach pin to/from SPI controller
	void begin(uint8_t _ss_pin);
	void end(uint8_t _ss_pin);

	// These methods sets a parameter on a single pin
	void setBitOrder(uint8_t _ss_pin, uint8_t);
	void setDataMode(uint8_t _ss_pin, uint8_t);
	void setClockDivider(uint8_t _ss_pin, uint8_t);

	// These methods sets the same parameters but on default pin BOARD_SPI_DEFAULT_SS
	void setBitOrder(uint8_t _order)
	{
		setBitOrder(BOARD_SPI_DEFAULT_SS, _order);
	}
	void setDataMode(uint8_t _mode)
	{
		setDataMode(BOARD_SPI_DEFAULT_SS, _mode);
	}
	void setClockDivider(uint8_t _div)
	{
		setClockDivider(BOARD_SPI_DEFAULT_SS, _div);
	}

private:
	void init();

	void *pSpi;
	uint32_t id;
	void (*initCb)(void);
	bool initialized;
};

extern SPIClass SPI;

// For compatibility with sketches designed for AVR @ 16 MHz
// New programs should use SPI.beginTransaction to set the SPI clock
#define SPI_CLOCK_DIV2	 11
#define SPI_CLOCK_DIV4	 21
#define SPI_CLOCK_DIV8	 42
#define SPI_CLOCK_DIV16	 84
#define SPI_CLOCK_DIV32	 168
#define SPI_CLOCK_DIV64	 255
#define SPI_CLOCK_DIV128 255

#endif
