// 
// Author: Rien Matthijsse
// 

#ifndef _MEMORY_h
#define _MEMORY_h

#define MEMORY_SIZE  0x10000 // 64k

#define DEFAULT_PORT 0xFF00

extern uint8_t  mem[];
extern uint16_t address;
extern uint8_t  data;

void initmemory();
void writememory();

#endif

