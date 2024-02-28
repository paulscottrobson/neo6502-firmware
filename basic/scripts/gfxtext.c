// *******************************************************************************************
// *******************************************************************************************
//
//		Name : 		gfxtext.c
//		Purpose :	Convert text file to graphics file
//		Date :		28th February 2024
//		Author : 	Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static uint8_t *graphMem;
static uint16_t maxSize,currentSize;
static char lineBuffer[128];

// *******************************************************************************************
//
//									Initialise the conversion
//
// *******************************************************************************************

static void _CTGInitialise(uint8_t *gMem,uint16_t size) {
	graphMem = gMem;maxSize = size;  													// Remember position and size
	currentSize = 256;  																// Allocated so far, just the header
	for (int i = 0;i < size;i++) graphMem[i] = 0;  										// Zero the lot.
	graphMem[0] = 1;  																	// Format ID marker.
}

// *******************************************************************************************
//
//									Get position of entry.
//
// *******************************************************************************************

static uint16_t _CTGGetPosition(uint8_t block,uint8_t id) {
	uint16_t addr = 0;
	switch(block) {
		case 1:
			addr = 256 + id * (16 * 16 / 2);
			break;
		case 2:
			addr = 256 + graphMem[1] * (16 * 16 / 2) + id * (16 * 16 / 2);
			break;
		case 3:
			addr = 256 + (graphMem[1]+graphMem[2]) * (16 * 16 / 2) + id * (32 * 32 / 2);
			break;
	}
	return addr;
}

// *******************************************************************************************
//
//									Insert new block into memory
//
// *******************************************************************************************

static int _CTGAppendBlock(uint8_t block) {
	uint16_t pos = _CTGGetPosition(block,graphMem[block]);  							// Where the new one will go.
	uint16_t required = (block == 3) ? 32*32/2 : 16*16/2;  								// Byte size.
	if (currentSize + required >= maxSize) return 1;
	// printf("@%x %d %d\n",pos,block,graphMem[block]);
	if (pos == currentSize) {  															// Appending.
		// printf("Appended.\n");
	} else {
		// printf("Insert %d at %x\n",required,pos);  										// Make space for it.
		uint16_t p = currentSize;  														// Presented out of order.
		while (p >= pos) {
			graphMem[p + required] = graphMem[p];
			p--;
		}
	}
	graphMem[block]++; 																	// One more block.
	currentSize += required;  															// Just make it bigger.
	return 0;
}

// *******************************************************************************************
//
//							Allocate a new block for a tile or sprite
//
// *******************************************************************************************

static int _CTGAllocate(uint8_t block,uint8_t id) {
	uint8_t maxObj = (block == 1) ? 128 : 64;  											// This many at most
	if (id >= maxObj) return 1;   														// Too many !
	while (graphMem[block] <= id) {  													// Insert blank blocks until required exists.
		int e = _CTGAppendBlock(block); 			 									// Until the current one is valid.
		if (e != 0) return e;  															// Or append failed (out of memory)
	}
	return 0;
}

// *******************************************************************************************
//
//									Read a line from the source
//
// *******************************************************************************************

static char *_CTGGetLine(FILE *f) {
	if (fgets(lineBuffer,sizeof(lineBuffer),f) == NULL) return NULL;  					// End of file.
	if (lineBuffer[0] == '#') return _CTGGetLine(f);  									// Comment
	while (lineBuffer[0] != '\0' && lineBuffer[strlen(lineBuffer)-1] <= ' ') { 			// Right strip
		lineBuffer[strlen(lineBuffer)-1] = '\0';
	}
	char *p = lineBuffer;  																// Capitalise.
	while (*p != '\0') {
		*p = toupper(*p);  																// Capitalise
		if (*p == '.') *p = '@';  														// . to A-1
		p++;
	}
	if (lineBuffer[0] == '\0') return _CTGGetLine(f);  									// Blank line.
	return lineBuffer;
}

// *******************************************************************************************
//
//											Load in a block
//
// *******************************************************************************************

static int _CTGLoadBlock(FILE *f,uint8_t block,uint8_t graphic) {
	int e = _CTGAllocate(block,graphic);  												// Check space available
	if (e != 0) return e; 
	uint8_t size = (block == 3) ? 32 : 16;  											// Object size.
	uint16_t pos = _CTGGetPosition(block,graphic);  									// Where it is in the memory block
	// printf("%d %d %d $%x\n",block,graphic,size,pos);
	for (int row = 0;row < size;row++) {  												// Read each line of graphic data
		char *line = _CTGGetLine(f);
		if (line == NULL) return 3;  													// EOF
		if (strlen(line) != size) return 3;  											// Line the wrong size
		for (int col = 0; col < size;col++) {  											// Each character
			if (line[col] < '@' || line[col] > 'O') return 4;   						// Invalid data
			if ((col & 1) == 0) {  														// First nibble
				graphMem[pos] = (line[col] & 0x0F) << 4;
			} else {  																	// Second nibble
				graphMem[pos] |= (line[col] & 0x0F);
				pos++;
			}
		}
	}
	return 0;
}

// *******************************************************************************************
//
//								Convert given file to graphics format
//
//							 returns size loaded, or < 256 if error occurs
//
// *******************************************************************************************

uint16_t ConvertTextToGraphics(FILE *source,uint8_t *graphicMemory,size_t size) {
	_CTGInitialise(graphicMemory,size);  												// Set up the buffer.
	uint8_t currentBlock = 1; 
	char *line;
	int isOk;
	while (line = _CTGGetLine(source), line != NULL) {  								// Scan through the source
		isOk = 0;
		if (strcmp(line,"[TILE:16X16]") == 0) isOk = currentBlock = 1;  				// Check for block switches.
		if (strcmp(line,"[SPRITE:16X16]") == 0) isOk = currentBlock = 2;
		if (strcmp(line,"[SPRITE:32X32]") == 0) isOk = currentBlock = 3;
		if (strcmp(line,"[]") == 0) {   												// Next in current block.
			int e = _CTGLoadBlock(source,currentBlock,graphMem[currentBlock]);
			if (e != 0) return e;  														// Creating that block failed.
			isOk = 1;
		}
		if (isOk == 0) {  																// Not processed
			if (line[0] != '[' || line[strlen(line)-1] != ']') return 2; 				// It must be [<n>]
			for (int i = 1;i < strlen(line)-1;i++) {
				if (!isdigit(line[i])) return 2;
			}
			line[strlen(line)-1] = '\0';  												// Strip ]
			int e = _CTGLoadBlock(source,currentBlock,atoi(line+1));   					// Load that numbered block graphic
			if (e != 0) return e;
		}	
	}
	return currentSize;
}

// *******************************************************************************************
//
//								Simple caller used for testing
//
// *******************************************************************************************

#ifdef NEO6502RUNTEST

static uint8_t buffer[32768];

int main() {
	FILE *f = fopen("graphics.txt","r");
	uint16_t size = ConvertTextToGraphics(f,buffer,sizeof(buffer));
	fclose(f);

	f = fopen("_cdemo.gfx","wb");
	fwrite(buffer,size,1,f);
	fclose(f);
	return 0;
}

#endif
