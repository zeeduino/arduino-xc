
/*
 * Wire.h - TWI/I2C library for Arduino Due
 * Copyright (c) 2011 Cristian Maglie <c.maglie@arduino.cc>
 * All rights reserved.
 * Copyright (c) 2016 Ravendyne Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

extern "C"
{
#include <string.h>
}

#include "lib/Wire.h"
#include "board/board_wire.h"
#include "board/board_variant.h"

WireClass::WireClass(void *pTwi, void (*beginCb)(void))
{
    pDeviceHandle = pTwi;
}

void WireClass::begin(void)
{
    if (onBeginCallback)
        onBeginCallback();

    Board_I2C_Master_Init(pDeviceHandle);

    status = MASTER_IDLE;
}

void WireClass::begin(uint8_t address)
{
    if (onBeginCallback)
        onBeginCallback();

    Board_I2C_Slave_Init(pDeviceHandle, address);

    status = SLAVE_IDLE;
}

void WireClass::begin(int address)
{
    begin((uint8_t) address);
}

void WireClass::setClock(uint32_t frequency)
{
    twiClock = frequency;
    Board_I2C_Set_Bus_Speed(pDeviceHandle, frequency);
}

uint8_t WireClass::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    if (quantity > BUFFER_LENGTH)
        quantity = BUFFER_LENGTH;

    status = MASTER_RECV;

    rxBytesRequested = quantity;

    Board_I2C_Master_Read_Blocking(pDeviceHandle, address, rxBuffer, rxBytesRequested);

    rxBytesReadFromBuffer = 0;

    int readed = 0;
    return readed;
}

uint8_t WireClass::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t WireClass::requestFrom(int address, int quantity)
{
    return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t WireClass::requestFrom(int address, int quantity, int sendStop)
{
    return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) sendStop);
}

void WireClass::beginTransmission(uint8_t address)
{
    status = MASTER_SEND;

    xferSlaveAddr = address;
    xferTxSz = 0;

}

void WireClass::beginTransmission(int address)
{
    beginTransmission((uint8_t) address);
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to
//	perform a repeated start.
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t WireClass::endTransmission(uint8_t sendStop)
{
    uint8_t error = 0;
    if(status == MASTER_SEND)
    {
        error = Board_I2C_Master_Write_Blocking(pDeviceHandle, xferSlaveAddr, txBuffer, xferTxSz);

        xferTxSz = 0;
    }
    status = MASTER_IDLE;
    return error;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t WireClass::endTransmission(void)
{
    return endTransmission(true);
}

size_t WireClass::write(uint8_t data)
{
//    if (status == MASTER_SEND)
//    {
        if (xferTxSz >= BUFFER_LENGTH)
            return 0;
        txBuffer[xferTxSz++] = data;
        return 1;
//    }
//    else
//    {
//        if (srvBufferLength >= BUFFER_LENGTH)
//            return 0;
//        srvBuffer[srvBufferLength++] = data;
//        return 1;
//    }
}

size_t WireClass::write(const uint8_t *data, size_t quantity)
{
//    if (status == MASTER_SEND)
//    {
        for (size_t i = 0; i < quantity; ++i)
        {
            if (xferTxSz >= BUFFER_LENGTH)
                return i;
            txBuffer[xferTxSz++] = data[i];
        }
//    }
//    else
//    {
//        for (size_t i = 0; i < quantity; ++i)
//        {
//            if (srvBufferLength >= BUFFER_LENGTH)
//                return i;
//            srvBuffer[srvBufferLength++] = data[i];
//        }
//    }
    return quantity;
}

int WireClass::available(void)
{
    return rxBytesRequested - rxBytesReadFromBuffer;
}

int WireClass::read(void)
{
    if (rxBytesReadFromBuffer < rxBytesRequested)
        return rxBuffer[rxBytesReadFromBuffer++];
    return -1;
}

int WireClass::peek(void)
{
    if (rxBytesReadFromBuffer < rxBytesRequested)
        return rxBuffer[rxBytesReadFromBuffer];
    return -1;
}

void WireClass::flush(void)
{
    // Do nothing, use endTransmission(..) to force
    // data transfer.
}

void WireClass::onReceive(void (*function)(int))
{
    onReceiveCallback = function;
}

void WireClass::onRequest(void (*function)(void))
{
    onRequestCallback = function;
}

// This is for slave device
void WireClass::onService(void)
{
/*
    // Retrieve interrupt status
    uint32_t sr = TWI_GetStatus(twi);

    if (status == SLAVE_IDLE && TWI_STATUS_SVACC(sr))
    {
        TWI_DisableIt(twi, TWI_IDR_SVACC);
        TWI_EnableIt(twi,
                TWI_IER_RXRDY | TWI_IER_GACC | TWI_IER_NACK | TWI_IER_EOSACC | TWI_IER_SCL_WS | TWI_IER_TXCOMP);

        srvBufferLength = 0;
        srvBufferIndex = 0;

        // Detect if we should go into RECV or SEND status
        // SVREAD==1 means *master* reading -> SLAVE_SEND
        if (!TWI_STATUS_SVREAD(sr))
        {
            status = SLAVE_RECV;
        }
        else
        {
            status = SLAVE_SEND;

            // Alert calling program to generate a response ASAP
            if (onRequestCallback)
                onRequestCallback();
            else
                // create a default 1-byte response
                write((uint8_t) 0);
        }
    }

    if (status != SLAVE_IDLE)
    {
        if (TWI_STATUS_TXCOMP(sr) && TWI_STATUS_EOSACC(sr))
        {
            if (status == SLAVE_RECV && onReceiveCallback)
            {
                // Copy data into rxBuffer
                // (allows to receive another packet while the
                // user program reads actual data)
                for (uint8_t i = 0; i < srvBufferLength; ++i)
                    rxBuffer[i] = srvBuffer[i];
                rxBufferIndex = 0;
                rxBufferLength = srvBufferLength;

                // Alert calling program
                onReceiveCallback(rxBufferLength);
            }

            // Transfer completed
            TWI_EnableIt(twi, TWI_SR_SVACC);
            TWI_DisableIt(twi,
                    TWI_IDR_RXRDY | TWI_IDR_GACC | TWI_IDR_NACK | TWI_IDR_EOSACC | TWI_IDR_SCL_WS | TWI_IER_TXCOMP);
            status = SLAVE_IDLE;
        }
    }

    if (status == SLAVE_RECV)
    {
        if (TWI_STATUS_RXRDY(sr))
        {
            if (srvBufferLength < BUFFER_LENGTH)
                srvBuffer[srvBufferLength++] = TWI_ReadByte(twi);
        }
    }

    if (status == SLAVE_SEND)
    {
        if (TWI_STATUS_TXRDY(sr) && !TWI_STATUS_NACK(sr))
        {
            uint8_t c = 'x';
            if (srvBufferIndex < srvBufferLength)
                c = srvBuffer[srvBufferIndex++];
            TWI_WriteByte(twi, c);
        }
    }
*/
}
