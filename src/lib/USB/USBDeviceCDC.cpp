/*
 * USBDeviceCDC.cpp
 *
 *  Created on: Aug 11, 2016
 *      Author: iceman
 */

#include "lib/USBDeviceCDC.h"
#include "board/board_usb_cdc.h"

// yield()
#include "board/variant.h"

USBDeviceCDC::USBDeviceCDC(void* uartHandle)
{
	this->uartHandle = uartHandle;
}

void USBDeviceCDC::begin()
{
	init();
}

void USBDeviceCDC::init()
{
	Board_USB_Device_Init();
}

void USBDeviceCDC::begin(unsigned long baudRate)
{
	begin();
}

void USBDeviceCDC::end()
{
}

int USBDeviceCDC::available(void)
{
    return RingBuffer_availableData(UART_CONTEXT(uartHandle)->rx_rbr);
}

int USBDeviceCDC::availableForWrite(void)
{
    return RingBuffer_availableSpace(UART_CONTEXT(uartHandle)->tx_rbr);
}

int USBDeviceCDC::peek(void)
{
    int8_t uc = -1;

    int result = RingBuffer_peek(UART_CONTEXT(uartHandle)->rx_rbr, &uc);

    if(result == 0)
        return -1;

    return (int)uc;
}

int USBDeviceCDC::read(void)
{
    int8_t uc = -1;
    if(RingBuffer_pop(UART_CONTEXT(uartHandle)->rx_rbr, (uint8_t *) &uc, 1) == 0)
    {
        uc = -1;
    }

    return uc;
}

void USBDeviceCDC::flush(void)
{
}

size_t USBDeviceCDC::write(uint8_t uc_data)
{
    return write((const uint8_t *) &uc_data, 1);
}

size_t USBDeviceCDC::write(const uint8_t *buffer, size_t size)
{
    size_t bytesSent = 0;

    bytesSent = (size_t) Board_USB_Device_CDC_Send((uint8_t*)buffer, (uint16_t)size);
    while(bytesSent < size)
    {
        size = size - bytesSent;
        while ( RingBuffer_isFull(UART_CONTEXT(uartHandle)->tx_rbr) ) yield();
        if(size > 0)
        {
            buffer = buffer + bytesSent;
            bytesSent = (size_t) Board_USB_Device_CDC_Send((uint8_t*)buffer, (uint16_t)size);
        }
    }

    return size;
}

USBDeviceCDC::operator bool()
{
	return isConnected();
}


bool USBDeviceCDC::isDeviceConfigured()
{
	return Board_USB_Device_isConfigured();
}

bool USBDeviceCDC::isInitialized()
{
	return Board_USB_Device_isInitialized();
}

bool USBDeviceCDC::isConnected()
{
	return Board_USB_Device_CDC_isConnected();
}

int USBDeviceCDC::sendBuffer(char* buffer, int len)
{
	return Board_USB_Device_CDC_Send((uint8_t*)buffer, (uint16_t)len);
}

int USBDeviceCDC::receiveBuffer(char* buffer, int len)
{
	return Board_USB_Device_CDC_Receive((uint8_t*)buffer, (uint16_t)len);
}

