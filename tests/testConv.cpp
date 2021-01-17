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
#include "conv.h"

TEST(Conv, testGetHexNibble) {
  uint8 nibble;
  bool status;
  status = getHexNibble('0', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(0, nibble);
  status = getHexNibble('1', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(1, nibble);
  status = getHexNibble('2', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(2, nibble);
  status = getHexNibble('3', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(3, nibble);
  status = getHexNibble('4', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(4, nibble);
  status = getHexNibble('5', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(5, nibble);
  status = getHexNibble('6', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(6, nibble);
  status = getHexNibble('7', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(7, nibble);
  status = getHexNibble('8', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(8, nibble);
  status = getHexNibble('9', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(9, nibble);
  status = getHexNibble('A', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(10, nibble);
  status = getHexNibble('B', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(11, nibble);
  status = getHexNibble('C', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(12, nibble);
  status = getHexNibble('D', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(13, nibble);
  status = getHexNibble('E', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(14, nibble);
  status = getHexNibble('F', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(15, nibble);
  status = getHexNibble('a', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(10, nibble);
  status = getHexNibble('b', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(11, nibble);
  status = getHexNibble('c', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(12, nibble);
  status = getHexNibble('d', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(13, nibble);
  status = getHexNibble('e', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(14, nibble);
  status = getHexNibble('f', &nibble);
  ASSERT_FALSE(status);
  ASSERT_EQ(15, nibble);
  status = getHexNibble('.', &nibble);
  ASSERT_EQ(true, status);
}

TEST(Conv, testGetHexByte) {
  uint8 byte;
  bool status;
  status = getHexByte("00", &byte);
  ASSERT_FALSE(status);
  ASSERT_EQ(0x00, byte);
  status = getHexByte("A5", &byte);
  ASSERT_FALSE(status);
  ASSERT_EQ(0xA5, byte);
  status = getHexByte("FF", &byte);
  ASSERT_FALSE(status);
  ASSERT_EQ(0xFF, byte);
  status = getHexByte("GF", &byte);
  ASSERT_EQ(true, status);
  status = getHexByte("FG", &byte);
  ASSERT_EQ(true, status);
}

TEST(Conv, testGetHexUpperNibble) {
  ASSERT_EQ('0', getHexUpperNibble(0x0F));
  ASSERT_EQ('1', getHexUpperNibble(0x1F));
  ASSERT_EQ('2', getHexUpperNibble(0x2F));
  ASSERT_EQ('3', getHexUpperNibble(0x3F));
  ASSERT_EQ('4', getHexUpperNibble(0x4F));
  ASSERT_EQ('5', getHexUpperNibble(0x5F));
  ASSERT_EQ('6', getHexUpperNibble(0x6F));
  ASSERT_EQ('7', getHexUpperNibble(0x7F));
  ASSERT_EQ('8', getHexUpperNibble(0x8F));
  ASSERT_EQ('9', getHexUpperNibble(0x9F));
  ASSERT_EQ('A', getHexUpperNibble(0xAF));
  ASSERT_EQ('B', getHexUpperNibble(0xBF));
  ASSERT_EQ('C', getHexUpperNibble(0xCF));
  ASSERT_EQ('D', getHexUpperNibble(0xDF));
  ASSERT_EQ('E', getHexUpperNibble(0xEF));
  ASSERT_EQ('F', getHexUpperNibble(0xFF));
}

TEST(Conv, testGetHexLowerNibble) {
  ASSERT_EQ('0', getHexLowerNibble(0xF0));
  ASSERT_EQ('1', getHexLowerNibble(0xF1));
  ASSERT_EQ('2', getHexLowerNibble(0xF2));
  ASSERT_EQ('3', getHexLowerNibble(0xF3));
  ASSERT_EQ('4', getHexLowerNibble(0xF4));
  ASSERT_EQ('5', getHexLowerNibble(0xF5));
  ASSERT_EQ('6', getHexLowerNibble(0xF6));
  ASSERT_EQ('7', getHexLowerNibble(0xF7));
  ASSERT_EQ('8', getHexLowerNibble(0xF8));
  ASSERT_EQ('9', getHexLowerNibble(0xF9));
  ASSERT_EQ('A', getHexLowerNibble(0xFA));
  ASSERT_EQ('B', getHexLowerNibble(0xFB));
  ASSERT_EQ('C', getHexLowerNibble(0xFC));
  ASSERT_EQ('D', getHexLowerNibble(0xFD));
  ASSERT_EQ('E', getHexLowerNibble(0xFE));
  ASSERT_EQ('F', getHexLowerNibble(0xFF));
}
