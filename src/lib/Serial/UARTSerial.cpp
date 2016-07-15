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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lib/UARTSerial.h"
#include "board/board_serial.h"
#include "board/variant.h" // yield();


// Constructors ////////////////////////////////////////////////////////////////

UARTSerial::UARTSerial( void* pUart, uint32_t dwIrq, uint32_t dwId, RingBuffer* pRx_buffer, RingBuffer* pTx_buffer )
{
  _rx_buffer = pRx_buffer;
  _tx_buffer = pTx_buffer;

  _pUart=pUart;
  _dwIrq=dwIrq;
}

// Public Methods //////////////////////////////////////////////////////////////

void UARTSerial::begin(const uint32_t dwBaudRate)
{
  begin(dwBaudRate, SERIAL_8N1);
}

void UARTSerial::begin(const uint32_t dwBaudRate, const uint32_t config)
{
  init(dwBaudRate, config);
}

void UARTSerial::init(const uint32_t dwBaudRate, const uint32_t configData)
{
    RingBuffer_flush(_rx_buffer);
    RingBuffer_flush(_tx_buffer);

    Serial_UART_Init(_pUart, _dwIrq, dwBaudRate, configData);
}

void UARTSerial::end( void )
{
  // Clear any received data
  RingBuffer_flush(_rx_buffer);

  // Wait for any outstanding data to be sent
  flush();

  /* DeInitialize UART0 peripheral */
  Serial_UART_End(_pUart, _dwIrq);
}

void UARTSerial::setInterruptPriority(uint32_t priority)
{
    Serial_UART_Set_Interrupt_Priority(_dwIrq, priority & 0x0F);
}

uint32_t UARTSerial::getInterruptPriority()
{
    return Serial_UART_Get_Interrupt_Priority(_dwIrq);
}

int UARTSerial::available( void )
{
    return RingBuffer_availableData(_rx_buffer);
}

int UARTSerial::availableForWrite(void)
{
    return RingBuffer_availableSpace(_tx_buffer);
}

int UARTSerial::peek( void )
{
    int8_t uc = -1;

    int result = RingBuffer_peek(_rx_buffer, &uc);

    if(result == 0)
        return -1;

    return (int)uc;
}

int UARTSerial::read( void )
{
    int8_t uc = -1;
    if(RingBuffer_pop(_rx_buffer, (uint8_t *) &uc, 1) == 0)
    {
        uc = -1;
    }

    return uc;
}

void UARTSerial::flush( void )
{
    while(!RingBuffer_isEmpty(_tx_buffer)) yield(); //wait for transmit data to be sent

    Serial_UART_Flush(_pUart);
}

size_t UARTSerial::write( const uint8_t uc_data )
{
    return write((const uint8_t *) &uc_data, 1);
}

// We use double buffering here: one is our ringbuffer, the other is hardware UART buffer (if any)
size_t UARTSerial::write(const uint8_t *buffer, size_t size)
{
    size_t bytesSent = 0;

    bytesSent = (size_t) sendRingBuffer(buffer, size);
    while(bytesSent < size)
    {
        size = size - bytesSent;
        while ( !Serial_UART_TxRegisterEmpty(_pUart) ) yield();
        if(size > 0)
        {
            buffer = buffer + bytesSent;
            bytesSent = (size_t) sendRingBuffer(buffer, size);
        }
    }

    return size;
}

uint32_t UARTSerial::sendRingBuffer(const void *buffer, int size)
{
    uint32_t ret;
    uint8_t *p8 = (uint8_t *) buffer;

    /*
     * We don't want UART IRQ handler to access ringbuffer  while
     * we fill it with data to be sent, so we'll disable UART
     * interrupt here
     */
    Serial_UART_Disable_Interrupt(_pUart, _dwIrq);

    /* Move as much data as possible into transmit ring buffer */
    ret = RingBuffer_push(_tx_buffer, p8, size);
    Serial_UART_Transmit(_pUart, _tx_buffer);

    /* Add additional data to transmit ring buffer if possible */
    ret += RingBuffer_push(_tx_buffer, (p8 + ret), (size - ret));

    /* Enable UART transmit interrupt */
    Serial_UART_Enable_Interrupt(_pUart, _dwIrq);

    return ret;
}
