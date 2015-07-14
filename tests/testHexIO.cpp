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
#include <cstdarg>
#include <string>
#include <fstream>
#include <cstring>
#include <UnitTest++.h>
#include "../private.h"
#include "../libbuffer.h"

using namespace std;

TEST(HexIO_testValidDataLine) {
	Buffer data, mask;
	BufferStatus status;
	const char *line = ":040BE10075820022F7\n";
	uint8 recordType;
	uint32 seg = 0x00000000;
	const uint8 expected[] = {0x75, 0x82, 0x00, 0x22};
	status = bufInitialise(&data, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufInitialise(&mask, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufProcessLine(line, 0, &data, &mask, &seg, &recordType, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(0x00, recordType);
	CHECK_EQUAL(0x0BE1+4UL, data.length);
	CHECK_EQUAL(0x0BE1+4UL, mask.length);
	CHECK_EQUAL(4096UL, data.capacity);
	CHECK_EQUAL(4096UL, mask.capacity);
	CHECK_ARRAY_EQUAL(expected, data.data + 0x0BE1, 4);
	bufDestroy(&mask);
	bufDestroy(&data);
}

TEST(HexIO_testValidEOF) {
	Buffer data, mask;
	BufferStatus status;
	const char *line = ":00000001FF\n";
	uint8 recordType;
	uint32 seg = 0x00000000;
	status = bufInitialise(&data, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufInitialise(&mask, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufProcessLine(line, 0, &data, &mask, &seg, &recordType, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(0x01, recordType);
	CHECK_EQUAL(0UL, data.length);
	CHECK_EQUAL(0UL, mask.length);
	bufDestroy(&mask);
	bufDestroy(&data);
}

TEST(HexIO_testJunkStartCode) {
	Buffer data;
	BufferStatus status;
	const char *line = ".040BE10075820022F7\n";
	uint8 recordType;
	uint32 seg = 0x00000000;
	status = bufInitialise(&data, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
	CHECK_EQUAL(HEX_JUNK_START_CODE, status);
	bufDestroy(&data);
}

TEST(HexIO_testJunkByteCount) {
	Buffer data;
	BufferStatus status;
	const char *line = ":..0BE10075820022F7\n";
	uint8 recordType;
	uint32 seg = 0x00000000;
	status = bufInitialise(&data, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
	CHECK_EQUAL(HEX_JUNK_BYTE_COUNT, status);
	bufDestroy(&data);
}

TEST(HexIO_testJunkAddrMsb) {
	Buffer data;
	BufferStatus status;
	const char *line = ":04..E10075820022F7\n";
	uint8 recordType;
	uint32 seg = 0x00000000;
	status = bufInitialise(&data, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
	CHECK_EQUAL(HEX_JUNK_ADDR_MSB, status);
	bufDestroy(&data);
}

TEST(HexIO_testJunkAddrLsb) {
	Buffer data;
	BufferStatus status;
	const char *line = ":040B..0075820022F7\n";
	uint8 recordType;
	uint32 seg = 0x00000000;
	status = bufInitialise(&data, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
	CHECK_EQUAL(HEX_JUNK_ADDR_LSB, status);
	bufDestroy(&data);
}

TEST(HexIO_testJunkRecType) {
	Buffer data;
	BufferStatus status;
	const char *line = ":040BE1..75820022F7\n";
	uint8 recordType;
	uint32 seg = 0x00000000;
	status = bufInitialise(&data, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
	CHECK_EQUAL(HEX_JUNK_REC_TYPE, status);
	bufDestroy(&data);
}

TEST(HexIO_testJunkDataByte) {
	Buffer data;
	BufferStatus status;
	const char *line = ":040BE100..820022F7\n";
	uint8 recordType;
	uint32 seg = 0x00000000;
	status = bufInitialise(&data, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
	CHECK_EQUAL(HEX_JUNK_DATA_BYTE, status);
	bufDestroy(&data);
}

TEST(HexIO_testJunkChecksum) {
	Buffer data;
	BufferStatus status;
	const char *line = ":040BE10075820022..\n";
	uint8 recordType;
	uint32 seg = 0x00000000;
	status = bufInitialise(&data, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
	CHECK_EQUAL(HEX_JUNK_CHECKSUM, status);
	bufDestroy(&data);
}

TEST(HexIO_testBadChecksum) {
	Buffer data;
	BufferStatus status;
	const char *line = ":040BE10075820022F8\n";
	uint8 recordType;
	uint32 seg = 0x00000000;
	status = bufInitialise(&data, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
	CHECK_EQUAL(HEX_BAD_CHECKSUM, status);
	bufDestroy(&data);
}

TEST(HexIO_testCorruptLine) {
	Buffer data;
	BufferStatus status;
	const char *line = ":040BE10075820022F7 \n";
	uint8 recordType;
	uint32 seg = 0x00000000;
	status = bufInitialise(&data, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
	CHECK_EQUAL(HEX_CORRUPT_LINE, status);
	bufDestroy(&data);
}

void testRoundTrip(const char *firstLine, ...) {
	const char *const FILENAME = "tmpFile.hex";
	Buffer data, mask, readbackData, readbackMask;
	BufferStatus status;
	va_list vl;
	const char *line;
	string expected;
	uint8 recordType;
	uint32 seg = 0x00000000;
	status = bufInitialise(&data, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufInitialise(&mask, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	va_start(vl, firstLine);
	line = firstLine;
	do {
		status = bufProcessLine(line, 0, &data, &mask, &seg, &recordType, NULL);
		CHECK_EQUAL(BUF_SUCCESS, status);
		expected += line;
		expected += '\n';
		line = va_arg(vl, const char *);
	} while ( line );
	expected += ":00000001FF\n";
	va_end(vl);
	status = bufWriteToIntelHexFile(&data, &mask, FILENAME, 16, false, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	ifstream file;
	streamoff length;
	file.open(FILENAME, ios::in|ios::ate);
	CHECK(file.is_open());
	length = file.tellg();
	CHECK_EQUAL(expected.size(), (unsigned int)length);
	char *fileData = new char[(unsigned int)length];
	file.seekg(0, ios::beg);
	file.read(fileData, (streamsize)length);
	file.close();
	CHECK_ARRAY_EQUAL(expected.c_str(), (const char *)fileData, (int)length);

	status = bufInitialise(&readbackData, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufInitialise(&readbackMask, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufReadFromIntelHexFile(&readbackData, &readbackMask, FILENAME, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(data.length, readbackData.length);
	CHECK_ARRAY_EQUAL(data.data, readbackData.data, (int)data.length);
	CHECK_EQUAL(mask.length, readbackMask.length);
	CHECK_ARRAY_EQUAL(mask.data, readbackMask.data, (int)mask.length);
	
	bufDestroy(&readbackMask);
	bufDestroy(&readbackData);
	bufDestroy(&mask);
	bufDestroy(&data);
}

TEST(HexIO_testRoundTrip) {
	testRoundTrip(
		":1011F000E2008E7F8093E2008091E2008061809324",
		":10120000E200A5DD80E060E042E18EDD71DF8091EB",
		":10121000E10082FF0AC08091E20082FF06C0809157",
		":101250000801882361F01091E9001092E900809163",
		":10126000E80083FF01C03DDC17701093E9001F9177",
		":061270000895F894FFCF81",
		NULL);
	testRoundTrip(
		":1011F000E2008E7F8093E2008091E2008061809324",
		":10120000E200A5DD80E060E042E18EDD71DF8091EB",
		":10121000E10082FF0AC08091E20082FF06C0809157",
		":061220000895F894FFCFD1",
		":101250000801882361F01091E9001092E900809163",
		":10126000E80083FF01C03DDC17701093E9001F9177",
		":061270000895F894FFCF81",
		NULL);
	testRoundTrip(
		":1000A00018958FEF8ABB1FBC8DE08EBD89E09DE364",
		":1000B0009BBD8ABD89B7806489BF789485B780620B",
		":0E00C00085BF889585B78F7D85BFF8CFF894F2",
		":0100CF00CF61",
		NULL);
	testRoundTrip(
		":1000B0009BBD8ABD89B7FFFFFFFFFF9485B78062B4",
		NULL);
	testRoundTrip(
		":10FFE000BEBAFECA6E3B8209D926430DADDEADDE38",
		":10FFF000BEBAFECA6E3B8209D926430DADDEADDE28",
		":020000021000EC",
		":10000000BEBAFECA6E3B8209D926430DADDEADDE17",
		":10001000BEBAFECA6E3B8209D926430DADDEADDE07",
		NULL);
	}


void testDeriveWriteMap(const char *inputData, const char *expectedWriteMap) {
	Buffer data, mask;
	BufferStatus status;
	status = bufInitialise(&data, 1024, '.', NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufInitialise(&mask, 1024, 0x00, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufWriteBlock(&data, 0x00000000, (const uint8 *)inputData, strlen(inputData), NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufDeriveMask(&data, &mask, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(data.length, mask.length);
	for ( size_t i = 0; i < mask.length; i++ ) {
		if ( mask.data[i] ) {
			mask.data[i] = '*';
		} else {
			mask.data[i] = '.';
		}
	}
	CHECK_ARRAY_EQUAL(expectedWriteMap, mask.data, (int)mask.length);
	bufDestroy(&data);
	bufDestroy(&mask);
}

TEST(HexIO_testDeriveWriteMap) {
	testDeriveWriteMap("Hello........World", "*****........*****");
	testDeriveWriteMap("Hello.......World", "*****************");
}
