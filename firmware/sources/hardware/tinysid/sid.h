/*
 *  sid.h - 6581 SID emulation
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
#pragma once

#include <cstdint>

typedef uint32_t cycle_t;
#define CYCLE_NEVER ((cycle_t) 0xffffffff);    // Infinitely into the future

/*
 *  Functions
 */

// Init SID emulation
extern void SIDInit();

// Exit SID emulation
extern void SIDExit();

// Reset SID emulation
extern void SIDReset(cycle_t now);

// Fill audio buffer with SID sound
extern void SIDCalcBuffer(uint8_t *buf, int count);

// Execute 6510 replay routine once
extern void SIDExecute();

// Set replay frequency and speed adjustment
extern void SIDSetReplayFreq(int freq);
extern void SIDAdjustSpeed(int percent);

// Write to CIA timer A (changes replay frequency)
extern void cia_tl_write(uint8_t byte);
extern void cia_th_write(uint8_t byte);

// Read from SID register
extern uint32_t sid_read(uint32_t adr, cycle_t now);

// Write to SID register
extern void sid_write(uint32_t adr, uint32_t byte);
