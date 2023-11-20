// 
// Author: Rien Matthijsse
// 
#include "common.h"
#include "data/binary.h"

uint8_t cpuMemory[MEMORY_SIZE];


static void loadROM(const uint8_t *vROM, uint16_t startAddress, uint16_t romSize) {
  for (uint16_t i = 0; i < romSize; i++) {
    cpuMemory[i + startAddress] = vROM[i];
  }
}

void MEMInitialiseMemory(void) {
  loadROM(kernel_bin,KERNEL_LOAD,KERNEL_SIZE);
  cpuMemory[DEFAULT_PORT] = 0x00;
}
