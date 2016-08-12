/*
 * USBDeviceCDC.h
 *
 *  Created on: Aug 11, 2016
 *      Author: iceman
 */

#ifndef LIB_USBDEVICECDC_H_
#define LIB_USBDEVICECDC_H_

#include "lib/USBDevice.h"


class USBDeviceCDC : public USBDevice
{
public:
	USBDeviceCDC();

	bool isConnected();
	int sendBuffer(char* buffer, int len);
	int receiveBuffer(char* buffer, int len);

private:
};



#endif /* LIB_USBDEVICECDC_H_ */
