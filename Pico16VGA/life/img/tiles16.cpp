#include "include.h"

// format: 4-bit pixel graphics
// image width: 16 pixels
// image height: 64 lines
// image pitch: 8 bytes
const u8 Tiles16Img[512] __attribute__ ((aligned(4))) = {
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x71, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x71, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x71, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x71, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x71, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x71, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x71, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x71, 0x71, 0x71, 0x71, 0x71, 0x71, 0x71, 0x71, 
	0x11, 0x11, 0x11, 0x88, 0x82, 0x11, 0x11, 0x11, 0x71, 0x11, 0x82, 0x22, 0x22, 0x2A, 0x11, 0x11, 
	0x11, 0x88, 0x2E, 0xE2, 0x22, 0x22, 0xA1, 0x11, 0x71, 0x82, 0xEE, 0x22, 0x22, 0x62, 0xAE, 0x11, 
	0x18, 0x22, 0xE2, 0x62, 0x26, 0x22, 0x2A, 0xE1, 0x78, 0x22, 0x22, 0x22, 0x22, 0x22, 0x2A, 0xE1, 
	0x82, 0x22, 0x2C, 0xC2, 0x22, 0x22, 0x2A, 0xE1, 0x82, 0x22, 0xCE, 0xC4, 0x22, 0x22, 0x2A, 0xE1, 
	0x82, 0x22, 0xCC, 0xC4, 0x62, 0x22, 0x2A, 0xE1, 0x82, 0x22, 0x24, 0x44, 0x22, 0x22, 0x2A, 0xE1, 
	0x12, 0x22, 0x22, 0x22, 0x22, 0x22, 0x2A, 0xE1, 0x72, 0x22, 0x22, 0x22, 0x22, 0x22, 0xAE, 0x11, 
	0x11, 0x22, 0x22, 0x22, 0x22, 0x2A, 0xEE, 0x11, 0x71, 0x12, 0x22, 0x22, 0x2A, 0xAE, 0xE1, 0x11, 
	0x11, 0x11, 0xAA, 0xAA, 0xAE, 0xE1, 0x11, 0x11, 0x71, 0x71, 0x71, 0x71, 0x71, 0x71, 0x71, 0x71, 
	0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 
	0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 
	0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 
	0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 
	0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 
	0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 
	0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 
	0x41, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 
	0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x41, 0x11, 0x82, 0x22, 0x22, 0x2A, 0x11, 0x14, 
	0x41, 0x88, 0x2E, 0xE2, 0x22, 0x22, 0xA1, 0x14, 0x41, 0x82, 0xEE, 0x22, 0x22, 0x62, 0xAE, 0x14, 
	0x48, 0x22, 0xE2, 0x62, 0x26, 0x22, 0x2A, 0xE4, 0x48, 0x22, 0x22, 0x22, 0x22, 0x22, 0x2A, 0xE4, 
	0x42, 0x22, 0x2C, 0xC2, 0x22, 0x22, 0x2A, 0xE4, 0x42, 0x22, 0xCE, 0xC4, 0x22, 0x22, 0x2A, 0xE4, 
	0x42, 0x22, 0xCC, 0xC4, 0x62, 0x22, 0x2A, 0xE4, 0x42, 0x22, 0x24, 0x44, 0x22, 0x22, 0x2A, 0xE4, 
	0x42, 0x22, 0x22, 0x22, 0x22, 0x22, 0x2A, 0xE4, 0x42, 0x22, 0x22, 0x22, 0x22, 0x22, 0xAE, 0x14, 
	0x41, 0x22, 0x22, 0x22, 0x22, 0x2A, 0xEE, 0x14, 0x41, 0x12, 0x22, 0x22, 0x2A, 0xAE, 0xE1, 0x14, 
	0x41, 0x11, 0xAA, 0xAA, 0xAE, 0xE1, 0x11, 0x14, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 
};
