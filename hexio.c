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

// Utility functions for reading and writing Intel Hex records. The data is stored in the "data"
// buffer, and a bitmap of the runs covered by the Intel Hex file is stored in the "writeMap"
// buffer. You would think that the "writeMap" buffer is not necessary (why not just assume blocks
// of four or more zeros should not appear in the file?). Unfortunately you cannot always assume
// such "holes" will have been properly initialised by the target system. In short, given a binary
// file, there's no way to tell which runs of zeros must be zero, and which are "don't care".
//
#include <stdio.h>
#include <string.h>
#include <liberror.h>
#include "libbuffer.h"
#include "conv.h"
#include "private.h"

#define LINE_MAX 512

typedef enum {
	DATA_RECORD = 0x00,
	EOF_RECORD,
	EXT_SEG_RECORD,
	START_SEG_RECORD,
	EXT_LIN_RECORD,
	START_LIN_RECORD
} RecordType;

// Process a single Intel hex record.
//   Data record:   ":CCAAAA00DD..SS"
//   EOF record:    ":00000001FF"
//   ExtSeg record: ":02000002AAAASS"
//
BufferStatus bufProcessLine(
	const char *sourceLine, uint32 lineNumber, struct Buffer *destData, struct Buffer *destMask,
	uint32 *segment, uint8 *recordType, const char **error)
{
	char reconstructedLine[LINE_MAX];
	uint8 thisByte;
	uint8 i, byteCount;
	uint16 address;
	uint8 dataBytes[LINE_MAX/2];
	uint8 readChecksum;
	uint8 calculatedChecksum;
	const char *p;
	BufferStatus status;

	p = sourceLine;
	// Read the start code - must be ':'
	//
	if ( *p++ != ':' ) {
		errRender(error, "Junk start code at line %lu", lineNumber);
		return HEX_JUNK_START_CODE;
	}
	
	// Read the byte count
	//
	if ( getHexByte(p, &byteCount) ) {
		errRender(error, "Junk byte count at line %lu", lineNumber);
		return HEX_JUNK_BYTE_COUNT;
	}
	p += 2;
	calculatedChecksum = byteCount;
	
	// Read the MSB of the address
	//
	if ( getHexByte(p, &thisByte) ) {
		errRender(error, "Junk address MSB at line %lu", lineNumber);
		return HEX_JUNK_ADDR_MSB;
	}
	p += 2;
	address = thisByte << 8;
	calculatedChecksum += thisByte;
	
	// Read the LSB of the address
	//
	if ( getHexByte(p, &thisByte) ) {
		errRender(error, "Junk address LSB at line %lu", lineNumber);
		return HEX_JUNK_ADDR_LSB;
	}
	p += 2;
	address |= thisByte;
	calculatedChecksum += thisByte;
	
	// Read the record type
	//
	if ( getHexByte(p, recordType) ) {
		errRender(error, "Junk record type at line %lu", lineNumber);
		return HEX_JUNK_REC_TYPE;
	}
	p += 2;
	calculatedChecksum += *recordType;
	
	// Read the data
	//
	for ( i = 0; i < byteCount; i++ ) {
		if ( getHexByte(p, &thisByte) ) {
			errRender(error, "Junk data byte %d at line %lu", i, lineNumber);
			return HEX_JUNK_DATA_BYTE;
		}
		p += 2;
		dataBytes[i] = thisByte;
		calculatedChecksum += thisByte;
	}
	
	// Read the checksum
	//
	if ( getHexByte(p, &readChecksum) ) {
		errRender(error, "Junk checksum at line %lu", lineNumber);
		return HEX_JUNK_CHECKSUM;
	}
	
	// Calculate the two's complement of the checksum
	//
	calculatedChecksum = (uint8)(256 - calculatedChecksum);
	if ( readChecksum != calculatedChecksum ) {
		errRender(
			error, "Read checksum 0x%02X differs from calculated checksum 0x%02X at line %lu",
			readChecksum, calculatedChecksum, lineNumber);
		return HEX_BAD_CHECKSUM;
	}
	
	// Recreate the input data
	//
	sprintf(reconstructedLine, ":%02X%04X%02X", byteCount, address, *recordType);
	for ( i = 0; i < byteCount; i++ ) {
		sprintf(reconstructedLine + 9 + 2*i, "%02X", dataBytes[i]);
	}
	sprintf(reconstructedLine + 9 + byteCount*2, "%02X", readChecksum);
	while ( *p && *p != 0x0D && *p != 0x0A ) {
		p++;
	}
	if ( strncmp(sourceLine, reconstructedLine, p - sourceLine) ) {
		errRender(
			error, "Some corruption detected at line %lu - some junk at the end of the line perhaps?",
			lineNumber);
		return HEX_CORRUPT_LINE;
	}
	
	if ( *recordType == DATA_RECORD ) {
		// Write into the binary buffer
		//
		status = bufWriteBlock(destData, *segment + address, dataBytes, byteCount, error);
		if ( status != BUF_SUCCESS ) {
			return status;
		}
		if ( destMask ) {
			status = bufWriteConst(destMask, *segment + address, 0x01, byteCount, error);
			if ( status != BUF_SUCCESS ) {
				return status;
			}
		}
		return BUF_SUCCESS;
	} else if ( *recordType == EOF_RECORD ) {
		return BUF_SUCCESS;
	} else if ( *recordType == EXT_SEG_RECORD ) {
		if ( address != 0x0000 || byteCount != 2 ) {
			errRender(
				error, "For record type EXT_SEG_RECORD, address must be 0x0000 and byteCount must be 0x02 at line %lu",
				lineNumber);
			return HEX_BAD_EXT_SEG;
		}
		*segment = ((dataBytes[0] << 8) + dataBytes[1]) << 4;
		return BUF_SUCCESS;
	} else if ( *recordType == START_SEG_RECORD ) {
		errRender(error, "Record type START_SEG_RECORD not supported at line %lu", lineNumber);
		return HEX_BAD_REC_TYPE;
	} else if ( *recordType == EXT_LIN_RECORD ) {
		errRender(error, "Record type EXT_LIN_RECORD not supported at line %lu", lineNumber);
		return HEX_BAD_REC_TYPE;
	} else if ( *recordType == START_LIN_RECORD ) {
		errRender(error, "Record type START_LIN_RECORD not supported at line %lu", lineNumber);
		return HEX_BAD_REC_TYPE;
	} else {
		errRender(error, "Record type 0x%02X not supported at line %lu", *recordType, lineNumber);
		return HEX_BAD_REC_TYPE;
	}
}

