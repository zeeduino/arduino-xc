
#ifndef BOARD_BOARD_SERIAL_CONTEXT_H_
#define BOARD_BOARD_SERIAL_CONTEXT_H_

#include "ringbuffer.h"

typedef struct _Serial_UART_Context
{
    uint32_t dwId;
	void *pUART;
    RingBuffer *rx_rbr;
    RingBuffer *tx_rbr;
    uint8_t *rx_buffer;
    uint8_t *tx_buffer;
    uint32_t dwIrq;
} Serial_UART_Context;

/* To help cast void* UART handle into the context struct */
#define UART_CONTEXT(p) ((Serial_UART_Context*)(p))

#endif /* BOARD_BOARD_SERIAL_CONTEXT_H_ */
