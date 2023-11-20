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

// 
// Author: Rien Matthijsse
//          Vlaidimir....
//          Andre Weissflog
// 
#include "common.h"

void writeCharacter(int);

#define PICO_NEO6502
#define CHIPS_IMPL

#include "system/wdc65C02cpu.h"

int controlPort = DEFAULT_PORT;
uint16_t iCount;

void sync();

void init6502() {
    wdc65C02cpu_init();
    writeCharacter('I');
    wdc65C02cpu_reset();
    writeCharacter('R');
    while(1) {
        uint16_t addr;
        uint8_t data;
        bool rw;
        iCount += 1;
        if ((iCount & 0xFFF) == 0) {
            sync();
        }
        wdc65C02cpu_tick(&addr, &rw);
        if (rw) {
            wdc65C02cpu_set_data(cpuMemory[addr]);
        } else {
            // Memory write
            data = cpuMemory[addr] = wdc65C02cpu_get_data();
            if (addr == controlPort && data != 0) {
               writeCharacter(data & 0x7F);
               cpuMemory[controlPort] = 0;
            }
        }       
    }
}


