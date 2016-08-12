/*
 * board_usb.h
 *
 *  Created on: Aug 11, 2016
 *      Author: iceman
 */

#ifndef BOARD_BOARD_USB_DEVICE_H_
#define BOARD_BOARD_USB_DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

bool Board_USB_Device_Init(void);
bool Board_USB_Device_isConfigured(void);
bool Board_USB_Device_isInitialized(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_BOARD_USB_DEVICE_H_ */
