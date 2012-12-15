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
#include <liberror.h>
#include "libbuffer.h"

DLLEXPORT(BufferStatus) bufAppendFromBinaryFile(
	struct Buffer *self, const char *fileName, const char **error)
{
	BufferStatus returnCode = BUF_SUCCESS;
	BufferStatus bStatus;
	uint32 length;
	uint32 actualLength;
	long ftellResult;
	const uint32 currentLength = self->length;
	FILE *file = fopen(fileName, "rb");
	if ( !file ) {
		errRenderStd(error);
		errPrefix(error, "bufAppendFromBinaryFile()");
		FAIL(BUF_FOPEN);
	}
	if ( fseek(file, 0, SEEK_END) ) {
		errRenderStd(error);
		errPrefix(error, "bufAppendFromBinaryFile()");
		FAIL(BUF_FSEEK);
	}
	ftellResult = ftell(file);
	if ( ftellResult < 0 ) {
		errRenderStd(error);
		errPrefix(error, "bufAppendFromBinaryFile()");
		FAIL(BUF_FTELL);
	}
	length = (size_t)ftellResult;
	bStatus = bufAppendConst(self, 0x00, length, error);
	CHECK_STATUS(bStatus, "bufAppendFromBinaryFile()", bStatus);
	rewind(file);
	actualLength = (uint32)fread(self->data + currentLength, 1, length, file);
	if ( actualLength != length ) {
		fclose(file);
		if ( feof(file) ) {
			errRender(error, "bufAppendFromBinaryFile(): Unexpectedly hit EOF after reading %lu bytes!\n", actualLength);
			FAIL(BUF_FEOF);
		} else if ( ferror(file) ) {
			errRenderStd(error);
			errPrefix(error, "bufAppendFromBinaryFile()");
			FAIL(BUF_FERROR);
		}
	}
cleanup:
	if ( file ) {
		fclose(file);
	}
	return returnCode;
}

DLLEXPORT(BufferStatus) bufWriteBinaryFile(
	const struct Buffer *self, const char *fileName, uint32 bufAddress, uint32 count,
	const char **error)
{
	BufferStatus returnCode = BUF_SUCCESS;
	uint32 actualLength;
	FILE *file = fopen(fileName, "wb");
	if ( !file ) {
		errRenderStd(error);
		errPrefix(error, "bufWriteBinaryFile()");
		FAIL(BUF_FOPEN);
	}
	actualLength = (uint32)fwrite(self->data + bufAddress, 1, count, file);
	if ( actualLength != count ) {
		errRenderStd(error);
		errPrefix(error, "bufWriteBinaryFile()");
		FAIL(BUF_FERROR);
	}
cleanup:
	if ( file ) {
		fclose(file);	
	}
	return returnCode;
}
