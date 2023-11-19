// 
// Author: Rien Matthijsse
// 
#include "pico/stdlib.h"
#include "memory.h"
#include "roms.h"

uint8_t mem[MEMORY_SIZE];

uint16_t address;
uint8_t  data;

void initmemory() {
  address = 0x0000UL;
  data = 0x00;

  // lets install some ROMS
  loadROMS();
  mem[KBD] = 0x00;
  mem[DSP] = 0x00;
}
