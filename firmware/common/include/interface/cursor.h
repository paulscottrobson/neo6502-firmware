// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      cursor.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st March 2024
//      Reviewed :  No
//      Purpose :   Mouse Cursor functionality
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _CURSOR_H
#define _CURSOR_H

void CURInitialise(void);
void CURSetPosition(uint16_t x,uint16_t y);
bool CURGetCursorDrawInformation(const uint8_t **pData,uint16_t *pX,uint16_t *pY);
void CURSetVisible(bool isVisible);

#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
