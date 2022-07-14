#include "include.h"

// format: 4-bit pixel graphics
// image width: 64 pixels
// image height: 145 lines
// image pitch: 32 bytes
const u8 RedImg[4640] __attribute__ ((aligned(4))) = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xCC, 
	0xCC, 0x7C, 0xC6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x44, 0x44, 0x4C, 0x4C, 
	0x44, 0x4C, 0x44, 0x44, 0x44, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC4, 0x44, 0x44, 0x44, 0x44, 0xC4, 
	0x44, 0xC4, 0x4C, 0x44, 0x44, 0x44, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF4, 0x44, 0x44, 0x44, 0xC4, 0xCC, 0x4C, 0x44, 
	0xCC, 0x44, 0xC4, 0x4C, 0x44, 0x44, 0xC4, 0x4F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF4, 0x44, 0x44, 0xC4, 0xC4, 0x4C, 0x44, 0xC4, 0xC4, 
	0x44, 0xC4, 0x4C, 0x44, 0xCC, 0x4C, 0x44, 0xC4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x4C, 0xCC, 0x4C, 0x4C, 0xC4, 0xC4, 0xC4, 0x4C, 
	0x4C, 0x4C, 0x44, 0xC4, 0x44, 0xC4, 0xC4, 0x44, 0xC4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF4, 0xC4, 0xCC, 0x4C, 0x4C, 0xC4, 0xCC, 0xCC, 0x4C, 0x4C, 
	0x44, 0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0xC4, 0x4C, 0x4F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF4, 0xCC, 0x4C, 0xC4, 0xCC, 0xCC, 0xCC, 0xCC, 0xC4, 0xCC, 0xC4, 
	0x4C, 0x44, 0xC4, 0x44, 0xC4, 0x44, 0xC4, 0xC4, 0xC4, 0x4C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x4C, 0x4C, 0x4C, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x4C, 
	0xC4, 0xC4, 0xC4, 0xC4, 0x4C, 0x4C, 0x4C, 0x44, 0xC4, 0xC4, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xF4, 0x44, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 
	0xC4, 0xC4, 0x4C, 0x4C, 0x44, 0xC4, 0x44, 0xC4, 0x4C, 0x44, 0x4C, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x4C, 0x44, 0xC4, 0xCC, 0xCC, 0xCE, 0xCC, 0xEC, 0xEC, 0xEC, 0xCC, 0xCC, 
	0xCC, 0x4C, 0x44, 0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0xC4, 0xC4, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xF4, 0x44, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xC7, 0xCE, 0xDC, 0xCC, 0xEC, 0xCC, 
	0xCC, 0xCC, 0xCC, 0x44, 0xC4, 0x44, 0xC4, 0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xF4, 0x4C, 0x4C, 0xCC, 0xCC, 0xCE, 0xC7, 0x77, 0x7C, 0x7E, 0xDE, 0xCC, 0xEC, 
	0xCC, 0xC4, 0xC4, 0x4C, 0x4C, 0x4C, 0x44, 0x4C, 0x44, 0xC4, 0x44, 0xC4, 0x4F, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x44, 0xC4, 0xCC, 0xCC, 0xCE, 0xCC, 0x7C, 0xC7, 0xE7, 0x7C, 0x7C, 0x7C, 0xCC, 
	0xCC, 0xCC, 0xCC, 0x44, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0x4C, 0xC4, 0x44, 0xC4, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xF4, 0xC4, 0x4C, 0x4C, 0xCC, 0xCC, 0xC7, 0xC7, 0x77, 0xD7, 0x77, 0x77, 0xC7, 0xCE, 
	0xCC, 0xC4, 0xC4, 0xCC, 0x44, 0x44, 0xC4, 0x4C, 0x44, 0xC4, 0x44, 0xC4, 0x4C, 0x0F, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFC, 0x44, 0xCC, 0xCC, 0xCC, 0xC7, 0xCC, 0xE7, 0x77, 0x77, 0x77, 0x7C, 0x7C, 0xCC, 
	0xCC, 0xCC, 0xCC, 0x44, 0xC4, 0xCC, 0x44, 0xC4, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0xCF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0x44, 0xC4, 0xC4, 0xCC, 0xCC, 0xEC, 0x77, 0x7D, 0x77, 0x77, 0x77, 0x77, 0x7E, 0xDE, 
	0xCC, 0xCC, 0xC4, 0xC4, 0x4C, 0x44, 0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0xC4, 0xC4, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xC4, 0x4C, 0x4C, 0xCC, 0xCC, 0xCC, 0xEC, 0x77, 0x77, 0x77, 0x77, 0x77, 0xC7, 0xCC, 
	0x7C, 0xCC, 0xCC, 0x4C, 0x44, 0xC4, 0x4C, 0x44, 0xC4, 0x44, 0xC4, 0xC4, 0x4C, 0x44, 0xFF, 0xFF, 
	0xFF, 0xF4, 0x44, 0xC4, 0xC4, 0xCC, 0xCC, 0x7C, 0xC7, 0xE7, 0x77, 0x77, 0x77, 0x77, 0x7E, 0x7C, 
	0xCC, 0xCC, 0xC4, 0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0xC4, 0x4C, 0x4F, 0xFF, 
	0xFF, 0xFC, 0x4C, 0x44, 0x4C, 0xCC, 0xCC, 0xCC, 0xCD, 0xC7, 0x77, 0x77, 0x77, 0x77, 0xC7, 0xCC, 
	0xEC, 0xCC, 0xCC, 0xC4, 0xC4, 0xC4, 0x44, 0xC4, 0x44, 0xC4, 0xC4, 0x4C, 0x44, 0xC4, 0x4F, 0xFF, 
	0xFF, 0x44, 0xC4, 0xCC, 0x4C, 0xCC, 0xCC, 0xCE, 0xDE, 0x7E, 0xD7, 0x77, 0x77, 0xC7, 0xEC, 0xCC, 
	0xCC, 0xCC, 0xC4, 0x44, 0x4C, 0x44, 0xC4, 0x4C, 0x4C, 0x44, 0x4C, 0x4C, 0x4C, 0x44, 0xCF, 0xFF, 
	0xFF, 0x4C, 0x44, 0x44, 0xC4, 0xCC, 0xCC, 0xCC, 0xCC, 0xC7, 0xE7, 0xC7, 0xE7, 0x7C, 0xDC, 0xEC, 
	0xCC, 0xCC, 0xCC, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0x44, 0xC4, 0x44, 0xFF, 
	0xFF, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0xCC, 0xCC, 0xCE, 0xCC, 0xDE, 0x7E, 0xDE, 0xCE, 0xC7, 0xCC, 
	0xCC, 0xCC, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0x4C, 0x44, 0x4C, 0x44, 0x44, 0xCC, 0x44, 0xC4, 0xFF, 
	0xFC, 0x44, 0x4C, 0x4C, 0x44, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xEC, 0xCC, 0xCD, 0xCD, 0xCC, 0xCC, 
	0xCC, 0xCC, 0xC4, 0x44, 0xC4, 0x44, 0x4C, 0xC4, 0xC4, 0xC4, 0xC4, 0xCC, 0x44, 0x4C, 0x44, 0xFF, 
	0xFC, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0xCC, 0xCC, 0xCC, 0x7C, 0xCC, 0x7C, 0xEC, 0xEC, 0xEC, 0xCC, 
	0xCC, 0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0x44, 0x4C, 0x44, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0xFF, 
	0xF4, 0x4C, 0xC4, 0x4C, 0x44, 0x44, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 
	0xCC, 0xC4, 0xC4, 0xC4, 0x44, 0xC4, 0xC4, 0xCC, 0x44, 0xCC, 0x44, 0xC4, 0x4C, 0x44, 0x44, 0x4F, 
	0xF4, 0x44, 0x44, 0xC4, 0xC4, 0xC4, 0x4C, 0xCC, 0xCC, 0xCC, 0xCC, 0x7C, 0xCC, 0xCC, 0xCC, 0xCC, 
	0xCC, 0x4C, 0x44, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0x4C, 0x44, 0x4C, 0x44, 0xC4, 0x4C, 0xC4, 0x4F, 
	0xC4, 0xC4, 0xCC, 0x44, 0x4C, 0x4C, 0x44, 0x4C, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xC4, 
	0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0x44, 0xC4, 0xC4, 0x4C, 0xC4, 0x4C, 0x44, 0xC4, 0x44, 0xCF, 
	0xC4, 0x4C, 0x44, 0xC4, 0xC4, 0x44, 0xC4, 0x4C, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xC4, 
	0x44, 0x44, 0x4C, 0x44, 0x4C, 0x44, 0xC4, 0x4C, 0x4C, 0x44, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0x4F, 
	0xCC, 0x44, 0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0x44, 0xCC, 0xCC, 0xCC, 0xCC, 0xC4, 0xC4, 0x4C, 
	0x4C, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0x44, 0xC4, 0xC4, 0xC4, 0x4C, 0x44, 0x44, 0xCF, 
	0xC4, 0xC4, 0xC4, 0xC4, 0x4C, 0x44, 0xC4, 0x4C, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0xC4, 
	0xC4, 0x4C, 0x44, 0xC4, 0x4C, 0x44, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0x4C, 0xC4, 0x4C, 0xC4, 0x4F, 
	0xC4, 0x4C, 0x44, 0xC4, 0xC4, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0xC4, 0x44, 0xC4, 0x4C, 0x4C, 0x44, 
	0x44, 0xC4, 0x4C, 0x44, 0xC4, 0x4C, 0xC4, 0x44, 0xC4, 0x44, 0x4C, 0xC4, 0x44, 0xCC, 0x44, 0x4F, 
	0x4C, 0xC4, 0xC4, 0x4C, 0x4C, 0x44, 0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 
	0xC4, 0x4C, 0x44, 0xC4, 0x4C, 0x44, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0x4C, 0x4C, 0x44, 0xC4, 0x4F, 
	0xC4, 0x4C, 0x4C, 0x44, 0xC4, 0xC4, 0x4C, 0x44, 0x44, 0x44, 0xC4, 0xC4, 0xC4, 0x4C, 0x44, 0x4C, 
	0x4C, 0x44, 0xC4, 0x4C, 0x44, 0xCC, 0x44, 0xC4, 0x44, 0xC4, 0xCC, 0x44, 0xC4, 0xC4, 0x4C, 0x4F, 
	0x44, 0xC4, 0x4C, 0x4C, 0x4C, 0x44, 0xC4, 0xCC, 0x4C, 0xC4, 0x4C, 0x44, 0x4C, 0x44, 0xC4, 0xC4, 
	0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0xC4, 0xC4, 0x4C, 0xC4, 0x4F, 
	0xC4, 0x44, 0xC4, 0xC4, 0xC4, 0x4C, 0x44, 0x44, 0xC4, 0x44, 0xC4, 0x4C, 0xC4, 0x4C, 0x4C, 0x44, 
	0x44, 0xC4, 0x44, 0xC4, 0xC4, 0xC4, 0xC4, 0x44, 0xC4, 0xC4, 0x4C, 0x4C, 0x44, 0xC4, 0x44, 0x4F, 
	0xC4, 0xCC, 0x4C, 0x44, 0x4C, 0xC4, 0xC4, 0xC4, 0x4C, 0x4C, 0x4C, 0x44, 0x4C, 0x44, 0xC4, 0x4C, 
	0xC4, 0xC4, 0xC4, 0x44, 0x44, 0x4C, 0x4C, 0x4C, 0x44, 0x4C, 0x4C, 0x44, 0xCC, 0x4C, 0x4C, 0x4F, 
	0xC4, 0x44, 0xC4, 0xC4, 0xC4, 0x44, 0x4C, 0x4C, 0x44, 0xC4, 0x44, 0xCC, 0x44, 0xC4, 0x4C, 0x44, 
	0x4C, 0x4C, 0x4C, 0xC4, 0xCC, 0x44, 0x44, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0x4C, 0x4C, 0x44, 0x4F, 
	0xC4, 0x4C, 0x44, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0x4C, 0x4C, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0xC4, 
	0xC4, 0x44, 0x44, 0x4C, 0x44, 0x4C, 0x4C, 0x44, 0x4C, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0xC4, 0x4F, 
	0xF4, 0xC4, 0x4C, 0xC4, 0x4C, 0x4C, 0x44, 0x4C, 0x4C, 0x44, 0xC4, 0x4C, 0x44, 0x4C, 0x44, 0x4C, 
	0x4C, 0x4C, 0xC4, 0xC4, 0x4C, 0xC4, 0xC4, 0x4C, 0xC4, 0xC4, 0x4C, 0x44, 0xC4, 0x44, 0xC4, 0x4F, 
	0xF4, 0x4C, 0xC4, 0x4C, 0xC4, 0xC4, 0xCC, 0x44, 0xC4, 0xC4, 0x4C, 0x44, 0xCC, 0x44, 0xC4, 0xC4, 
	0x44, 0xC4, 0x4C, 0x4C, 0x44, 0x4C, 0x4C, 0x44, 0x44, 0x4C, 0xC4, 0xC4, 0x4C, 0xC4, 0x44, 0x4F, 
	0xF4, 0x44, 0x4C, 0x44, 0x4C, 0x44, 0x44, 0xC4, 0x4C, 0x4C, 0x4C, 0xC4, 0x44, 0xC4, 0x4C, 0x4C, 
	0x4C, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0x4C, 0x4C, 0xC4, 0xC4, 0x4C, 0x4C, 0x44, 0x4C, 0xC4, 0x4F, 
	0xF4, 0xC4, 0xC4, 0xC4, 0xC4, 0x4C, 0xC4, 0xC4, 0xC4, 0x44, 0xC4, 0x44, 0xC4, 0xC4, 0xC4, 0x44, 
	0xC4, 0x44, 0x4C, 0x44, 0x4C, 0x44, 0xC4, 0xC4, 0x4C, 0x44, 0xC4, 0xC4, 0x4C, 0xC4, 0x4C, 0x0F, 
	0xF4, 0x4C, 0x44, 0x4C, 0x44, 0xC4, 0x4C, 0x4C, 0x44, 0xCC, 0x44, 0xCC, 0x4C, 0x44, 0xC4, 0xCC, 
	0x44, 0xCC, 0xC4, 0x4C, 0xC4, 0x4C, 0x4C, 0x44, 0xC4, 0x4C, 0x4C, 0x44, 0xC4, 0x4C, 0x44, 0xFF, 
	0xFF, 0x44, 0xC4, 0xC4, 0xC4, 0x4C, 0x44, 0xC4, 0x4C, 0x44, 0xC4, 0x44, 0xC4, 0xC4, 0x4C, 0x44, 
	0xC4, 0x44, 0x4C, 0x44, 0x4C, 0x4C, 0x44, 0xCC, 0x4C, 0xC4, 0x44, 0xCC, 0x4C, 0x44, 0xC4, 0xFF, 
	0xFF, 0x44, 0x4C, 0x44, 0xCC, 0x44, 0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0xC4, 0x4C, 0x4C, 0x44, 0xC4, 
	0xC4, 0xC4, 0xC4, 0xCC, 0x4C, 0x44, 0xC4, 0x44, 0xC4, 0x4C, 0xC4, 0x44, 0xC4, 0xC4, 0x44, 0xFF, 
	0xFF, 0x4C, 0xC4, 0x4C, 0x44, 0x4C, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0x44, 0xC4, 0xC4, 0xCC, 0x4C, 
	0x4C, 0x4C, 0x4C, 0x44, 0xC4, 0x4C, 0x44, 0xCC, 0x44, 0xC4, 0x44, 0xC4, 0x4C, 0x44, 0x44, 0xFF, 
	0xFF, 0x84, 0x44, 0xC4, 0x4C, 0xC4, 0x4C, 0x4C, 0x44, 0x4C, 0x44, 0xCC, 0x44, 0x4C, 0x44, 0x44, 
	0xC4, 0x44, 0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0x4C, 0x44, 0xCC, 0x44, 0xC4, 0xC4, 0x4F, 0xFF, 
	0xFF, 0xF4, 0x4C, 0x4C, 0x44, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0xC4, 0x44, 0xCC, 0x44, 0xC4, 0xC4, 
	0x4C, 0xC4, 0x4C, 0xC4, 0xC4, 0x44, 0xC4, 0xC4, 0xC4, 0xC4, 0x44, 0xCC, 0x44, 0x4C, 0x4F, 0xFF, 
	0xFF, 0xF4, 0xC4, 0x44, 0xCC, 0x44, 0xC4, 0xC4, 0xC4, 0xC4, 0x4C, 0x4C, 0x44, 0xC4, 0x4C, 0x4C, 
	0x44, 0x4C, 0x44, 0x44, 0x4C, 0xC4, 0x44, 0xC4, 0xC4, 0x4C, 0xC4, 0x44, 0xCC, 0x44, 0x4F, 0xFF, 
	0xFF, 0xF8, 0x44, 0xCC, 0x44, 0x4C, 0x44, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0x4C, 0x44, 0xC4, 0xC4, 
	0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0x44, 0xCC, 0x4C, 0x44, 0xC4, 0x4C, 0x4C, 0x44, 0xC4, 0xFF, 0xFF, 
	0xFF, 0xFF, 0x4C, 0x44, 0x4C, 0xC4, 0x4C, 0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0x4C, 
	0x4C, 0x44, 0x4C, 0x4C, 0x44, 0xCC, 0x44, 0xC4, 0x4C, 0x44, 0xC4, 0xC4, 0x44, 0x44, 0xFF, 0xFF, 
	0xFF, 0xFF, 0x44, 0x4C, 0x44, 0x4C, 0x44, 0x4C, 0x4C, 0x44, 0xC4, 0x4C, 0x44, 0xC4, 0xCC, 0x44, 
	0xC4, 0xC4, 0xC4, 0x44, 0xC4, 0x44, 0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0xCF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xF4, 0x44, 0xC4, 0x44, 0xCC, 0x44, 0xC4, 0xC4, 0x4C, 0x44, 0xC4, 0xC4, 0x4C, 0x4C, 
	0x44, 0xC4, 0xC4, 0xC4, 0x4C, 0x4C, 0x4C, 0x44, 0xC4, 0xC4, 0x44, 0x44, 0xC4, 0x4F, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xF4, 0xC4, 0x44, 0x4C, 0x44, 0xC4, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0x4C, 0x44, 0xC4, 
	0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0xC4, 0x44, 0xCC, 0x44, 0x4C, 0x4C, 0x44, 0x4C, 0x0F, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x44, 0x44, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0x4C, 0x44, 0xC4, 0x4C, 0x44, 
	0xC4, 0x4C, 0x44, 0x4C, 0x4C, 0x44, 0xCC, 0x44, 0xC4, 0xC4, 0xC4, 0x44, 0x44, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x44, 0x4C, 0x44, 0x44, 0x4C, 0x4C, 0x4C, 0x44, 0xC4, 0xC4, 0xC4, 0xC4, 0xCC, 
	0x4C, 0xC4, 0xC4, 0xC4, 0x44, 0xCC, 0x44, 0xC4, 0x4C, 0x44, 0x44, 0x44, 0xC0, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xF4, 0x44, 0x44, 0x44, 0x44, 0xC4, 0x44, 0xC4, 0x4C, 0x44, 0x4C, 0x4C, 0x44, 
	0x44, 0xC4, 0x4C, 0x4C, 0xC4, 0x44, 0xC4, 0x4C, 0x44, 0xCC, 0x44, 0x44, 0x4F, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xF4, 0xC4, 0x44, 0xC4, 0x44, 0x44, 0x4C, 0x4C, 0xC4, 0x4C, 0xC4, 0x44, 0x4C, 
	0x4C, 0x4C, 0x4C, 0x44, 0x4C, 0xC4, 0xC4, 0xC4, 0xCC, 0x44, 0x4C, 0x44, 0x8F, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x44, 0x44, 0xC4, 0x44, 0x44, 0x44, 0x44, 0xC4, 0x44, 0xCC, 0xC4, 
	0xC4, 0x44, 0xC4, 0x4C, 0xC4, 0x4C, 0x4C, 0x44, 0x44, 0xC4, 0xC4, 0x44, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x84, 0x44, 0x44, 0x44, 0x4C, 0x44, 0x44, 0x44, 0x4C, 0x4C, 0x44, 0x4C, 
	0x4C, 0x4C, 0x4C, 0xC4, 0x4C, 0x44, 0xC4, 0xCC, 0x4C, 0x44, 0x44, 0x4F, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xF4, 0x44, 0x44, 0x44, 0x44, 0x44, 0xC4, 0xC4, 0x44, 0x44, 0x44, 0x44, 
	0x44, 0xC4, 0x44, 0x4C, 0x44, 0xC4, 0x4C, 0x44, 0xC4, 0xC4, 0x44, 0x4F, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x4C, 0xC4, 
	0xCC, 0x4C, 0xC4, 0xC4, 0xCC, 0x4C, 0xC4, 0xC4, 0xC4, 0x44, 0x44, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x84, 0x44, 0x44, 0x44, 0xC4, 0x44, 0x44, 0xC4, 0xC4, 0x44, 0x4C, 
	0x44, 0x44, 0x4C, 0x4C, 0x44, 0xC4, 0x44, 0x4C, 0x44, 0x44, 0x4F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF4, 0x44, 0x44, 0x44, 0x44, 0x44, 0x4C, 0x44, 0x44, 0x4C, 0x44, 
	0xC4, 0xCC, 0x44, 0xC4, 0xC4, 0x4C, 0x4C, 0x44, 0x44, 0x44, 0x4F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0xC4, 0x44, 0xC4, 
	0x4C, 0x44, 0xC4, 0x4C, 0x4C, 0x4C, 0x4C, 0xC4, 0x44, 0x44, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF4, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 
	0xC4, 0x4C, 0x4C, 0xC4, 0x44, 0xC4, 0x44, 0x44, 0x44, 0x4F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x4C, 
	0x44, 0xC4, 0xC4, 0x44, 0xCC, 0x44, 0xC4, 0x44, 0x44, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 
	0x4C, 0x4C, 0x4C, 0x4C, 0x44, 0x44, 0x44, 0x44, 0x48, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 
	0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x4F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 
	0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x48, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x44, 0x84, 0x44, 0x44, 0x44, 0x44, 0x44, 
	0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 
	0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x48, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x44, 0x84, 0x44, 0x84, 0x44, 0x44, 
	0x44, 0x44, 0x44, 0x44, 0x84, 0x44, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x84, 0x44, 0x84, 0x44, 0x44, 0x84, 
	0x48, 0x44, 0x48, 0x44, 0x44, 0x4F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x44, 0x48, 0x44, 0x44, 
	0x44, 0x48, 0x44, 0x44, 0x48, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x44, 0x44, 0x44, 0x84, 
	0x44, 0x44, 0x44, 0x48, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x44, 0x44, 0x44, 
	0x44, 0x84, 0x48, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x84, 0x84, 
	0x84, 0x48, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x40, 
	0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x08, 
	0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x84, 
	0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x08, 
	0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
};
