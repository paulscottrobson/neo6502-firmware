// 
// Author: Rien Matthijsse
// 
#include "pico/stdlib.h"
#include "roms.h"

#include "memory.h"

#include "binary.h"



static void loadROM(const uint8_t *vROM, uint16_t startAddress, uint16_t romSize) {
  for (uint16_t i = 0; i < romSize; i++) {
    mem[i + startAddress] = vROM[i];
  }
}

void loadROMS() {
  loadROM(test_bin,TESTLOAD,TESTSIZE);
}
