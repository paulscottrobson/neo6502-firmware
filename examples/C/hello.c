/*\
|*| Hello World C example for the Neo6502
|*| SPDX-License-Identifier: CC0-1.0
|*|
|*| This is a very basic C program,
|*| which sounds a beep and displays some text at a precise screen location.
|*| Refer to api.pdf for more API functions.
|*|
|*| To compile and run:
|*|   $ export CC65_HOME=/usr/share/cc65
|*|   $ ca65 crt0.s
|*|   $ ar65 a neo6502.lib crt0.o
|*|   $ cl65 --static-locals -t none -C neo6502.cfg -O --cpu 65c02 -m hello.map \
|*|          -o hello.neo hello.c neo6502.lib
|*|   $ ../../bin/neo hello.neo@800 cold
\*/


// CC65 C libs
#include <stdio.h>
#include <stdint.h>

// Neo6502 Kernel API convenience macros
#include "neo6502.h"


int main()
{
  SoundEffect(API_SOUND_CH_00 , API_SFX_COIN) ;

  SetCursorPosition(0 , 22) ;
  puts("                    Hello world!") ;
  puts("                                                     ") ; // 53 blanks

  return 0 ;
}
