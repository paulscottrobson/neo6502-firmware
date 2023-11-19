#include "hardware/gpio.h"
#include "bsp/board.h"
#include "tusb.h"
#include "memory.h"

int startVideo();
void writeCharacter(int n);

void loop1() {
    gpio_init(20);
    gpio_set_dir(20, GPIO_OUT);
    while (1) {
        // gpio_put(20, 0);
        sleep_ms(50);
        tuh_task();
        gpio_put(20, 1);
        // sleep_ms(50);
  		//writeCharacter('.');
  		tuh_task();
    }	
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

int main() {
    initmemory();
    startVideo();    
    board_init();
    tusb_init();
    init6502();
    loop1();
}
