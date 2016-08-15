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

#ifndef _UART_SERIAL_
#define _UART_SERIAL_

#include "HardwareSerial.h"
#include "ringbuffer.h"

/*
    SERIAL_5N1
    SERIAL_6N1
    SERIAL_7N1
    SERIAL_8N1 (the default)
    SERIAL_5N2
    SERIAL_6N2
    SERIAL_7N2
    SERIAL_8N2
    SERIAL_5E1
    SERIAL_6E1
    SERIAL_7E1
    SERIAL_8E1
    SERIAL_5E2
    SERIAL_6E2
    SERIAL_7E2
    SERIAL_8E2
    SERIAL_5O1
    SERIAL_6O1
    SERIAL_7O1
    SERIAL_8O1
    SERIAL_5O2
    SERIAL_6O2
    SERIAL_7O2
    SERIAL_8O2
*/

// Values for these are platform specific
// and they are defined in a platform specific library
// (i.e. in arduino-xc-lpc13xx)
extern const uint32_t SERIAL_8N1;
extern const uint32_t SERIAL_8E1;
extern const uint32_t SERIAL_8O1;
extern const uint32_t SERIAL_8M1;
extern const uint32_t SERIAL_8S1;


class UARTSerial : public HardwareSerial
{
  public:
    UARTSerial(){};
    UARTSerial(void* pUart);

    void begin(const uint32_t dwBaudRate);
    void begin(const uint32_t dwBaudRate, const uint32_t config);
    void end(void);
    int available(void);
    int availableForWrite(void);
    int peek(void);
    int read(void);
    void flush(void);
    size_t write(const uint8_t c);
    // We have our own write() for buffers, faster than default implementation in Print
    size_t write(const uint8_t *buffer, size_t size);
    // For write(str), pull in from Print
    using Print::write;

    void setInterruptPriority(uint32_t priority);
    uint32_t getInterruptPriority();

    operator bool() { return true; }; // UART always active

  protected:
    void init(const uint32_t dwBaudRate, const uint32_t config);
    uint32_t sendRingBuffer(const void *buffer, int size);

    void* uartHandle;

};

#endif // _UART_SERIAL_
