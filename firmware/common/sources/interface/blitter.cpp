// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      blitter.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      24th March 2024
//      Reviewed :  No
//      Purpose :   Blitter code
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//						Convert page/address to a physical address
//
// ***************************************************************************************

static uint8_t *_BLTGetRealAddress(uint8_t page,uint16_t address) {
	int pos;
	uint8_t *ptr = NULL;
	switch(page) {
		case 0x00: 																	// CPU Memory
			ptr = cpuMemory + address;break;
		case 0x80:  																// Video RAM
		case 0x81:
			pos = ((page - 0x80) << 16) | address;  								// Offset in video RAM
			if (pos < 320*240) ptr = graphicsMemory + pos; 							// Position in bitmap RAM
			break;
		case 0x90: 																	// Graphic storage RAM
			if (address < GFX_MEMORY_SIZE) ptr = gfxObjectMemory + address;
			break;
	}
	return ptr;
}

// ***************************************************************************************
//
//								Copy Blitter, very simple
//
// ***************************************************************************************

uint8_t BLTSimpleCopy(uint8_t pageFrom,uint16_t addressFrom, uint8_t pageTo, uint16_t addressTo, uint16_t transferSize) {	
	printf("Blit: %02x:%04x to %02x:%04x bytes %04x\n",pageFrom,addressFrom,pageTo,addressTo,transferSize);
	if (transferSize == 0) return 0;
	uint8_t *src = _BLTGetRealAddress(pageFrom,addressFrom);  						// Copy from here
	uint8_t *dst = _BLTGetRealAddress(pageTo,addressTo);  							// To here.
	if (src == NULL || dst == NULL) return 1;  										// Start both legitimate addresses
	if (_BLTGetRealAddress(pageFrom,addressFrom+transferSize-1) == NULL) return 1; 	// Check end both legitimate addresses
	if (_BLTGetRealAddress(pageTo,addressTo+transferSize-1) == NULL) return 1;
	memmove(dst,src,transferSize); 													// Copy it.

	return 0;
}

// ***************************************************************************************
//
//						Add constant to a physical address
//
// ***************************************************************************************

void _BLTAddAddress(struct BlitterArea *ba,uint32_t add) {
	uint32_t a = ba->address + add;  												// Work out the result
	ba->address = a & 0xFFFF; 														// Put address part back
	if (a & 0x10000) ba->page++;  													// If carry out, go to next page.
}

// ***************************************************************************************
//
//							 Load a blitter object structure
//
// ***************************************************************************************

void _BLTLoadBlitterAreaObject(uint16_t addr,struct BlitterArea *b) {
	b->address = cpuMemory[addr] + (cpuMemory[addr+1] << 8);
	b->page = cpuMemory[addr+2];
	// 1 byte of padding
	b->stride = cpuMemory[addr+4] + (cpuMemory[addr+5] << 8);
	b->format = cpuMemory[addr+6];
	// The rest are source-only.
	b->transparent = cpuMemory[addr+7];
	b->solid = cpuMemory[addr+8];
	b->height = cpuMemory[addr+9];
	b->width = cpuMemory[addr+10] + (cpuMemory[addr+11] << 8);
}

// ***************************************************************************************
//
//					BLTComplexCopy() line helpers: straight copy
//
// ***************************************************************************************

// Definition for our line-copy routines.
typedef void (*copyFn)(uint8_t *, const uint8_t *, size_t );

// Copy src to target, whole bytes.
static void copy_ByteToByte(uint8_t *tgt, const uint8_t *src, size_t n) {
	memmove(tgt, src, n);
}

//  Copy src to target low nibble (target high nibble is unchanged).
static void copy_ByteToLow(uint8_t *tgt, const uint8_t *src, size_t n) {
	while(n > 0) {
		*tgt = (*tgt & 0xF0) | (*src & 0x0F);
		++tgt;
		++src;
		--n;
	}
}

