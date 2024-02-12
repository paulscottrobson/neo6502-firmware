/*\
|*| Hello World C example for the Neo6502
|*| SPDX-License-Identifier: CC0-1.0
|*|
|*| This is a very basic C program,
|*| which sounds a beep and displays some text at a precise screen location.
|*| Refer to documents/release/api.pdf for more API functions.
|*|
|*| To compile and run:
|*|   $ export CC65_HOME=/usr/share/cc65
|*|   $ ca65 crt0.s
|*|   $ ar65 a neo6502.lib crt0.o
|*|   $ cl65 --static-locals -C neo6502.cfg -O --cpu 65c02 -m hello.map \
|*|          -o hello.neo hello.c neo6502.lib
|*|   $ ../../bin/neo hello.neo@800 cold
\*/


// CC65 C libs
#include <stdio.h>
#include <stdint.h>

// Neo6502 Kernel API convenience macros
#include "neo6502.h"


/*\
|*| Play sound effect - (API Group 8, Function 5)
\*/
void SoundEffect(uint8_t sound_ch , uint8_t sound_effect)
{
  *API_FUNCTION_ADDR     = API_FN_PLAY_SOUND ;
  API_PARAMETERS_ADDR[0] = sound_ch          ;
  API_PARAMETERS_ADDR[1] = sound_effect      ;
  *API_COMMAND_ADDR      = API_GROUP_SOUND   ;
}

/*\
|*| Set cursor position - (API Group 2, Function 7)
\*/
void SetCursorPosition(uint8_t x_pos , uint8_t y_pos)
{
  *API_FUNCTION_ADDR     = API_FN_CURSOR_POS ;
  API_PARAMETERS_ADDR[0] = x_pos             ;
  API_PARAMETERS_ADDR[1] = y_pos             ;
  *API_COMMAND_ADDR      = API_GROUP_CONSOLE ;
}

/*\
|*| Write character to console - (API Group 2, Function 6)
\*/
int write(int /* fildes */ , const unsigned char* buf , unsigned count)
{
  while (count--)
  {
    while(*API_COMMAND_ADDR) {}

    *API_FUNCTION_ADDR   = API_FN_WRITE_CHAR ;
    *API_PARAMETERS_ADDR = *buf++            ;
    *API_COMMAND_ADDR    = API_GROUP_CONSOLE ;
  }

  return 0 ;
}

int demo_bug()
{
  unsigned char buf[33] ;
  uint8_t char_n ;

  SetCursorPosition(0 , 0) ;
  puts("->these should be lower-case<-") ; /* BUG: displays all CAPS */
  puts("->THESE SHOULD BE UPPER-CASE<-") ; /* BUG: displays nothing  */

  buf[0] = 'a'  ; buf[1] = 'A'  ; buf[2] = '\0' ; puts(buf) ;
  buf[0] = 65   ; buf[1] = 97   ; buf[2] = '\0' ; puts(buf) ;
  buf[0] = 0x41 ; buf[1] = 0x61 ; buf[2] = '\0' ; puts(buf) ;

  for (char_n = 32 ; char_n < 64  ; ++char_n) buf[char_n - 32] = char_n ;
  buf[32] = '\0' ; puts(buf) ;
  for (char_n = 64 ; char_n < 96  ; ++char_n) buf[char_n - 64] = char_n ;
  buf[32] = '\0' ; puts(buf) ;
  for (char_n = 96 ; char_n < 128 ; ++char_n) buf[char_n - 96] = char_n ;
  buf[32] = '\0' ; puts(buf) ;
}

int main()
{
  SoundEffect(API_SOUND_CH_00 , API_SOUND_EFFECT_06) ;

  SetCursorPosition(0 , 22) ;
  puts("                   Hello world!") ;
  puts("                                                    ") ; // 52 blanks


  // BUG: https://github.com/paulscottrobson/neo6502-firmware/issues/98
  //      the above example is the one we want.
  //      However, upper-case alpha chars are not displayed
  //      (or the FG color is the same as the BG color).
  //      Also, lower-case alpha chars are displayed as upper-case.
  //      This is the case only when literal chars and strings are
  //      converted to bytes. The entire character set is accessible
  //      by filling a stored array with raw byes or ints.
  demo_bug() ;

  return 0 ;
}
