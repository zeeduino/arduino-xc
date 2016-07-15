/*
  ringbuffer.c - ring buffer implementation for arbitrary data
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

#include <string.h>

#include "ringbuffer.h"

//=============================================================================
uint32_t RingBuffer_capacity(RingBuffer *rbuff)
{
	return rbuff->length;
}

// We need this one since we don't keep track of tail
uint32_t RingBuffer_tail(RingBuffer *rbuff)
{
	uint32_t tail;

	tail = rbuff->head + rbuff->itemCount;
	// wrap around
	if(tail >= rbuff->length)
		tail = tail - rbuff->length;
	return tail;
}
// This one is just for consistency sake
uint32_t RingBuffer_head(RingBuffer *rbuff)
{
	return rbuff->head;
}

void* RingBuffer_headPtr(RingBuffer *rbuff)
{
	uint8_t *ptr = rbuff->buffer;

	ptr += rbuff->head * rbuff->itemSize;

	return (void*)ptr;
}

void* RingBuffer_tailPtr(RingBuffer *rbuff)
{
	uint8_t *ptr = rbuff->buffer;
	uint32_t tail;

	tail = RingBuffer_tail(rbuff);
	ptr += tail * rbuff->itemSize;

	return (void*)ptr;
}

void RingBuffer_headAdvance(RingBuffer *rbuff, uint32_t count)
{
	rbuff->head += count;
	// wrap around
	if(rbuff->head >= rbuff->length)
		rbuff->head -= rbuff->length;

	rbuff->itemCount -= count;
}

void RingBuffer_headAdvanceOne(RingBuffer *rbuff)
{
	rbuff->head++;
	// wrap around
	if(rbuff->head == rbuff->length)
		rbuff->head = 0;

	rbuff->itemCount--;
}

void RingBuffer_tailAdvance(RingBuffer *rbuff, uint32_t count)
{
	rbuff->itemCount += count;
}

void RingBuffer_tailAdvanceOne(RingBuffer *rbuff)
{
	rbuff->itemCount++;
}

void* RingBuffer_pushUnchecked(RingBuffer *rbuff, const void *data,
		uint32_t itemCount)
{
	void *ptr;

	// get tail position
	ptr = RingBuffer_tailPtr(rbuff);

	// save data where tail is
	memcpy(ptr, data, rbuff->itemSize * itemCount);

	// move tail forward
	RingBuffer_tailAdvance(rbuff, itemCount);

	ptr = (void*)((uint8_t*)data + rbuff->itemSize * itemCount);

	return ptr;
}

void* RingBuffer_popUnchecked(RingBuffer *rbuff, void *data,
		uint32_t itemCount)
{
	void *ptr;

	// get head position
	ptr = RingBuffer_headPtr(rbuff);

	// save data
	memcpy(data, ptr, rbuff->itemSize * itemCount);

	// move head forward
	RingBuffer_headAdvance(rbuff, itemCount);

	ptr = (void*)((uint8_t*)data + rbuff->itemSize * itemCount);

	return ptr;
}

uint32_t RingBuffer_spaceBetweenTailAndEnd(RingBuffer *rbuff)
{
	return RingBuffer_capacity(rbuff) - RingBuffer_tail(rbuff);
}

//=============================================================================
void RingBuffer_init(RingBuffer *rbuff, void *buffer, uint32_t count,
		uint32_t itemSize)
{
	rbuff->buffer = buffer;
	rbuff->length = count;
	rbuff->itemCount = 0;
	rbuff->itemSize = itemSize;
	rbuff->head = 0;
}

bool RingBuffer_isEmpty(RingBuffer *rbuff)
{
	return RingBuffer_availableData(rbuff) == 0;
}

bool RingBuffer_isFull(RingBuffer *rbuff)
{
	return RingBuffer_availableData(rbuff) == RingBuffer_capacity(rbuff);
}

uint32_t RingBuffer_availableData(RingBuffer *rbuff)
{
	return rbuff->itemCount;
}

uint32_t RingBuffer_availableSpace(RingBuffer *rbuff)
{
	return RingBuffer_capacity(rbuff) - RingBuffer_availableData(rbuff);
}

uint32_t RingBuffer_push(RingBuffer *rbuff, const void *data,
		uint32_t itemCount)
{
	uint32_t spaceBetweenTailAndEnd;
	uint32_t chunkACount;
	uint32_t chunkBCount;

	if(RingBuffer_isFull(rbuff))
		return 0;

	// This makes sure we don't overrun the head (and thus the buffer)
	if(itemCount > RingBuffer_availableSpace(rbuff))
		itemCount = RingBuffer_availableSpace(rbuff);

	spaceBetweenTailAndEnd = RingBuffer_capacity(rbuff) - RingBuffer_tail(rbuff);

	if(itemCount < spaceBetweenTailAndEnd)
	{
		RingBuffer_pushUnchecked(rbuff, data, itemCount);
	}
	else
	{
		chunkACount = spaceBetweenTailAndEnd;
		chunkBCount = itemCount - chunkACount;

		data = RingBuffer_pushUnchecked(rbuff, data, chunkACount);
		RingBuffer_pushUnchecked(rbuff, data, chunkBCount);
	}

	return itemCount;
}

uint32_t RingBuffer_pushOne(RingBuffer *rbuff, const void *data)
{
	void *ptr;

	if(RingBuffer_isFull(rbuff))
		return 0;

	// get tail position
	ptr = RingBuffer_tailPtr(rbuff);

	// save data where tail is
	memcpy(ptr, data, rbuff->itemSize);

	// move tail forward
	RingBuffer_tailAdvanceOne(rbuff);

	return 1;
}

uint32_t RingBuffer_pop(RingBuffer *rbuff, void *dataBuffer,
		uint32_t itemCount)
{
	uint32_t spaceBetweenHeadAndEnd;
	uint32_t chunkACount;
	uint32_t chunkBCount;

	if(RingBuffer_isEmpty(rbuff))
		return 0;

	// this makes sure we don't overrun the tail (and thus the buffer)
	if(itemCount > RingBuffer_availableData(rbuff))
		itemCount = RingBuffer_availableData(rbuff);

	if(RingBuffer_head(rbuff) < RingBuffer_tail(rbuff))
	{
		RingBuffer_popUnchecked(rbuff, dataBuffer, itemCount);
	}
	else
	{
		spaceBetweenHeadAndEnd = RingBuffer_capacity(rbuff) - RingBuffer_head(rbuff);
		chunkACount = spaceBetweenHeadAndEnd;
		if(itemCount < chunkACount)
		{
			dataBuffer = RingBuffer_popUnchecked(rbuff, dataBuffer, itemCount);
		}
		else
		{
			chunkBCount = itemCount - chunkACount;

			dataBuffer = RingBuffer_popUnchecked(rbuff, dataBuffer, chunkACount);
			RingBuffer_popUnchecked(rbuff, dataBuffer, chunkBCount);
		}
	}

	return itemCount;
}

uint32_t RingBuffer_popOne(RingBuffer *rbuff, void *dataBuffer)
{
	void *ptr;

	if(RingBuffer_isEmpty(rbuff))
		return 0;

	// get head position
	ptr = RingBuffer_headPtr(rbuff);

	// get data from where head is
	memcpy(dataBuffer, ptr, rbuff->itemSize);

	// move head forward
	RingBuffer_headAdvanceOne(rbuff);

	return 1;
}

uint32_t RingBuffer_peek(RingBuffer *rbuff, void *dataBuffer)
{
    void *ptr;

    if(RingBuffer_isEmpty(rbuff))
        return 0;

    // get head position
    ptr = RingBuffer_headPtr(rbuff);

    // get data from where head is
    memcpy(dataBuffer, ptr, rbuff->itemSize);

    return 1;

}

void RingBuffer_flush(RingBuffer *rbuff)
{
	rbuff->itemCount = 0;
	rbuff->head = 0;
}
