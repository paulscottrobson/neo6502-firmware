#include "hardware/gpio.h"
#include "bsp/board.h"
#include "tusb.h"

#include "memory.h"
#include "mos65C02.h"

unsigned long  lastClockTS;
unsigned long  frameClockTS;

uint32_t       hasDisplayUpdate = 0;

int startVideo();
void writeCharacter(int n);

void loop1() {

  if (mem[DSP] != 0) {
   writeCharacter(mem[DSP] & 0x7F);
   hasDisplayUpdate++;
   //writeChar("0123456789abcdef"[mem[DSP] >> 4]);
   //writeChar("0123456789abcdef"[mem[DSP] & 15]);
   //writeChar(32);
   mem[DSP] = 0;
    }
}
  // if (f-- == 0) {
  //   if ((millis() - frameClockTS) >= FRAMETIME) {
  //     if (hasDisplayUpdate > 0) {
  //       swapDisplay();
  //       hasDisplayUpdate = 0;
  //     }
  //     frameClockTS = millis();
  //   }
  //   f = 7500;
//   }
// }

void loop2() {
    gpio_init(20);
    gpio_set_dir(20, GPIO_OUT);
    while (1) {
        // gpio_put(20, 0);
        sleep_ms(50);
        tuh_task();
        gpio_put(20, 1);
        // sleep_ms(50);
        writeCharacter('.');
        tuh_task();
    }
}

int main() {
  sleep_ms(2000);

	startVideo();
	board_init();
	tusb_init();

  initmemory();
  init6502();
  loop2();
}
