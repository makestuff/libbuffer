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
	struct Buffer *self, size_t initialSize, uint8 fill, const char **error)
{
	BufferStatus retVal = BUF_SUCCESS;
	uint8 *ptr;
	const uint8 *endPtr;
	self->fill = fill;
	self->data = (uint8 *)malloc(initialSize);
	CHECK_STATUS(
		!self->data, BUF_NO_MEM, cleanup,
		"bufInitialise(): Cannot allocate memory for buffer");
	ptr = self->data;
	endPtr = ptr + initialSize;
	while ( ptr < endPtr ) {
		*ptr++ = self->fill;
	}
	self->capacity = initialSize;
	self->length = 0;
cleanup:
	return retVal;
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
	BufferStatus retVal = BUF_SUCCESS;
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
		CHECK_STATUS(
			!dst->data, BUF_NO_MEM, cleanup,
			"bufDeepCopy(): Cannot allocate memory for buffer");
	}
	dst->length = src->length;
	dst->fill = src->fill;
	memcpy(dst->data, src->data, dst->length);
	ptr = dst->data + dst->length;
	endPtr = dst->data + dst->capacity;
	while ( ptr < endPtr ) {
		*ptr++ = dst->fill;
	}
cleanup:
	return retVal;
}

// Swap the actual byte[] owned by each Buffer
//
DLLEXPORT(void) bufSwap(
	struct Buffer *x, struct Buffer *y)
{
	uint8 *const tmpData = x->data;
	const size_t tmpLength = x->length;
	const size_t tmpCapacity = x->capacity;
	const uint8 tmpFill = x->fill;

	x->data = y->data;
	x->length = y->length;
	x->capacity = y->capacity;
	x->fill = y->fill;

	y->data = tmpData;
	y->length = tmpLength;
	y->capacity = tmpCapacity;
	y->fill = tmpFill;
}

// Clean the buffer structure so it can be reused.
//
DLLEXPORT(void) bufZeroLength(struct Buffer *self) {
	size_t i;
	self->length = 0;
	for ( i = 0; i < self->capacity; i++ ) {
		self->data[i] = self->fill;
	}
}

// Reallocate the memory for the buffer by doubling the capacity and zeroing the extra storage.
//
static BufferStatus reallocate(
	struct Buffer *self, size_t newCapacity, size_t blockEnd, const char **error)
{
	BufferStatus retVal = BUF_SUCCESS;
	uint8 *ptr;
	const uint8 *endPtr;
	do {
		newCapacity *= 2;
	} while ( blockEnd > newCapacity );
	ptr = (uint8 *)realloc(self->data, newCapacity);
	CHECK_STATUS(!ptr, BUF_NO_MEM, cleanup, "Cannot reallocate memory for buffer");
	self->data = ptr;
	self->capacity = newCapacity;
	
	// Now zero from the end of the block to the end of the new capacity
	//
	ptr = self->data + blockEnd;
	endPtr = self->data + newCapacity;
	while ( ptr < endPtr ) {
		*ptr++ = self->fill;
	}
cleanup:
	return retVal;
}

// If the data will not fit in the buffer, make the buffer bigger
//
#define ENSURE_CAPACITY(prefix) \
	if ( blockEnd > self->capacity ) { \
		BufferStatus status = reallocate(self, self->capacity, blockEnd, error); \
		CHECK_STATUS(status, status, cleanup, prefix); \
	}

DLLEXPORT(BufferStatus) bufAppendByte(struct Buffer *self, uint8 byte, const char **error) {
	BufferStatus retVal = BUF_SUCCESS;
	const size_t blockEnd = self->length + 1;
	ENSURE_CAPACITY("bufAppendByte()");
	*(self->data + self->length) = byte;
	self->length++;
cleanup:
	return retVal;
}

DLLEXPORT(BufferStatus) bufAppendWordLE(struct Buffer *self, uint16 word, const char **error) {
	BufferStatus retVal = BUF_SUCCESS;
	const size_t blockEnd = self->length + 2;
	union {
		uint16 word;
		uint8 byte[2];
	} u;
	u.word = word;
	ENSURE_CAPACITY("bufAppendWordLE()");
	#if BYTE_ORDER == 1234
		*(self->data + self->length) = u.byte[0];
		*(self->data + self->length + 1) = u.byte[1];
	#else
		*(self->data + self->length) = u.byte[1];
		*(self->data + self->length + 1) = u.byte[0];
	#endif
	self->length += 2;
cleanup:
	return retVal;
}

