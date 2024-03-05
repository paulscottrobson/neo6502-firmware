/*
 *  mem.h - Emulator memory management
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

#ifndef MEM_H
#define MEM_H

#include "types.h"


/*
 *  Definitions
 */

// Sizes of memory area
#define RAM_SIZE ((const int) 0x10000)

// Memory area
extern uint8 ram[RAM_SIZE];


/*
 *  Functions
 */

// Init memory management
extern void MemoryInit();

// Exit memory management
extern void MemoryExit();

// Clear memory contents
extern void MemoryClear();

#endif
