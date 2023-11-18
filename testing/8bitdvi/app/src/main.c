#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"

#include "dvi.h"
#include "dvi_serialiser.h"
#include "common_dvi_pin_configs.h"

//#include "testcard_320x240_rgb565.h"

// DVDD 1.2V (1.1V seems ok too)
#define FRAME_WIDTH 320
#define FRAME_HEIGHT 240
#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_640x480p_60hz

struct dvi_inst dvi0;

void __not_in_flash_func(core1_main)() {
	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
	dvi_start(&dvi0);
	dvi_scanbuf_main_16bpp(&dvi0);
}

uint8_t screen[FRAME_WIDTH * FRAME_HEIGHT];

uint16_t buffer1[FRAME_WIDTH],buffer2[FRAME_WIDTH];
	uint frameCounter = 0,lineCounter;

void __not_in_flash_func(_scanline_callback)(void) {
	uint16_t *scanline;
	while (queue_try_remove_u32(&dvi0.q_colour_free, &scanline));
	scanline = (lineCounter & 1) ? buffer1 : buffer2;
	queue_add_blocking_u32(&dvi0.q_colour_valid, &scanline);
	lineCounter++;
	scanline = (lineCounter & 1) ? buffer1 : buffer2;
	for (int i = 0;i < FRAME_WIDTH;i++) {
		scanline[i] = (i >> 2)+(lineCounter << 6);
	}
	if (lineCounter == FRAME_HEIGHT) {
		frameCounter++;
		lineCounter = 0;
	}
}

static const struct dvi_serialiser_cfg pico_neo6502_cfg = {
   .pio = DVI_DEFAULT_PIO_INST,
   .sm_tmds = {0, 1, 2},
   .pins_tmds = {14, 18, 16},
   .pins_clk = 12,
   .invert_diffpairs = true
};

int main() {
	vreg_set_voltage(VREG_VSEL);
	sleep_ms(10);
	set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);

	setup_default_uart();

	dvi0.timing = &DVI_TIMING;
	dvi0.ser_cfg = pico_neo6502_cfg;
	dvi0.scanline_callback = _scanline_callback;

	dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

	// Core 1 will wait until it sees the first colour buffer, then start up the
	// DVI signalling.
	multicore_launch_core1(core1_main);

	// Pass out pointers into our preprepared image, discard the pointers when
	// returned to us. Use frameCounter to scroll the image

	uint16_t *scanline = buffer1;
	lineCounter = 1;
	queue_add_blocking_u32(&dvi0.q_colour_valid, &scanline);

    gpio_init(20);
    gpio_set_dir(20, GPIO_OUT);
    while (1) {
        gpio_put(20, 0);
        sleep_ms(250);
        gpio_put(20, 1);
        sleep_ms(100);
    }	
}
