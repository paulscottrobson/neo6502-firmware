// 
// Author: Rien Matthijsse
// 
#include "pico/stdlib.h"
#include "mos65C02.h"
#include "memory.h"

void writeCharacter(int);

#define PICO_NEO6502
#define CHIPS_IMPL

#include "wdc65C02cpu.h"

void writeHex(int n) {
    const char *c = "0123456789abcdef";
    writeCharacter(32);
    writeCharacter(c[(n >> 12) & 0x0F]);
    writeCharacter(c[(n >> 8) & 0x0F]);
    writeCharacter(c[(n >> 4) & 0x0F]);
    writeCharacter(c[(n >> 0) & 0x0F]);
}

    int iCount;

void init6502() {
    wdc65C02cpu_init();
    writeCharacter('I');
    wdc65C02cpu_reset();
    writeCharacter('R');
    while(1) {
        uint16_t addr;
        bool rw;
        wdc65C02cpu_tick(&addr, &rw);
        if (rw) {
            // Memory read
            wdc65C02cpu_set_data(mem[addr]);
        } else {
            // Memory write
            mem[addr] = wdc65C02cpu_get_data();
            if (mem[DSP] != 0) {
               writeCharacter(mem[DSP] & 0x7F);
               //writeChar("0123456789abcdef"[mem[DSP] >> 4]);
               //writeChar("0123456789abcdef"[mem[DSP] & 15]);
               //writeChar(32);
               mem[DSP] = 0;
            }
        }       
    }
}


