// 
// Author: Rien Matthijsse
// 
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "roms.h"

#include "memory.h"
#include "binary.h"

#define MAX_ROMS  1
#define ROM_FOLDER  "ROM"

// ROM image:
typedef struct _sROMImage {
  char     name[16];
  uint16_t startAddress;
  uint16_t romsize;
  const uint8_t  *image;
} sROMImage;

const uint8_t vectors_bin[] = {
  0x00, 0x00, // NMI
  0x00, 0xFC, // RESET
  0x86, 0xFD  // IRQ/BRK
};

sROMImage ROMs[] = {
  { "Test Program", TESTLOAD,      TESTSIZE,     test_bin },
  { "Vectors",      0xFFFA,        0X0006,       vectors_bin }
};

// uint8_t* readROM(char *vROMname) {
//   return 0;
// }

bool loadROM(const uint8_t *vROM, uint16_t startAddress, uint16_t romSize) {
  // copy ROM in memory space
  for (uint16_t i = 0; i < romSize; i++) {
    mem[i + startAddress] = vROM[i];
  }

  return true;
}

bool loadROMS() {
  // for (uint8_t i = 0; i < MAX_ROMS; i++) {
  //   if (strcmp(ROMs[i].name, "") != 0) {
  //     // Serial.printf("ROM: %16s\t@0X%04X\t[0X%04x]\n", ROMs[i].name, ROMs[i].startAddress,ROMs[i].romsize);
  //     loadROM(ROMs[i].image, ROMs[i].startAddress, ROMs[i].romsize);
  //   }
  //   else
  //     break;
  loadROM(test_bin,TESTLOAD,TESTSIZE);
  return true;
}
