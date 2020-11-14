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
#include <cstring>
#include <makestuff/libbuffer.h>

TEST(Core, testInitialiseZero) {
	Buffer buf;
	BufferStatus status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(0UL, buf.length);
	ASSERT_EQ(8UL, buf.capacity);
	const unsigned char expected[] = {0, 0, 0, 0, 0, 0, 0, 0};
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testInitialiseFF) {
	Buffer buf;
	BufferStatus status = bufInitialise(&buf, 8, 0xFF, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(0UL, buf.length);
	ASSERT_EQ(8UL, buf.capacity);

	const unsigned char expected[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testSmallAppendBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char expected[] = {1, 2, 3, 4, 0, 0, 0, 0};
	status = bufAppendBlock(&buf, expected, 4, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(8UL, buf.capacity);
	ASSERT_EQ(4UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testCapacityAppendBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, expected, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(8UL, buf.capacity);
	ASSERT_EQ(8UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testLargeAppendBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0};
	status = bufAppendBlock(&buf, expected, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(9UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testVeryLargeAppendBlockInitZero) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0};
	status = bufAppendBlock(&buf, expected, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(9UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testVeryLargeAppendBlockInitFF) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0xFF, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	status = bufAppendBlock(&buf, expected, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(9UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testAppendByte) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0};
	status = bufAppendByte(&buf, 1, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 2, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 3, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 4, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 5, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 6, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 7, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&buf, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(9UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testAppendWordLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0xAA, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char expected[] = {
		0xAD, 0xDE, 0xFE, 0xCA, 0x0D, 0xF0, 0xBE, 0xBA,
		0x34, 0x12, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA
	};
	status = bufAppendWordLE(&buf, 0xDEAD, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendWordLE(&buf, 0xCAFE, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendWordLE(&buf, 0xF00D, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendWordLE(&buf, 0xBABE, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendWordLE(&buf, 0x1234, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(10UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testAppendWordBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0xAA, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char expected[] = {
		0xDE, 0xAD, 0xCA, 0xFE, 0xF0, 0x0D, 0xBA, 0xBE,
		0x12, 0x34, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA
	};
	status = bufAppendWordBE(&buf, 0xDEAD, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendWordBE(&buf, 0xCAFE, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendWordBE(&buf, 0xF00D, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendWordBE(&buf, 0xBABE, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendWordBE(&buf, 0x1234, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(10UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testAppendLongLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0xAA, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char expected[] = {
		0xBE, 0xBA, 0xFE, 0xCA, 0x0D, 0xF0, 0xAD, 0xDE,
		0x78, 0x56, 0x34, 0x12, 0xAA, 0xAA, 0xAA, 0xAA
	};
	status = bufAppendLongLE(&buf, 0xCAFEBABE, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendLongLE(&buf, 0xDEADF00D, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendLongLE(&buf, 0x12345678, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(12UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testAppendLongBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 4, 0xAA, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char expected[] = {
		0xCA, 0xFE, 0xBA, 0xBE, 0xDE, 0xAD, 0xF0, 0x0D,
		0x12, 0x34, 0x56, 0x78, 0xAA, 0xAA, 0xAA, 0xAA
	};
	status = bufAppendLongBE(&buf, 0xCAFEBABE, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendLongBE(&buf, 0xDEADF00D, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendLongBE(&buf, 0x12345678, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(12UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testSmallAppendZeros) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 0, 0, 0, 0, 7, 8};
	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x00, 4, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(6UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testCapacityAppendZeros) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 0, 0, 0, 0, 0, 0};
	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x00, 6, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(8UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testLargeAppendZeros) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0};
	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x00, 7, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(9UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testVeryLargeAppendZeros) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4};
	const unsigned char expected[] = {1, 2, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0};
	status = bufInitialise(&buf, 4, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 4);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x00, 14, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testSmallAppendConst) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 9, 9, 9, 9, 7, 8};
	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x09, 4, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(6UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testCapacityAppendConst) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 9, 9, 9, 9, 9, 9};
	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x09, 6, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(8UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testLargeAppendConst) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {1, 2, 9, 9,  9, 9, 9, 9,  9, 0, 0, 0,  0, 0, 0, 0};
	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 8);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x09, 7, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(9UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testVeryLargeAppendConst) {
	Buffer buf;
	BufferStatus status;
	const unsigned char junk[] = {1, 2, 3, 4};
	const unsigned char expected[] = {1, 2, 9, 9,  9, 9, 9, 9,  9, 9, 9, 9,  9, 9, 9, 9};
	status = bufInitialise(&buf, 4, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	memcpy(buf.data, junk, 4);
	buf.length = 2;
	status = bufAppendConst(&buf, 0x09, 14, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testHugeAppendConst) {
	Buffer buf;
	BufferStatus status;
	status = bufInitialise(&buf, 4, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendConst(&buf, 0x09, 0x18000, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(0x18000UL, buf.length);
	ASSERT_EQ(0x20000UL, buf.capacity);
	bufDestroy(&buf);
}

TEST(Core, testInsideWriteBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 1, 2, 3, 4};
	status = bufWriteBlock(&buf, 4, expected+4, 4, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(8UL, buf.capacity);
	ASSERT_EQ(8UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testOutsideWriteBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  0, 0, 0, 0,  1, 2, 3, 4};
	status = bufWriteBlock(&buf, 12, expected+12, 4, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(16UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testStraddledWriteBlock) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 1,  2, 3, 4, 0,  0, 0, 0, 0};
	status = bufWriteBlock(&buf, 7, expected+7, 4, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(11UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testInsideWriteConst) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 9, 9, 9, 9};
	status = bufWriteConst(&buf, 4, 9, 4, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(8UL, buf.capacity);
	ASSERT_EQ(8UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testOutsideWriteConst) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  0, 0, 0, 0,  9, 9, 9, 9};
	status = bufWriteConst(&buf, 12, 9, 4, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(16UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testStraddledWriteConst) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 9,  9, 9, 9, 0,  0, 0, 0, 0};
	status = bufWriteConst(&buf, 7, 9, 4, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(11UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testInsideWriteWordLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 0xAD, 0xDE, 7, 8};
	status = bufWriteWordLE(&buf, 4, 0xDEAD, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(8UL, buf.capacity);
	ASSERT_EQ(8UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testOutsideWriteWordLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  9, 9, 9, 9,  0xAD, 0xDE, 9, 9};
	status = bufWriteWordLE(&buf, 12, 0xDEAD, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(14UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testStraddledWriteWordLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 0xAD,  0xDE, 9, 9, 9,  9, 9, 9, 9};
	status = bufWriteWordLE(&buf, 7, 0xDEAD, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(9UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testInsideWriteWordBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 0xDE, 0xAD, 7, 8};
	status = bufWriteWordBE(&buf, 4, 0xDEAD, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(8UL, buf.capacity);
	ASSERT_EQ(8UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testOutsideWriteWordBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  9, 9, 9, 9,  0xDE, 0xAD, 9, 9};
	status = bufWriteWordBE(&buf, 12, 0xDEAD, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(14UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testStraddledWriteWordBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 0xDE,  0xAD, 9, 9, 9,  9, 9, 9, 9};
	status = bufWriteWordBE(&buf, 7, 0xDEAD, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(9UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testInsideWriteLongLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 0xBE, 0xBA, 0xFE, 0xCA};
	status = bufWriteLongLE(&buf, 4, 0xCAFEBABE, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(8UL, buf.capacity);
	ASSERT_EQ(8UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testOutsideWriteLongLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  9, 9, 9, 9,  0xBE, 0xBA, 0xFE, 0xCA};
	status = bufWriteLongLE(&buf, 12, 0xCAFEBABE, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(16UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testStraddledWriteLongLE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 0xBE,  0xBA, 0xFE, 0xCA, 9,  9, 9, 9, 9};
	status = bufWriteLongLE(&buf, 7, 0xCAFEBABE, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(11UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testInsideWriteLongBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 0xCA, 0xFE, 0xBA, 0xBE};
	status = bufWriteLongBE(&buf, 4, 0xCAFEBABE, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(8UL, buf.capacity);
	ASSERT_EQ(8UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testOutsideWriteLongBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 8,  9, 9, 9, 9,  0xCA, 0xFE, 0xBA, 0xBE};
	status = bufWriteLongBE(&buf, 12, 0xCAFEBABE, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(16UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testStraddledWriteLongBE) {
	Buffer buf;
	BufferStatus status;

	status = bufInitialise(&buf, 8, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	const unsigned char initData[] = {1, 2, 3, 4, 5, 6, 7, 8};
	status = bufAppendBlock(&buf, initData, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4,  5, 6, 7, 0xCA,  0xFE, 0xBA, 0xBE, 9,  9, 9, 9, 9};
	status = bufWriteLongBE(&buf, 7, 0xCAFEBABE, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, buf.capacity);
	ASSERT_EQ(11UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 16), 0);
	bufDestroy(&buf);
}

TEST(Core, testZeroLength) {
	Buffer buf;
	BufferStatus status;
	status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char junk[] = {1, 2, 3, 4, 5, 6, 7, 8};
	const unsigned char expected[] = {0, 0, 0, 0, 0, 0, 0, 0};
	memcpy(buf.data, junk, 8);
	bufZeroLength(&buf);
	ASSERT_EQ(8UL, buf.capacity);
	ASSERT_EQ(0UL, buf.length);
	ASSERT_EQ(std::memcmp(expected, buf.data, 8), 0);
	bufDestroy(&buf);
}

TEST(Core, testCopyConstruct) {
	Buffer src, dst = {0, 0, 0, 0};
	BufferStatus status;
	status = bufInitialise(&src, 8, 23, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 23, 23, 23, 23, 23, 23, 23};
	status = bufAppendBlock(&src, expected, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, src.capacity);
	ASSERT_EQ(9UL, src.length);
	ASSERT_EQ(23U, src.fill);
	ASSERT_EQ(std::memcmp(expected, src.data, 16), 0);
	status = bufDeepCopy(&dst, &src, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, dst.capacity);
	ASSERT_EQ(9UL, dst.length);
	ASSERT_EQ(23U, dst.fill);
	ASSERT_EQ(std::memcmp(expected, dst.data, 16), 0);
	bufDestroy(&dst);
	bufDestroy(&src);
}

TEST(Core, testAssignRealloc) {
	Buffer src, dst;
	BufferStatus status;
	status = bufInitialise(&src, 8, 23, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufInitialise(&dst, 8, 23, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 23, 23, 23, 23, 23, 23, 23};
	status = bufAppendBlock(&src, expected, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, src.capacity);
	ASSERT_EQ(9UL, src.length);
	ASSERT_EQ(23U, src.fill);
	ASSERT_EQ(std::memcmp(expected, src.data, 16), 0);
	status = bufDeepCopy(&dst, &src, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, dst.capacity);
	ASSERT_EQ(9UL, dst.length);
	ASSERT_EQ(23U, dst.fill);
	ASSERT_EQ(std::memcmp(expected, dst.data, 16), 0);
	bufDestroy(&dst);
	bufDestroy(&src);
}

TEST(Core, testAssignNoRealloc) {
	Buffer src, dst;
	BufferStatus status;
	status = bufInitialise(&src, 8, 23, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufInitialise(&dst, 16, 23, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	const unsigned char expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 23, 23, 23, 23, 23, 23, 23};
	status = bufAppendBlock(&src, expected, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, src.capacity);
	ASSERT_EQ(9UL, src.length);
	ASSERT_EQ(23U, src.fill);
	ASSERT_EQ(std::memcmp(expected, src.data, 16), 0);
	status = bufDeepCopy(&dst, &src, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(16UL, dst.capacity);
	ASSERT_EQ(9UL, dst.length);
	ASSERT_EQ(23U, dst.fill);
	ASSERT_EQ(std::memcmp(expected, dst.data, 16), 0);
	bufDestroy(&dst);
	bufDestroy(&src);
}

TEST(Core, testSwap) {
	Buffer x, y;
	BufferStatus status;
	const unsigned char expx[] = {6, 5, 4, 3, 2, 1, 8, 8};
	const unsigned char expy[] = {1, 2, 3, 9};

	status = bufInitialise(&x, 4, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufInitialise(&y, 4, 8, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	status = bufAppendByte(&x, 1, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&x, 2, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&x, 3, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	status = bufAppendByte(&y, 6, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&y, 5, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&y, 4, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&y, 3, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&y, 2, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendByte(&y, 1, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	bufSwap(&x, &y);

	ASSERT_EQ(8UL, x.capacity);
	ASSERT_EQ(6UL, x.length);
	ASSERT_EQ(std::memcmp(expx, x.data, 8), 0);

	ASSERT_EQ(4UL, y.capacity);
	ASSERT_EQ(3UL, y.length);
	ASSERT_EQ(std::memcmp(expy, y.data, 4), 0);

	bufDestroy(&x);
	bufDestroy(&y);
}
