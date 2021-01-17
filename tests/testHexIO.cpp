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
#include <gtest/gtest.h>
#include <cstdarg>
#include <string>
#include <fstream>
#include <cstring>
#include "private.h"

TEST(HexIO, testValidDataLine) {
  Buffer data, mask;
  BufferStatus status;
  const char *line = ":040BE10075820022F7\n";
  uint8 recordType;
  uint32 seg = 0x00000000;
  const uint8 expected[] = {0x75, 0x82, 0x00, 0x22};
  status = bufInitialise(&data, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufInitialise(&mask, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufProcessLine(line, 0, &data, &mask, &seg, &recordType, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  ASSERT_EQ(0x00, recordType);
  ASSERT_EQ(0x0BE1+4UL, data.length);
  ASSERT_EQ(0x0BE1+4UL, mask.length);
  ASSERT_EQ(4096UL, data.capacity);
  ASSERT_EQ(4096UL, mask.capacity);
  ASSERT_EQ(std::memcmp(expected, data.data + 0x0BE1, 4), 0);
  bufDestroy(&mask);
  bufDestroy(&data);
}

TEST(HexIO, testValidEOF) {
  Buffer data, mask;
  BufferStatus status;
  const char *line = ":00000001FF\n";
  uint8 recordType;
  uint32 seg = 0x00000000;
  status = bufInitialise(&data, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufInitialise(&mask, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufProcessLine(line, 0, &data, &mask, &seg, &recordType, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  ASSERT_EQ(0x01, recordType);
  ASSERT_EQ(0UL, data.length);
  ASSERT_EQ(0UL, mask.length);
  bufDestroy(&mask);
  bufDestroy(&data);
}

TEST(HexIO, testJunkStartCode) {
  Buffer data;
  BufferStatus status;
  const char *line = ".040BE10075820022F7\n";
  uint8 recordType;
  uint32 seg = 0x00000000;
  status = bufInitialise(&data, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
  ASSERT_EQ(HEX_JUNK_START_CODE, status);
  bufDestroy(&data);
}

TEST(HexIO, testJunkByteCount) {
  Buffer data;
  BufferStatus status;
  const char *line = ":..0BE10075820022F7\n";
  uint8 recordType;
  uint32 seg = 0x00000000;
  status = bufInitialise(&data, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
  ASSERT_EQ(HEX_JUNK_BYTE_COUNT, status);
  bufDestroy(&data);
}

TEST(HexIO, testJunkAddrMsb) {
  Buffer data;
  BufferStatus status;
  const char *line = ":04..E10075820022F7\n";
  uint8 recordType;
  uint32 seg = 0x00000000;
  status = bufInitialise(&data, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
  ASSERT_EQ(HEX_JUNK_ADDR_MSB, status);
  bufDestroy(&data);
}

TEST(HexIO, testJunkAddrLsb) {
  Buffer data;
  BufferStatus status;
  const char *line = ":040B..0075820022F7\n";
  uint8 recordType;
  uint32 seg = 0x00000000;
  status = bufInitialise(&data, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
  ASSERT_EQ(HEX_JUNK_ADDR_LSB, status);
  bufDestroy(&data);
}

TEST(HexIO, testJunkRecType) {
  Buffer data;
  BufferStatus status;
  const char *line = ":040BE1..75820022F7\n";
  uint8 recordType;
  uint32 seg = 0x00000000;
  status = bufInitialise(&data, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
  ASSERT_EQ(HEX_JUNK_REC_TYPE, status);
  bufDestroy(&data);
}

TEST(HexIO, testJunkDataByte) {
  Buffer data;
  BufferStatus status;
  const char *line = ":040BE100..820022F7\n";
  uint8 recordType;
  uint32 seg = 0x00000000;
  status = bufInitialise(&data, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
  ASSERT_EQ(HEX_JUNK_DATA_BYTE, status);
  bufDestroy(&data);
}

TEST(HexIO, testJunkChecksum) {
  Buffer data;
  BufferStatus status;
  const char *line = ":040BE10075820022..\n";
  uint8 recordType;
  uint32 seg = 0x00000000;
  status = bufInitialise(&data, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
  ASSERT_EQ(HEX_JUNK_CHECKSUM, status);
  bufDestroy(&data);
}

TEST(HexIO, testBadChecksum) {
  Buffer data;
  BufferStatus status;
  const char *line = ":040BE10075820022F8\n";
  uint8 recordType;
  uint32 seg = 0x00000000;
  status = bufInitialise(&data, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
  ASSERT_EQ(HEX_BAD_CHECKSUM, status);
  bufDestroy(&data);
}

TEST(HexIO, testCorruptLine) {
  Buffer data;
  BufferStatus status;
  const char *line = ":040BE10075820022F7 \n";
  uint8 recordType;
  uint32 seg = 0x00000000;
  status = bufInitialise(&data, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufProcessLine(line, 0, &data, NULL, &seg, &recordType, NULL);
  ASSERT_EQ(HEX_CORRUPT_LINE, status);
  bufDestroy(&data);
}

void testRoundTrip(const char *firstLine, ...) {
  const char *const FILENAME = "tmpFile.hex";
  Buffer data, mask, readbackData, readbackMask;
  BufferStatus status;
  va_list vl;
  const char *line;
  std::string expected;
  uint8 recordType;
  uint32 seg = 0x00000000;
  status = bufInitialise(&data, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufInitialise(&mask, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  va_start(vl, firstLine);
  line = firstLine;
  do {
    status = bufProcessLine(line, 0, &data, &mask, &seg, &recordType, NULL);
    ASSERT_EQ(BUF_SUCCESS, status);
    expected += line;
    expected += '\n';
    line = va_arg(vl, const char *);
  } while (line);
  expected += ":00000001FF\n";
  va_end(vl);
  status = bufWriteToIntelHexFile(&data, &mask, FILENAME, 16, false, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);

  std::ifstream file;
  std::streamoff length;
  file.open(FILENAME, std::ios::in|std::ios::ate);
  ASSERT_TRUE(file.is_open());
  length = file.tellg();
  ASSERT_EQ(expected.size(), (unsigned int)length);
  char *fileData = new char[(unsigned int)length];
  file.seekg(0, std::ios::beg);
  file.read(fileData, (std::streamsize)length);
  file.close();
  ASSERT_EQ(std::memcmp(expected.c_str(), (const char *)fileData, length), 0);
  delete[] fileData;

  status = bufInitialise(&readbackData, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufInitialise(&readbackMask, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufReadFromIntelHexFile(&readbackData, &readbackMask, FILENAME, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  ASSERT_EQ(data.length, readbackData.length);
  ASSERT_EQ(std::memcmp(data.data, readbackData.data, data.length), 0);
  ASSERT_EQ(mask.length, readbackMask.length);
  ASSERT_EQ(std::memcmp(mask.data, readbackMask.data, mask.length), 0);

  bufDestroy(&readbackMask);
  bufDestroy(&readbackData);
  bufDestroy(&mask);
  bufDestroy(&data);
}

TEST(HexIO, testRoundTrip) {
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
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufInitialise(&mask, 1024, 0x00, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufWriteBlock(&data, 0x00000000, (const uint8 *)inputData, strlen(inputData), NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  status = bufDeriveMask(&data, &mask, NULL);
  ASSERT_EQ(BUF_SUCCESS, status);
  ASSERT_EQ(data.length, mask.length);
  for (size_t i = 0; i < mask.length; i++) {
    if (mask.data[i]) {
      mask.data[i] = '*';
    } else {
      mask.data[i] = '.';
    }
  }
  ASSERT_EQ(std::memcmp(expectedWriteMap, mask.data, mask.length), 0);
  bufDestroy(&data);
  bufDestroy(&mask);
}

TEST(HexIO, testDeriveWriteMap) {
  testDeriveWriteMap("Hello........World", "*****........*****");
  testDeriveWriteMap("Hello.......World", "*****************");
}
