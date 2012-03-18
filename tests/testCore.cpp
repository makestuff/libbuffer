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
#include <string.h>
#include <UnitTest++.h>
#include "../libbuffer.h"

TEST(Core_testInitialiseZero) {
	Buffer buf;
	BufferStatus status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(0UL, buf.length);
	CHECK_EQUAL(8UL, buf.capacity);
	const unsigned char expected[] = {0, 0, 0, 0, 0, 0, 0, 0};
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testInitialiseFF) {
	Buffer buf;
	BufferStatus status = bufInitialise(&buf, 8, 0xFF, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(0UL, buf.length);
	CHECK_EQUAL(8UL, buf.capacity);

	const unsigned char expected[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testSmallAppendBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char expected[] = {1, 2, 3, 4, 0, 0, 0, 0};
	status = bufAppendBlock(&buf, expected, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.capacity);
	CHECK_EQUAL(4UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testCapacityAppendBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, expected, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.capacity);
	CHECK_EQUAL(8UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testLargeAppendBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0};
	status = bufAppendBlock(&buf, expected, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(9UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testVeryLargeAppendBlockInitZero) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0};
	status = bufAppendBlock(&buf, expected, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(9UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testVeryLargeAppendBlockInitFF) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0xFF, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	status = bufAppendBlock(&buf, expected, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(9UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testAppendByte) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0};
	status = bufAppendByte(&buf, 1, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 2, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 3, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 5, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 6, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 7, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(9UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testAppendWordLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0xAA, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char expected[] = {
		0xAD, 0xDE, 0xFE, 0xCA, 0x0D, 0xF0, 0xBE, 0xBA,
		0x34, 0x12, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA
	};
	status = bufAppendWordLE(&buf, 0xDEAD, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendWordLE(&buf, 0xCAFE, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendWordLE(&buf, 0xF00D, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendWordLE(&buf, 0xBABE, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendWordLE(&buf, 0x1234, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(10UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testAppendWordBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0xAA, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char expected[] = {
		0xDE, 0xAD, 0xCA, 0xFE, 0xF0, 0x0D, 0xBA, 0xBE,
		0x12, 0x34, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA
	};
	status = bufAppendWordBE(&buf, 0xDEAD, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendWordBE(&buf, 0xCAFE, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendWordBE(&buf, 0xF00D, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendWordBE(&buf, 0xBABE, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendWordBE(&buf, 0x1234, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(10UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testAppendLongLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0xAA, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char expected[] = {
		0xBE, 0xBA, 0xFE, 0xCA, 0x0D, 0xF0, 0xAD, 0xDE,
		0x78, 0x56, 0x34, 0x12, 0xAA, 0xAA, 0xAA, 0xAA
	};
	status = bufAppendLongLE(&buf, 0xCAFEBABE, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendLongLE(&buf, 0xDEADF00D, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendLongLE(&buf, 0x12345678, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(12UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testAppendLongBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0xAA, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char expected[] = {
		0xCA, 0xFE, 0xBA, 0xBE, 0xDE, 0xAD, 0xF0, 0x0D,
		0x12, 0x34, 0x56, 0x78, 0xAA, 0xAA, 0xAA, 0xAA
	};
	status = bufAppendLongBE(&buf, 0xCAFEBABE, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendLongBE(&buf, 0xDEADF00D, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendLongBE(&buf, 0x12345678, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(12UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testSmallAppendZeros) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 0, 0, 0, 0, 7, 8};
	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x00, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(6UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testCapacityAppendZeros) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 0, 0, 0, 0, 0, 0};
	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x00, 6, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testLargeAppendZeros) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0};
	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x00, 7, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(9UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testVeryLargeAppendZeros) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4};
	const unsigned char expected[] = {1, 2, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0};
	status = bufInitialise(&buf, 4, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 4);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x00, 14, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testSmallAppendConst) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 9, 9, 9, 9, 7, 8};
	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x09, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(6UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testCapacityAppendConst) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 9, 9, 9, 9, 9, 9};
	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x09, 6, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testLargeAppendConst) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 9, 9,  9, 9, 9, 9,  9, 0, 0, 0,  0, 0, 0, 0};
	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x09, 7, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(9UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testVeryLargeAppendConst) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4};
	const unsigned char expected[] = {1, 2, 9, 9,  9, 9, 9, 9,  9, 9, 9, 9,  9, 9, 9, 9};
	status = bufInitialise(&buf, 4, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 4);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x09, 14, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testHugeAppendConst) {
	Buffer buf;
	BufferStatus status;
	status = bufInitialise(&buf, 4, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendConst(&buf, 0x09, 0x18000, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(0x18000UL, buf.length);
	CHECK_EQUAL(0x20000UL, buf.capacity);
	bufDestroy(&buf);
}

TEST(Core_testInsideWriteBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 1, 2, 3, 4};
	status = bufWriteBlock(&buf, 4, expected+4, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.capacity);
	CHECK_EQUAL(8UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testOutsideWriteBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  0, 0, 0, 0,  1, 2, 3, 4};
	status = bufWriteBlock(&buf, 12, expected+12, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(16UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testStraddledWriteBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 1,  2, 3, 4, 0,  0, 0, 0, 0};
	status = bufWriteBlock(&buf, 7, expected+7, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(11UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testInsideWriteConst) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 9, 9, 9, 9};
	status = bufWriteConst(&buf, 4, 9, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.capacity);
	CHECK_EQUAL(8UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testOutsideWriteConst) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  0, 0, 0, 0,  9, 9, 9, 9};
	status = bufWriteConst(&buf, 12, 9, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(16UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testStraddledWriteConst) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 9,  9, 9, 9, 0,  0, 0, 0, 0};
	status = bufWriteConst(&buf, 7, 9, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(11UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testInsideWriteWordLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 0xAD, 0xDE, 7, 8};
	status = bufWriteWordLE(&buf, 4, 0xDEAD, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.capacity);
	CHECK_EQUAL(8UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testOutsideWriteWordLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  9, 9, 9, 9,  0xAD, 0xDE, 9, 9};
	status = bufWriteWordLE(&buf, 12, 0xDEAD, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(14UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testStraddledWriteWordLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 0xAD,  0xDE, 9, 9, 9,  9, 9, 9, 9};
	status = bufWriteWordLE(&buf, 7, 0xDEAD, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(9UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testInsideWriteWordBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 0xDE, 0xAD, 7, 8};
	status = bufWriteWordBE(&buf, 4, 0xDEAD, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.capacity);
	CHECK_EQUAL(8UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testOutsideWriteWordBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  9, 9, 9, 9,  0xDE, 0xAD, 9, 9};
	status = bufWriteWordBE(&buf, 12, 0xDEAD, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(14UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testStraddledWriteWordBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 0xDE,  0xAD, 9, 9, 9,  9, 9, 9, 9};
	status = bufWriteWordBE(&buf, 7, 0xDEAD, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(9UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testInsideWriteLongLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 0xBE, 0xBA, 0xFE, 0xCA};
	status = bufWriteLongLE(&buf, 4, 0xCAFEBABE, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.capacity);
	CHECK_EQUAL(8UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testOutsideWriteLongLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  9, 9, 9, 9,  0xBE, 0xBA, 0xFE, 0xCA};
	status = bufWriteLongLE(&buf, 12, 0xCAFEBABE, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(16UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testStraddledWriteLongLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 0xBE,  0xBA, 0xFE, 0xCA, 9,  9, 9, 9, 9};
	status = bufWriteLongLE(&buf, 7, 0xCAFEBABE, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(11UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testInsideWriteLongBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 0xCA, 0xFE, 0xBA, 0xBE};
	status = bufWriteLongBE(&buf, 4, 0xCAFEBABE, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.capacity);
	CHECK_EQUAL(8UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testOutsideWriteLongBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  9, 9, 9, 9,  0xCA, 0xFE, 0xBA, 0xBE};
	status = bufWriteLongBE(&buf, 12, 0xCAFEBABE, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(16UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testStraddledWriteLongBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 0xCA,  0xFE, 0xBA, 0xBE, 9,  9, 9, 9, 9};
	status = bufWriteLongBE(&buf, 7, 0xCAFEBABE, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(11UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testZeroLength) {
	Buffer buf;
	BufferStatus status;
	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {0, 0, 0, 0, 0, 0, 0, 0};
	memcpy(buf.data, junk, 8);
	bufZeroLength(&buf);
	CHECK_EQUAL(8UL, buf.capacity);
	CHECK_EQUAL(0UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testCopyConstruct) {
	Buffer src, dst = {0, 0, 0, 0};
	BufferStatus status;
	status = bufInitialise(&src, 8, 23, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 23, 23, 23, 23, 23, 23, 23};
	status = bufAppendBlock(&src, expected, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, src.capacity);
	CHECK_EQUAL(9UL, src.length);
	CHECK_EQUAL(23U, src.fill);
	CHECK_ARRAY_EQUAL(expected, src.data, 16);
	status = bufDeepCopy(&dst, &src, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, dst.capacity);
	CHECK_EQUAL(9UL, dst.length);
	CHECK_EQUAL(23U, dst.fill);
	CHECK_ARRAY_EQUAL(expected, dst.data, 16);
	bufDestroy(&dst);
	bufDestroy(&src);
}

TEST(Core_testAssignRealloc) {
	Buffer src, dst;
	BufferStatus status;
	status = bufInitialise(&src, 8, 23, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufInitialise(&dst, 8, 23, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 23, 23, 23, 23, 23, 23, 23};
	status = bufAppendBlock(&src, expected, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, src.capacity);
	CHECK_EQUAL(9UL, src.length);
	CHECK_EQUAL(23U, src.fill);
	CHECK_ARRAY_EQUAL(expected, src.data, 16);
	status = bufDeepCopy(&dst, &src, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, dst.capacity);
	CHECK_EQUAL(9UL, dst.length);
	CHECK_EQUAL(23U, dst.fill);
	CHECK_ARRAY_EQUAL(expected, dst.data, 16);
	bufDestroy(&dst);
	bufDestroy(&src);
}

TEST(Core_testAssignNoRealloc) {
	Buffer src, dst;
	BufferStatus status;
	status = bufInitialise(&src, 8, 23, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufInitialise(&dst, 16, 23, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 23, 23, 23, 23, 23, 23, 23};
	status = bufAppendBlock(&src, expected, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, src.capacity);
	CHECK_EQUAL(9UL, src.length);
	CHECK_EQUAL(23U, src.fill);
	CHECK_ARRAY_EQUAL(expected, src.data, 16);
	status = bufDeepCopy(&dst, &src, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, dst.capacity);
	CHECK_EQUAL(9UL, dst.length);
	CHECK_EQUAL(23U, dst.fill);
	CHECK_ARRAY_EQUAL(expected, dst.data, 16);
	bufDestroy(&dst);
	bufDestroy(&src);
}

TEST(Core_testSwap) {
	Buffer x, y;
	BufferStatus status;
	const unsigned char expx[] = {6, 5, 4, 3, 2, 1, 8, 8};
	const unsigned char expy[] = {1, 2, 3, 9};

	status = bufInitialise(&x, 4, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufInitialise(&y, 4, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	status = bufAppendByte(&x, 1, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&x, 2, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&x, 3, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	status = bufAppendByte(&y, 6, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&y, 5, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&y, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&y, 3, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&y, 2, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendByte(&y, 1, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	bufSwap(&x, &y);

	CHECK_EQUAL(8UL, x.capacity);
	CHECK_EQUAL(6UL, x.length);
	CHECK_ARRAY_EQUAL(expx, x.data, 8);

	CHECK_EQUAL(4UL, y.capacity);
	CHECK_EQUAL(3UL, y.length);
	CHECK_ARRAY_EQUAL(expy, y.data, 4);

	bufDestroy(&x);
	bufDestroy(&y);
}
