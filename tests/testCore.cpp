/* 
 * Copyright (C) 2009 Chris McClelland
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
#include <string.h>
#include <UnitTest++.h>
#include "../buffer.h"

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

TEST(Core_testSmallAppendZeros) {
	Buffer buf;
	BufferStatus status;
	unsigned char *ptr;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 0, 0, 0, 0, 7, 8};
	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendZeros(&buf, 4, &ptr, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(6UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	CHECK_EQUAL(buf.data+2, ptr);
	bufDestroy(&buf);
}

TEST(Core_testCapacityAppendZeros) {
	Buffer buf;
	BufferStatus status;
	unsigned char *ptr;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 0, 0, 0, 0, 0, 0};
	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendZeros(&buf, 6, &ptr, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	CHECK_EQUAL(buf.data+2, ptr);
	bufDestroy(&buf);
}

TEST(Core_testLargeAppendZeros) {
	Buffer buf;
	BufferStatus status;
	unsigned char *ptr;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0};
	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendZeros(&buf, 7, &ptr, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(9UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	CHECK_EQUAL(buf.data+2, ptr);
	bufDestroy(&buf);
}

TEST(Core_testVeryLargeAppendZeros) {
	Buffer buf;
	BufferStatus status;
	unsigned char *ptr;
	const unsigned char junk[] = {1, 2, 3, 4};
	const unsigned char expected[] = {1, 2, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0};
	status = bufInitialise(&buf, 4, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 4);
	buf.length = 2;
	status = bufAppendZeros(&buf, 14, &ptr, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	CHECK_EQUAL(buf.data+2, ptr);
	bufDestroy(&buf);
}

TEST(Core_testSmallAppendConst) {
	Buffer buf;
	BufferStatus status;
	unsigned char *ptr;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 9, 9, 9, 9, 7, 8};
	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 4, 9, &ptr, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(6UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	CHECK_EQUAL(buf.data+2, ptr);
	bufDestroy(&buf);
}

TEST(Core_testCapacityAppendConst) {
	Buffer buf;
	BufferStatus status;
	unsigned char *ptr;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 9, 9, 9, 9, 9, 9};
	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 6, 9, &ptr, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	CHECK_EQUAL(buf.data+2, ptr);
	bufDestroy(&buf);
}

TEST(Core_testLargeAppendConst) {
	Buffer buf;
	BufferStatus status;
	unsigned char *ptr;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 9, 9,  9, 9, 9, 9,  9, 0, 0, 0,  0, 0, 0, 0};
	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 7, 9, &ptr, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(9UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	CHECK_EQUAL(buf.data+2, ptr);
	bufDestroy(&buf);
}

TEST(Core_testVeryLargeAppendConst) {
	Buffer buf;
	BufferStatus status;
	unsigned char *ptr;
	const unsigned char junk[] = {1, 2, 3, 4};
	const unsigned char expected[] = {1, 2, 9, 9,  9, 9, 9, 9,  9, 9, 9, 9,  9, 9, 9, 9};
	status = bufInitialise(&buf, 4, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 4);
	buf.length = 2;
	status = bufAppendConst(&buf, 14, 9, &ptr, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	CHECK_EQUAL(buf.data+2, ptr);
	bufDestroy(&buf);
}

TEST(Core_testHugeAppendConst) {
	Buffer buf;
	BufferStatus status;
	unsigned char *ptr;
	status = bufInitialise(&buf, 4, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendConst(&buf, 0x18000, 9, &ptr, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(0x18000UL, buf.length);
	CHECK_EQUAL(0x20000UL, buf.capacity);
	CHECK_EQUAL(buf.data, ptr);
	bufDestroy(&buf);
}

TEST(Core_testInsideCopyBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 1, 2, 3, 4};
	status = bufCopyBlock(&buf, 4, expected+4, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.capacity);
	CHECK_EQUAL(8UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testOutsideCopyBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  0, 0, 0, 0,  1, 2, 3, 4};
	status = bufCopyBlock(&buf, 12, expected+12, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(16UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testStraddledCopyBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 1,  2, 3, 4, 0,  0, 0, 0, 0};
	status = bufCopyBlock(&buf, 7, expected+7, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(11UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testInsideSetBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 9, 9, 9, 9};
	status = bufSetBlock(&buf, 4, 9, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(8UL, buf.capacity);
	CHECK_EQUAL(8UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 8);
	bufDestroy(&buf);
}

TEST(Core_testOutsideSetBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  0, 0, 0, 0,  9, 9, 9, 9};
	status = bufSetBlock(&buf, 12, 9, 4, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(16UL, buf.capacity);
	CHECK_EQUAL(16UL, buf.length);
	CHECK_ARRAY_EQUAL(expected, buf.data, 16);
	bufDestroy(&buf);
}

TEST(Core_testStraddledSetBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 9,  9, 9, 9, 0,  0, 0, 0, 0};
	status = bufSetBlock(&buf, 7, 9, 4, NULL);
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
