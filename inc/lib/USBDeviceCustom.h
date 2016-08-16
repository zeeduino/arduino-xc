/*
 * USBDeviceCustom.h
 *
 *  Created on: Aug 11, 2016
 *      Author: iceman
 */

#ifndef LIB_USBDEVICECUSTOM_H_
#define LIB_USBDEVICECUSTOM_H_

#include <inttypes.h>
#include <stddef.h>

class USBDeviceCustom
{
public:
	USBDeviceCustom();


	void begin();
	bool isConnected();
	bool isDeviceConfigured();
	bool isInitialized();

	bool dataAvailable();

	int write(const uint8_t *data, size_t size);
	int read(uint8_t *data, size_t size);

private:
	void init();
};

extern USBDeviceCustom UsbBulk;

#endif /* LIB_USBDEVICECUSTOM_H_ */
