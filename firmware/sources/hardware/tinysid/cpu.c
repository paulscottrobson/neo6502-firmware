/*
 *  cpu.cpp - 6510 CPU emulation
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

#include "mem.h"
#include "sid.h"

#define DEBUG 0
#include "debug.h"


// Memory access functions
typedef uint32 (*mem_read_func)(uint32, cycle_t);
typedef void (*mem_write_func)(uint32, uint32, cycle_t, bool);

static mem_read_func mem_read_table[256];       // Table of read/write functions for 256 pages
static mem_write_func mem_write_table[256];


// Memory access function prototypes
static uint32 ram_read(uint32 adr, cycle_t now);
static void ram_write(uint32 adr, uint32 byte, cycle_t now, bool rmw);
static void cia_write(uint32 adr, uint32 byte, cycle_t now, bool rmw);


/*
 *  Init CPU emulation
 */

static void set_memory_funcs(uint16 from, uint16 to, mem_read_func r, mem_write_func w)
{
        int page;
    for (page = (from >> 8); page <= (to >> 8); page++) {
        mem_read_table[page] = r;
        mem_write_table[page] = w;
    }
}

void CPUInit()
{
    // Set up memory access tables
    set_memory_funcs(0x0000, 0xffff, ram_read, ram_write);
    set_memory_funcs(0xd400, 0xd7ff, sid_read, sid_write);
    set_memory_funcs(0xdc00, 0xdcff, ram_read, cia_write);
}


/*
 *  Exit CPU emulation
 */

void CPUExit()
{
}


/*
 *  Memory access functions
 */

static uint32 ram_read(uint32 adr, cycle_t now)
{
    return ram[adr];
}

static void ram_write(uint32 adr, uint32 byte, cycle_t now, bool rmw)
{
    ram[adr] = byte;
}

static void cia_write(uint32 adr, uint32 byte, cycle_t now, bool rmw)
{
    if (adr == 0xdc04)
        cia_tl_write(byte);
    else if (adr == 0xdc05)
        cia_th_write(byte);
    else
        ram[adr] = byte;
}


/*
 *  CPU emulation loop
 */

void CPUExecute(uint16 startadr, uint8 init_ra, uint8 init_rx, uint8 init_ry, cycle_t max_cycles)
{
    // 6510 registers
    register uint8 a = init_ra, x = init_rx, y = init_ry;
    register uint8 n_flag = 0, z_flag = 0;
    uint8 sp = 0xff, pflags = 0;

    // Program counter
    register uint8 *pc;

    // Temporary address storage
    uint16 adr;

    // Phi 2 cycle counter
    register cycle_t current_cycle = 0;

#define RA a
#define RX x
#define RY y
#define RSP sp
#define RPC (pc - ram)
#define N_FLAG n_flag
#define Z_FLAG z_flag
#define PFLAGS pflags
#define ADR adr

#define read_byte(adr) \
    mem_read_table[(adr) >> 8](adr, current_cycle)
#define read_zp(adr) \
    ram[adr]

#define write_byte(adr, byte) \
    mem_write_table[(adr) >> 8](adr, byte, current_cycle, false)
#define write_byte_rmw(adr, byte) \
    mem_write_table[(adr) >> 8](adr, byte, current_cycle, true)
#define write_zp(adr, byte) \
    ram[adr] = (byte)

#define read_idle(adr)
#define read_idle_zp(adr)
#define read_idle_stack(sp)

#define read_opcode \
    (*pc)
#define read_idle_opcode

#define push_byte(byte) \
{ \
    if (sp == 0) \
        quit = true; \
    (ram + 0x100)[sp--] = (byte); \
}
#define pop_byte \
    (sp == 0xff ? (quit = true, 0) : (ram + 0x100)[++sp])

#define jump(adr) \
    pc = ram + (adr)
#define inc_pc \
    pc++

#define next_cycle \
    current_cycle++

#include "cpu_macros.h"

    // Jump to specified start address
    jump(startadr);

    // Main loop: execute opcodes until stack under-/overflow, RTI, illegal opcode, or max_cycles reached
    bool quit = false;
    while (current_cycle < max_cycles && !quit) {

        // Fetch opcode
        uint8 opcode = read_opcode; inc_pc; next_cycle;

        // Execute opcode
        switch (opcode) {
#include "cpu_opcodes.h"
            case 0xf2:
            default:
illegal_op:        quit = true;
                break;
        }
    }
}
