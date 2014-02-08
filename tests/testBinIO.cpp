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
#include <fstream>
#include <UnitTest++.h>
#include "../libbuffer.h"

using namespace std;

TEST(BinIO_testReadNonExistentFile) {
	Buffer buf;
	BufferStatus status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendFromBinaryFile(&buf, "nonExistentFile.bin", NULL);
	CHECK_EQUAL(BUF_FOPEN, status);
	bufDestroy(&buf);
}

TEST(BinIO_testReadExistingFile) {
	const char *const FILENAME = "tmpFile.bin";
	const char *const DATA = "Just some test data";
	ofstream file;
	Buffer buf;
	BufferStatus status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	file.open(FILENAME, ios::out|ios::binary);
	file << DATA;
	file.close();
	status = bufAppendFromBinaryFile(&buf, FILENAME, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(strlen(DATA), buf.length);
	CHECK_ARRAY_EQUAL(DATA, buf.data, (int)buf.length);
	status = bufAppendFromBinaryFile(&buf, FILENAME, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	CHECK_EQUAL(2 * strlen(DATA), buf.length);
	CHECK_ARRAY_EQUAL(DATA, buf.data + strlen(DATA), (int)strlen(DATA));
	bufDestroy(&buf);
}

TEST(BinIO_testWriteFile) {
	const char *const FILENAME = "tmpFile.bin";
	const char *const DATA = "Just some test data";
	ifstream file;
	streamoff length;
	//ifstream::pos_type length;
	char *fileData;
	Buffer buf;
	BufferStatus status = bufInitialise(&buf, 8, 0, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	status = bufAppendBlock(&buf, (const uint8 *)DATA, strlen(DATA), NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);

	status = bufWriteBinaryFile(&buf, FILENAME, 0, buf.length, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	file.open(FILENAME, ios::in|ios::binary|ios::ate);
	CHECK(file.is_open());
	length = file.tellg();
	CHECK_EQUAL(strlen(DATA), (size_t)length);
	fileData = new char[(unsigned int)length];
	file.seekg(0, ios::beg);
	file.read(fileData, (streamsize)length);
	file.close();
	CHECK_ARRAY_EQUAL(DATA, fileData, (int)length);

	status = bufWriteBinaryFile(&buf, FILENAME, 5, 9, NULL);
	CHECK_EQUAL(BUF_SUCCESS, status);
	file.open(FILENAME, ios::in|ios::binary|ios::ate);
	CHECK(file.is_open());
	length = file.tellg();
	CHECK_EQUAL(9, length);
	file.seekg(0, ios::beg);
	file.read(fileData, (streamsize)length);
	file.close();
	CHECK_ARRAY_EQUAL(DATA+5, fileData, (int)length);

	delete[] fileData;
	bufDestroy(&buf);
}
