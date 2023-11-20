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

void writeCharacter(int);

#define PICO_NEO6502
#define CHIPS_IMPL

#include "system/wdc65C02cpu.h"

static int controlPort = DEFAULT_PORT;
uint16_t iCount;
uint16_t addr;
uint8_t data;
bool rw;

void CPUStart(void) {
    wdc65C02cpu_init();
    wdc65C02cpu_reset();
    while(1) {
        if (!iCount++) CPUSync();
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

void CPUSync(void) {
    USBSync();
}
