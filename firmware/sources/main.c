// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      main.c
//      Author :    Paul Robson (paul@robsons.org.uk)
//      Date :      20th November 2023
//      Reviewed :  No
//      Purpose :   Main program.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

void writeCharacter(int n);

int main() {
    const char bootString[] = "NEO6502 Retrocomputer\r\r";
    MEMInitialiseMemory();
    GFXSetMode(0);
    const char *c = bootString;
    while (*c != '\0') writeCharacter(*c++);
    USBInitialise();
    CPUStart();
}

