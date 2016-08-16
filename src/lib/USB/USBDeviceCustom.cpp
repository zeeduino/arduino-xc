/*
 * USBDeviceCustom.cpp
 *
 *  Created on: Aug 11, 2016
 *      Author: iceman
 */

#include "lib/USBDeviceCustom.h"
#include "board/board_usb_custom.h"

USBDeviceCustom::USBDeviceCustom()
{
}

void USBDeviceCustom::begin()
{
	init();
}

void USBDeviceCustom::init()
{
	Board_USB_Device_Init();
}

bool USBDeviceCustom::isDeviceConfigured()
{
	return Board_USB_Device_isConfigured();
}

bool USBDeviceCustom::isInitialized()
{
	return Board_USB_Device_isInitialized();
}

bool USBDeviceCustom::isConnected()
{
	return Board_USB_Device_Custom_isReadyToSend();
}

bool USBDeviceCustom::dataAvailable()
{
	return Board_USB_Device_Custom_availableDataCount() > 0;
}

int USBDeviceCustom::write(const uint8_t *data, size_t size)
{
	return Board_USB_Device_Custom_Send(data, (uint32_t) size);
}

int USBDeviceCustom::read(uint8_t *data, size_t size)
{
	return Board_USB_Device_Custom_Receive(data, (uint32_t) size);
}
