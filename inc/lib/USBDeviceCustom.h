/*
 * USBDeviceCustom.h
 *
 *  Created on: Aug 11, 2016
 *      Author: iceman
 */

#ifndef LIB_USBDEVICECUSTOM_H_
#define LIB_USBDEVICECUSTOM_H_

class USBDeviceCustom
{
public:
	USBDeviceCustom();

	bool isConnected();
	bool isDeviceConfigured();
	bool isInitialized();

private:
	void init();
};



#endif /* LIB_USBDEVICECUSTOM_H_ */
