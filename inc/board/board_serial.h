/*
  board_serial.h - Board HAL API for serial comm
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

#ifndef BOARD_SERIAL_H_
#define BOARD_SERIAL_H_

#include <stddef.h>
#include "ringbuffer.h"

void Serial_Init(void);

void Serial_UART_Init(void *pUart, uint32_t dwIrq, const uint32_t dwBaudRate, const uint32_t configData);
void Serial_UART_End(void *pUart, uint32_t dwIrq);
void Serial_UART_Flush(void *pUart);

void Serial_UART_Transmit(void *pUart, RingBuffer *_tx_buffer);
void Serial_UART_Receive(void *pUart, RingBuffer *_rx_buffer);
bool Serial_UART_TxRegisterEmpty(void *pUART);
bool Serial_UART_RxRegisterHasData(void *pUART);

void Serial_UART_Set_Interrupt_Priority(uint32_t dwIrq, uint32_t priority);
uint32_t Serial_UART_Get_Interrupt_Priority(uint32_t dwIrq);
void Serial_UART_Disable_Interrupt(void *pUART, uint32_t dwIrq);
void Serial_UART_Enable_Interrupt(void *pUART, uint32_t dwIrq);

#endif /* BOARD_SERIAL_H_ */
