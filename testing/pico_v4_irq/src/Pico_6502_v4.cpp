/*
 Name:		Pico_NEO6502
 Author:	Rien Matthijsse

 Inspired by work  http://www.8bitforce.com/blog/2019/03/12/retroshield-6502-operation/
*/

#include "pico/stdio.h"

#include "pico/stdlib.h"
#include "pico/time.h"

#define  UCASE_ONLY

#include <PicoDVI.h>

#include "mos65C02.h"
#include "memory.h"

#define FRAMERATE       10 // frames per sec

#define FRAMETIME     1000 / FRAMERATE  // msec
#define WIDTH          320
#define HEIGHT         240
#define FONT_CHAR_WIDTH  6  
#define FONT_CHAR_HEIGHT 8 
#define LINES       HEIGHT / (FONT_CHAR_HEIGHT + 1)
#define LINECHARS   WIDTH / FONT_CHAR_WIDTH

// Here's how an 320x240 256 colors graphics display is declared.
DVIGFX8 display(DVI_RES_320x240p60, true, pico_neo6502_cfg);

//
extern uint32_t       clockCount;

unsigned long  lastClockTS;
unsigned long  frameClockTS;

uint32_t       hasDisplayUpdate = 0;

inline __attribute__((always_inline))

void displayWrite(uint8_t c) {
  int16_t cursor_x, cursor_y;

  cursor_x = display.getCursorX();
  cursor_y = display.getCursorY();

  //  ();
  if (c == '\n') { // Carriage return
    cursor_x = 0;
  }
  else if ((c == '\r') || (cursor_x >= WIDTH)) { // Newline OR right edge
    cursor_x = 0;
    if (cursor_y >= (HEIGHT - 9)) { // Vert scroll?
      memmove(display.getBuffer(), display.getBuffer() + WIDTH * (FONT_CHAR_HEIGHT + 1), WIDTH * (HEIGHT - (FONT_CHAR_HEIGHT + 1)));
      display.drawFastHLine(0, HEIGHT - 9, WIDTH, 0); // Clear bottom line
      display.fillRect(0, HEIGHT - 9, WIDTH, FONT_CHAR_HEIGHT + 1, 0);

      cursor_y = HEIGHT - 9;
    }
    else {
      cursor_y += FONT_CHAR_HEIGHT + 1;
    }

    display.setCursor(cursor_x, cursor_y);
  }

  switch (c) {
  case '\r':
  case '\n':
    break;
  case 0x08:
    display.fillRect(cursor_x - FONT_CHAR_WIDTH, cursor_y, FONT_CHAR_WIDTH, FONT_CHAR_HEIGHT, 0);
    display.setCursor(cursor_x - FONT_CHAR_WIDTH, cursor_y);
    break;
  case 0x20:
    display.fillRect(cursor_x, cursor_y, FONT_CHAR_WIDTH, FONT_CHAR_HEIGHT, 0);
    display.setCursor(cursor_x + FONT_CHAR_WIDTH, cursor_y);
    break;
  default:
    display.setTextColor(6);
    display.write(c);
    break;
  }
  //setCursor();

  hasDisplayUpdate++;
}


void writeChar(uint8_t vChar) {
//  Serial.printf("out [%02X]\n", vChar);

  displayWrite(vChar);
  hasDisplayUpdate++;
}

///
void initDisplay() {
  display.setColor(0, 0x0000);   // Black
  display.setColor(1, 0XF800);   // Red
  display.setColor(2, 0x07e0);   // Green
  display.setColor(3, 0xffe0);   // Yellow
  display.setColor(4, 0x001f);   // Blue
  display.setColor(5, 0xFA80);   // Orange
  display.setColor(6, 0xF8F9);   // Magenta
  display.setColor(255, 0xFFFF); // Last palette entry = White
  // Clear back framebuffer
  display.fillScreen(0);
  display.setFont();             // Use default font
  display.setCursor(0, 0);       // Initial cursor position
  display.setTextSize(1);        // Default size
  display.setTextWrap(false);
  display.swap(false, true);     // Duplicate same palette into front & back buffers
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// setup emulator
/// </summary>
void setup() {
  Serial.begin(115200);
  //  while (!Serial);

  sleep_ms(5000);
  Serial.println("NEO6502 memulator v0.021a");

  if (!display.begin()) {
    Serial.println("ERROR: not enough RAM available");
    for (;;);
  }

  Serial.printf("Running at %dMHz\n", clock_get_hz(clk_sys)/1000000);


  Serial.printf("Starting ...\n");

  initDisplay();

  initmemory();
  init6502();
  //reset6502();

  clockCount = 0UL;
  lastClockTS = millis();
  display.print("NEO6502");
  display.println(" RetroComputer v0.001a");

  hasDisplayUpdate++;
}

/*
  SerialEvent occurs whenever a new data comes in the
  hardware serial RX. Multiple bytes of data may be available.
 */
inline __attribute__((always_inline))
void serialEvent1()
{
  if (mem[DSP] != 0x00) {
    writeChar(mem[DSP] & 0x7F);
    mem[DSP] = 0x00; // we're done
  }
}

/// <summary>
/// 
/// </summary>
void loop() {
  static uint32_t i, f = 1;

  serialEvent1();

  if (f-- == 0) {
    if ((millis() - frameClockTS) >= FRAMETIME) {
      if (hasDisplayUpdate > 0) {
        display.swap(true, false);
        hasDisplayUpdate = 0;
      }

      frameClockTS = millis();
    }

    f = 7500;
  }
}
