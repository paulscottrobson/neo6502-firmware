// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      maths.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      26th November 2023
//      Reviewed :  No
//      Purpose :   Mathematics
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _MATHS_H
#define _MATHS_H

#define MATH_REG1 	(0)
#define MATH_REG2 	(1)

void MATHCommon(uint8_t *params);
void MATHProcessDecimal(uint8_t *params);
float MATHReadFloat(uint8_t stackOffset);
uint32_t MATHReadInt(uint8_t stackOffset);
#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
