/*
 * board_usb_cdc.h
 *
 *  Created on: Aug 11, 2016
 *      Author: iceman
 */

#ifndef BOARD_BOARD_USB_CDC_H_
#define BOARD_BOARD_USB_CDC_H_

#include "board/board_usb_device.h"
#include "board/board_serial_context.h"

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

bool Board_USB_Device_CDC_putc(uint8_t c);
bool Board_USB_Device_CDC_getc(uint8_t *c);
bool Board_USB_Device_CDC_isConnected();

uint16_t Board_USB_Device_CDC_Send(uint8_t* buffer, uint16_t count);
uint16_t Board_USB_Device_CDC_Receive(uint8_t* buffer, uint16_t max);

#ifdef __cplusplus
}
#endif



#endif /* BOARD_BOARD_USB_CDC_H_ */
