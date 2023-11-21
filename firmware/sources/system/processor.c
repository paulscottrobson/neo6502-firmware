// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      processor.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Andre Weissflog
//                  Rien Matthijsse
//                  Veselin Sladkov
//      Date :      20th November 2023
//      Reviewed :  No
//      Purpose :   Drive the 65C02 processor
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

#define PICO_NEO6502
#define CHIPS_IMPL

#include "system/wdc65C02cpu.h"

static int controlPort = DEFAULT_PORT;
uint16_t iCount;
uint16_t addr;
uint8_t data;
bool rw;

// ***************************************************************************************
//
//                      Start and run the CPU. Does not have to return.
//
// ***************************************************************************************

void CPUStart(void) {
    wdc65C02cpu_init();
    wdc65C02cpu_reset();
    while(1) {
        if (!iCount++) CPUSync();                                               // 1 time in 64k. About 25-30Hz.
        wdc65C02cpu_tick(&addr, &rw);                                           // Tick the processor

        if (rw) {                                                               // Read put data on data lines.
            wdc65C02cpu_set_data(cpuMemory[addr]);
        } else {                                                                // Write get it and store in memory.
            // Memory write
            data = cpuMemory[addr] = wdc65C02cpu_get_data();
            if (addr == controlPort && data != 0) {                             // Message passed
               CONWrite(data & 0xFF);                                           // Execute the message (temp)
               cpuMemory[controlPort] = 0;                                      // Clear the message indicating completion.
            }
        }       
    }
}

// ***************************************************************************************
//
//                      Hardware that requires periodic updates
//
// ***************************************************************************************

void CPUSync(void) {
    KBDSync();
}
