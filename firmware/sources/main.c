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
        gpio_put(20, 0);
        sleep_ms(50);
        tuh_task();
        gpio_put(20, 1);
        sleep_ms(50);
  		writeCharacter('.');
  		tuh_task();
    }	
}

int main() {
    initmemory();
    startVideo();    
    board_init();
    tusb_init();
    init6502();
//    loop1();
}

void sync() {
    tuh_task();
}
