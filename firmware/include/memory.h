// 
// Author: Rien Matthijsse
// 

#ifndef _MEMORY_h
#define _MEMORY_h

#define MEMORY_SIZE  0x10000 // 64k

#define KBD   0xd010
#define DSP   0xd012
#define VDU   0xd020

extern uint8_t  mem[];
extern uint16_t address;
extern uint8_t  data;

void initmemory();
void writememory();

#endif

