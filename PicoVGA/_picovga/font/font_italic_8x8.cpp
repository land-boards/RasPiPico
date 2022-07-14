#include "include.h"

// format: 1-bit pixel graphics
// image width: 2048 pixels
// image height: 8 lines
// image pitch: 256 bytes
const u8 FontItalic8x8[2048] __attribute__ ((aligned(4))) = {
	0x00, 0xAA, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x1E, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 
	0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 
	0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1, 
	0x00, 0x55, 0x0C, 0xF3, 0x36, 0x08, 0x18, 0x08, 0x21, 0x3F, 0x14, 0x1C, 0x00, 0x1C, 0x22, 0x22, 
	0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 
	0x00, 0x08, 0x12, 0x12, 0x04, 0x31, 0x08, 0x04, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x1E, 0x0C, 0x1E, 0x1E, 0x04, 0x3F, 0x1E, 0x3F, 0x1E, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 
	0x1E, 0x1E, 0x3E, 0x1E, 0x3C, 0x3F, 0x3F, 0x1E, 0x21, 0x1F, 0x01, 0x22, 0x20, 0x21, 0x21, 0x1E, 
	0x3E, 0x1E, 0x3E, 0x1E, 0x7F, 0x21, 0x21, 0x21, 0x21, 0x41, 0x3F, 0x07, 0x40, 0x38, 0x08, 0x00, 
	0x0E, 0x00, 0x10, 0x00, 0x02, 0x00, 0x0E, 0x00, 0x20, 0x08, 0x02, 0x10, 0x08, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x04, 0x38, 0x0A, 0x21, 
	0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 
	0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 
	0xFF, 0xF7, 0xED, 0xED, 0xFB, 0xCE, 0xF7, 0xFB, 0xFD, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xE1, 0xF3, 0xE1, 0xE1, 0xFB, 0xC0, 0xE1, 0xC0, 0xE1, 0xE1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1, 
	0xE1, 0xE1, 0xC1, 0xE1, 0xC3, 0xC0, 0xC0, 0xE1, 0xDE, 0xE0, 0xFE, 0xDD, 0xDF, 0xDE, 0xDE, 0xE1, 
	0xC1, 0xE1, 0xC1, 0xE1, 0x80, 0xDE, 0xDE, 0xDE, 0xDE, 0xBE, 0xC0, 0xF8, 0xBF, 0xC7, 0xF7, 0xFF, 
	0xF1, 0xFF, 0xEF, 0xFF, 0xFD, 0xFF, 0xF1, 0xFF, 0xDF, 0xF7, 0xFD, 0xEF, 0xF7, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xFB, 0xC7, 0xF5, 0xDE, 
	0x00, 0xAA, 0x12, 0xED, 0xFF, 0x1C, 0x38, 0x1C, 0x53, 0xED, 0x12, 0x1C, 0x2A, 0x3E, 0x14, 0x1C, 
	0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 
	0x00, 0x08, 0x24, 0x7F, 0x1F, 0x36, 0x14, 0x08, 0x04, 0x08, 0x14, 0x04, 0x00, 0x00, 0x00, 0x01, 
	0x21, 0x34, 0x21, 0x21, 0x1C, 0x20, 0x20, 0x01, 0x21, 0x21, 0x00, 0x08, 0x02, 0x00, 0x10, 0x21, 
	0x2D, 0x21, 0x21, 0x21, 0x22, 0x20, 0x20, 0x21, 0x21, 0x04, 0x01, 0x2C, 0x20, 0x37, 0x31, 0x21, 
	0x21, 0x21, 0x21, 0x20, 0x08, 0x21, 0x21, 0x21, 0x12, 0x22, 0x02, 0x04, 0x20, 0x08, 0x3C, 0x00, 
	0x11, 0x1C, 0x10, 0x0E, 0x02, 0x3C, 0x10, 0x3E, 0x20, 0x00, 0x00, 0x14, 0x08, 0x3C, 0x3C, 0x3C, 
	0x3C, 0x3E, 0x0E, 0x3C, 0x3C, 0x22, 0x22, 0x22, 0x22, 0x22, 0x3E, 0x0C, 0x04, 0x08, 0x14, 0x5D, 
	0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 
	0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 
	0xFF, 0xF7, 0xDB, 0x80, 0xE0, 0xC9, 0xEB, 0xF7, 0xFB, 0xF7, 0xEB, 0xFB, 0xFF, 0xFF, 0xFF, 0xFE, 
	0xDE, 0xCB, 0xDE, 0xDE, 0xE3, 0xDF, 0xDF, 0xFE, 0xDE, 0xDE, 0xFF, 0xF7, 0xFD, 0xFF, 0xEF, 0xDE, 
	0xD2, 0xDE, 0xDE, 0xDE, 0xDD, 0xDF, 0xDF, 0xDE, 0xDE, 0xFB, 0xFE, 0xD3, 0xDF, 0xC8, 0xCE, 0xDE, 
	0xDE, 0xDE, 0xDE, 0xDF, 0xF7, 0xDE, 0xDE, 0xDE, 0xED, 0xDD, 0xFD, 0xFB, 0xDF, 0xF7, 0xC3, 0xFF, 
	0xEE, 0xE3, 0xEF, 0xF1, 0xFD, 0xC3, 0xEF, 0xC1, 0xDF, 0xFF, 0xFF, 0xEB, 0xF7, 0xC3, 0xC3, 0xC3, 
	0xC3, 0xC1, 0xF1, 0xC3, 0xC3, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xC1, 0xF3, 0xFB, 0xF7, 0xEB, 0xA2, 
	0x00, 0x55, 0x22, 0xDD, 0xFF, 0x7E, 0x54, 0x7C, 0x81, 0xFF, 0x24, 0x10, 0x3C, 0x56, 0x7C, 0xFE, 
	0x18, 0xF0, 0x10, 0xF0, 0x1F, 0xFF, 0x1F, 0xFF, 0x10, 0xF0, 0x10, 0xF0, 0x1F, 0xFF, 0x1F, 0xFF, 
	0x00, 0x10, 0x00, 0x24, 0x28, 0x08, 0x18, 0x00, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x06, 
	0x4A, 0x08, 0x02, 0x0E, 0x28, 0x7C, 0x7C, 0x06, 0x7E, 0x42, 0x10, 0x00, 0x0C, 0x3E, 0x08, 0x06, 
	0x55, 0x42, 0x7E, 0x40, 0x42, 0x7C, 0x7C, 0x40, 0x7E, 0x08, 0x02, 0x70, 0x40, 0x5A, 0x52, 0x42, 
	0x42, 0x42, 0x42, 0x3C, 0x10, 0x42, 0x42, 0x42, 0x1C, 0x2C, 0x0C, 0x08, 0x20, 0x10, 0x54, 0x00, 
	0x78, 0x04, 0x3C, 0x10, 0x3C, 0x44, 0x38, 0x44, 0x78, 0x30, 0x04, 0x38, 0x10, 0x54, 0x44, 0x44, 
	0x44, 0x44, 0x30, 0x40, 0x10, 0x44, 0x44, 0x44, 0x2C, 0x44, 0x08, 0x30, 0x08, 0x04, 0x00, 0xA1, 
	0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 
	0xE7, 0x0F, 0xEF, 0x0F, 0xE0, 0x00, 0xE0, 0x00, 0xEF, 0x0F, 0xEF, 0x0F, 0xE0, 0x00, 0xE0, 0x00, 
	0xFF, 0xEF, 0xFF, 0xDB, 0xD7, 0xF7, 0xE7, 0xFF, 0xF7, 0xF7, 0xF7, 0xF7, 0xFF, 0xFF, 0xFF, 0xF9, 
	0xB5, 0xF7, 0xFD, 0xF1, 0xD7, 0x83, 0x83, 0xF9, 0x81, 0xBD, 0xEF, 0xFF, 0xF3, 0xC1, 0xF7, 0xF9, 
	0xAA, 0xBD, 0x81, 0xBF, 0xBD, 0x83, 0x83, 0xBF, 0x81, 0xF7, 0xFD, 0x8F, 0xBF, 0xA5, 0xAD, 0xBD, 
	0xBD, 0xBD, 0xBD, 0xC3, 0xEF, 0xBD, 0xBD, 0xBD, 0xE3, 0xD3, 0xF3, 0xF7, 0xDF, 0xEF, 0xAB, 0xFF, 
	0x87, 0xFB, 0xC3, 0xEF, 0xC3, 0xBB, 0xC7, 0xBB, 0x87, 0xCF, 0xFB, 0xC7, 0xEF, 0xAB, 0xBB, 0xBB, 
	0xBB, 0xBB, 0xCF, 0xBF, 0xEF, 0xBB, 0xBB, 0xBB, 0xD3, 0xBB, 0xF7, 0xCF, 0xF7, 0xFB, 0xFF, 0x5E, 
	0x00, 0xAA, 0x44, 0xBB, 0xFE, 0xFE, 0xFE, 0xFE, 0xA5, 0xFF, 0x28, 0x7C, 0xFE, 0x56, 0xD6, 0x54, 
	0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
	0x00, 0x10, 0x00, 0x24, 0x3E, 0x10, 0x6A, 0x00, 0x10, 0x10, 0x3E, 0x3E, 0x00, 0x3E, 0x00, 0x08, 
	0x52, 0x08, 0x3C, 0x02, 0x48, 0x02, 0x42, 0x18, 0x82, 0x3E, 0x00, 0x00, 0x10, 0x00, 0x04, 0x08, 
	0x5E, 0x7E, 0x42, 0x40, 0x42, 0x40, 0x40, 0x4E, 0x42, 0x08, 0x42, 0x48, 0x40, 0x42, 0x4A, 0x42, 
	0x7C, 0x52, 0x7C, 0x02, 0x10, 0x42, 0x44, 0x42, 0x18, 0x10, 0x30, 0x08, 0x10, 0x10, 0x10, 0x00, 
	0x20, 0x7C, 0x22, 0x20, 0x44, 0x78, 0x10, 0x44, 0x44, 0x10, 0x04, 0x30, 0x10, 0x54, 0x44, 0x44, 
	0x44, 0x44, 0x20, 0x38, 0x10, 0x44, 0x48, 0x54, 0x30, 0x44, 0x30, 0x08, 0x08, 0x18, 0x00, 0xA1, 
	0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 
	0xFF, 0xEF, 0xFF, 0xDB, 0xC1, 0xEF, 0x95, 0xFF, 0xEF, 0xEF, 0xC1, 0xC1, 0xFF, 0xC1, 0xFF, 0xF7, 
	0xAD, 0xF7, 0xC3, 0xFD, 0xB7, 0xFD, 0xBD, 0xE7, 0x7D, 0xC1, 0xFF, 0xFF, 0xEF, 0xFF, 0xFB, 0xF7, 
	0xA1, 0x81, 0xBD, 0xBF, 0xBD, 0xBF, 0xBF, 0xB1, 0xBD, 0xF7, 0xBD, 0xB7, 0xBF, 0xBD, 0xB5, 0xBD, 
	0x83, 0xAD, 0x83, 0xFD, 0xEF, 0xBD, 0xBB, 0xBD, 0xE7, 0xEF, 0xCF, 0xF7, 0xEF, 0xEF, 0xEF, 0xFF, 
	0xDF, 0x83, 0xDD, 0xDF, 0xBB, 0x87, 0xEF, 0xBB, 0xBB, 0xEF, 0xFB, 0xCF, 0xEF, 0xAB, 0xBB, 0xBB, 
	0xBB, 0xBB, 0xDF, 0xC7, 0xEF, 0xBB, 0xB7, 0xAB, 0xCF, 0xBB, 0xCF, 0xF7, 0xF7, 0xE7, 0xFF, 0x5E, 
	0x00, 0x55, 0x48, 0xB7, 0xF8, 0xF8, 0xA8, 0xFC, 0xBB, 0xB6, 0x40, 0x20, 0x70, 0xFC, 0xFC, 0xF8, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
	0x00, 0x00, 0x00, 0xFE, 0x14, 0x6C, 0x8C, 0x00, 0x10, 0x20, 0x10, 0x10, 0x10, 0x00, 0x30, 0x30, 
	0xC4, 0x10, 0xC0, 0x84, 0xFC, 0x84, 0x84, 0x20, 0x84, 0x04, 0x00, 0x20, 0x08, 0x7C, 0x18, 0x00, 
	0x80, 0x84, 0x84, 0x84, 0x8C, 0x80, 0x80, 0x84, 0x84, 0x10, 0x84, 0x88, 0x80, 0x84, 0x8C, 0x84, 
	0x80, 0x94, 0x88, 0x84, 0x20, 0x84, 0x48, 0xB4, 0x68, 0x20, 0x40, 0x10, 0x10, 0x20, 0x20, 0x00, 
	0x40, 0x88, 0x44, 0x40, 0x88, 0x80, 0x20, 0x78, 0x88, 0x20, 0x08, 0x50, 0x20, 0xA8, 0x88, 0x88, 
	0xF8, 0x78, 0x40, 0x08, 0x20, 0x88, 0x50, 0xA8, 0x50, 0x78, 0x40, 0x10, 0x10, 0x20, 0x00, 0xB2, 
	0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 
	0xFF, 0xFF, 0xFF, 0x01, 0xEB, 0x93, 0x73, 0xFF, 0xEF, 0xDF, 0xEF, 0xEF, 0xEF, 0xFF, 0xCF, 0xCF, 
	0x3B, 0xEF, 0x3F, 0x7B, 0x03, 0x7B, 0x7B, 0xDF, 0x7B, 0xFB, 0xFF, 0xDF, 0xF7, 0x83, 0xE7, 0xFF, 
	0x7F, 0x7B, 0x7B, 0x7B, 0x73, 0x7F, 0x7F, 0x7B, 0x7B, 0xEF, 0x7B, 0x77, 0x7F, 0x7B, 0x73, 0x7B, 
	0x7F, 0x6B, 0x77, 0x7B, 0xDF, 0x7B, 0xB7, 0x4B, 0x97, 0xDF, 0xBF, 0xEF, 0xEF, 0xDF, 0xDF, 0xFF, 
	0xBF, 0x77, 0xBB, 0xBF, 0x77, 0x7F, 0xDF, 0x87, 0x77, 0xDF, 0xF7, 0xAF, 0xDF, 0x57, 0x77, 0x77, 
	0x07, 0x87, 0xBF, 0xF7, 0xDF, 0x77, 0xAF, 0x57, 0xAF, 0x87, 0xBF, 0xEF, 0xEF, 0xDF, 0xFF, 0x4D, 
	0x00, 0xAA, 0x30, 0xCF, 0x70, 0x70, 0x20, 0x20, 0x84, 0xCC, 0xC0, 0x50, 0xA8, 0xFC, 0x8C, 0xF8, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
	0x00, 0x20, 0x00, 0x48, 0x7C, 0x8C, 0x74, 0x00, 0x08, 0x40, 0x28, 0x20, 0x10, 0x00, 0x30, 0x40, 
	0x78, 0x7C, 0xFC, 0x78, 0x10, 0x78, 0x78, 0x20, 0x78, 0x78, 0x20, 0x20, 0x04, 0x00, 0x20, 0x10, 
	0x78, 0x84, 0xF8, 0x78, 0xF0, 0xFC, 0x80, 0x78, 0x84, 0x7C, 0x78, 0x84, 0xFC, 0x84, 0x84, 0x78, 
	0x80, 0x78, 0x84, 0x78, 0x20, 0x78, 0x30, 0x48, 0x84, 0x20, 0xFC, 0x1C, 0x08, 0xE0, 0x20, 0x00, 
	0xFC, 0x78, 0x78, 0x38, 0x78, 0x78, 0x20, 0x08, 0x88, 0x70, 0x48, 0x48, 0x18, 0xA8, 0x88, 0x70, 
	0x80, 0x08, 0x40, 0xF0, 0x18, 0x70, 0x20, 0x50, 0x88, 0x08, 0xF8, 0x1C, 0x10, 0xE0, 0x00, 0x84, 
	0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 
	0xFF, 0xDF, 0xFF, 0xB7, 0x83, 0x73, 0x8B, 0xFF, 0xF7, 0xBF, 0xD7, 0xDF, 0xEF, 0xFF, 0xCF, 0xBF, 
	0x87, 0x83, 0x03, 0x87, 0xEF, 0x87, 0x87, 0xDF, 0x87, 0x87, 0xDF, 0xDF, 0xFB, 0xFF, 0xDF, 0xEF, 
	0x87, 0x7B, 0x07, 0x87, 0x0F, 0x03, 0x7F, 0x87, 0x7B, 0x83, 0x87, 0x7B, 0x03, 0x7B, 0x7B, 0x87, 
	0x7F, 0x87, 0x7B, 0x87, 0xDF, 0x87, 0xCF, 0xB7, 0x7B, 0xDF, 0x03, 0xE3, 0xF7, 0x1F, 0xDF, 0xFF, 
	0x03, 0x87, 0x87, 0xC7, 0x87, 0x87, 0xDF, 0xF7, 0x77, 0x8F, 0xB7, 0xB7, 0xE7, 0x57, 0x77, 0x8F, 
	0x7F, 0xF7, 0xBF, 0x0F, 0xE7, 0x8F, 0xDF, 0xAF, 0x77, 0xF7, 0x07, 0xE3, 0xEF, 0x1F, 0xFF, 0x7B, 
	0x00, 0x55, 0x00, 0xFF, 0x20, 0x20, 0x70, 0x70, 0x78, 0x78, 0xC0, 0x88, 0x20, 0x54, 0xD8, 0x54, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
	0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 
	0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0x7F, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 
};
