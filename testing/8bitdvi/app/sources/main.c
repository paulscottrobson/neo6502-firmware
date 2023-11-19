#include "hardware/gpio.h"
#include "bsp/board.h"
#include "tusb.h"

int startVideo();
void writeCharacter(int n);

int main() {
	startVideo();
	// board_init();
	// tusb_init();
    // gpio_init(20);
    // gpio_set_dir(20, GPIO_OUT);
    // while (1) {
  	// 	sleep_ms(50);
  	// 	writeCharacter('.');
  	// 	tuh_task();
    // }	
}
