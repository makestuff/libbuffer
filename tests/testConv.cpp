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
#include <UnitTest++.h>
#include "../conv.h"

TEST(Conv_testGetHexNibble) {
	uint8 nibble;
	bool status;
	status = getHexNibble('0', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(0, nibble);
	status = getHexNibble('1', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(1, nibble);
	status = getHexNibble('2', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(2, nibble);
	status = getHexNibble('3', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(3, nibble);
	status = getHexNibble('4', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(4, nibble);
	status = getHexNibble('5', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(5, nibble);
	status = getHexNibble('6', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(6, nibble);
	status = getHexNibble('7', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(7, nibble);
	status = getHexNibble('8', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(8, nibble);
	status = getHexNibble('9', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(9, nibble);
	status = getHexNibble('A', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(10, nibble);
	status = getHexNibble('B', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(11, nibble);
	status = getHexNibble('C', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(12, nibble);
	status = getHexNibble('D', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(13, nibble);
	status = getHexNibble('E', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(14, nibble);
	status = getHexNibble('F', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(15, nibble);
	status = getHexNibble('a', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(10, nibble);
	status = getHexNibble('b', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(11, nibble);
	status = getHexNibble('c', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(12, nibble);
	status = getHexNibble('d', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(13, nibble);
	status = getHexNibble('e', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(14, nibble);
	status = getHexNibble('f', &nibble);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(15, nibble);
	status = getHexNibble('.', &nibble);
	CHECK_EQUAL(true, status);
}

TEST(Conv_testGetHexByte) {
	uint8 byte;
	bool status;
	status = getHexByte("00", &byte);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(0x00, byte);
	status = getHexByte("A5", &byte);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(0xA5, byte);
	status = getHexByte("FF", &byte);
	CHECK_EQUAL(false, status);
	CHECK_EQUAL(0xFF, byte);
	status = getHexByte("GF", &byte);
	CHECK_EQUAL(true, status);
	status = getHexByte("FG", &byte);
	CHECK_EQUAL(true, status);
}

TEST(Conv_testGetHexUpperNibble) {
	CHECK_EQUAL('0', getHexUpperNibble(0x0F));
	CHECK_EQUAL('1', getHexUpperNibble(0x1F));
	CHECK_EQUAL('2', getHexUpperNibble(0x2F));
	CHECK_EQUAL('3', getHexUpperNibble(0x3F));
	CHECK_EQUAL('4', getHexUpperNibble(0x4F));
	CHECK_EQUAL('5', getHexUpperNibble(0x5F));
	CHECK_EQUAL('6', getHexUpperNibble(0x6F));
	CHECK_EQUAL('7', getHexUpperNibble(0x7F));
	CHECK_EQUAL('8', getHexUpperNibble(0x8F));
	CHECK_EQUAL('9', getHexUpperNibble(0x9F));
	CHECK_EQUAL('A', getHexUpperNibble(0xAF));
	CHECK_EQUAL('B', getHexUpperNibble(0xBF));
	CHECK_EQUAL('C', getHexUpperNibble(0xCF));
	CHECK_EQUAL('D', getHexUpperNibble(0xDF));
	CHECK_EQUAL('E', getHexUpperNibble(0xEF));
	CHECK_EQUAL('F', getHexUpperNibble(0xFF));
}

TEST(Conv_testGetHexLowerNibble) {
	CHECK_EQUAL('0', getHexLowerNibble(0xF0));
	CHECK_EQUAL('1', getHexLowerNibble(0xF1));
	CHECK_EQUAL('2', getHexLowerNibble(0xF2));
	CHECK_EQUAL('3', getHexLowerNibble(0xF3));
	CHECK_EQUAL('4', getHexLowerNibble(0xF4));
	CHECK_EQUAL('5', getHexLowerNibble(0xF5));
	CHECK_EQUAL('6', getHexLowerNibble(0xF6));
	CHECK_EQUAL('7', getHexLowerNibble(0xF7));
	CHECK_EQUAL('8', getHexLowerNibble(0xF8));
	CHECK_EQUAL('9', getHexLowerNibble(0xF9));
	CHECK_EQUAL('A', getHexLowerNibble(0xFA));
	CHECK_EQUAL('B', getHexLowerNibble(0xFB));
	CHECK_EQUAL('C', getHexLowerNibble(0xFC));
	CHECK_EQUAL('D', getHexLowerNibble(0xFD));
	CHECK_EQUAL('E', getHexLowerNibble(0xFE));
	CHECK_EQUAL('F', getHexLowerNibble(0xFF));
}
