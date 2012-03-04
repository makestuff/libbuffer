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
#ifndef PRIVATE_H
#define PRIVATE_H

#include <makestuff.h>
#include "libbuffer.h"

#ifdef __cplusplus
extern "C" {
#endif
	BufferStatus bufProcessLine(
		const char *sourceLine, uint32 lineNumber, struct Buffer *destData, struct Buffer *destMask,
		uint32 *seg, uint8 *recordType, const char **error
	) WARN_UNUSED_RESULT;
	BufferStatus bufDeriveMask(
		const struct Buffer *sourceData, struct Buffer *destMask, const char **error
	) WARN_UNUSED_RESULT;
#ifdef __cplusplus
}
#endif

#endif
