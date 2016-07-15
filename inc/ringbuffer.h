/*
  ringbuffer.h - ring buffer implementation for arbitrary data
  Copyright (c) 2016 Ravendyne Inc.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *buffer;
    uint32_t length;
    uint32_t itemCount;
    uint32_t itemSize;
    uint32_t head;
    //uint32_t tail; // we use fill count to track position
} RingBuffer;

void RingBuffer_init(RingBuffer *rbuff, void *buffer, uint32_t count, uint32_t itemSize);

bool RingBuffer_isEmpty(RingBuffer *rbuff);

bool RingBuffer_isFull(RingBuffer *rbuff);

uint32_t RingBuffer_availableData(RingBuffer *rbuff);

uint32_t RingBuffer_availableSpace(RingBuffer *rbuff);

uint32_t RingBuffer_push(RingBuffer *rbuff, const void *data, uint32_t itemCount);

uint32_t RingBuffer_pushOne(RingBuffer *rbuff, const void *data);

uint32_t RingBuffer_pop(RingBuffer *rbuff, void *dataBuffer, uint32_t itemCount);

uint32_t RingBuffer_popOne(RingBuffer *rbuff, void *dataBuffer);

uint32_t RingBuffer_peek(RingBuffer *rbuff, void *dataBuffer);

void RingBuffer_flush(RingBuffer *rbuff);

//void RingBuffer_destroy(RingBuffer *buffer);

#ifdef __cplusplus
}
#endif

#endif /* RINGBUFFER_H_ */
