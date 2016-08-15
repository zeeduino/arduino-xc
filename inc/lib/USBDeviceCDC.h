/*
 * USBDeviceCDC.h
 *
 *  Created on: Aug 11, 2016
 *      Author: iceman
 */

#ifndef LIB_USBDEVICECDC_H_
#define LIB_USBDEVICECDC_H_


class USBDeviceCDC
{
public:
	USBDeviceCDC();

	void begin();

	bool isConnected();
	bool isDeviceConfigured();
	bool isInitialized();
	int sendBuffer(char* buffer, int len);
	int receiveBuffer(char* buffer, int len);

private:
	void init();
};



#endif /* LIB_USBDEVICECDC_H_ */
