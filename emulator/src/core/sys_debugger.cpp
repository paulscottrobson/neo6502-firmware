// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		sys_debugger.c
//		Purpose:	Debugger Code (System Dependent)
//		Created:	22nd November 2023
//		Author:		Paul Robson (paul@robsons->org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gfx.h"
#include "sys_processor.h"
#include "debugger.h"
#include "hardware.h"

#include "common.h"

#include "6502/__6502mnemonics.h"

#define DBGC_ADDRESS 	(0x0F0)														// Colour scheme.
#define DBGC_DATA 		(0x0FF)														// (Background is in main.c)
#define DBGC_HIGHLIGHT 	(0xFF0)

static int renderCount = 0;
static BYTE8 *videoRAM = NULL;														// VRAM simple pattern.
static BYTE8 *isExtArray = NULL;
static uint16_t palette[256]; 														// Palette

// *******************************************************************************************************************************
//
//											Handle palette changes
//
// *******************************************************************************************************************************

void RNDSetPalette(uint8_t colour,uint8_t r,uint8_t g,uint8_t b) {
	palette[colour] = ((r >> 4) << 8) | (g & 0xF0) | (b >> 4);
}

// *******************************************************************************************************************************
//
//											Handle mode start
//
// *******************************************************************************************************************************

void RNDStartMode0(struct GraphicsMode *gMode) {
	videoRAM = gMode->graphicsMemory;
	isExtArray = gMode->isExtLine;
}

// *******************************************************************************************************************************
//
//								Get information about the active part of the display
//
// *******************************************************************************************************************************

void DGBXGetActiveDisplayInfo(SDL_Rect *r,int *pxs,int *pys,int *pxc,int *pyc) {
		*pxs = 3;*pys = 3;
		*pxc = 320;*pyc = 240;
		r->w = (*pxs) * (*pxc);r->h = (*pys) * (*pyc);
		r->x = WIN_WIDTH/2-r->w/2;r->y = WIN_HEIGHT/2-r->h/2;
}

// *******************************************************************************************************************************
//
//											This renders the debug screen
//
// *******************************************************************************************************************************

static const char *labels[] = { "A","X","Y","PC","SP","SR","CY","N","V","B","D","I","Z","C", NULL };

// Disassemble the instruction at addr, writing it into buffer.
// Returns address of next instruction.
int DBGXDasm65(int addr, char* buffer) {
	int p = addr;
	int opc = CPUReadMemory(p);p = (p + 1) & 0xFFFF;							// Read opcode.
	strcpy(buffer,_mnemonics[opc]);												// Work out the opcode.
	char *at = strchr(buffer,'@');												// Look for '@'
	if (at != NULL) {															// Operand ?
		char hex[6],temp[32];	
		if (at[1] == '1') {
			sprintf(hex,"%02x",CPUReadMemory(p));
			p = (p+1) & 0xFFFF;
		}
		if (at[1] == '2') {
			sprintf(hex,"%02x%02x",CPUReadMemory(p+1),CPUReadMemory(p));
			p = (p+2) & 0xFFFF;
		}
		if (at[1] == 'r') {
			int addr = CPUReadMemory(p);
			p = (p+1) & 0xFFFF;
			if ((addr & 0x80) != 0) addr = addr-256;
			sprintf(hex,"%04x",addr+p);
		}
		strcpy(temp,buffer);
		strcpy(temp+(at-buffer),hex);
		strcat(temp,at+2);
		strcpy(buffer,temp);
	}
	return p;
}

#define SW16_MNEMONICS
#include "data/sweet_opcodes.h"

