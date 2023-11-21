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
#ifndef _COMMON_h
#define _COMMON_h
//
//		RP2040 includes
//
#include "pico/stdlib.h"
#include "hardware/gpio.h"
//
//		Neo6502 Includes
//
#include "system/memory.h"
#include "system/processor.h"
#include "system/usbdriver.h"
#include "system/dvi_video.h"
#endif
//
//		Void Function Pointer.
//
typedef void (*VOIDFUNC)();