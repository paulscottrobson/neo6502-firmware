// 
// Author: Rien Matthijsse
// 

#ifndef _MEMORY_h
#define _MEMORY_h

#define MEMORY_SIZE  0x10000 // 64k
#define DEFAULT_PORT 0xFF00

extern uint8_t  cpuMemory[];

void initmemory();

#endif