//  Copy src to target high nibble (target low nibble is unchanged).
static void copy_ByteToHigh(uint8_t *tgt, const uint8_t *src, size_t n) {
	while(n > 0) {
		*tgt = (*tgt & 0x0F) | (*src << 4);
		++tgt;
		++src;
		--n;
	}
}

// Expand src nibbles into consecutive bytes in target (high nibbles of target will be zeroed).
static void copy_PairToByte(uint8_t *tgt, const uint8_t *src, size_t n) {
	n = n / 2;	// 2 nibbles per byte.
	while(n > 0) {
		*tgt++ = *src >> 4;
		*tgt++ = *src & 0x0F;
		++src;
		--n;
	}
}


// Expand src nibbles into consecutive low nibbles in target (leaving target high nibbles unchanged).
static void copy_PairToLow(uint8_t *tgt, const uint8_t *src, size_t n) {
	n = n / 2;	// 2 nibbles per byte.
	while(n > 0) {
		*tgt = (*tgt & 0xF0) | (*src >> 4);
		++tgt;
		*tgt = (*tgt & 0xF0) | (*src & 0x0F);
		++tgt;
		++src;
		--n;
	}
}

// Expand src nibbles into consecutive high nibbles in target (leaving target low nibbles unchanged).
static void copy_PairToHigh(uint8_t *tgt, const uint8_t *src, size_t n) {
	n = n / 2;	// 2 nibbles per byte.
	while(n > 0) {
		*tgt = (*tgt & 0x0F) | (*src & 0xF0);
		++tgt;
		*tgt = (*tgt & 0x0F) | (*src << 4);
		++tgt;
		++src;
		--n;
	}
}


// Expand src bits into consecutive bytes in target.
static void copy_BitsToByte(uint8_t *tgt, const uint8_t *src, size_t n) {
	n = n / 8;	// 8 bits per byte.
	while(n > 0) {
		for (int i = 7; i >= 0; --i) {
			*tgt++ = (*src >> i) & 0x01;
		}
		++src;
		--n;
	}
}


// Expand src bits into consecutive low nibbles in target (leaving target high nibbles unchanged).
static void copy_BitsToLow(uint8_t *tgt, const uint8_t *src, size_t n) {
	n = n / 8;	// 8 bits per byte.
	while(n > 0) {
		for (int i = 7; i >= 0; --i) {
			*tgt = (*tgt & 0xF0) | ((*src >> i) & 0x01);
			++tgt;
		}
		++src;
		--n;
	}
}

// Expand src bits into consecutive high nibbles in target (leaving target low nibbles unchanged).
static void copy_BitsToHigh(uint8_t *tgt, const uint8_t *src, size_t n) {
	n = n / 8;	// 8 bits per byte.
	while(n > 0) {
		for (int i = 7; i >= 0; --i) {
			*tgt = (*tgt & 0x0F) | (((*src >> i) & 0x01) << 4);
			++tgt;
		}
		++src;
		--n;
	}
}


static copyFn pickCopyFn(uint8_t srcFormat, uint8_t tgtFormat)
{
	switch (srcFormat) {
		case BLTFMT_BYTE:
			switch (tgtFormat) {
				case BLTFMT_BYTE: return copy_ByteToByte;
				case BLTFMT_HIGH: return copy_ByteToHigh;
				case BLTFMT_LOW: return copy_ByteToLow;
				default: return nullptr;
			}
		case BLTFMT_PAIR:
			switch (tgtFormat) {
				case BLTFMT_BYTE: return copy_PairToByte;
				case BLTFMT_HIGH: return copy_PairToHigh;
				case BLTFMT_LOW: return copy_PairToLow;
				default: return nullptr;
			}
		case BLTFMT_BITS:
			switch (tgtFormat) {
				case BLTFMT_BYTE: return copy_BitsToByte;
				case BLTFMT_HIGH: return copy_BitsToHigh;
				case BLTFMT_LOW: return copy_BitsToLow;
				default: return nullptr;
			}
		default: return nullptr;
	}
}


// ***************************************************************************************
//
//			BLTComplexCopy() line helpers: copy with src masking (transparency)
//
// ***************************************************************************************

