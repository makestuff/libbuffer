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
#ifndef CONV_H
#define CONV_H

#include <makestuff.h>

#ifdef __cplusplus
extern "C" {
#endif
	// Updates *outputNibble with the nibble represented by the supplied ascii hex digit.
	// Return false on success or true for illegal char.
	//
	bool getHexNibble(char hexDigit, uint8 *outputNibble);
	
	// Updates *outputByte with the byte represented by the two ascii hex digits pointed to by hexDigitPair.
	// Return false on success or true for illegal char.
	//
	bool getHexByte(const char *hexDigitPair, uint8 *outputByte);

	// Return the ascii hex digit representing the most significant nibble of the supplied byte.
	//
	char getHexUpperNibble(uint8 byte);

	// Return the ascii hex digit representing the least significant nibble of the supplied byte.
	//
	char getHexLowerNibble(uint8 byte);

#ifdef __cplusplus
}
#endif

#endif
