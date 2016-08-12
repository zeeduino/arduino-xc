/*
 * board_usb_custom.h
 *
 *  Created on: Aug 11, 2016
 *      Author: iceman
 */

#ifndef BOARD_BOARD_USB_CUSTOM_H_
#define BOARD_BOARD_USB_CUSTOM_H_

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

bool Board_USB_Device_Custom_isReadyToSend(void);
bool Board_USB_Device_Custom_Send(uint8_t const * p_data, uint32_t length);
bool Board_USB_Device_Custom_Receive(uint8_t * p_data, uint32_t length);

#ifdef __cplusplus
}
#endif



#endif /* BOARD_BOARD_USB_CUSTOM_H_ */
