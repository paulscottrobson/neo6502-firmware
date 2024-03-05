/*
 *  psid.h - PSID file format definitions
 *
 *  SIDPlayer (C) Copyright 1996-2004 Christian Bauer
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef PSID_H
#define PSID_H

#include "types.h"


// Minimum and maximum header length
const int PSID_MIN_HEADER_LENGTH = 118;        // Version 1
const int PSID_MAX_HEADER_LENGTH = 124;        // Version 2

// Offsets of fields in header (all fields big-endian)
enum {
    PSID_ID = 0,            // 'PSID'
    PSID_VERSION = 4,        // 1 or 2
    PSID_LENGTH = 6,        // Header length
    PSID_START = 8,            // C64 load address
    PSID_INIT = 10,            // C64 init routine address
    PSID_MAIN = 12,            // C64 replay routine address
    PSID_NUMBER = 14,        // Number of subsongs
    PSID_DEFSONG = 16,        // Main subsong number
    PSID_SPEED = 18,        // Speed flags (1 bit/song)
    PSID_NAME = 22,            // Module name (ISO Latin1 character set)
    PSID_AUTHOR = 54,        // Author name (dto.)
    PSID_COPYRIGHT = 86,    // Copyright info (dto.)
    PSID_FLAGS = 118,        // Flags (only in version 2 header)
    PSID_RESERVED = 120
};

// Read 16-bit quantity from PSID header
inline uint16 read_psid_16(const uint8 *p, int offset)
{
    return (p[offset] << 8) | p[offset + 1];
}

// Read 32-bit quantity from PSID header
inline uint32 read_psid_32(const uint8 *p, int offset)
{
    return (p[offset] << 24) | (p[offset + 1] << 16) | (p[offset + 2] << 8) | p[offset + 3];
}

#endif
