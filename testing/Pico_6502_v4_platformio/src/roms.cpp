// 
// Author: Rien Matthijsse
// 

#include "roms.h"

#include "memory.h"
#include "ewoz.h"

#define MAX_ROMS  2
#define ROM_FOLDER  "ROM"

// ROM image:
typedef struct _sROMImage {
  char     name[16];
  uint16_t startAddress;
  uint16_t romsize;
  const uint8_t  *image;
} sROMImage;


sROMImage ROMs[] = {
  { "Ewoz v1.2",    EWOZ_START,    EWOZ_SIZE,    ewoz_bin },
  { "Vectors",      0xFFFA,        0X0006,       vectors_bin },
};

uint8_t* readROM(String vROMname) {
  return 0;
}

bool loadROM(const uint8_t *vROM, uint16_t startAddress, uint16_t romSize) {
  // copy ROM in memory space
  for (uint16_t i = 0; i < romSize; i++) {
    mem[i + startAddress] = vROM[i];
  }

  return true;
}

bool loadROMS() {
  for (uint8_t i = 0; i < MAX_ROMS; i++) {
    if (strcmp(ROMs[i].name, "") != 0) {
      Serial.printf("ROM: %16s\t@0X%04X\t[0X%04x]\n", ROMs[i].name, ROMs[i].startAddress,ROMs[i].romsize);
      loadROM(ROMs[i].image, ROMs[i].startAddress, ROMs[i].romsize);
    }
    else
      break;
  }

  return true;
}