// Read Intel Hex records from a file.
//
DLLEXPORT(BufferStatus) bufReadFromIntelHexFile(
	struct Buffer *destData, struct Buffer *destMask, const char *fileName, const char **error)
{
	uint32 lineNumber;
	uint32 segment = 0x00000000;
	char readLine[LINE_MAX];
	BufferStatus status;
	uint8 recordType;

	// Open the file...
	//
	FILE *file = fopen(fileName, "rb");
	if ( !file ) {
		errRenderStd(error);
		return BUF_FOPEN;
	}

	// Clear the existing data in the buffer, if any.
	//
	bufZeroLength(destData);
	if ( destMask ) {
		bufZeroLength(destMask);
	}

	// Iterate over every line
	//
	lineNumber = 1;
	if ( !fgets(readLine, LINE_MAX, file) ) {
		errRender(error, "Empty file!");
		fclose(file);
		return HEX_EMPTY_FILE;
	}
	do {
		status = bufProcessLine(
			readLine, lineNumber, destData, destMask, &segment, &recordType, error);
		if ( status != BUF_SUCCESS ) {
			fclose(file);
			return status;
		}
		lineNumber++;
	} while ( (recordType == DATA_RECORD || recordType == EXT_SEG_RECORD) &&
	          fgets(readLine, LINE_MAX, file) );

	// Make sure the file terminated correctly
	//
	if ( recordType != EOF_RECORD ) {
		errRender(error, "Premature end of file - no EOF_RECORD found!");
		fclose(file);
		return HEX_MISSING_EOF;
	}

	// Close the file and exit
	//
	fclose(file);
	return BUF_SUCCESS;
}

// Write the supplied byte as two hex digits
//
static void writeHexByte(uint8 byte, FILE *file) {
	fputc(getHexUpperNibble(byte), file);
	fputc(getHexLowerNibble(byte), file);
}

// Write the supplied word as four hex digits, in big-endian format (most significant byte first).
//
static void writeHexWordBE(uint16 word, FILE *file) {
	fputc(getHexUpperNibble(word >> 8), file);
	fputc(getHexLowerNibble(word >> 8), file);
	fputc(getHexUpperNibble(word & 0xFF), file);
	fputc(getHexLowerNibble(word & 0xFF), file);
}

BufferStatus bufDeriveMask(
	const struct Buffer *sourceData, struct Buffer *destMask, const char **error)
{
	uint32 address, count, i;
	BufferStatus bStatus;
	bufZeroLength(destMask);
	bStatus = bufAppendConst(destMask, 0x01, sourceData->length, error);
	if ( bStatus != BUF_SUCCESS ) {
		return bStatus;
	}
	address = 0x00000000;
	while ( address < destMask->length ) {
		while ( address < destMask->length && sourceData->data[address] != sourceData->fill ) {
			address++;
		}
		if ( address == destMask->length ) {
			break;
		}
		count = 1;
		while ( address + count < destMask->length &&
		        sourceData->data[address + count] == sourceData->fill )
		{
			count++;
		}
		if ( count >= 8 ) {
			for ( i = 0; i < count; i++ ) {
				destMask->data[address + i] = 0x00;
			}
		}
		address += count;
	}
	return BUF_SUCCESS;
}

