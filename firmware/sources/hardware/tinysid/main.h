/*
 *  main.h - SIDPlayer common routines
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

#ifndef MAIN_H
#define MAIN_H

#include "types.h"


/*
 *  Definitions
 */

// C64 replay routine address (set by UpdatePlayAdr(), used by sid.cpp)
extern uint16 play_adr;

// Module name, author name, copyright info in ISO Latin1 (BeOS: UTF8) charset (set by LoadPSIDFile())
extern char module_name[64], author_name[64], copyright_info[64];

// Total number of songs in module and currently played song number (0..n)
extern int number_of_songs, current_song;


/*
 *  Functions
 */

// Init everything
extern void InitAll(int argc, char **argv);

// Exit everything
extern void ExitAll();

// Read PSID file header to buffer
extern bool LoadPSIDHeader(const char *file, uint8 *p);

// Check for PSID header
extern bool IsPSIDHeader(const uint8 *p);

// Check whether file is a PSID file
extern bool IsPSIDFile(const char *file);

// Load PSID file for playing
extern bool LoadPSIDFile(const char *file);

// PSID file loaded and ready?
extern bool IsPSIDLoaded();

// Select song for playing
extern void SelectSong(int num);

// Update play_adr if necessary
extern void UpdatePlayAdr();

// Adjust replay speed
extern void AdjustSpeed(int percent);

// Show About window
extern void AboutWindow();

// Fast pseudo-random number generator
extern uint32 f_rand_seed;

inline static uint8 f_rand()
{
    f_rand_seed = f_rand_seed * 1103515245 + 12345;
    return f_rand_seed >> 16;
}

#endif