DLLEXPORT(BufferStatus) bufAppendWordBE(struct Buffer *self, uint16 word, const char **error) {
	BufferStatus retVal = BUF_SUCCESS;
	const size_t blockEnd = self->length + 2;
	union {
		uint16 word;
		uint8 byte[2];
	} u;
	u.word = word;
	ENSURE_CAPACITY("bufAppendWordBE()");
	#if BYTE_ORDER == 1234
		*(self->data + self->length) = u.byte[1];
		*(self->data + self->length + 1) = u.byte[0];
	#else
		*(self->data + self->length) = u.byte[0];
		*(self->data + self->length + 1) = u.byte[1];
	#endif
	self->length += 2;
cleanup:
	return retVal;
}

DLLEXPORT(BufferStatus) bufAppendLongLE(struct Buffer *self, uint32 lword, const char **error) {
	BufferStatus retVal = BUF_SUCCESS;
	const size_t blockEnd = self->length + 4;
	union {
		uint32 lword;
		uint8 byte[4];
	} u;
	u.lword = lword;
	ENSURE_CAPACITY("bufAppendLongLE()");
	#if BYTE_ORDER == 1234
		*(self->data + self->length) = u.byte[0];
		*(self->data + self->length + 1) = u.byte[1];
		*(self->data + self->length + 2) = u.byte[2];
		*(self->data + self->length + 3) = u.byte[3];
	#else
		*(self->data + self->length) = u.byte[3];
		*(self->data + self->length + 1) = u.byte[2];
		*(self->data + self->length + 2) = u.byte[1];
		*(self->data + self->length + 3) = u.byte[0];
	#endif
	self->length += 4;
cleanup:
	return retVal;
}

DLLEXPORT(BufferStatus) bufAppendLongBE(struct Buffer *self, uint32 lword, const char **error) {
	BufferStatus retVal = BUF_SUCCESS;
	const size_t blockEnd = self->length + 4;
	union {
		uint32 lword;
		uint8 byte[4];
	} u;
	u.lword = lword;
	ENSURE_CAPACITY("bufAppendLongBE()");
	#if BYTE_ORDER == 1234
		*(self->data + self->length) = u.byte[3];
		*(self->data + self->length + 1) = u.byte[2];
		*(self->data + self->length + 2) = u.byte[1];
		*(self->data + self->length + 3) = u.byte[0];
	#else
		*(self->data + self->length) = u.byte[0];
		*(self->data + self->length + 1) = u.byte[1];
		*(self->data + self->length + 2) = u.byte[2];
		*(self->data + self->length + 3) = u.byte[3];
	#endif
	self->length += 4;
cleanup:
	return retVal;
}

// Append a block of a given constant to the end of the buffer, and return a ptr to the next free
// byte after the end.
//
DLLEXPORT(BufferStatus) bufAppendConst(
	struct Buffer *self, uint8 value, size_t count, const char **error)
{
	BufferStatus retVal = BUF_SUCCESS;
	const size_t blockEnd = self->length + count;
	ENSURE_CAPACITY("bufAppendConst()");
	memset(self->data + self->length, value, count);
	self->length = blockEnd;
cleanup:
	return retVal;
}

// Write the supplied data to the buffer structure.
// Returns BUF_SUCCESS or BUF_NO_MEM.
//
DLLEXPORT(BufferStatus) bufAppendBlock(
	struct Buffer *self, const uint8 *srcPtr, size_t count, const char **error)
{
	BufferStatus retVal = BUF_SUCCESS;
	const size_t blockEnd = self->length + count;
	ENSURE_CAPACITY("bufAppendBlock()");
	memcpy(self->data + self->length, srcPtr, count);
	self->length = blockEnd;
cleanup:
	return retVal;
}

// Used by bufWriteXXX() to ensure sufficient capacity for the operation.
//
static BufferStatus maybeReallocate(
	struct Buffer *const self, const size_t bufAddress, const size_t count, const char **error)
{
	// There are three possibilities:
	//   * The block to be written starts after the end of the current buffer
	//   * The block to be written starts within the current buffer, but ends beyond it
	//   * The block to be written ends within the current buffer
	//
	BufferStatus retVal = BUF_SUCCESS;
	const size_t blockEnd = bufAddress + count;
	if ( bufAddress >= self->length ) {
		// Begins outside - reallocation may be necessary, zeroing definitely necessary
		//
		uint8 *ptr, *endPtr;
		ENSURE_CAPACITY("maybeReallocate()");
		
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
		ENSURE_CAPACITY("maybeReallocate()");
		self->length = blockEnd;
	}
cleanup:
	return retVal;
}

// Write a single byte into the target buffer. The target offset may be outside the current extent
// (or even capacity) of the target buffer.
//
DLLEXPORT(BufferStatus) bufWriteByte(
	struct Buffer *self, size_t offset, uint8 byte, const char **error)
{
	BufferStatus retVal = maybeReallocate(self, offset, 1, error);
	CHECK_STATUS(retVal, retVal, cleanup, "bufWriteByte()");
	self->data[offset] = byte;
cleanup:
	return retVal;
}

