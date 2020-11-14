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
#include <fstream>
#include <makestuff/libbuffer.h>

TEST(BinIO, testReadNonExistentFile) {
	Buffer buf;
	BufferStatus status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendFromBinaryFile(&buf, "nonExistentFile.bin", NULL);
	ASSERT_EQ(BUF_FOPEN, status);
	bufDestroy(&buf);
}

TEST(BinIO, testReadExistingFile) {
	const char *const FILENAME = "tmpFile.bin";
	const char *const DATA = "Just some test data";
	std::ofstream file;
	Buffer buf;
	BufferStatus status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	file.open(FILENAME, std::ios::out|std::ios::binary);
	file << DATA;
	file.close();
	status = bufAppendFromBinaryFile(&buf, FILENAME, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(std::strlen(DATA), buf.length);
	ASSERT_EQ(std::memcmp(DATA, buf.data, buf.length), 0);
	status = bufAppendFromBinaryFile(&buf, FILENAME, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	ASSERT_EQ(2 * strlen(DATA), buf.length);
	ASSERT_EQ(std::memcmp(DATA, buf.data + std::strlen(DATA), std::strlen(DATA)), 0);
	bufDestroy(&buf);
}

TEST(BinIO, testWriteFile) {
	const char *const FILENAME = "tmpFile.bin";
	const char *const DATA = "Just some test data";
	std::ifstream file;
	std::streamoff length;
	//ifstream::pos_type length;
	char *fileData;
	Buffer buf;
	BufferStatus status = bufInitialise(&buf, 8, 0, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	status = bufAppendBlock(&buf, (const uint8 *)DATA, strlen(DATA), NULL);
	ASSERT_EQ(BUF_SUCCESS, status);

	status = bufWriteBinaryFile(&buf, FILENAME, 0, buf.length, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	file.open(FILENAME, std::ios::in|std::ios::binary|std::ios::ate);
	ASSERT_TRUE(file.is_open());
	length = file.tellg();
	ASSERT_EQ(strlen(DATA), (size_t)length);
	fileData = new char[(unsigned int)length];
	file.seekg(0, std::ios::beg);
	file.read(fileData, (std::streamsize)length);
	file.close();
	ASSERT_EQ(std::memcmp(DATA, fileData, length), 0);

	status = bufWriteBinaryFile(&buf, FILENAME, 5, 9, NULL);
	ASSERT_EQ(BUF_SUCCESS, status);
	file.open(FILENAME, std::ios::in|std::ios::binary|std::ios::ate);
	ASSERT_TRUE(file.is_open());
	length = file.tellg();
	ASSERT_EQ(9, length);
	file.seekg(0, std::ios::beg);
	file.read(fileData, (std::streamsize)length);
	file.close();
	ASSERT_EQ(std::memcmp(DATA+5, fileData, length), 0);

	delete[] fileData;
	bufDestroy(&buf);
}
