#include "common.h"
#include "bsp/board.h"
#include "tusb.h"

int startVideo();
void writeCharacter(int n);

int main() {
    initmemory();
    startVideo();    
    board_init();
    tusb_init();
    init6502();
}

void sync() {
    tuh_task();
}