int DBGXDasm16(int addr, char* buffer) {
	int p = addr;
	int operand;
	int opc = CPUReadMemory(p);p = (p + 1) & 0xFFFF;							// Read opcode.
	strcpy(buffer,sw16Mnemonics[opc]);											// Work out the opcode.
	char *at = strchr(buffer,'$');												// Look for '$'
	if (at != NULL) {															// Operand ?
		char hex[6],temp[32];	
		if (at[1] == '1') {
			operand = CPUReadMemory(p);
			sprintf(hex,"%02x",operand);
			p = (p+1) & 0xFFFF;
		}
		if (at[1] == '2') {
			operand = CPUReadMemory(p) + CPUReadMemory(p+1)*256;
			if (opc == 0x0D) operand = (operand + 2 + p) & 0xFFFF;
			sprintf(hex,"%04x",operand);
			p = (p+2) & 0xFFFF;
		}
		strcpy(temp,buffer);
		strcpy(temp+(at-buffer),hex);
		strcat(temp,at+2);
		strcpy(buffer,temp);	
	}
	return p;
}

// *******************************************************************************************************************************
//
// 							Return the number of bytes (1, 2 or 3) occupied by the instruction at addr.
//
// *******************************************************************************************************************************

int DBGXInstructionSize65(int addr) {
	int opcode = CPUReadMemory(addr);
	const char *at = strchr(_mnemonics[opcode],'@');
	if (at != NULL) {
		switch(at[1]) {
			case '1': return 2;
			case 'r': return 2;
			case '2': return 3;
			default: break; // shouldn't happen...
		}
	}
	return 1;   // It's a bare opcode.
}


// *******************************************************************************************************************************
//
// 									Dump out nbytes of memory to a string buffer.
//
// *******************************************************************************************************************************

// Buffer must have room for at least 3 bytes per memory location,
// plus an extra byte for null-termination.
// e.g for nbytes=3: "XX XX XX \0"
void DBGXDumpMem(int addr, int nbytes, char* buffer) {
	char* p = buffer;
	for (int i = 0; i < nbytes; ++i) {
		int b = CPUReadMemory((addr + i) & 0xFFFF);
		p += sprintf(p, "%02x ", b);
	}
}


// *******************************************************************************************************************************
//
// 									Render debug information and/or display
//
// *******************************************************************************************************************************

#define REG(n) 	(CPUReadMemory((n)*2)+CPUReadMemory((n)*2+1) * 256)

