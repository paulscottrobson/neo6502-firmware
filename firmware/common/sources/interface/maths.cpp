// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      maths.cpp
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
static union _regConvert {
	uint8_t  bytes[4];
	float 	 f;
	uint32_t i;
} sc;

//
//		Degrees / Radians flag.
//
static bool bUseDegrees = true;

//
//		Map offset to actual register start address (Placeholder)
//
#define REGADDR(ofst) (regAddress+(ofst))

// ***************************************************************************************
//
//								Handle degrees and radians
//
// ***************************************************************************************

float MATHConvertAngleToDefault(float angle) {
	return bUseDegrees ? angle * M_PI / 180.0 : angle;
}

float MATHConvertAngleFromDefault(float angle) {
	return bUseDegrees ? angle * 180.0 / M_PI : angle;
}

void MATHSetAngleMeasure(bool bDegrees) {
	bUseDegrees = bDegrees;
}

// ***************************************************************************************
//
//			Read a register 1/2 into the conversion structure, returns control byte
//
// ***************************************************************************************

static uint8_t MATHRead(uint8_t offset) {
	uint8_t *pReg = REGADDR(offset);
	sc.bytes[0] = pReg[readOffset*1];
	sc.bytes[1] = pReg[readOffset*2];
	sc.bytes[2] = pReg[readOffset*3];
	sc.bytes[3] = pReg[readOffset*4];
	return *pReg;
}

// ***************************************************************************************
//
//		Write conversion structure back using the control type given in register 1/2
//
// ***************************************************************************************

static void MATHWrite(uint8_t offset,uint8_t type) {
	uint8_t *pReg = REGADDR(offset);
	pReg[0] = type;
	pReg[readOffset*1] = sc.bytes[0];
	pReg[readOffset*2] = sc.bytes[1];
	pReg[readOffset*3] = sc.bytes[2];
	pReg[readOffset*4] = sc.bytes[3];
}

// ***************************************************************************************
//
//				Helper functions to read/write math values to registers
//
// ***************************************************************************************

float MATHReadFloat(uint8_t regOffset) {
	uint8_t type = MATHRead(regOffset);
	return (type & 0x40) ? sc.f : (float)(int)sc.i;
}

int32_t MATHReadInt(uint8_t regOffset) {
	uint8_t type = MATHRead(regOffset);
	return (type & 0x40) ? (int)sc.f : sc.i ;
}

void MATHWriteFloat(float f,uint8_t regOffset) {
	sc.f = f;
	MATHWrite(regOffset,0x40);
	//printf("Written %f f\n",f);
}

void MATHWriteInt(uint32_t i,uint8_t regOffset) {
	sc.i = i;
	MATHWrite(regOffset,0x00);
	//printf("Written %d i\n",(int)i);
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
//				Check Reg1 or Reg2 float - if so, use float arithmetic
//
// ***************************************************************************************

bool MATHIsFloatBinary(void) {
	return ((*REGADDR(MATH_REG1)|(*REGADDR(MATH_REG2))) & 0x40) != 0;
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
//								Convert string to number
//
// ***************************************************************************************

static bool _MATHStringToFloatChecked(const char *szBuffer,float *pfResult) {
	char *endPtr;
    *pfResult = (float)(strtod(szBuffer, &endPtr));
    return endPtr != szBuffer && *endPtr == '\0';
}

void MATHConvertStringToNumber(uint8_t *command) {
	char szBuffer[32];	
	float f1;
	uint8_t *mem = cpuMemory + command[8] + (command[9] << 8); 					// From here.
	memcpy(szBuffer,mem+1,*mem);												// Make ASCIIZ string
	szBuffer[*mem] = '\0';
	if (_MATHStringToFloatChecked(szBuffer,&f1)) {  							// Try conversion
		command[2] = 0;
		if (f1 != floor(f1)) {
			MATHWriteFloat(f1,MATH_REG1);
		} else {
			MATHWriteInt((int)f1,MATH_REG1);			
		}
	} else {
		command[2] = 1;
	}
}

// ***************************************************************************************
//
//								Convert number to string
//
// ***************************************************************************************

void MATHConvertNumberToString(uint8_t *command) {
	uint8_t *mem = cpuMemory + command[8] + (command[9] << 8); 					// To here.
	if (MATHIsFloatUnary()) {  													// Convert accordingly
		sprintf((char *)(mem+1),"%f",MATHReadFloat(MATH_REG1));
	} else {
		sprintf((char *)(mem+1),"%d",(int)MATHReadInt(MATH_REG1));
	}
	mem[0] = strlen((char *)(mem+1)); 													// Length of string.
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