// Definition for our masked-copy routines.
typedef void (*copyMaskedFn)(uint8_t *, const uint8_t *, size_t, uint8_t);

// Copy src to target, whole bytes.
static void copy_masked_ByteToByte(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent) {
	while (n>0) {
		if (*src != transparent) {
			*tgt = *src;
		}
		++tgt;
		++src;
		--n;
	}
}

//  Copy src to target low nibble (target high nibble is unchanged).
static void copy_masked_ByteToLow(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent) {
	while(n > 0) {
		if (*src != transparent) {
			*tgt = (*tgt & 0xF0) | (*src & 0x0F);
		}
		++tgt;
		++src;
		--n;
	}
}

//  Copy src to target high nibble (target low nibble is unchanged).
static void copy_masked_ByteToHigh(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent) {
	while(n > 0) {
		if (*src != transparent) {
			*tgt = (*tgt & 0x0F) | (*src << 4);
		}
		++tgt;
		++src;
		--n;
	}
}

// Expand src nibbles into consecutive bytes in target (high nibbles of target will be zeroed).
static void copy_masked_PairToByte(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent) {
	n = n / 2;	// 2 nibbles per byte.
	while(n > 0) {
		if ((*src >> 4) != transparent) {
			*tgt = *src >> 4;
		}
		++tgt;
		if ((*src & 0x0F) != transparent) {
			*tgt = *src & 0x0F;
		}
		++tgt;
		++src;
		--n;
	}
}

// Expand src nibbles into consecutive low nibbles in target (leaving target high nibbles unchanged).
static void copy_masked_PairToLow(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent) {
	n = n / 2;	// 2 nibbles per byte.
	while(n > 0) {
		if ((*src >> 4) != transparent) {
			*tgt = (*tgt & 0xF0) | (*src >> 4);
		}
		++tgt;
		if ((*src & 0x0F) != transparent) {
			*tgt = (*tgt & 0xF0) | (*src & 0x0F);
		}
		++tgt;
		++src;
		--n;
	}
}

// Expand src nibbles into consecutive high nibbles in target (leaving target low nibbles unchanged).
static void copy_masked_PairToHigh(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent) {
	n = n / 2;	// 2 nibbles per byte.
	while(n > 0) {
		uint8_t v = *src >> 4;
		if (v != transparent) {
			*tgt = (*tgt & 0x0F) | (v << 4);
		}
		++tgt;
		v = *src & 0x0F;
		if (v != transparent) {
			*tgt = (*tgt & 0x0F) | (v << 4);
		}
		++tgt;
		++src;
		--n;
	}
}


// Expand src bits into consecutive bytes in target.
static void copy_masked_BitsToByte(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent) {
	n = n / 8;	// 8 bits per byte.
	while(n > 0) {
		for (int i = 7; i >= 0; --i) {
			uint8_t v = (*src >> i) & 0x01;
			if (v != transparent) {
				*tgt = v;
			}
			++tgt;
		}
		++src;
		--n;
	}
}


// Expand src bits into consecutive low nibbles in target (leaving target high nibbles unchanged).
static void copy_masked_BitsToLow(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent) {
	n = n / 8;	// 8 bits per byte.
	while(n > 0) {
		for (int i = 7; i >= 0; --i) {
			uint8_t v = (*src >> i) & 0x01;
			if (v != transparent) {
				*tgt = (*tgt & 0xF0) | v;
			}
			++tgt;
		}
		++src;
		--n;
	}
}

// Expand src bits into consecutive high nibbles in target (leaving target low nibbles unchanged).
static void copy_masked_BitsToHigh(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent) {
	n = n / 8;	// 8 bits per byte.
	while(n > 0) {
		for (int i = 7; i >= 0; --i) {
			uint8_t v = (*src >> i) & 0x01;
			if (v != transparent) {
				*tgt = (*tgt & 0x0F) | (v << 4);
			}
			++tgt;
		}
		++src;
		--n;
	}
}

