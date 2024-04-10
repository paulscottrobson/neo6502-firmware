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

#define READ8(a)  	cpuMemory[a]
#define READ16(a)  	(READ8(a) | (READ8((a)+1) << 8))

#define WRITE8(a,d) cpuMemory[a] = (d) &0xFF
#define WRITE16(a,d) { WRITE8(a,d);WRITE8((a)+1,(d) >> 8); }

#define SETCOMP(r) 	*r14 = (*r14 & 0x00FF) | (r << 8)
#define CLEARCARRY() *r14 &= 0xFFFE
#define CARRY()   	((*r14) & 0x1)
#define TESTVALUE()  registers[(*r14) >> 8]

#define BRANCHIF(t) _SW16ConditionalBranch(t)

#define ADD16(a,b,c) _SWAdd16(a,b,c)

static inline uint16_t _SWFetch16(void) {
	uint8_t v = FETCH8();
	return v | (FETCH8() << 8);
}

static inline uint16_t _SWAdd16(uint16_t a,uint16_t b,uint16_t c) {
	uint32_t result = a + b + c;
	if (result & 0x10000) {
		*r14 |= 0x0001;
	} else {
		*r14 &= 0xFFFE;
	}
	return result & 0xFFFF;
}

static inline void _SW16ConditionalBranch(bool test) {
	if (test != 0) {
		uint16_t offset = FETCH8();
		if (offset & 0x80) offset |= 0xFF00;
		*pctr = (*pctr) + offset;
	} else {
		(*pctr)++;
	}
}

// ***************************************************************************************
//
//							Dump Registers (if not zero)
//
// ***************************************************************************************

static void _SW16Status() {
	for (int i = 0;i < 16;i++) {
		if (registers[i] != 0) printf("R%-2d: $%04x (%d) $%02x\n",i,registers[i],registers[i],cpuMemory[registers[i]]);
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
	uint16_t temp;
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
