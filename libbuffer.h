/* 
 * Copyright (C) 2009-2010 Chris McClelland
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBUFFER_H
#define LIBBUFFER_H

#include <stddef.h>
#include <makestuff.h>

#ifdef __cplusplus
extern "C" {
#endif
	struct Buffer {
		uint8 *data;
		uint32 length;
		uint32 capacity;
		uint8 fill;
	};
	
	typedef enum {
		BUF_SUCCESS = 0,
		BUF_NO_MEM,
		BUF_FOPEN,
		BUF_FSEEK,
		BUF_FEOF,
		BUF_FERROR,
		HEX_EMPTY_FILE,
		HEX_JUNK_START_CODE,
		HEX_JUNK_BYTE_COUNT,
		HEX_JUNK_ADDR_MSB,
		HEX_JUNK_ADDR_LSB,
		HEX_JUNK_REC_TYPE,
		HEX_JUNK_DATA_BYTE,
		HEX_JUNK_CHECKSUM,
		HEX_BAD_CHECKSUM,
		HEX_CORRUPT_LINE,
		HEX_MISSING_EOF,
		HEX_BAD_REC_TYPE,
		HEX_BAD_EXT_SEG
	} BufferStatus;
	
	// Defined in core.c:
	DLLEXPORT(void) bufDestroy(
		struct Buffer *self
	);
	DLLEXPORT(void) bufZeroLength(
		struct Buffer *self
	);
	DLLEXPORT(BufferStatus) bufInitialise(
		struct Buffer *self, uint32 initialSize, uint8 fill, const char **error
	) WARN_UNUSED_RESULT;
	DLLEXPORT(BufferStatus) bufAppendBlock(
		struct Buffer *self, const uint8 *ptr, uint32 count, const char **error
	) WARN_UNUSED_RESULT;
	DLLEXPORT(BufferStatus) bufAppendByte(
		struct Buffer *self, uint8 byte, const char **error
	) WARN_UNUSED_RESULT;
	DLLEXPORT(BufferStatus) bufAppendZeros(
		struct Buffer *self, uint32 count, uint8 **ptr, const char **error
	) WARN_UNUSED_RESULT;
	DLLEXPORT(BufferStatus) bufAppendConst(
		struct Buffer *self, uint32 count, uint8 value, uint8 **ptr, const char **error
	) WARN_UNUSED_RESULT;
	DLLEXPORT(BufferStatus) bufCopyBlock(
		struct Buffer *self, uint32 bufAddress, const uint8 *ptr, uint32 count, const char **error
	) WARN_UNUSED_RESULT;
	DLLEXPORT(BufferStatus) bufSetBlock(
		struct Buffer *self, uint32 bufAddress, uint8 value, uint32 count, const char **error
	) WARN_UNUSED_RESULT;

	// Defined in binio.c:
	DLLEXPORT(BufferStatus) bufAppendFromBinaryFile(
		struct Buffer *self, const char *fileName, const char **error
	) WARN_UNUSED_RESULT;
	DLLEXPORT(BufferStatus) bufWriteBinaryFile(
		const struct Buffer *self, const char *fileName, uint32 bufAddress, uint32 count,
		const char **error
	) WARN_UNUSED_RESULT;

	// Defined in hexio.c:
	DLLEXPORT(BufferStatus) bufReadFromIntelHexFile(
		struct Buffer *destData, struct Buffer *destMask, const char *fileName, const char **error
	) WARN_UNUSED_RESULT;
	DLLEXPORT(BufferStatus) bufWriteToIntelHexFile(
		const struct Buffer *sourceData, const struct Buffer *sourceMask,
		const char *fileName, uint8 lineLength, bool compress, const char **error
	);

#ifdef __cplusplus
}
#endif

#endif
