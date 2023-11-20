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
        if ((iCount & 0xFF) == 0) {
            sync();
        }
        wdc65C02cpu_tick(&addr, &rw);
        if (rw) {
            // Memory read
            wdc65C02cpu_set_data(mem[addr]);
        } else {
            // Memory write
            data = mem[addr] = wdc65C02cpu_get_data();
            if (addr == controlPort && data != 0) {
               writeCharacter(data & 0x7F);
               //writeChar("0123456789abcdef"[mem[DSP] >> 4]);
               //writeChar("0123456789abcdef"[mem[DSP] & 15]);
               //writeChar(32);
               mem[controlPort] = 0;
            }
        }       
    }
}