// Write a uint16 into the target buffer in little-endian format. The target offset may be outside
// the current extent (or even capacity) of the target buffer.
//
DLLEXPORT(BufferStatus) bufWriteWordLE(
	struct Buffer *self, size_t offset, uint16 word, const char **error)
{
	BufferStatus retVal;
	union {
		uint16 word;
		uint8 byte[2];
	} u;
	u.word = word;
	retVal = maybeReallocate(self, offset, 2, error);
	CHECK_STATUS(retVal, retVal, cleanup, "bufWriteWordLE()");
	#if BYTE_ORDER == 1234
		*(self->data + offset) = u.byte[0];
		*(self->data + offset + 1) = u.byte[1];
	#else
		*(self->data + offset) = u.byte[1];
		*(self->data + offset + 1) = u.byte[0];
	#endif
cleanup:
	return retVal;
}

// Write a uint16 into the target buffer in big-endian format. The target offset may be outside
// the current extent (or even capacity) of the target buffer.
//
DLLEXPORT(BufferStatus) bufWriteWordBE(
	struct Buffer *self, size_t offset, uint16 word, const char **error)
{
	BufferStatus retVal;
	union {
		uint16 word;
		uint8 byte[2];
	} u;
	u.word = word;
	retVal = maybeReallocate(self, offset, 2, error);
	CHECK_STATUS(retVal, retVal, cleanup, "bufWriteWordBE()");
	#if BYTE_ORDER == 1234
		*(self->data + offset) = u.byte[1];
		*(self->data + offset + 1) = u.byte[0];
	#else
		*(self->data + offset) = u.byte[0];
		*(self->data + offset + 1) = u.byte[1];
	#endif
cleanup:
	return retVal;
}

// Write a uint16 into the target buffer in little-endian format. The target offset may be outside
// the current extent (or even capacity) of the target buffer.
//
DLLEXPORT(BufferStatus) bufWriteLongLE(
	struct Buffer *self, size_t offset, uint32 lword, const char **error)
{
	BufferStatus retVal;
	union {
		uint32 lword;
		uint8 byte[4];
	} u;
	u.lword = lword;
	retVal = maybeReallocate(self, offset, 4, error);
	CHECK_STATUS(retVal, retVal, cleanup, "bufWriteLongLE()");
	#if BYTE_ORDER == 1234
		*(self->data + offset) = u.byte[0];
		*(self->data + offset + 1) = u.byte[1];
		*(self->data + offset + 2) = u.byte[2];
		*(self->data + offset + 3) = u.byte[3];
	#else
		*(self->data + offset) = u.byte[3];
		*(self->data + offset + 1) = u.byte[2];
		*(self->data + offset + 2) = u.byte[1];
		*(self->data + offset + 3) = u.byte[0];
	#endif
cleanup:
	return retVal;
}

// Write a uint16 into the target buffer in little-endian format. The target offset may be outside
// the current extent (or even capacity) of the target buffer.
//
DLLEXPORT(BufferStatus) bufWriteLongBE(
	struct Buffer *self, size_t offset, uint32 lword, const char **error)
{
	BufferStatus retVal;
	union {
		uint32 lword;
		uint8 byte[4];
	} u;
	u.lword = lword;
	retVal = maybeReallocate(self, offset, 4, error);
	CHECK_STATUS(retVal, retVal, cleanup, "bufWriteLongBE()");
	#if BYTE_ORDER == 1234
		*(self->data + offset) = u.byte[3];
		*(self->data + offset + 1) = u.byte[2];
		*(self->data + offset + 2) = u.byte[1];
		*(self->data + offset + 3) = u.byte[0];
	#else
		*(self->data + offset) = u.byte[0];
		*(self->data + offset + 1) = u.byte[1];
		*(self->data + offset + 2) = u.byte[2];
		*(self->data + offset + 3) = u.byte[3];
	#endif
cleanup:
	return retVal;
}

// Set a range of bytes of the target buffer to a given value. The target offset may be outside the
// current extent (or even capacity) of the target buffer.
//
DLLEXPORT(BufferStatus) bufWriteConst(
	struct Buffer *self, size_t offset, uint8 value, size_t count, const char **error)
{
	BufferStatus retVal = maybeReallocate(self, offset, count, error);
	CHECK_STATUS(retVal, retVal, cleanup, "bufWriteConst()");
	memset(self->data + offset, value, count);
cleanup:
	return retVal;
}

// Copy a bunch of bytes from a source pointer into the buffer. The target address may be outside
// the current extent (or even capacity) of the target buffer.
//
DLLEXPORT(BufferStatus) bufWriteBlock(
	struct Buffer *self, size_t offset, const uint8 *ptr, size_t count, const char **error)
{
	BufferStatus retVal = maybeReallocate(self, offset, count, error);
	CHECK_STATUS(retVal, retVal, cleanup, "bufWriteConst()");
	memcpy(self->data + offset, ptr, count);
cleanup:
	return retVal;
}