static copyMaskedFn pickCopyMaskedFn(uint8_t srcFormat, uint8_t tgtFormat)
{
	switch (srcFormat) {
		case BLTFMT_BYTE:
			switch (tgtFormat) {
				case BLTFMT_BYTE: return copy_masked_ByteToByte;
				case BLTFMT_HIGH: return copy_masked_ByteToHigh;
				case BLTFMT_LOW: return copy_masked_ByteToLow;
				default: return nullptr;
			}
		case BLTFMT_PAIR:
			switch (tgtFormat) {
				case BLTFMT_BYTE: return copy_masked_PairToByte;
				case BLTFMT_HIGH: return copy_masked_PairToHigh;
				case BLTFMT_LOW: return copy_masked_PairToLow;
				default: return nullptr;
			}
		case BLTFMT_BITS:
			switch (tgtFormat) {
				case BLTFMT_BYTE: return copy_masked_BitsToByte;
				case BLTFMT_HIGH: return copy_masked_BitsToHigh;
				case BLTFMT_LOW: return copy_masked_BitsToLow;
				default: return nullptr;
			}
		default: return nullptr;
	}
}



// ***************************************************************************************
//
//					BLTComplexCopy() line helpers: solid fill with src masking
//
// ***************************************************************************************

// Definition for our masked-solid routines.
typedef void (*solidMaskedFn)(uint8_t *, const uint8_t *, size_t, uint8_t, uint8_t);

// Copy src to target, whole bytes.
static void solid_masked_ByteToByte(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent, uint8_t solid) {
	while (n>0) {
		if (*src != transparent) {
			*tgt = solid;
		}
		++tgt;
		++src;
		--n;
	}
}

//  Copy src to target low nibble (target high nibble is unchanged).
static void solid_masked_ByteToLow(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent, uint8_t solid) {
	solid &= 0x0F;
	while(n > 0) {
		if (*src != transparent) {
			*tgt = (*tgt & 0xF0) | solid;
		}
		++tgt;
		++src;
		--n;
	}
}

//  Copy src to target high nibble (target low nibble is unchanged).
static void solid_masked_ByteToHigh(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent, uint8_t solid) {
	solid <<= 4;
	while(n > 0) {
		if (*src != transparent) {
			*tgt = (*tgt & 0x0F) | solid;
		}
		++tgt;
		++src;
		--n;
	}
}

// Expand src nibbles into consecutive bytes in target (high nibbles of target will be zeroed).
static void solid_masked_PairToByte(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent, uint8_t solid) {
	solid &= 0x0F;
	n = n / 2;	// 2 nibbles per byte.
	while(n > 0) {
		if ((*src >> 4) != transparent) {
			*tgt = solid;
		}
		++tgt;
		if ((*src & 0x0F) != transparent) {
			*tgt = solid;
		}
		++tgt;
		++src;
		--n;
	}
}

// Expand src nibbles into consecutive low nibbles in target (leaving target high nibbles unchanged).
static void solid_masked_PairToLow(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent, uint8_t solid) {
	solid &= 0x0F;
	n = n / 2;	// 2 nibbles per byte.
	while(n > 0) {
		if ((*src >> 4) != transparent) {
			*tgt = (*tgt & 0xF0) | solid;
		}
		++tgt;
		if ((*src & 0x0F) != transparent) {
			*tgt = (*tgt & 0xF0) | solid;
		}
		++tgt;
		++src;
		--n;
	}
}

// Expand src nibbles into consecutive high nibbles in target (leaving target low nibbles unchanged).
static void solid_masked_PairToHigh(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent, uint8_t solid) {
	solid <<= 4;
	n = n / 2;	// 2 nibbles per byte.
	while(n > 0) {
		uint8_t v = *src >> 4;
		if (v != transparent) {
			*tgt = (*tgt & 0x0F) | solid;
		}
		++tgt;
		v = *src & 0x0F;
		if (v != transparent) {
			*tgt = (*tgt & 0x0F) | solid;
		}
		++tgt;
		++src;
		--n;
	}
}


