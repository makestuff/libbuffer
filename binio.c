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
	BufferStatus bStatus;
	long length;
	long actualLength;
	const uint32 currentLength = self->length;
	uint8 *ptr;
	FILE *file = fopen(fileName, "rb");
	if ( !file ) {
		errRenderStd(error);
		return BUF_FOPEN;
	}
	if ( fseek(file, 0, SEEK_END) ) {
		errRenderStd(error);
		return BUF_FSEEK;
	}
	length = ftell(file);
	bStatus = bufAppendConst(self, 0x00, length, error);
	if ( bStatus != BUF_SUCCESS ) {
		fclose(file);
		return bStatus;
	}
	rewind(file);
	actualLength = fread(self->data + currentLength, 1, length, file);
	if ( actualLength != length ) {
		fclose(file);
		if ( feof(file) ) {
			errRender(error, "Unexpectedly hit EOF after reading %lu bytes!\n", actualLength);
			return BUF_FEOF;
		} else if ( ferror(file) ) {
			errRenderStd(error);
			return BUF_FERROR;
		}
	}
	fclose(file);
	return BUF_SUCCESS;
}

DLLEXPORT(BufferStatus) bufWriteBinaryFile(
	const struct Buffer *self, const char *fileName, uint32 bufAddress, uint32 count,
	const char **error)
{
	uint32 actualLength;
	FILE *file = fopen(fileName, "wb");
	if ( !file ) {
		errRenderStd(error);
		return BUF_FOPEN;
	}
	actualLength = fwrite(self->data + bufAddress, 1, count, file);
	if ( actualLength != count ) {
		fclose(file);
		errRenderStd(error);
		return BUF_FERROR;
	}
	fclose(file);	
	return BUF_SUCCESS;
}
