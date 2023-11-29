// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      maths.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      26th November 2023
//      Reviewed :  No
//      Purpose :   Mathematics
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

//
//		Address of register 1.
//
static uint8_t *regAddress;
//
//		Byte space between numeric data items. 5 of these complete
//		one float or integer (1 control, 4 data). This is to allow interleaved
//		stacks
//
static uint8_t readOffset;	

//
//		Converting byte groups to or from types.
//
static union _stackConvert {
	uint8_t  bytes[4];
	float 	 f;
	uint32_t i;
} sc;

// ***************************************************************************************
//
//			Read a register 1/2 into the conversion structure, returns control byte
//
// ***************************************************************************************

static uint8_t MATHRead(uint8_t offset) {
	uint8_t *pStack = regAddress + offset * readOffset * 5;
	sc.bytes[0] = pStack[readOffset*1];
	sc.bytes[1] = pStack[readOffset*2];
	sc.bytes[2] = pStack[readOffset*3];
	sc.bytes[3] = pStack[readOffset*4];
	return *pStack;
}

// ***************************************************************************************
//
//		Write conversion structure back using the control type given in register 1/2
//
// ***************************************************************************************

static void MATHWrite(uint8_t offset,uint8_t type) {
	uint8_t *pStack = regAddress + offset * readOffset * 5;
	pStack[0] = type;
	pStack[readOffset*1] = sc.bytes[0];
	pStack[readOffset*2] = sc.bytes[1];
	pStack[readOffset*3] = sc.bytes[2];
	pStack[readOffset*4] = sc.bytes[3];
}

// ***************************************************************************************
//
//				Helper functions to read/write math values to registers
//
// ***************************************************************************************

float MATHReadFloat(uint8_t stackOffset) {
	uint8_t type = MATHRead(stackOffset);
	return (type & 0x40) ? sc.f : (float)sc.i;
}

uint32_t MATHReadInt(uint8_t stackOffset) {
	uint8_t type = MATHRead(stackOffset);
	return (type & 0x40) ? (uint32_t)sc.f : sc.i ;
}

void MATHWriteFloat(float f,uint8_t stackOffset) {
	sc.f = f;
	MATHWrite(stackOffset,0x40);
	printf("Written %f f\n",f);
}

void MATHWriteInt(uint32_t i,uint8_t stackOffset) {
	sc.i = i;
	MATHWrite(stackOffset,0x00);
	printf("Written %d i\n",(int)i);
}

// ***************************************************************************************
//
//		Check Reg1 float - used for functs that have int/float commonalities like abs
//
// ***************************************************************************************

bool MATHIsFloatUnary(void) {
	return (*regAddress & 0x40) != 0;
}

// ***************************************************************************************
//
//					Common code execute for all math operations
//
// ***************************************************************************************

void MATHCommon(uint8_t *params) {
	regAddress = cpuMemory + (*params) + (params[1] << 8);
	readOffset = params[2];
}

// ***************************************************************************************
//
//				Process [!!DEC] [length] [BCD Packed decimals ending with $F]
//
// ***************************************************************************************

void MATHProcessDecimal(uint8_t *command) {
	float f = MATHReadFloat(MATH_REG1);  										// Current value as a float.
	uint8_t *mem = cpuMemory + command[8] + (command[9] << 8); 					// From here.
	uint8_t done = false;
	uint32_t decimal = 0,decimalDivide = 1;  									// Store dec as int, divide by 10^n at the end.
	while (!done) {  													
		uint8_t nibble = (*mem) >> 4; 											// Upper nibble
		if (nibble != 0x0F) {
			decimal = decimal * 10 + nibble;
			decimalDivide *= 10;
		}
		nibble = (*mem & 0x0F);  												// Lower nibble
		if (nibble != 0x0F) {
			decimal = decimal * 10 + nibble;
			decimalDivide *= 10;
		}
		mem++;
		done = (nibble == 0x0F);
	}
	f += ((float)decimal / decimalDivide);  									// Work out result and store it.
	MATHWriteFloat(f,MATH_REG1);
}



// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
