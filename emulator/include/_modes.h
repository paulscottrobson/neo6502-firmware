// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		modes.h
//		Purpose:	Modes header file
//		Created:	22nd November 2023
//		Author:		Paul Robson (paul@robsons->org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#ifndef _MODES_H
#define _MODES_H

#define MODEHANDLER(c,a,d) HWCallHandler(c,a,d)

int HWCallHandler(int cmd,int address,int data);

void mode0WriteChar(int x,int y,char c,int reverse);
int mode0Handler(int cmd,int address,int data);
int mode3Handler(int cmd,int address,int data);

#endif

