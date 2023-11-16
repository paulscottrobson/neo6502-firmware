// 8-bit Adafruit_GFX-compatible framebuffer for PicoDVI.

#include "Arduino.h"
#include <PicoDVI.h>

 // Pico HDMI for Olimex Neo6502 
static const struct dvi_serialiser_cfg pico_neo6502_cfg = {
   .pio = DVI_DEFAULT_PIO_INST,
   .sm_tmds = {0, 1, 2},
   .pins_tmds = {14, 18, 16},
   .pins_clk = 12,
   .invert_diffpairs = true
};

// Here's how a 320x240 8-bit (color-paletted) framebuffer is declared.
// Second argument ('false' here) means NO double-buffering; all drawing
// operations are shown as they occur. Third argument is a hardware
// configuration -- examples are written for Adafruit Feather RP2040 DVI,
// but that's easily switched out for boards like the Pimoroni Pico DV
// (use 'pimoroni_demo_hdmi_cfg') or Pico DVI Sock ('pico_sock_cfg').
DVIGFX8 display(DVI_RES_320x240p60, false, pico_neo6502_cfg);

// A 400x240 mode is possible but pushes overclocking even higher than
// 320x240 mode. SOME BOARDS MIGHT SIMPLY NOT BE COMPATIBLE WITH THIS.
// May require selecting QSPI div4 clock (Tools menu) to slow down flash
// accesses, may require further over-volting the CPU to 1.25 or 1.3 V.
//DVIGFX8 display(DVI_RES_400x240p60, false, adafruit_feather_dvi_cfg);

void setup() { // Runs once on startup
  if (!display.begin()) { // Blink LED if insufficient RAM
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }

  // Randomize color palette. First entry is left black, last is set white.
  for (int i=1; i<255; i++) display.setColor(i, random(65536));
  display.setColor(255, 0xFFFF);
}

int n = 0;

void loop() {
  // Draw random lines
  display.drawLine(random(display.width()), random(display.height()), random(display.width()), random(display.height()),random(256));
 // display.fillRect(0,0,64,32,0);
  display.setTextColor(0,255);
  display.setCursor(0,0);
  display.print(n++);
}

