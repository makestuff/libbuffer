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
#include "conv.h"

// Updates *outputNibble with the nibble represented by the supplied ascii hex digit.
// Return false on success or true for illegal char.
//
bool getHexNibble(char hexDigit, uint8 *outputNibble) {
	if ( hexDigit >= '0' && hexDigit <= '9' ) {
		*outputNibble = hexDigit - '0';
		return false;
	} else if ( hexDigit >= 'a' && hexDigit <= 'f' ) {
		*outputNibble = hexDigit - 'a' + 10;
		return false;
	} else if ( hexDigit >= 'A' && hexDigit <= 'F' ) {
		*outputNibble = hexDigit - 'A' + 10;
		return false;
	} else {
		return true;
	}
}

// Updates *outputByte with the byte represented by the two ascii hex digits pointed to by hexDigitPair.
// Return false on success or true for illegal char.
//
bool getHexByte(const char *hexDigitPair, uint8 *outputByte) {
	uint8 upperNibble;
	uint8 lowerNibble;
	if ( !getHexNibble(hexDigitPair[0], &upperNibble) && !getHexNibble(hexDigitPair[1], &lowerNibble) ) {
		*outputByte = (upperNibble << 4) | lowerNibble;
		return false;
	} else {
		return true;
	}
}

// Return the ascii hex digit representing the most significant nibble of the supplied byte.
//
char getHexUpperNibble(uint8 byte) {
	uint8 tmp = byte >> 4;
	tmp += (tmp < 10) ? '0' : 'A' - 10;
	return tmp;
}

// Return the ascii hex digit representing the least significant nibble of the supplied byte.
//
char getHexLowerNibble(uint8 byte) {
	uint8 tmp = byte & 0x0F;
	tmp += (tmp < 10) ? '0' : 'A' - 10;
	return tmp;
}
