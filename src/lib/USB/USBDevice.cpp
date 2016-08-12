/*
 * USBDevice.cpp
 *
 *  Created on: Aug 11, 2016
 *      Author: iceman
 */

#include "board/board_usb_device.h"
#include "lib/USBDevice.h"

USBDevice::USBDevice()
{
}

void USBDevice::init()
{
	Board_USB_Device_Init();
}

bool USBDevice::isConfigured()
{
	return Board_USB_Device_isConfigured();
}

bool USBDevice::isInitialized()
{
	return Board_USB_Device_isInitialized();
}
