#include "pico/stdlib.h"
#include "pico/multicore.h"
// #include "hardware/clocks.h"
// #include "hardware/irq.h"
// #include "hardware/sync.h"
// #include "hardware/gpio.h"
#include "hardware/vreg.h"

#include "dvi.h"
#include "dvi_serialiser.h"
#include "common_dvi_pin_configs.h"

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

uint16_t palette[256];
uint8_t screen[FRAME_WIDTH * FRAME_HEIGHT];

uint16_t buffer1[FRAME_WIDTH],buffer2[FRAME_WIDTH];
uint frameCounter = 0,lineCounter = 0;

#include "font_5x7.h"

void drawCharacter(int x,int y,int ch,int col) {
	for (int y1 = 0;y1 < 7;y1++) {
		int b = font_5x7[ch*7 + y1];
		for (int x1 = 0;x1 < 5;x1++) {
			int p = (x+x1)+(y+y1)*320;
			screen[p] = (b & 0x80) ? col : 0;
			b = b << 1;
		}
	}
}

int xc = 0,yc = 0;
void writeCharacter(int c) {
	drawCharacter(xc*6+5,yc*8,c,7);
	xc += 1;
	if (xc == 52) { xc = 0;yc = (yc + 1) % 30; }
}

void __not_in_flash_func(_scanline_callback)(void) {
	uint16_t *scanline;
	while (queue_try_remove_u32(&dvi0.q_colour_free, &scanline));
	if (lineCounter >= 0) {
		scanline = (lineCounter & 1) ? buffer1 : buffer2;
	}
	queue_add_blocking_u32(&dvi0.q_colour_valid, &scanline);
	lineCounter++;
	if (lineCounter == FRAME_HEIGHT) {
	 	frameCounter++;
	 	lineCounter = 0;
	}
	scanline = (lineCounter & 1) ? buffer1 : buffer2;
	uint8_t *screenPos = screen + lineCounter * FRAME_WIDTH;
	for (int i = 0;i < FRAME_WIDTH;i++) {
		scanline[i] = palette[screenPos[i]];
	}
}

static const struct dvi_serialiser_cfg pico_neo6502_cfg = {
   .pio = DVI_DEFAULT_PIO_INST,
   .sm_tmds = {0, 1, 2},
   .pins_tmds = {14, 18, 16},
   .pins_clk = 12,
   .invert_diffpairs = true
};

int startVideo() {
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

	// Pass out pointers into our preprepared image, discard the pointers when
	// returned to us. Use frameCounter to scroll the image

	for (int i = 0;i < 256;i++) {
		int p = 0;
		if (i & 1) p |= 0xF800;
		if (i & 2) p |= 0x07E0;
		if (i & 4) p |= 0x001F;
		palette[i] = p;
	}

	for (int x = 0;x < 320;x++) {
		for (int y = 0;y < 240;y++) {
			screen[x+y*320] = (x >> 3) + (y >> 3) * 3;
		}
	}

	lineCounter = 0;
	_scanline_callback();
	multicore_launch_core1(core1_main);

	return 0;
}

