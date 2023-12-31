// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      dispatch.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      22nd November 2023
//      Reviewed :  No
//      Purpose :   Message dispatcher
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "data/prompt.h"
#include "interface/kbdcodes.h"

#define DCOMMAND    (cBlock+0)
#define DFUNCTION   (cBlock+1)
#define DERROR      (cBlock+2)
#define DSTATUS     (cBlock+3)
#define DPARAMS     (cBlock+4)

#define float_abs(n) (((n) < 0.0) ? -(n):(n))

#ifdef PICO
#define TIMECRITICAL(x) __time_critical_func(x)
#else
#define TIMECRITICAL(x) x
#endif

#include "_who.c"

// ***************************************************************************************
//
//							Handle commands sent by message
//
// ***************************************************************************************

void TIMECRITICAL(DSPHandler)(uint8_t *cBlock, uint8_t *memory) 
{
    float f1,f2;
    int i1,i2,r;
    uint32_t u1;
    *DERROR = 0;                                                                // Clear error state.
    #include "data/dispatch_code.h"  
    *DCOMMAND = 0;					     										// Clear the message indicating completion.
}

// ***************************************************************************************
//
//     	  Polling code. Called at low repeat rate. About every 64k cycles is right.
//
// ***************************************************************************************

void TIMECRITICAL(DSPSync)(void) 
{
    KBDSync();
}

// ***************************************************************************************
//
//     	  		Reset interfaces. Called at start, and also on command 0,0
//
// ***************************************************************************************

void DSPReset(void) {
    const char bootString[] = PROMPT;
    MEMInitialiseMemory();                                                      // Set up memory, load kernel ROM
    GFXSetMode(0);                                                              // Initialise graphics
    CONWrite(0x80+3);                                                           // Yellow
    const char *c = bootString;
    while (*c != '\0') CONWrite(*c++);	
    KBDInitialise();                                                            // Initialise keyboard & USB system.
    KBDEvent(0,0xFF,0);                                                         // Reset the keyboard manager
    SNDInitialise();                                                            // Initialise sound
    CONWrite(0x80+6);
    STOSynchronise();                                                           // Synchronise storage
    CONWrite(0x80+2);
    CFGProcess();                                                               // Process configuration file.
}

// ***************************************************************************************
//
//                              Access helper functions
//
// ***************************************************************************************

static char szBuffer[32];  

char *DSPGetString(uint8_t *command,uint8_t paramOffset) {
    uint8_t *mem = cpuMemory+command[paramOffset]+(command[paramOffset+1]<<8);  // From here.
    memcpy(szBuffer,mem+1,*mem);                                                // Make ASCIIZ string
    szBuffer[*mem] = '\0';
    return szBuffer;
}

uint16_t DSPGetInt16(uint8_t *command,uint8_t paramOffset) {
    return command[paramOffset] + (command[paramOffset+1] << 8);
}

// ***************************************************************************************
//
//      Date        Revision
//      ====        ========
//
// ***************************************************************************************