void DBGXRender(int *address,int showDisplay) {
	int n = 0;
	char buffer[32];
	CPUSTATUS65 *s = CPUGetStatus65();

	if (showDisplay == 0) {
		GFXSetCharacterSize(36,24);
		if (CPUGetID() == 65) {
			DBGVerticalLabel(21,0,labels,DBGC_ADDRESS,-1);								// Draw the labels for the register

			#define DN(v,w) GFXNumber(GRID(24,n++),v,16,w,GRIDSIZE,DBGC_DATA,-1)		// Helper macro

			DN(s->a,2);DN(s->x,2);DN(s->y,2);DN(s->pc,4);DN(s->sp+0x100,4);DN(s->status,2);DN(s->cycles,4);
			DN(s->sign,1);DN(s->overflow,1);DN(s->brk,1);DN(s->decimal,1);DN(s->interruptDisable,1);DN(s->zero,1);DN(s->carry,1);

		} else {
			for (int i = 0;i < 16;i++) {
				bool isTest = ((REG(14) >> 8) & 0x0F) == i;
				sprintf(buffer,"R%d:",i);
				GFXString(GRID(21,i),buffer,GRIDSIZE, DBGC_ADDRESS,-1);
				sprintf(buffer,"%04X",REG(i));
				GFXString(GRID(26,i),buffer,GRIDSIZE, isTest ? DBGC_DATA:DBGC_HIGHLIGHT,-1);
				GFXString(GRID(32,0),"ACC",GRIDSIZE,DBGC_ADDRESS,-1);
				GFXString(GRID(32,13),"CMP",GRIDSIZE,DBGC_ADDRESS,-1);
				GFXString(GRID(32,14),"SP",GRIDSIZE,DBGC_ADDRESS,-1);
				GFXString(GRID(32,15),"PCTR",GRIDSIZE,DBGC_ADDRESS,-1);
			}
		}
		n = 0;
		int a = address[1];																// Dump Memory.
		for (int row = 17;row < 24;row++) {
			GFXNumber(GRID(0,row),a,16,4,GRIDSIZE,DBGC_ADDRESS,-1);
			for (int col = 0;col < 8;col++) {
				int c = CPUReadMemory(a);
				GFXNumber(GRID(5+col*3,row),c,16,2,GRIDSIZE,DBGC_DATA,-1);
				c = (c & 0x7F);if (c < ' ') c = '.';
				GFXCharacter(GRID(30+col,row),c,GRIDSIZE,DBGC_DATA,-1);
				a = (a + 1) & 0xFFFF;
			}		
		}

		int p = address[0];																// Dump program code. 

		for (int row = 0;row < 16;row++) {
			int isPC = (p == ((s->pc) & 0xFFFF));										// Tests.
			int isBrk = (p == address[3]);
			GFXNumber(GRID(0,row),p,16,4,GRIDSIZE,isPC ? DBGC_HIGHLIGHT:DBGC_ADDRESS,	// Display address / highlight / breakpoint
																		isBrk ? 0xF00 : -1);
			if (CPUGetID() == 65) {
            	p = DBGXDasm65(p, buffer);
            } else {
            	p = DBGXDasm16(p, buffer);           	
            }
			GFXString(GRID(5,row),buffer,GRIDSIZE,isPC ? DBGC_HIGHLIGHT:DBGC_DATA,-1);	// Print the mnemonic
		}
		
	}
	renderCount++;
	if (showDisplay != 0) {
		SDL_Rect r;
		int xc,yc,xs,ys;
		DGBXGetActiveDisplayInfo(&r,&xs,&ys,&xc,&yc);
		SDL_Rect rc2;rc2 = r;
		rc2.w += 8;rc2.h += 8;rc2.x -=4;rc2.y -= 4;
		GFXRectangle(&rc2,0);
		//GFXRectangle(&r,0);
		rc2.w = xs;rc2.h = ys;
		BYTE8 *vPtr = videoRAM;
		if (vPtr != NULL) {
			for (int y = 0;y < 240;y++) {
				rc2.y = r.y + y*ys;rc2.x = r.x;
				for (int x = 0;x < 320;x++) {
					int col = palette[*vPtr++];
					if (col != 0) GFXRectangle(&rc2,col);
					rc2.x += xs;
				}
			}
			const uint8_t *cursorImage;
			uint16_t cursorX,cursorY;
			uint8_t xHit,yHit;
			if (MSEGetCursorDrawInformation(&cursorX,&cursorY)) {
				cursorImage = CURGetCurrent(&xHit,&yHit);				
				cursorX -= xHit;cursorY -= yHit;
				uint8_t w = 16,h = 16;
				if (cursorX + 16 >= 320) w = 320-cursorX;
				if (cursorY + 16 >= 240) h = 240-cursorY;
				rc2.w = xs;rc2.h = ys;
				for (int x = 0;x < w;x++) {
					for (int y = 0;y < h;y++) {
						rc2.x = r.x + (x+cursorX)*xs;
						rc2.y = r.y + (y+cursorY)*ys;
						uint8_t pixel = cursorImage[x+y*16];
						if (pixel != 0xFF) GFXRectangle(&rc2,palette[pixel]);
					}
				}
			}
			for (int y = 0; y < 240/8;y++) {
			 	rc2.x = r.x + r.w + 4;
			 	rc2.y = r.y + y * ys * 8 + 2;
			 	rc2.w = xs * 2;rc2.h = ys * 8 - 4;
			 	GFXRectangle(&rc2,isExtArray[y] ? 0x0F0 : 0xF00);
			}
		}	
	}
}
