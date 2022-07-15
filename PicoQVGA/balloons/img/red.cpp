#include "include.h"

// format: 8-bit pixel graphics
// image width: 40 pixels
// image height: 91 lines
// image pitch: 40 bytes
const u8 RedImg[3640] __attribute__ ((aligned(4))) = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xA0, 0xA4, 0xA0, 0xC4, 0xC0, 0xA4, 0xA0, 0xA0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0xA4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 
	0xE4, 0xC0, 0xA4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0xC0, 0xE4, 0xE0, 0xE4, 0xE0, 
	0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xC0, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xC0, 0xE4, 0xE4, 0xE4, 0xE4, 0xE4, 0xE4, 0xE4, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE4, 0xE0, 
	0xE0, 0xE4, 0xE4, 0xE0, 0xE0, 0xE4, 0xA0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0xE4, 0xE4, 0xE4, 0xE4, 0xE4, 0xE5, 0xE4, 0xE5, 
	0xE4, 0xE4, 0xE4, 0xE4, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE4, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA4, 0xE0, 
	0xE4, 0xE4, 0xE5, 0xE9, 0xE9, 0xE9, 0xE9, 0xE9, 0xE9, 0xE9, 0xE5, 0xE4, 0xE4, 0xE4, 0xE0, 0xE0, 
	0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0xE0, 0xE4, 0xE4, 0xE9, 0xE8, 0xE9, 0xED, 0xED, 0xED, 0xED, 
	0xED, 0xED, 0xE9, 0xE9, 0xE4, 0xE4, 0xE4, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 
	0xE0, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE4, 0xE4, 0xE4, 
	0xE9, 0xE9, 0xE9, 0xED, 0xED, 0xED, 0xF2, 0xED, 0xED, 0xED, 0xED, 0xE9, 0xE9, 0xE5, 0xE4, 0xE0, 
	0xE0, 0xE0, 0xE0, 0xE4, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE4, 0xE5, 0xE9, 0xE9, 0xED, 0xF1, 0xF2, 0xF2, 0xF1, 0xF2, 
	0xF2, 0xF1, 0xED, 0xED, 0xE9, 0xE8, 0xE4, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE4, 0xE0, 
	0xE0, 0xE4, 0xE0, 0xC4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC4, 0xE0, 0xE4, 0xE4, 0xE8, 
	0xE9, 0xED, 0xED, 0xF2, 0xF1, 0xF6, 0xD6, 0xF6, 0xF2, 0xF2, 0xED, 0xED, 0xE9, 0xE9, 0xE4, 0xE4, 
	0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE0, 0xA0, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xE4, 0xE0, 0xE4, 0xE4, 0xE9, 0xE9, 0xED, 0xED, 0xF2, 0xF2, 0xD6, 0xF6, 0xD6, 
	0xD6, 0xF2, 0xF2, 0xED, 0xED, 0xE9, 0xE5, 0xE4, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 
	0xE0, 0xE4, 0xE4, 0xE0, 0xE4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC4, 0xE0, 0xE0, 0xE4, 0xE4, 0xE9, 
	0xE9, 0xED, 0xF1, 0xF2, 0xF6, 0xD6, 0xF6, 0xD6, 0xF6, 0xF2, 0xF1, 0xED, 0xE9, 0xE9, 0xE8, 0xE4, 
	0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xA0, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xE0, 0xE4, 0xE0, 0xE4, 0xE4, 0xE9, 0xE9, 0xED, 0xEE, 0xF1, 0xD2, 0xF6, 0xF6, 0xD6, 
	0xF2, 0xF1, 0xEE, 0xED, 0xED, 0xE9, 0xE4, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 
	0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xFF, 0xFF, 0xFF, 0xA0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE4, 
	0xE9, 0xED, 0xED, 0xED, 0xF2, 0xF1, 0xF2, 0xF1, 0xF2, 0xF1, 0xED, 0xED, 0xE9, 0xE9, 0xE4, 0xE4, 
	0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0x80, 0xFF, 
	0xFF, 0xC4, 0xE0, 0xE0, 0xE0, 0xE0, 0xE4, 0xE4, 0xE9, 0xE9, 0xED, 0xED, 0xED, 0xF2, 0xED, 0xF2, 
	0xED, 0xED, 0xED, 0xE9, 0xE9, 0xE4, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 
	0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xA4, 0xFF, 0xFF, 0xE0, 0xE0, 0xE4, 0xE4, 0xE4, 0xE0, 0xE4, 
	0xE4, 0xE8, 0xE9, 0xE9, 0xED, 0xED, 0xED, 0xED, 0xED, 0xED, 0xE9, 0xE9, 0xE4, 0xE4, 0xE4, 0xE4, 
	0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xFF, 
	0x60, 0xE4, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE5, 0xE8, 0xE9, 0xE9, 0xE9, 0xE9, 0xE9, 
	0xE9, 0xE9, 0xE9, 0xE4, 0xE4, 0xE4, 0xE0, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 
	0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xFF, 0xA0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 
	0xE0, 0xE4, 0xE4, 0xE4, 0xE4, 0xE9, 0xE8, 0xE9, 0xE4, 0xE4, 0xE4, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 
	0xE4, 0xE0, 0xE0, 0xE4, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xFF, 
	0xC4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE4, 0xE4, 0xE4, 0xE4, 
	0xE4, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 
	0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0x80, 0xC0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE4, 
	0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 
	0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0x80, 
	0xA4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE4, 0xE0, 0xE4, 0xE4, 
	0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 
	0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0x84, 0xA0, 0xE4, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 
	0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 
	0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE4, 0xE0, 0xE0, 0x80, 
	0xC4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 
	0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 
	0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE4, 0x60, 0xA0, 0xC0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 
	0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 
	0xE0, 0xE0, 0xE0, 0xE0, 0xE4, 0xE4, 0xE0, 0xE0, 0xE4, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xFF, 
	0x80, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 
	0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 
	0xE0, 0xE0, 0xE4, 0xE4, 0xE0, 0xC4, 0xC0, 0xFF, 0xFF, 0xC0, 0xC4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 
	0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 
	0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xC4, 0xFF, 
	0xFF, 0xC4, 0xE0, 0xC4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 
	0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 
	0xE0, 0xE4, 0xE0, 0xE4, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 0xA0, 0xC4, 0xC0, 0xE0, 0xE4, 0xE4, 0xE0, 
	0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 
	0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xC0, 0xC4, 0xA4, 0xFF, 
	0xFF, 0x84, 0xC0, 0xE0, 0xC4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 
	0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 
	0xE0, 0xE4, 0xC0, 0xC4, 0xC0, 0xC0, 0x80, 0xFF, 0xFF, 0x60, 0xC4, 0xC0, 0xC0, 0xC4, 0xE0, 0xC0, 
	0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 
	0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE4, 0xC0, 0xE0, 0xC0, 0xE0, 0xC4, 0xC4, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xA4, 0xC0, 0xC4, 0xC0, 0xC4, 0xE0, 0xC0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 
	0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xC0, 
	0xE4, 0xC0, 0xC4, 0xC4, 0xC0, 0xA0, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0xC4, 0xC0, 0xC4, 0xC0, 0xC0, 
	0xC4, 0xE0, 0xC0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE0, 
	0xE0, 0xE4, 0xE4, 0xE0, 0xC4, 0xE0, 0xC0, 0xE4, 0xC0, 0xC4, 0xC0, 0xC0, 0xC0, 0x84, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xC0, 0xC4, 0xC0, 0xC0, 0xC4, 0xC0, 0xC4, 0xE0, 0xC4, 0xE0, 0xC0, 0xE4, 0xE0, 
	0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE4, 0xE0, 0xE0, 0xE0, 0xE4, 0xE0, 0xC4, 0xE0, 0xC0, 
	0xC4, 0xC0, 0xC4, 0xC0, 0xA4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA0, 0xA0, 0xC4, 0xC0, 0xC4, 
	0xC0, 0xC0, 0xC4, 0xC0, 0xC4, 0xE0, 0xC4, 0xC0, 0xE0, 0xC4, 0xE0, 0xC0, 0xE4, 0xC0, 0xE0, 0xC4, 
	0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xC4, 0xC0, 0xC0, 0xC4, 0xA0, 0xC0, 0x80, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x60, 0xC4, 0xA0, 0xC4, 0xC0, 0xC0, 0xC4, 0xC0, 0xC0, 0xC0, 0xC4, 0xE0, 0xC0, 
	0xC4, 0xC0, 0xC4, 0xE0, 0xC4, 0xC0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xC0, 
	0xC4, 0xC0, 0xC0, 0xA4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA4, 0xA0, 0xA0, 0xA4, 
	0xC0, 0xC4, 0xC0, 0xC4, 0xC4, 0xC0, 0xC0, 0xC4, 0xC0, 0xE4, 0xC0, 0xC4, 0xC0, 0xE0, 0xE4, 0xE0, 
	0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xC0, 0xC4, 0xC0, 0xA0, 0xA4, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x60, 0xA4, 0xC0, 0xC0, 0xA4, 0xC0, 0xC0, 0xC0, 0xC0, 0xC4, 0xC0, 0xC4, 
	0xC0, 0xC0, 0xC0, 0xC4, 0xE0, 0xE4, 0xE0, 0xE4, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xE4, 0xE4, 0xC0, 
	0xC4, 0xA0, 0xA4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0xA4, 0xA0, 
	0xA0, 0xA4, 0xC4, 0xA0, 0xC4, 0xC0, 0xC4, 0xC0, 0xC4, 0xC4, 0xC0, 0xC0, 0xC4, 0xE0, 0xE0, 0xE0, 
	0xE0, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xC0, 0xC4, 0xC0, 0xA4, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA4, 0xA0, 0xC4, 0xA0, 0xA4, 0xC0, 0xA0, 0xA4, 0xC0, 0xA0, 
	0xC0, 0xC0, 0xC0, 0xC4, 0xE0, 0xE0, 0xE4, 0xE4, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xC4, 0xC0, 
	0xA4, 0xA0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x60, 0xA0, 
	0xA0, 0xA4, 0xA0, 0xA0, 0xA4, 0xA0, 0xC4, 0xA0, 0xC4, 0xA0, 0xC4, 0xC0, 0xC0, 0xE4, 0xE0, 0xE0, 
	0xE4, 0xE0, 0xE0, 0xE4, 0xE0, 0xC4, 0xC0, 0xA4, 0xA0, 0x60, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x84, 0xA0, 0xA0, 0xA4, 0xA4, 0xA0, 0xC0, 0xA0, 0xA4, 
	0xC0, 0xA4, 0xC0, 0xC4, 0xC0, 0xE0, 0xE4, 0xE0, 0xE0, 0xE4, 0xE4, 0xE0, 0xC4, 0xC0, 0xA0, 0xA0, 
	0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0x84, 0xA0, 0xA0, 0xA0, 0xA4, 0xA0, 0xA4, 0xA0, 0xA0, 0xA0, 0xA4, 0xC0, 0xC4, 0xC0, 0xE4, 0xE0, 
	0xE4, 0xE0, 0xC0, 0xC4, 0xC0, 0xA4, 0xA0, 0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x84, 0x80, 0xA4, 0xA0, 0xA4, 0xA0, 0xA4, 
	0xA0, 0xA4, 0xA0, 0xA0, 0xC0, 0xC4, 0xC0, 0xC4, 0xC0, 0xC0, 0xC4, 0xC0, 0xA0, 0xA0, 0xA0, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0x60, 0xA4, 0xA0, 0x84, 0xA0, 0xA0, 0xA0, 0xA4, 0xA0, 0xA0, 0xA4, 0xA4, 0xA0, 0xA4, 0xC0, 
	0xC0, 0xA4, 0xA0, 0xA4, 0xA0, 0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x60, 0x84, 0x80, 0x80, 0x84, 0xA0, 
	0xA0, 0xA4, 0xA0, 0xA0, 0xA0, 0xA4, 0xA0, 0xA0, 0xA4, 0xA0, 0xA0, 0x80, 0x84, 0x60, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x60, 0x84, 0x80, 0x84, 0x84, 0x80, 0x80, 0x84, 0x84, 0xA0, 0x80, 0xA4, 0x80, 
	0x84, 0x80, 0x84, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x84, 0x80, 0x80, 
	0x80, 0x84, 0xA0, 0x80, 0x80, 0x84, 0x80, 0x84, 0x80, 0x84, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x64, 0x80, 0x84, 0x80, 0x80, 0x84, 0x84, 0x80, 0x80, 0x80, 
	0x80, 0x60, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0x60, 0x84, 0x80, 0x80, 0x80, 0x84, 0x80, 0x64, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x64, 0x60, 0x64, 0x60, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x40, 0x20, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x44, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
};