// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		debugger.h
//		Purpose:	Debugger Code (System Independent) Header
//		Created:	22nd November 2023
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#ifndef _DEBUGGER_H
#define _DEBUGGER_H

void DBGVerticalLabel(int x,int y,const char *labels[],int fgr,int bgr);
void DBGDefineKey(int keyID,int gfxKey);

#include "sys_debug_system.h"

#define DBGKEY_RESET	(0)															// Debugger key IDs.
#define DBGKEY_SHOW		(1)
#define DBGKEY_STEP		(2)
#define DBGKEY_STEPOVER	(3)
#define DBGKEY_RUN		(4)
#define DBGKEY_BREAK	(5)
#define DBGKEY_HOME		(6)
#define DBGKEY_SETBREAK	(7)

#endif

