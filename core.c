/*
 * Copyright (C) 2009-2012 Chris McClelland
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <liberror.h>
#include "libbuffer.h"

// Initialise the promRecords structure.
// Returns BUF_SUCCESS or BUF_NO_MEM.
//
DLLEXPORT(BufferStatus) bufInitialise(
	struct Buffer *self, uint32 initialSize, uint8 fill, const char **error)
{
	uint8 *ptr;
	const uint8 *endPtr;
	self->fill = fill;
	self->data = (uint8 *)malloc(initialSize);
	if ( !self->data ) {
		errRender(error, "Cannot allocate memory for buffer");
		return BUF_NO_MEM;
	}
	ptr = self->data;
	endPtr = ptr + initialSize;
	while ( ptr < endPtr ) {
		*ptr++ = self->fill;
	}
	self->capacity = initialSize;
	self->length = 0;
	return BUF_SUCCESS;
}

// Free up any memory associated with the buffer structure.
//
DLLEXPORT(void) bufDestroy(struct Buffer *self) {
	free(self->data);
	self->data = NULL;
	self->capacity = 0;
	self->length = 0;
	self->fill = 0;
}

// Either deep copy into an already-constructed buffer, or copy-construct into an uninitialised
// buffer.
//
DLLEXPORT(BufferStatus) bufDeepCopy(
	struct Buffer *dst, const struct Buffer *src, const char **error)
{
	uint8 *ptr;
	const uint8 *endPtr;
	if ( dst->data && dst->capacity < src->capacity ) {
		// The dst has been initialised, but there is not enough room for the copy.
		bufDestroy(dst);
	}
	if ( !dst->data ) {
		// The dst needs to be allocated.
		dst->capacity = src->capacity;
		dst->data = (uint8 *)malloc(dst->capacity);
		if ( !dst->data ) {
			errRender(error, "Cannot allocate memory for buffer");
			return BUF_NO_MEM;
		}
	}
	dst->length = src->length;
	dst->fill = src->fill;
	memcpy(dst->data, src->data, dst->length);
	ptr = dst->data + dst->length;
	endPtr = dst->data + dst->capacity;
	while ( ptr < endPtr ) {
		*ptr++ = dst->fill;
	}
	return BUF_SUCCESS;
}

// Clean the buffer structure so it can be reused.
//
DLLEXPORT(void) bufZeroLength(struct Buffer *self) {
	uint32 i;
	self->length = 0;
	for ( i = 0; i < self->capacity; i++ ) {
		self->data[i] = self->fill;
	}
}

// Reallocate the memory for the buffer by doubling the capacity and zeroing the extra storage.
//
static BufferStatus reallocate(
	struct Buffer *self, uint32 newCapacity, uint32 blockEnd, const char **error)
{
	// The data will not fit in the buffer - we need to make the buffer bigger
	//
	uint8 *ptr;
	const uint8 *endPtr;
	do {
		newCapacity *= 2;
	} while ( blockEnd > newCapacity );
	self->data = (uint8 *)realloc(self->data, newCapacity);
	if ( !self->data ) {
		errRender(error, "Cannot reallocate memory for buffer");
		return BUF_NO_MEM;
	}
	self->capacity = newCapacity;
	
	// Now zero from the end of the block to the end of the new capacity
	//
	ptr = self->data + blockEnd;
	endPtr = self->data + newCapacity;
	while ( ptr < endPtr ) {
		*ptr++ = self->fill;
	}
	return BUF_SUCCESS;
}

// Write the supplied data to the buffer structure.
// Returns BUF_SUCCESS or BUF_NO_MEM.
//
DLLEXPORT(BufferStatus) bufAppendBlock(
	struct Buffer *self, const uint8 *srcPtr, uint32 count, const char **error)
{
	const uint32 blockEnd = self->length + count;
	if ( blockEnd > self->capacity ) {
		// The data will not fit in the buffer - we need to make the buffer bigger
		//
		BufferStatus status = reallocate(self, self->capacity, blockEnd, error);
		if ( status != BUF_SUCCESS ) {
			return status;
		}
	}
	memcpy(self->data + self->length, srcPtr, count);
	self->length = blockEnd;
	return BUF_SUCCESS;
}

DLLEXPORT(BufferStatus) bufAppendByte(struct Buffer *self, uint8 byte, const char **error) {
	const uint32 blockEnd = self->length + 1;
	if ( blockEnd > self->capacity ) {
		// The data will not fit in the buffer - we need to make the buffer bigger
		//
		BufferStatus status = reallocate(self, self->capacity, blockEnd, error);
		if ( status != BUF_SUCCESS ) {
			return status;
		}
	}
	*(self->data + self->length) = byte;
	self->length++;
	return BUF_SUCCESS;
}

// Append some zeros to the end of the buffer, and return a ptr to the next free byte after the end.
//
DLLEXPORT(BufferStatus) bufAppendZeros(
	struct Buffer *self, uint32 count, uint8 **ptr, const char **error)
{
	const uint32 blockEnd = self->length + count;
	if ( blockEnd > self->capacity ) {
		// The data will not fit in the buffer - we need to make the buffer bigger
		//
		BufferStatus status = reallocate(self, self->capacity, blockEnd, error);
		if ( status != BUF_SUCCESS ) {
			return status;
		}
	}
	memset(self->data + self->length, 0x00, count);
	if ( ptr ) {
		*ptr = self->data + self->length;
	}
	self->length = blockEnd;
	return BUF_SUCCESS;
}

// Append a block of a given constant to the end of the buffer, and return a ptr to the next free
// byte after the end.
//
DLLEXPORT(BufferStatus) bufAppendConst(
	struct Buffer *self, uint32 count, uint8 value, uint8 **ptr, const char **error)
{
	const uint32 blockEnd = self->length + count;
	if ( blockEnd > self->capacity ) {
		// The data will not fit in the buffer - we need to make the buffer bigger
		//
		BufferStatus status = reallocate(self, self->capacity, blockEnd, error);
		if ( status != BUF_SUCCESS ) {
			return status;
		}
	}
	memset(self->data + self->length, value, count);
	if ( ptr ) {
		*ptr = self->data + self->length;
	}
	self->length = blockEnd;
	return BUF_SUCCESS;
}

// Used by bufCopyBlock() and bufSetBlock() to ensure sufficient capacity for the operation.
//
static BufferStatus maybeReallocate(
	struct Buffer *const self, const uint32 bufAddress, const uint32 count, const char **error)
{
	// There are three possibilities:
	//   * The block to be written starts after the end of the current buffer
	//   * The block to be written starts within the current buffer, but ends beyond it
	//   * The block to be written ends within the current buffer
	//
	const uint32 blockEnd = bufAddress + count;
	if ( bufAddress >= self->length ) {
		// Begins outside - reallocation may be necessary, zeroing definitely necessary
		//
		uint8 *ptr, *endPtr;
		if ( blockEnd > self->capacity ) {
			// The data will not fit in the buffer - we need to make the buffer bigger
			//
			BufferStatus status = reallocate(self, self->capacity, blockEnd, error);
			if ( status != BUF_SUCCESS ) {
				return status;
			}
		}
		
		// Now fill from the end of the old length to the start of the block
		//
		ptr = self->data + self->length;
		endPtr = self->data + bufAddress;
		while ( ptr < endPtr ) {
			*ptr++ = self->fill;
		}
		
		self->length = blockEnd;
	} else if ( bufAddress < self->length && blockEnd > self->length ) {
		// Begins inside, ends outside - reallocation and zeroing may be necessary
		//
		if ( blockEnd > self->capacity ) {
			// The data will not fit in the buffer - we need to make the buffer bigger
			//
			BufferStatus status = reallocate(self, self->capacity, blockEnd, error);
			if ( status != BUF_SUCCESS ) {
				return status;
			}
		}
		
		self->length = blockEnd;
	}
	return BUF_SUCCESS;
}

// Copy a bunch of bytes from a source pointer into the buffer. The target address may be outside
// the current extent (or even capacity) of the target buffer.
//
DLLEXPORT(BufferStatus) bufCopyBlock(
	struct Buffer *self, uint32 bufAddress, const uint8 *srcPtr, uint32 count, const char **error)
{
	if ( !count ) {
		return BUF_SUCCESS;
	} else {
		maybeReallocate(self, bufAddress, count, error);
		memcpy(self->data + bufAddress, srcPtr, count);
		return BUF_SUCCESS;
	}
}

// Set a range of bytes of the target buffer to a given value. The target address may be outside
// the current extent (or even capacity) of the target buffer.
//
DLLEXPORT(BufferStatus) bufSetBlock(
	struct Buffer *self, uint32 bufAddress, uint8 value, uint32 count, const char **error)
{
	if ( !count ) {
		return BUF_SUCCESS;
	} else {
		maybeReallocate(self, bufAddress, count, error);
		memset(self->data + bufAddress, value, count);
		return BUF_SUCCESS;
	}
}
