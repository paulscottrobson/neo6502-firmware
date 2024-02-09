// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		common.h	
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		20th November 2023
//		Reviewed :	No
//		Purpose :	General Include File.
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <string>

//
//		RP2040 specific includes
//
#ifdef PICO
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/gpio.h"
#endif
//
//		PC specific includes
//
#ifdef IBMPC
#include <stdint.h>
#include <stdbool.h>
#include <hardware.h>
#endif
//
//		Neo6502 Includes
//
#include "interface/keyboard.h"
#include "interface/graphics.h"
#include "interface/console.h"
#include "interface/timer.h"
#include "interface/sound.h"
#include "interface/memory.h"
#include "interface/dispatch.h"
#include "interface/maths.h"
#include "interface/fdebug.h"
#include "interface/filesystem.h"
#include "interface/config.h"
#include "interface/miscellany.h"
#include "interface/sprites.h"
#include "interface/tilemap.h"
#include "interface/serial.h"
#include "interface/turtle.h"
#include "interface/locale.h"

#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
