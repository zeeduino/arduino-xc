/*
 * USBDevice.h
 *
 *  Created on: Aug 11, 2016
 *      Author: iceman
 */

#ifndef LIB_USBDEVICE_H_
#define LIB_USBDEVICE_H_

class USBDevice
{
public:
	USBDevice();
	void init();
	bool isConfigured();
	bool isInitialized();

private:
};


#endif /* LIB_USBDEVICE_H_ */
