/*
 * USBDeviceCDC.cpp
 *
 *  Created on: Aug 11, 2016
 *      Author: iceman
 */

#include "lib/USBDeviceCDC.h"
#include "board/board_usb_cdc.h"

USBDeviceCDC::USBDeviceCDC()
{
}

void USBDeviceCDC::begin()
{
	init();
}

void USBDeviceCDC::init()
{
	Board_USB_Device_Init();
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

