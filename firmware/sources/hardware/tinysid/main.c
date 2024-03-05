/*
 *  main.cpp - SIDPlayer common routines
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

#include <stdio.h>
#include <string.h>

#include "main.h"
#include "prefs.h"
#include "mem.h"
#include "cpu.h"
#include "sid.h"
#include "psid.h"


// Global variables
uint32 f_rand_seed = 1;
int number_of_songs, current_song;
char module_name[64];
char author_name[64];
char copyright_info[64];

// Flag: PSID file loaded and ready
static bool psid_loaded = false;

// Data from PSID header
static uint16 init_adr;                // C64 init routine address
uint16 play_adr;                    // C64 replay routine address
static bool play_adr_from_irq_vec;    // Flag: dynamically update play_adr from IRQ vector ($0314/$0315 or $fffe/$ffff)
static uint32 speed_flags;            // Speed flags (1 bit/song)



/*
 *  Init everything
 */

void InitAll(int argc, char **argv)
{
    PrefsInit(argc, argv);
    MemoryInit();
    SIDInit();
    CPUInit();
}


/*
 *  Exit everything
 */

void ExitAll()
{
    CPUExit();
    SIDExit();
    MemoryExit();
    PrefsExit();
}


/*
 *  Read PSID file header to buffer
 */

bool LoadPSIDHeader(const char *file, uint8 *p)
{
    // Read header
    memset(p, 0, PSID_MAX_HEADER_LENGTH);
    FILE *f = fopen(file, "rb");
    if (f == NULL)
        return false;
    size_t actual = fread(p, 1, PSID_MAX_HEADER_LENGTH, f);
    fclose(f);
    return actual >= PSID_MIN_HEADER_LENGTH;
}


/*
 *  Check for PSID header
 */

bool IsPSIDHeader(const uint8 *p)
{
    // Check signature and version
    uint32 id = read_psid_32(p, PSID_ID);
    uint16 version = read_psid_16(p, PSID_VERSION);
    return id == 0x50534944 && (version == 1 || version == 2);
}


/*
 *  Check whether file is a PSID file
 */

bool IsPSIDFile(const char *file)
{
    // Load header
    uint8 header[PSID_MAX_HEADER_LENGTH];
    if (!LoadPSIDHeader(file, header))
        return false;

    // Check header
    return IsPSIDHeader(header);
}


/*
 *  Load PSID file for playing
 */

bool LoadPSIDFile(const char *file)
{
    // Open file
    FILE *f = fopen(file, "rb");
    if (f == NULL)
        return false;

    // Clear C64 RAM
    MemoryClear();
    psid_loaded = false;

    // Load and check header
    uint8 header[PSID_MAX_HEADER_LENGTH];
    memset(header, 0, PSID_MAX_HEADER_LENGTH);
    size_t actual = fread(header, 1, PSID_MAX_HEADER_LENGTH, f);
    if (actual < PSID_MIN_HEADER_LENGTH || !IsPSIDHeader(header)) {
        fclose(f);
        return false;
    }

    // Extract data from header
    number_of_songs = read_psid_16(header, PSID_NUMBER);
    if (number_of_songs == 0)
        number_of_songs = 1;
    current_song = read_psid_16(header, PSID_DEFSONG);
    if (current_song)
        current_song--;
    if (current_song >= number_of_songs)
        current_song = 0;

    init_adr = read_psid_16(header, PSID_INIT);
    play_adr = read_psid_16(header, PSID_MAIN);
    play_adr_from_irq_vec = (play_adr == 0);

    speed_flags = read_psid_32(header, PSID_SPEED);

    strncpy(module_name, (char *)(header + PSID_NAME), 32);
    strncpy(author_name, (char *)(header + PSID_AUTHOR), 32);
    strncpy(copyright_info, (char *)(header + PSID_COPYRIGHT), 32);
    module_name[32] = 0;
    author_name[32] = 0;
    copyright_info[32] = 0;

    // Seek to start of module data
    fseek(f, read_psid_16(header, PSID_LENGTH), SEEK_SET);

    // Find load address
    uint16 load_adr = read_psid_16(header, PSID_START);
    if (load_adr == 0) {    // Load address is at start of module data
        uint8 lo = fgetc(f);
        uint8 hi = fgetc(f);
        load_adr = (hi << 8) | lo;
    }
    if (init_adr == 0)        // Init routine address is equal to load address
        init_adr = load_adr;

    // Load module data to C64 RAM
    fread(ram + load_adr, 1, RAM_SIZE - load_adr, f);
    fclose(f);

    // Select default song
    SelectSong(current_song);

    // Everything OK
    psid_loaded = true;
    return true;
}


/*
 *  PSID file loaded and ready?
 */

bool IsPSIDLoaded()
{
    return psid_loaded;
}


/*
 *  Select song for playing
 */

void SelectSong(int num)
{
    if (num >= number_of_songs)
        num = 0;
    current_song = num;    

    // Reset SID
    SIDReset(0);

    // Set replay frequency
    int freq = 50;
    if (num < 32)
        freq = speed_flags & (1 << num) ? 60 : 50;
    SIDSetReplayFreq(freq);
    SIDAdjustSpeed(100);

    // Execute init routine
    CPUExecute(init_adr, current_song, 0, 0, 1000000);
}


/*
 *  Update play_adr from IRQ vector if necessary
 */

void UpdatePlayAdr()
{
    if (play_adr_from_irq_vec) {
        if (ram[1] & 2)        // Kernal ROM switched in
            play_adr = (ram[0x0315] << 8) | ram[0x0314];
        else                // Kernal ROM switched out
            play_adr = (ram[0xffff] << 8) | ram[0xfffe];
    }
}
