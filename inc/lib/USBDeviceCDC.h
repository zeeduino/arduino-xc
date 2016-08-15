/*
 * USBDeviceCDC.h
 *
 *  Created on: Aug 11, 2016
 *      Author: iceman
 */

#ifndef LIB_USBDEVICECDC_H_
#define LIB_USBDEVICECDC_H_

#include "HardwareSerial.h"

class USBDeviceCDC : public HardwareSerial
{
public:
	USBDeviceCDC(void* uartHandle);

	void begin();
    void begin(unsigned long baudRate);
    void end();
    int available(void);
    int availableForWrite(void);
    int peek(void);
    int read(void);
    void flush(void);
    size_t write(uint8_t);
    // We have our own write() for buffers, faster than default implementation in Print
    size_t write(const uint8_t *buffer, size_t size);
    operator bool();

	bool isConnected();
	bool isDeviceConfigured();
	bool isInitialized();
	int sendBuffer(char* buffer, int len);
	int receiveBuffer(char* buffer, int len);

private:
	void init();

	void* uartHandle;
};

extern USBDeviceCDC UsbSerial;

#endif /* LIB_USBDEVICECDC_H_ */
