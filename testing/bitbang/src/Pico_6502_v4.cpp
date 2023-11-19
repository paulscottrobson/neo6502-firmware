#include "Arduino.h"
#include "pico/stdlib.h"
#include "memory.h"
#include "mos65C02.h"

void initDisplay();
void writeCharacter(int c);
void writeHex(int n);

void setup() {
  initDisplay();
  initmemory();
  init6502();
}

void loop() {
  static uint32_t i, f = 1;

  if (mem[DSP] != 0) {
   writeCharacter(mem[DSP] & 0x7F);
   //writeChar("0123456789abcdef"[mem[DSP] >> 4]);
   //writeChar("0123456789abcdef"[mem[DSP] & 15]);
   //writeChar(32);
   mem[DSP] = 0;
  }
}
