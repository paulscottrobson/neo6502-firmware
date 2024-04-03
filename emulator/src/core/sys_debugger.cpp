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
//											Handle palette changes
// *******************************************************************************************************************************

void RNDSetPalette(uint8_t colour,uint8_t r,uint8_t g,uint8_t b) {
	palette[colour] = ((r >> 4) << 8) | (g & 0xF0) | (b >> 4);
}

// *******************************************************************************************************************************
//											Handle mode start
// *******************************************************************************************************************************

void RNDStartMode0(struct GraphicsMode *gMode) {
	videoRAM = gMode->graphicsMemory;
	isExtArray = gMode->isExtLine;
}

// *******************************************************************************************************************************
//											This renders the debug screen
// *******************************************************************************************************************************

static const char *labels[] = { "A","X","Y","PC","SP","SR","CY","N","V","B","D","I","Z","C", NULL };

void DBGXRender(int *address,int showDisplay) {
	int n = 0;
	char buffer[32];
	CPUSTATUS *s = CPUGetStatus();

	if (showDisplay == 0) {
		GFXSetCharacterSize(36,24);
		DBGVerticalLabel(21,0,labels,DBGC_ADDRESS,-1);									// Draw the labels for the register

		#define DN(v,w) GFXNumber(GRID(24,n++),v,16,w,GRIDSIZE,DBGC_DATA,-1)			// Helper macro

		DN(s->a,2);DN(s->x,2);DN(s->y,2);DN(s->pc,4);DN(s->sp+0x100,4);DN(s->status,2);DN(s->cycles,4);
		DN(s->sign,1);DN(s->overflow,1);DN(s->brk,1);DN(s->decimal,1);DN(s->interruptDisable,1);DN(s->zero,1);DN(s->carry,1);

		n = 0;
		int a = address[1];																// Dump Memory.
		for (int row = 15;row < 24;row++) {
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
		int opc;

		for (int row = 0;row < 14;row++) {
			int isPC = (p == ((s->pc) & 0xFFFF));										// Tests.
			int isBrk = (p == address[3]);
			GFXNumber(GRID(0,row),p,16,4,GRIDSIZE,isPC ? DBGC_HIGHLIGHT:DBGC_ADDRESS,	// Display address / highlight / breakpoint
																		isBrk ? 0xF00 : -1);
			opc = CPUReadMemory(p);p = (p + 1) & 0xFFFF;								// Read opcode.
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
			GFXString(GRID(5,row),buffer,GRIDSIZE,isPC ? DBGC_HIGHLIGHT:DBGC_DATA,-1);	// Print the mnemonic
		}
		
	}
	renderCount++;
	if (showDisplay != 0) {
		int xc = 320;int yc = 240;
		int xs = 3;int ys = 3;
		SDL_Rect r;
		r.w = xs*xc;r.h = ys*yc;
		r.x = WIN_WIDTH/2-r.w/2;r.y = WIN_HEIGHT/2-r.h/2;
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
			if (CURGetCursorDrawInformation(&cursorImage,&cursorX,&cursorY)) {
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
			for (int y; y < 240/8;y++) {
				rc2.x = r.x + r.w + 4;
				rc2.y = r.y + y * ys * 8 + 2;
				rc2.w = xs * 2;rc2.h = ys * 8 - 4;
				GFXRectangle(&rc2,isExtArray[y] ? 0x0F0 : 0xF00);
			}
		}	
	}
}
