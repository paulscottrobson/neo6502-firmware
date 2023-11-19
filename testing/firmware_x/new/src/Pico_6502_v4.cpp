#include "Arduino.h"
#include "memory.h"
#include "mos65C02.h"

#define FRAMETIME     1000 / FRAMERATE  // msec
#define FRAMERATE       10 // frames per sec

unsigned long  lastClockTS;
unsigned long  frameClockTS;

uint32_t       hasDisplayUpdate = 0;

void initDisplay();
void writeChar(int c);
void swapDisplay();

extern uint32_t       clockCount;

void setup() {
  sleep_ms(2000);


  initDisplay();
  initmemory();
  init6502();
  clockCount = 0UL;
  lastClockTS = millis();
  hasDisplayUpdate++;
}

void loop() {
  static uint32_t i, f = 1;

  if (mem[DSP] != 0) {
   writeChar(mem[DSP] & 0x7F);
   hasDisplayUpdate++;
   //writeChar("0123456789abcdef"[mem[DSP] >> 4]);
   //writeChar("0123456789abcdef"[mem[DSP] & 15]);
   //writeChar(32);
   mem[DSP] = 0;
  }


  if (f-- == 0) {
    if ((millis() - frameClockTS) >= FRAMETIME) {
      if (hasDisplayUpdate > 0) {
        swapDisplay();
        hasDisplayUpdate = 0;
      }
      frameClockTS = millis();
    }
    f = 7500;
  }
}
