/*
 *  mem.cpp - Emulator memory management
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

#include "sys.h"

#include <string.h>

#ifndef __psx__
#include <errno.h>
#endif

#include "mem.h"


// Memory area
uint8 ram[RAM_SIZE];


/*
 *  Init memory management
 */

void MemoryInit()
{
    // Clear memory contents
    MemoryClear();
}


/*
 *  Exit memory management
 */

void MemoryExit()
{
}


/*
 *  Clear memory contents
 */

void MemoryClear()
{
    memset(ram, 0, RAM_SIZE - 0x2000);
    memset(ram + 0xe000, 0x40, 0x2000);        // Fill kernal ROM area with RTI
    ram[1] = 7;                                // 6510 I/O port
    ram[0x0314] = 0x31;                        // IRQ vector
    ram[0x0315] = 0xea;
}
