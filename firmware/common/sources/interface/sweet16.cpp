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

// ***************************************************************************************
//
//						  Register values during emulation
//
// ***************************************************************************************

static uint16_t sweet_reg[16];

// ***************************************************************************************
//
//							  Helper macros/functions
//
// ***************************************************************************************

#define R(r)		sweet_reg[r]

#define FETCH8() 	cpuMemory[sweet_reg[15]++]
#define FETCH16() 	_SWFetch16();

#define READ8(a)  	cpuMemory[a]
#define READ16(a)  	(READ8(a) | (READ8((a)+1) << 8))

#define WRITE8(a,d)  _SWWrite8(a,d & 0xFF)
#define WRITE16(a,d) { WRITE8(a,d);WRITE8((a)+1,(d) >> 8); }

#define SETCOMP(r) 	sweet_reg[14] = (sweet_reg[14] & 0x00FF) | (r << 8)
#define CLEARCARRY() sweet_reg[14] &= 0xFFFE
#define CARRY()   	(sweet_reg[14] & 0x1)
#define TESTVALUE()  sweet_reg[(sweet_reg[14] >> 8) & 0x0F]

#define BRANCHIF(t) _SW16ConditionalBranch(t)

#define ADD16(a,b,c) _SWAdd16(a,b,c)

//
//		8 bit write with system trigger.
//
static void _SWWrite8(uint16_t a,uint8_t d) {
	cpuMemory[a] = d;
	if (a == controlPort) DSPHandler(cpuMemory + controlPort, cpuMemory);
}
//
//		16 bit fetch
//
static inline uint16_t _SWFetch16(void) {
	uint8_t v = FETCH8();
	return v | (FETCH8() << 8);
}

//
//		16 bit 2's complement addition with carry in, used for add & subtract
//
static inline uint16_t _SWAdd16(uint16_t a,uint16_t b,uint16_t c) {
	uint32_t result = a + b + c;
	if (result & 0x10000) {
		sweet_reg[14] |= 0x0001;
	} else {
		sweet_reg[14] &= 0xFFFE;
	}
	return result & 0xFFFF;
}

//
//		Short (e.g. 8 bit) conditional branch, same as 65C02 basically.
//
static inline void _SW16ConditionalBranch(bool test) {
	if (test != 0) {
		uint16_t offset = FETCH8();
		if (offset & 0x80) offset |= 0xFF00;
		sweet_reg[15] += offset;
	} else {
		sweet_reg[15]++;
	}
}

// ***************************************************************************************
//
//							Dump Registers (if not zero)
//
// ***************************************************************************************

static void _SW16Status() {
 	for (int i = 0;i < 16;i++) {
 		if (sweet_reg[i] != 0) {
 			CONWriteString("R%-2d: $%04x (%d) $%02x\r",i,sweet_reg[i],sweet_reg[i],cpuMemory[sweet_reg[i]]);
 		}
 	}
}

// ***************************************************************************************
//
//		Execute sweet 16 code. Returns true to exit, false to yield (for Windows)
//
// ***************************************************************************************

bool SW16Execute(uint16_t reg) {
	bool bQuitSweet = false;  													// Set by RTN.
	int32_t yieldCounter = 6000000/50;  										// 6 MIPS , 50 frames per second.	
	uint16_t temp;

	for (int i = 0;i < 16;i++) {  												// Copy cpu memory to working registers
		sweet_reg[i] = cpuMemory[reg+i*2]+(cpuMemory[reg+i*2+1] << 8);
	}
//	_SW16Status();
 	while (!bQuitSweet && yieldCounter-- > 0) {  								// Keep executing till quit or yield time.
 		switch(FETCH8()) {
 			#include "data/sweet_opcodes.h"
 		}
//	_SW16Status();
 	}
	for (int i = 0;i < 16;i++) {  												// Copy working registers to CPU memory
		cpuMemory[reg+i*2] = sweet_reg[i] & 0xFF;
		cpuMemory[reg+i*2+1] = sweet_reg[i] >> 8;
	}
	return bQuitSweet;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