// Write the supplied buffer as Intel hex records with the stated line length to a file, using the
// supplied mask. If the mask is null, one is derived from the data, either compressed or
// uncompressed.
//
DLLEXPORT(BufferStatus) bufWriteToIntelHexFile(
	const struct Buffer *sourceData, const struct Buffer *sourceMask, const char *fileName,
	uint8 lineLength, bool compress, const char **error)
{
	BufferStatus status, returnCode = BUF_SUCCESS;
	struct Buffer tmpSourceMask;
	bool usedTmpSourceMask = false;
	uint32 address = 0x00000000;
	uint32 ceiling = 0x00000000;
	uint32 segment;
	uint8 i, calculatedChecksum, maxBytesToWrite, bytesToWrite;
	FILE *file = fopen(fileName, "wb");
	if ( !file ) {
		errRenderStd(error);
		return BUF_FOPEN;
	}
	if ( !sourceMask ) {
		// No sourceMask was supplied; we can either assume we need to write everything,
		// or we can try to compress the data, assuming holes where there exist ranges
		// of the sourceData's fill byte.
		//
		status = bufInitialise(&tmpSourceMask, 1024, 0x00, error);
		if ( status != BUF_SUCCESS ) {
			returnCode = status;
			goto cleanupFile;
		}
		sourceMask = &tmpSourceMask;
		usedTmpSourceMask = true;
		if ( compress ) {
			status = bufDeriveMask(sourceData, &tmpSourceMask, error);
			if ( status != BUF_SUCCESS ) {
				returnCode = status;
				goto cleanupBuffer;
			}
		} else {
			status = bufAppendConst(&tmpSourceMask, 0x01, sourceData->length, error);
			if ( status != BUF_SUCCESS ) {
				returnCode = status;
				goto cleanupBuffer;
			}
		}
	}

	do {
		ceiling += 0x10000;
		if ( ceiling > sourceMask->length ) {
			ceiling = sourceMask->length;
		}
		while ( address < ceiling ) {
			// Find the next run in the sourceMask
			while ( address < ceiling && !sourceMask->data[address] ) {
				address++;
			}
			// If we hit the end of the sourceMask, break out of this while loop
			if ( address == ceiling ) {
				break;
			}
			if ( address + lineLength > ceiling ) {
				// there are fewer than lineLength bytes remaining
				maxBytesToWrite = (uint8)(ceiling - address);
			} else {
				// there are lineLength or more bytes remaining
				maxBytesToWrite = lineLength;
			}
			// find out how many bytes are in this run
			bytesToWrite = 0;
			while ( bytesToWrite < maxBytesToWrite && sourceMask->data[address + bytesToWrite] ) {
				bytesToWrite++;
			}
			fputc(':', file);
			writeHexByte(bytesToWrite, file);
			writeHexWordBE(address & 0xFFFF, file);
			writeHexByte(DATA_RECORD, file);
			calculatedChecksum = bytesToWrite;
			calculatedChecksum += (uint8)(address >> 8);
			calculatedChecksum += address & 0xFF;
			for ( i = 0; i < bytesToWrite; i++ ) {
				writeHexByte(sourceData->data[address + i], file);
				calculatedChecksum += sourceData->data[address + i];
			}
			calculatedChecksum = (uint8)(256 - calculatedChecksum);
			writeHexByte(calculatedChecksum, file);
			fputc('\n', file);
			address += bytesToWrite;
		}
		if ( address < sourceMask->length ) {
			segment = address >> 4;
			if ( segment > 0xFFFF ) {
				errRender(error, "Segment addresses > 0xFFFF are not supported");
				returnCode = HEX_BAD_EXT_SEG;
				goto cleanupBuffer;
			}
			calculatedChecksum =
				(uint8)(256 - 2 - EXT_SEG_RECORD - (uint8)(segment >> 8) - (segment & 0xFF));
			fwrite(":020000", 1, 7, file);
			writeHexByte(EXT_SEG_RECORD, file);
			writeHexWordBE((uint16)segment, file);
			writeHexByte(calculatedChecksum, file);
			fputc('\n', file);
		}
	} while ( address < sourceMask->length );
	fwrite(":00000001FF\n", 1, 12, file);
cleanupBuffer:
	if ( usedTmpSourceMask ) {
		bufDestroy(&tmpSourceMask);
	}
cleanupFile:
	fclose(file);	
	return returnCode;
}
