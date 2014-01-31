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
	BufferStatus retVal = BUF_SUCCESS;
	BufferStatus bStatus;
	size_t length;
	size_t actualLength;
	long ftellResult;
	const size_t currentLength = self->length;
	FILE *file = fopen(fileName, "rb");
	if ( !file ) {
		errRenderStd(error);
		errPrefix(error, "bufAppendFromBinaryFile()");
		FAIL(BUF_FOPEN, cleanup);
	}
	if ( fseek(file, 0, SEEK_END) ) {
		errRenderStd(error);
		errPrefix(error, "bufAppendFromBinaryFile()");
		FAIL(BUF_FSEEK, cleanup);
	}
	ftellResult = ftell(file);
	if ( ftellResult < 0 ) {
		errRenderStd(error);
		errPrefix(error, "bufAppendFromBinaryFile()");
		FAIL(BUF_FTELL, cleanup);
	}
	length = (size_t)ftellResult;
	bStatus = bufAppendConst(self, 0x00, length, error);
	CHECK_STATUS(bStatus, bStatus, cleanup, "bufAppendFromBinaryFile()");
	rewind(file);
	actualLength = fread(self->data + currentLength, 1, length, file);
	if ( actualLength != length ) {
		CHECK_STATUS(
			feof(file), BUF_FEOF, cleanup,
			"bufAppendFromBinaryFile(): Unexpectedly hit EOF after reading %lu bytes!", actualLength);
		if ( ferror(file) ) {
			errRenderStd(error);
			errPrefix(error, "bufAppendFromBinaryFile()");
			FAIL(BUF_FERROR, cleanup);
		}
	}
cleanup:
	if ( file ) {
		fclose(file);
	}
	return retVal;
}

DLLEXPORT(BufferStatus) bufWriteBinaryFile(
	const struct Buffer *self, const char *fileName, size_t bufAddress, size_t count,
	const char **error)
{
	BufferStatus retVal = BUF_SUCCESS;
	size_t actualLength;
	FILE *file = fopen(fileName, "wb");
	if ( !file ) {
		errRenderStd(error);
		errPrefix(error, "bufWriteBinaryFile()");
		FAIL(BUF_FOPEN, cleanup);
	}
	actualLength = fwrite(self->data + bufAddress, 1, count, file);
	if ( actualLength != count ) {
		errRenderStd(error);
		errPrefix(error, "bufWriteBinaryFile()");
		FAIL(BUF_FERROR, cleanup);
	}
cleanup:
	if ( file ) {
		fclose(file);	
	}
	return retVal;
}
