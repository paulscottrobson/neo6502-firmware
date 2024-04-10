// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sweet16.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      10th April 2024
//      Reviewed :  No
//      Purpose :   Sweet 16 virtual machine
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

static uint16_t *registers;
static uint16_t *pctr,*r14;

// ***************************************************************************************
//
//							  Helper macros/functions
//
// ***************************************************************************************

#define R(r)		registers[r]
#define FETCH8() 	(cpuMemory[(*pctr)++])
#define FETCH16() 	_SWFetch16();
#define SETCOMP(r) 	*r14 = (*r14 & 0xF0) | (r << 8)

static inline uint16_t _SWFetch16(void) {
	uint8_t v = FETCH8();
	return v | (FETCH8() << 8);
}

// ***************************************************************************************
//
//							Dump Registers (if not zero)
//
// ***************************************************************************************

static void _SW16Status() {
	for (int i = 0;i < 16;i++) {
		if (registers[i] != 0) printf("R%-2d: $%04x %d\n",i,registers[i],registers[i]);
	}
	printf("\n");
}

// ***************************************************************************************
//
//		Execute sweet 16 code. Returns true to exit, false to yield (for Windows)
//
// ***************************************************************************************

bool SW16Execute(uint16_t *reg) {
	bool bQuitSweet = false;
	bool bYieldSweet = false;
	registers = reg;
	pctr = &reg[15];
	r14 = &reg[14];
	_SW16Status();

	while (!bQuitSweet &&  !bYieldSweet) {
		switch(FETCH8()) {
			#include "data/sweet_opcodes.h"
		}
		_SW16Status();
	}
	return true;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