// Expand src bits into consecutive bytes in target.
static void solid_masked_BitsToByte(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent, uint8_t solid) {
	n = n / 8;	// 8 bits per byte.
	while(n > 0) {
		for (int i = 7; i >= 0; --i) {
			uint8_t v = (*src >> i) & 0x01;
			if (v != transparent) {
				*tgt = solid;
			}
			++tgt;
		}
		++src;
		--n;
	}
}


// Expand src bits into consecutive low nibbles in target (leaving target high nibbles unchanged).
static void solid_masked_BitsToLow(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent, uint8_t solid) {
	solid &= 0x0F;
	n = n / 8;	// 8 bits per byte.
	while(n > 0) {
		for (int i = 7; i >= 0; --i) {
			uint8_t v = (*src >> i) & 0x01;
			if (v != transparent) {
				*tgt = (*tgt & 0xF0) | solid;
			}
			++tgt;
		}
		++src;
		--n;
	}
}

// Expand src bits into consecutive high nibbles in target (leaving target low nibbles unchanged).
static void solid_masked_BitsToHigh(uint8_t *tgt, const uint8_t *src, size_t n, uint8_t transparent, uint8_t solid) {
	solid <<= 4;
	n = n / 8;	// 8 bits per byte.
	while(n > 0) {
		for (int i = 7; i >= 0; --i) {
			uint8_t v = (*src >> i) & 0x01;
			if (v != transparent) {
				*tgt = (*tgt & 0x0F) | solid;
			}
			++tgt;
		}
		++src;
		--n;
	}
}

static solidMaskedFn pickSolidMaskedFn(uint8_t srcFormat, uint8_t tgtFormat)
{
	switch (srcFormat) {
		case BLTFMT_BYTE:
			switch (tgtFormat) {
				case BLTFMT_BYTE: return solid_masked_ByteToByte;
				case BLTFMT_HIGH: return solid_masked_ByteToHigh;
				case BLTFMT_LOW: return solid_masked_ByteToLow;
				default: return nullptr;
			}
		case BLTFMT_PAIR:
			switch (tgtFormat) {
				case BLTFMT_BYTE: return solid_masked_PairToByte;
				case BLTFMT_HIGH: return solid_masked_PairToHigh;
				case BLTFMT_LOW: return solid_masked_PairToLow;
				default: return nullptr;
			}
		case BLTFMT_BITS:
			switch (tgtFormat) {
				case BLTFMT_BYTE: return solid_masked_BitsToByte;
				case BLTFMT_HIGH: return solid_masked_BitsToHigh;
				case BLTFMT_LOW: return solid_masked_BitsToLow;
				default: return nullptr;
			}
		default: return nullptr;
	}
}



// ***************************************************************************************
//
//								More Complex Blitter Copy
//
// ***************************************************************************************

static uint8_t internalBLTComplexCopy(uint8_t action, const struct BlitterArea *source, const struct BlitterArea *target) {

	switch (action) {
		case BLTACT_COPY:
			{
				copyFn copy = pickCopyFn(source->format, target->format);
				if (!copy) {
					return 1;	// Unsupported combination
				}

				uint8_t *src = _BLTGetRealAddress(source->page, source->address);
				uint8_t *tgt = _BLTGetRealAddress(target->page, target->address);
				if (src == NULL || tgt == NULL) return 1;
				for (uint8_t l = source->height; l > 0; --l) {
					// Process a line.
					(*copy)(tgt, src, source->width);
					src += source->stride;
					tgt += target->stride;
				}
			}
			break;
		case BLTACT_MASK:
			{
				copyMaskedFn copyMasked = pickCopyMaskedFn(source->format, target->format);
				if (!copyMasked) {
					return 1;	// Unsupported combination
				}

				uint8_t *src = _BLTGetRealAddress(source->page, source->address);
				uint8_t *tgt = _BLTGetRealAddress(target->page, target->address);
				if (src == NULL || tgt == NULL) return 1;
				for (uint8_t l = source->height; l > 0; --l) {
					(*copyMasked)(tgt, src, source->width, source->transparent);
					src += source->stride;
					tgt += target->stride;
				}
			}
			break;
		case BLTACT_SOLID:
			{
				solidMaskedFn solidMasked = pickSolidMaskedFn(source->format, target->format);
				if (!solidMasked) {
					return 1;	// Unsupported combination
				}

				uint8_t *src = _BLTGetRealAddress(source->page, source->address);
				uint8_t *tgt = _BLTGetRealAddress(target->page, target->address);
				if (src == NULL || tgt == NULL) return 1;
				for (uint8_t l = source->height; l > 0; --l) {
					(*solidMasked)(tgt, src, source->width, source->transparent, source->solid);
					src += source->stride;
					tgt += target->stride;
				}
			}
			break;
	}
	return 0;
}

