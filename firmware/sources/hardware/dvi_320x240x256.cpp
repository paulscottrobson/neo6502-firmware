// ***************************************************************************************
// ***************************************************************************************
//
//	  Name :	  usbdriver.cpp
//	  Authors :   Paul Robson (paul@robsons.org.uk)
//				  Luke Wren (PicoDVI Library)
//				  Phillip Burgess (adafruit extensions used to understand it)
//	  Date :	  20th November 2023
//	  Reviewed :  No
//	  Purpose :   Drive the 65C02 processor
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "system/dvi_video.h"

#include "pico/multicore.h"
#include "hardware/vreg.h"
#include "dvi.h"
#include "dvi_serialiser.h"
#include "system/common_dvi_pin_configs.h"
#include "hardware/structs/bus_ctrl.h"

// ***************************************************************************************
//
//								Configuration for DVI
//
// ***************************************************************************************

// DVDD 1.2V (1.1V seems ok too)
#define FRAME_WIDTH 320
#define FRAME_HEIGHT 240
#define VREG_VSEL VREG_VOLTAGE_1_20

// ***************************************************************************************
//
//								  DVI Generation Data
//
// ***************************************************************************************

struct dvi_inst dvi0;								   							// DVI information structure

uint16_t palette[256];														  	// Current DVI palette (RGB565)
uint8_t  *screenMemory;															// Graphics RAM (max 320x240)
uint16_t buffer1[FRAME_WIDTH+32],buffer2[FRAME_WIDTH+32];					   	// 2 buffers for scanline used alternatively
uint16_t frameCounter = 0,lineCounter = 0;									  	// Tracking line/frame counts.

bool  isInitialised = false;													// Only start core once.

// ***************************************************************************************
//
//	   		Called every scanline to update display
//
// ***************************************************************************************

static void __not_in_flash_func(_scanline_callback)(void) {
	uint16_t *scanline;
	while (queue_try_remove_u32(&dvi0.q_colour_free, &scanline));	 			// Remove unused buffers from queue
	if (lineCounter >= 0) {				   										// Which buffer to send ?
		scanline = (lineCounter & 1) ? buffer1 : buffer2;
	}
	queue_add_blocking_u32(&dvi0.q_colour_valid, &scanline);   					// Send buffer to queue

	lineCounter++;					  // Adjust line and frame.
	if (lineCounter == FRAME_HEIGHT) {
		frameCounter++;
		lineCounter = 0;
	}
	scanline = (lineCounter & 1) ? buffer1 : buffer2;		   					// Buffer to create (e.g. the other one)
	uint8_t *screenPos = screenMemory + lineCounter * FRAME_WIDTH;	  			// Data to use in screen memory.
	for (int i = 0;i < FRAME_WIDTH;i++) {			  							// For each pixel
		*scanline++ = palette[*screenPos++];			   						// convert using palette => buffer.
	}
}

// ***************************************************************************************
//
//   	  Start core 1, which spends most of its time calling _scanline_callback
//
// ***************************************************************************************

static void __not_in_flash_func(core1_main)() {
	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_1);		   						// Enable IRQs
	dvi_start(&dvi0);						 									// Start DVI library
	dvi_scanbuf_main_16bpp(&dvi0);				  								// State we are using 16 bit (e.g. 565) render
}

// ***************************************************************************************
//
//	   		Serialiser (e.g. the pinout) for Neo6502
//
// ***************************************************************************************

static const struct dvi_serialiser_cfg pico_neo6502_cfg = {
	.pio = DVI_DEFAULT_PIO_INST,
	.sm_tmds = {0, 1, 2},
	.pins_tmds = {14, 18, 16},
	.pins_clk = 12,
	.invert_diffpairs = true
};

// ***************************************************************************************
//
//							Called on start.
//
// ***************************************************************************************

void RNDStartMode0(struct GraphicsMode *gMode) {
   screenMemory = gMode->graphicsMemory; 										// Remember where drawing.
   if (!isInitialised) DVIStart();												// Start hardware, only once (!)
   isInitialised = true;
}

// ***************************************************************************************
//
//								Get frame counter
//
// ***************************************************************************************

int  RNDGetFrameCount(void) {
	return frameCounter;
}

// ***************************************************************************************
//
//			Update the palette of whatever is generating the video
//
// ***************************************************************************************

void RNDSetPalette(uint8_t colour,uint8_t r,uint8_t g,uint8_t b) {
	palette[colour] = ((r & 0xF8) << (11-3)) + ((g & 0xFC) << (5-2)) + (b >> 3);
}

// ***************************************************************************************
//
//			Physically start the DVI hardware - called once, starts up core1.
//
// ***************************************************************************************

void DVIStart(void) {
	vreg_set_voltage(VREG_VSEL);												// Set Voltage on CPU
	sleep_ms(10);
	set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);	 						// Set the correct clock speed.

	dvi0.timing = &DVI_TIMING;					  								// Set up timing, config, callback.
	dvi0.ser_cfg = pico_neo6502_cfg;
	dvi0.scanline_callback = _scanline_callback;

	dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());// Initialise DVI. 

	lineCounter = 2;						  									// We send two lines to kick off.
	uint16_t *scanline;					   										// Send junk, only lasts one frame.
	scanline = buffer1;queue_add_blocking_u32(&dvi0.q_colour_valid, &scanline);
	scanline = buffer2;queue_add_blocking_u32(&dvi0.q_colour_valid, &scanline);
	multicore_launch_core1(core1_main);				   							// Start DVI worker core (RP2040 core 1)
}

// ***************************************************************************************
//
//		Date	 Revision
//		====	 ========
//
// ***************************************************************************************
