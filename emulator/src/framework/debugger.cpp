// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		debugger.c
//		Purpose:	Debugger Code (System Independent)
//		Created:	22nd November 2023
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#include <stdio.h>
#include <ctype.h>
#include "gfx.h"
#include "sys_processor.h"
#include "debugger.h"
#include "common.h"	

static int isInitialised = 0; 														// Flag to initialise first time
static int addressSettings[] = { 0,0,0,0x20FFFF }; 									// Adjustable values : Code, Data, Other, Break.
static int keyMapping[16];															// Mapping for control keys to key values
static int inRunMode = -1;															// Non zero when free Running
static int lastKey,currentKey;														// Last and Current key state
static int stepBreakPoint;															// Extra breakpoint used for step over.
static Uint32 nextFrame = 0;														// Time of next frame.
static int frameCount = 0;

#define FRAMESKIP 	(0)

// *******************************************************************************************************************************
//								Handle one frame of rendering etc. for the debugger.
// *******************************************************************************************************************************

int GFXXRender(SDL_Surface *surface) {

	frameCount++;
	int repaint = (frameCount & FRAMESKIP) == 0;

	if (isInitialised == 0) {														// Check if first time
		isInitialised = 1;															// Now initialised
		addressSettings[0] = DEBUG_HOMEPC();										// Set default locations
		addressSettings[1] = DEBUG_RAMSTART;
		addressSettings[3] = 0x000000;
		DBGDefineKey(DBGKEY_RESET,GFXKEY_F1);										// Assign default keys
		DBGDefineKey(DBGKEY_SHOW,GFXKEY_TAB);
		DBGDefineKey(DBGKEY_STEP,GFXKEY_F7);		
		DBGDefineKey(DBGKEY_STEPOVER,GFXKEY_F8);
		DBGDefineKey(DBGKEY_RUN,GFXKEY_F5);	
		DBGDefineKey(DBGKEY_BREAK,GFXKEY_F6);	
		DBGDefineKey(DBGKEY_HOME,GFXKEY_F2);		
		DBGDefineKey(DBGKEY_SETBREAK,GFXKEY_F9);		
		lastKey = currentKey = -1;
	}

	if (repaint) {
		if (inRunMode != 0 || GFXIsKeyPressed(keyMapping[DBGKEY_SHOW]))				// Display system screen if Run or Sjhow
			DEBUG_VDURENDER(addressSettings);
		else 																		// Otherwise show Debugger screen
			DEBUG_CPURENDER(addressSettings);
	}

	#ifdef INCLUDE_DEBUGGING_SUPPORT
	currentKey = -1;																// Identify which key is pressed.
	for (int i = 0;i < 128;i++) {
		if (!GFXISMODIFIERKEY(i)) {													// Don't return SHIFT and CTRL as keypresses.
			if (GFXIsKeyPressed(i)) {
				currentKey = i;
			}
		}
	}
	if (currentKey != lastKey) {													// Key changed
		lastKey = currentKey;														// Update current key.
		currentKey = DEBUG_KEYMAP(currentKey,inRunMode != 0);						// Pass keypress to called.
		if (currentKey >= 0) {														// Key depressed ?
			currentKey = toupper(currentKey);										// Make it capital.

																					// Debugger keys enabled ?
			#define CMDKEY(n) (GFXIsKeyPressed(keyMapping[n]) && (CPUUseDebugKeys() == 0))

			if (CMDKEY(DBGKEY_RESET)) {												// Reset processor (F1)
				DEBUG_RESET();					
				addressSettings[0] = DEBUG_HOMEPC();
				SNDMuteAllChannels();
			}

			if (inRunMode == 0) {
				SNDMuteAllChannels();
				if (isxdigit(currentKey)) {											// Is it a hex digit 0-9 A-F.
					int digit = isdigit(currentKey)?currentKey:(currentKey-'A'+10);	// Convert to a number.
					int setting = 0;												// Which value is being changed ?
					if (GFXIsKeyPressed(GFXKEY_SHIFT)) setting = 1;
					if (GFXIsKeyPressed(GFXKEY_CONTROL)) setting = 1;
					addressSettings[setting] = 										// Shift it using this macro (so we could use Octal, say)
									DEBUG_SHIFT(addressSettings[setting],(digit & 0x0F));
				}

				if (CMDKEY(DBGKEY_HOME)) {											// Home pointer (F2)
					addressSettings[0] = DEBUG_HOMEPC();
				}
				if (CMDKEY(DBGKEY_RUN)) {											// Run program (F5)
					inRunMode = 1;											
					stepBreakPoint = -1;
				}
				if (CMDKEY(DBGKEY_STEP)) {											// Execute a single instruction (F7)
					DEBUG_SINGLESTEP();
					addressSettings[0] = DEBUG_HOMEPC();
				}
				if (CMDKEY(DBGKEY_STEPOVER)) {										// Step over program calls (F8)
					stepBreakPoint = DEBUG_GETOVERBREAK();							// Where if anywhere should we break ?
					if (stepBreakPoint == 0) {										// No step over, just single step.
						DEBUG_SINGLESTEP();
						addressSettings[0] = DEBUG_HOMEPC();
					} else {
						inRunMode = 1;												// Run until step break or normal break.
					}
				}
				if (CMDKEY(DBGKEY_SETBREAK)) {										// Set Breakpoint (F9)
						addressSettings[3] = addressSettings[0];
				}
			} else {																// In Run mode.
				if (CMDKEY(DBGKEY_BREAK)) {
					inRunMode = 0;
					addressSettings[0] = DEBUG_HOMEPC();
				}
			}
		} 
	}
	#endif
	if (inRunMode != 0) {															// Running a program.
		int frameRate = DEBUG_RUN(addressSettings[3],stepBreakPoint);				// Run a frame, or try to.
		if (frameRate == 0) {														// Run code with step breakpoint, maybe.
			inRunMode = 0;															// Break has occurred.
		} else {
			while (SDL_GetTicks() < nextFrame) {};									// Wait for frame timer to elapse.
			nextFrame = SDL_GetTicks() + 1000 / frameRate;							// And calculate the next sync time.
		}
		addressSettings[0] = DEBUG_HOMEPC();
	}	
	return repaint;
}

// *******************************************************************************************************************************
//													Redefine a key
// *******************************************************************************************************************************

void DBGDefineKey(int keyID,int gfxKey) {
	keyMapping[keyID] = gfxKey;
}

// *******************************************************************************************************************************
//											Draw a vertical set of labels (helper)
// *******************************************************************************************************************************

void DBGVerticalLabel(int x,int y,const char *labels[],int fgr,int bgr) {
	int n = 0;
	while (labels[n] != NULL) {
		GFXString(GRID(x,y),labels[n],GRIDSIZE,fgr,bgr);
		y++;n++;
	}
}