uint8_t BLTComplexCopy(uint8_t action,uint16_t aSource,uint16_t aTarget) {
	struct BlitterArea source, target;
	_BLTLoadBlitterAreaObject(aSource,&source);
	_BLTLoadBlitterAreaObject(aTarget,&target);
	return internalBLTComplexCopy(action, &source, &target);
}

// ***************************************************************************************
//
//						Image-oriented blit function with clipping.
//
// ***************************************************************************************

// Note: no handy width and height defines available, so we'll define them here.
// But they should probably be somewhere else.
#define FRAME_WIDTH 320
#define FRAME_HEIGHT 240

// Clip rectangle. Maybe it'd make sense to let user set custom clipping area, but for now
// we'll just hardcode them here.
#define LCLIP 0
#define RCLIP FRAME_WIDTH
#define TCLIP 0
#define BCLIP FRAME_HEIGHT

uint8_t BLTImage(uint8_t action, uint16_t sourceArea, int16_t x, int16_t y, uint8_t destFmt)
{
	struct BlitterArea src;
	_BLTLoadBlitterAreaObject(sourceArea, &src);

	// Clip against left.
	int16_t w = src.width;
	if (x < LCLIP) {
		// Left-clipping is a little fiddly for sub-byte source formats.
		// We need to round up to next byte boundary (maybe leaving
		// up to 7 pixels missing on the left).
		int16_t adj = (LCLIP - x);
		switch (src.format) {
			case BLTFMT_BYTE:
				src.address += adj;
				w -= adj;
				x += adj;
				break;
			case BLTFMT_PAIR:
				src.address += (adj + 1) >> 1;
				w -= (adj + 1) & ~1;
				x += (adj + 1) & ~1;
				break;
			case BLTFMT_BITS:
				src.address += (adj + 7) >> 3;
				w -= (adj + 7) & ~7;
				x += (adj + 7) & ~7;
				break;
			default: return 1;  // bad format.
		}
		if (w <= 0) {
			return 0;   // Totally off left.
		}
	}

	// Clip against right.
	if ((x + w) > RCLIP) {
		// Don't worry about byte boundaries here - the blit
		// will just skip any sub-byte data at the end of each line.
		w -= ((x+w) - RCLIP);
		if (w <= 0) {
			return 0;   // Totally off right.
		}
	}
	src.width = w;

	int16_t h = src.height;
	// Clip against top.
	if (y < TCLIP) {
		int16_t adj = TCLIP-y;
		h -= adj;
		if (h <= 0) {
			return 0;   // Totally off top.
		}
		y += adj;
		// Stride is in bytes.
		src.address += adj * src.stride;
	}

	// Clip against bottom.
	if ((y + h) > BCLIP) {
		h -= ((y + h) - BCLIP);
		if (h <= 0) {
			return 0;   // Totally off bottom.
		}
	}
	src.height = h;

	uint32_t offset = (y * FRAME_WIDTH) + x;
	struct BlitterArea target = {
		.address = (uint16_t)(offset & 0xFFFF),
		.page = (uint8_t)(0x80 + (offset >> 16)),
		.padding = 0,
		.stride = FRAME_WIDTH,
		.format = destFmt,
	};

	return internalBLTComplexCopy(action, &src, &target);
}


// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
