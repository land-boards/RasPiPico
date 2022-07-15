#include "include.h"

// sound format: PCM mono 8-bit 22050Hz
const u8 CardSnd[2388] = {
	0x81, 0x81, 0x80, 0x80, 0x7E, 0x80, 0x81, 0x7E, 0x80, 0x80, 0x83, 0x80, 0x7D, 0x7E, 0x82, 0x83, 
	0x81, 0x7F, 0x80, 0x80, 0x7C, 0x7F, 0x82, 0x85, 0x82, 0x7C, 0x7B, 0x80, 0x83, 0x82, 0x7D, 0x7F, 
	0x82, 0x81, 0x7F, 0x7C, 0x7D, 0x84, 0x85, 0x81, 0x7F, 0x81, 0x84, 0x82, 0x7D, 0x7C, 0x7F, 0x80, 
	0x7D, 0x7D, 0x7D, 0x80, 0x80, 0x80, 0x82, 0x7F, 0x7D, 0x7D, 0x82, 0x85, 0x81, 0x7E, 0x7D, 0x83, 
	0x82, 0x81, 0x81, 0x83, 0x84, 0x83, 0x7E, 0x7A, 0x7B, 0x7E, 0x84, 0x82, 0x7E, 0x7E, 0x83, 0x89, 
	0x82, 0x74, 0x74, 0x80, 0x88, 0x82, 0x76, 0x78, 0x88, 0x8B, 0x82, 0x77, 0x7C, 0x89, 0x87, 0x7A, 
	0x78, 0x81, 0x8B, 0x84, 0x79, 0x77, 0x83, 0x87, 0x85, 0x7D, 0x7E, 0x80, 0x80, 0x7F, 0x80, 0x84, 
	0x83, 0x81, 0x7F, 0x7F, 0x81, 0x82, 0x7F, 0x7B, 0x7B, 0x82, 0x89, 0x88, 0x7D, 0x7B, 0x7C, 0x7F, 
	0x7B, 0x79, 0x80, 0x85, 0x86, 0x82, 0x83, 0x82, 0x80, 0x7B, 0x7C, 0x81, 0x82, 0x81, 0x84, 0x85, 
	0x7E, 0x75, 0x7A, 0x88, 0x8B, 0x81, 0x79, 0x7D, 0x83, 0x80, 0x7C, 0x7E, 0x81, 0x81, 0x7D, 0x7F, 
	0x82, 0x87, 0x83, 0x7D, 0x7E, 0x81, 0x83, 0x82, 0x7D, 0x7B, 0x80, 0x83, 0x83, 0x7C, 0x7A, 0x7F, 
	0x83, 0x82, 0x7C, 0x7C, 0x82, 0x80, 0x7F, 0x7C, 0x7D, 0x83, 0x80, 0x83, 0x82, 0x81, 0x7E, 0x7E, 
	0x81, 0x82, 0x7E, 0x7B, 0x7F, 0x84, 0x86, 0x7D, 0x7C, 0x7F, 0x84, 0x7C, 0x78, 0x7E, 0x86, 0x8A, 
	0x80, 0x7C, 0x82, 0x88, 0x84, 0x7A, 0x79, 0x82, 0x86, 0x82, 0x7B, 0x7E, 0x83, 0x80, 0x7D, 0x80, 
	0x84, 0x86, 0x82, 0x7E, 0x7F, 0x80, 0x81, 0x7F, 0x7D, 0x7E, 0x83, 0x83, 0x80, 0x7E, 0x7E, 0x83, 
	0x83, 0x7F, 0x7A, 0x7B, 0x81, 0x85, 0x82, 0x7C, 0x7F, 0x82, 0x82, 0x7D, 0x7D, 0x80, 0x83, 0x81, 
	0x7F, 0x83, 0x87, 0x82, 0x78, 0x76, 0x81, 0x8B, 0x87, 0x7C, 0x76, 0x7B, 0x7F, 0x82, 0x81, 0x83, 
	0x83, 0x82, 0x83, 0x81, 0x7F, 0x7C, 0x7F, 0x81, 0x7E, 0x7E, 0x83, 0x86, 0x82, 0x7D, 0x7D, 0x7E, 
	0x7E, 0x7C, 0x7E, 0x82, 0x80, 0x7D, 0x82, 0x8A, 0x86, 0x76, 0x75, 0x82, 0x89, 0x7D, 0x72, 0x7D, 
	0x8D, 0x8A, 0x7C, 0x7D, 0x87, 0x85, 0x7C, 0x7C, 0x83, 0x80, 0x7A, 0x7A, 0x83, 0x87, 0x7C, 0x79, 
	0x84, 0x87, 0x80, 0x7C, 0x7D, 0x80, 0x7B, 0x7C, 0x82, 0x8B, 0x83, 0x7D, 0x7C, 0x81, 0x7F, 0x81, 
	0x89, 0x86, 0x7D, 0x77, 0x7E, 0x85, 0x80, 0x7A, 0x7E, 0x85, 0x83, 0x7C, 0x80, 0x84, 0x80, 0x79, 
	0x7C, 0x85, 0x87, 0x81, 0x7B, 0x80, 0x83, 0x80, 0x7E, 0x82, 0x85, 0x81, 0x7B, 0x79, 0x7F, 0x83, 
	0x85, 0x85, 0x7F, 0x7B, 0x7A, 0x7E, 0x84, 0x88, 0x85, 0x7D, 0x78, 0x7B, 0x83, 0x82, 0x7D, 0x7D, 
	0x83, 0x86, 0x7D, 0x75, 0x80, 0x89, 0x87, 0x7C, 0x7D, 0x86, 0x84, 0x7A, 0x7A, 0x85, 0x8A, 0x7F, 
	0x78, 0x80, 0x88, 0x7F, 0x78, 0x80, 0x8D, 0x85, 0x78, 0x76, 0x82, 0x88, 0x83, 0x7E, 0x7B, 0x7C, 
	0x7E, 0x80, 0x86, 0x7F, 0x77, 0x79, 0x89, 0x8D, 0x84, 0x76, 0x75, 0x7D, 0x87, 0x83, 0x7B, 0x7A, 
	0x7E, 0x88, 0x88, 0x7D, 0x79, 0x7E, 0x84, 0x7E, 0x7A, 0x7F, 0x88, 0x82, 0x78, 0x79, 0x82, 0x8A, 
	0x81, 0x79, 0x7E, 0x86, 0x83, 0x7C, 0x7D, 0x85, 0x87, 0x82, 0x7B, 0x7D, 0x80, 0x83, 0x86, 0x89, 
	0x82, 0x77, 0x76, 0x7B, 0x86, 0x87, 0x82, 0x80, 0x7F, 0x80, 0x7E, 0x7E, 0x7D, 0x80, 0x80, 0x7E, 
	0x7E, 0x85, 0x88, 0x81, 0x78, 0x76, 0x85, 0x8B, 0x7C, 0x73, 0x81, 0x93, 0x91, 0x78, 0x72, 0x82, 
	0x89, 0x79, 0x6E, 0x7F, 0x93, 0x8D, 0x79, 0x71, 0x7E, 0x85, 0x82, 0x83, 0x89, 0x82, 0x76, 0x72, 
	0x83, 0x8D, 0x84, 0x7B, 0x7E, 0x87, 0x83, 0x77, 0x7A, 0x84, 0x87, 0x82, 0x7D, 0x7F, 0x7B, 0x76, 
	0x79, 0x81, 0x89, 0x85, 0x84, 0x7F, 0x7C, 0x77, 0x7E, 0x87, 0x86, 0x79, 0x72, 0x7C, 0x8B, 0x8A, 
	0x80, 0x76, 0x7A, 0x80, 0x85, 0x89, 0x8A, 0x81, 0x78, 0x79, 0x7F, 0x7F, 0x7B, 0x7D, 0x88, 0x8C, 
	0x82, 0x76, 0x7B, 0x86, 0x86, 0x77, 0x72, 0x82, 0x92, 0x8C, 0x7A, 0x74, 0x7A, 0x81, 0x7F, 0x7F, 
	0x84, 0x86, 0x83, 0x81, 0x80, 0x7D, 0x76, 0x7B, 0x88, 0x8F, 0x85, 0x79, 0x7A, 0x83, 0x83, 0x7E, 
	0x7B, 0x7D, 0x82, 0x88, 0x85, 0x81, 0x78, 0x7B, 0x83, 0x88, 0x7F, 0x74, 0x7D, 0x8B, 0x8D, 0x80, 
	0x75, 0x77, 0x84, 0x8D, 0x8A, 0x7F, 0x74, 0x73, 0x82, 0x8C, 0x84, 0x77, 0x7A, 0x86, 0x8C, 0x7F, 
	0x79, 0x7C, 0x82, 0x7A, 0x7A, 0x80, 0x87, 0x7F, 0x7A, 0x81, 0x85, 0x81, 0x7C, 0x7D, 0x7F, 0x80, 
	0x80, 0x85, 0x85, 0x7D, 0x81, 0x8C, 0x91, 0x7A, 0x68, 0x6E, 0x83, 0x86, 0x7C, 0x81, 0x90, 0x8D, 
	0x76, 0x6F, 0x7E, 0x86, 0x7C, 0x77, 0x85, 0x90, 0x83, 0x73, 0x7A, 0x8B, 0x8E, 0x7D, 0x76, 0x7E, 
	0x86, 0x82, 0x78, 0x74, 0x7B, 0x84, 0x8D, 0x8D, 0x84, 0x76, 0x73, 0x79, 0x7E, 0x80, 0x7E, 0x83, 
	0x89, 0x88, 0x87, 0x7E, 0x75, 0x6D, 0x75, 0x84, 0x8F, 0x8F, 0x85, 0x83, 0x80, 0x7C, 0x74, 0x75, 
	0x7D, 0x86, 0x88, 0x88, 0x84, 0x83, 0x7C, 0x79, 0x7B, 0x7E, 0x81, 0x82, 0x86, 0x8E, 0x87, 0x74, 
	0x6F, 0x7B, 0x8A, 0x87, 0x7A, 0x7A, 0x84, 0x8B, 0x84, 0x78, 0x77, 0x7F, 0x82, 0x83, 0x83, 0x86, 
	0x84, 0x7F, 0x7E, 0x7F, 0x7D, 0x7B, 0x7F, 0x85, 0x85, 0x7E, 0x7E, 0x82, 0x88, 0x84, 0x7B, 0x79, 
	0x80, 0x84, 0x7D, 0x75, 0x7C, 0x88, 0x8B, 0x7F, 0x73, 0x76, 0x86, 0x8A, 0x80, 0x7B, 0x82, 0x87, 
	0x82, 0x7E, 0x81, 0x80, 0x7A, 0x76, 0x7F, 0x8B, 0x87, 0x74, 0x6E, 0x7C, 0x8C, 0x89, 0x7F, 0x76, 
	0x7D, 0x87, 0x8B, 0x84, 0x7B, 0x76, 0x80, 0x87, 0x82, 0x7F, 0x85, 0x8B, 0x84, 0x75, 0x76, 0x84, 
	0x86, 0x7C, 0x76, 0x7F, 0x89, 0x84, 0x7A, 0x79, 0x83, 0x87, 0x81, 0x7E, 0x7E, 0x80, 0x7F, 0x81, 
	0x85, 0x82, 0x7F, 0x82, 0x85, 0x80, 0x79, 0x7B, 0x83, 0x85, 0x7D, 0x77, 0x7D, 0x87, 0x85, 0x83, 
	0x7E, 0x79, 0x79, 0x7D, 0x83, 0x81, 0x7C, 0x82, 0x8D, 0x8B, 0x79, 0x6D, 0x75, 0x86, 0x8D, 0x85, 
	0x80, 0x80, 0x7D, 0x7A, 0x7F, 0x88, 0x88, 0x7D, 0x7D, 0x82, 0x84, 0x7E, 0x7D, 0x7F, 0x80, 0x78, 
	0x7C, 0x8A, 0x8E, 0x84, 0x77, 0x79, 0x83, 0x81, 0x7C, 0x7B, 0x84, 0x87, 0x83, 0x83, 0x84, 0x84, 
	0x7C, 0x75, 0x79, 0x83, 0x88, 0x86, 0x7E, 0x7A, 0x7B, 0x80, 0x81, 0x7D, 0x7F, 0x82, 0x81, 0x7E, 
	0x7A, 0x7E, 0x81, 0x82, 0x80, 0x81, 0x85, 0x83, 0x7C, 0x7B, 0x80, 0x86, 0x88, 0x85, 0x7D, 0x7C, 
	0x7C, 0x83, 0x84, 0x80, 0x7B, 0x79, 0x7A, 0x7F, 0x83, 0x87, 0x83, 0x7D, 0x77, 0x7C, 0x82, 0x84, 
	0x81, 0x82, 0x83, 0x84, 0x80, 0x7F, 0x7F, 0x80, 0x7F, 0x82, 0x87, 0x87, 0x7D, 0x78, 0x7B, 0x81, 
	0x83, 0x86, 0x86, 0x86, 0x7C, 0x77, 0x78, 0x7F, 0x85, 0x84, 0x83, 0x82, 0x7E, 0x79, 0x78, 0x7D, 
	0x89, 0x8B, 0x82, 0x78, 0x78, 0x84, 0x89, 0x82, 0x76, 0x77, 0x80, 0x88, 0x88, 0x83, 0x82, 0x7B, 
	0x79, 0x7D, 0x88, 0x84, 0x7B, 0x76, 0x7D, 0x86, 0x87, 0x83, 0x81, 0x7D, 0x76, 0x77, 0x85, 0x8D, 
	0x8C, 0x81, 0x79, 0x79, 0x7C, 0x7D, 0x82, 0x87, 0x81, 0x7E, 0x7E, 0x80, 0x80, 0x81, 0x83, 0x86, 
	0x81, 0x7B, 0x7B, 0x80, 0x81, 0x79, 0x79, 0x81, 0x87, 0x7E, 0x7B, 0x7D, 0x84, 0x82, 0x7E, 0x80, 
	0x85, 0x85, 0x81, 0x7E, 0x80, 0x7E, 0x7D, 0x80, 0x85, 0x84, 0x80, 0x7D, 0x82, 0x86, 0x84, 0x7E, 
	0x7C, 0x7F, 0x82, 0x7E, 0x7C, 0x7C, 0x80, 0x83, 0x82, 0x7E, 0x80, 0x83, 0x83, 0x81, 0x7C, 0x7C, 
	0x7D, 0x7D, 0x80, 0x87, 0x87, 0x7B, 0x70, 0x73, 0x88, 0x93, 0x8C, 0x7D, 0x7B, 0x80, 0x7E, 0x7D, 
	0x84, 0x86, 0x7E, 0x73, 0x76, 0x85, 0x8C, 0x81, 0x7B, 0x7E, 0x82, 0x81, 0x7C, 0x7F, 0x84, 0x7E, 
	0x79, 0x82, 0x8A, 0x83, 0x77, 0x78, 0x84, 0x8C, 0x86, 0x80, 0x7F, 0x7B, 0x75, 0x7C, 0x87, 0x88, 
	0x81, 0x7C, 0x80, 0x82, 0x7F, 0x78, 0x7E, 0x83, 0x81, 0x7E, 0x7F, 0x84, 0x82, 0x7E, 0x80, 0x83, 
	0x84, 0x7F, 0x80, 0x83, 0x85, 0x80, 0x7E, 0x80, 0x7F, 0x7D, 0x7B, 0x7D, 0x80, 0x7E, 0x79, 0x79, 
	0x81, 0x86, 0x81, 0x7C, 0x7F, 0x86, 0x85, 0x7E, 0x7A, 0x7F, 0x85, 0x80, 0x7B, 0x7C, 0x82, 0x83, 
	0x83, 0x84, 0x83, 0x78, 0x73, 0x83, 0x94, 0x8C, 0x75, 0x6F, 0x7C, 0x82, 0x7E, 0x7A, 0x83, 0x87, 
	0x7F, 0x7B, 0x85, 0x8B, 0x7B, 0x72, 0x7A, 0x8B, 0x89, 0x7F, 0x7E, 0x85, 0x80, 0x7A, 0x7F, 0x8C, 
	0x84, 0x75, 0x74, 0x87, 0x8D, 0x83, 0x76, 0x7F, 0x85, 0x81, 0x78, 0x7F, 0x87, 0x81, 0x7D, 0x7F, 
	0x84, 0x7D, 0x77, 0x7E, 0x88, 0x89, 0x80, 0x7D, 0x7F, 0x7F, 0x7F, 0x85, 0x87, 0x82, 0x7A, 0x7D, 
	0x82, 0x7E, 0x78, 0x7C, 0x89, 0x88, 0x7B, 0x76, 0x80, 0x89, 0x81, 0x7A, 0x7D, 0x84, 0x82, 0x7D, 
	0x81, 0x86, 0x83, 0x7C, 0x7B, 0x84, 0x87, 0x7C, 0x74, 0x7A, 0x86, 0x8B, 0x81, 0x7C, 0x78, 0x79, 
	0x83, 0x87, 0x84, 0x7C, 0x78, 0x80, 0x86, 0x86, 0x81, 0x7D, 0x7E, 0x7C, 0x7D, 0x84, 0x85, 0x7E, 
	0x7A, 0x7D, 0x86, 0x86, 0x81, 0x7C, 0x7D, 0x7C, 0x81, 0x87, 0x89, 0x83, 0x7B, 0x80, 0x84, 0x80, 
	0x78, 0x7C, 0x84, 0x85, 0x7C, 0x79, 0x80, 0x80, 0x7A, 0x7B, 0x84, 0x88, 0x7E, 0x79, 0x7E, 0x86, 
	0x83, 0x80, 0x81, 0x82, 0x7D, 0x7C, 0x83, 0x8D, 0x86, 0x79, 0x77, 0x80, 0x84, 0x7F, 0x80, 0x82, 
	0x81, 0x80, 0x80, 0x84, 0x7F, 0x78, 0x7E, 0x87, 0x89, 0x7E, 0x79, 0x7E, 0x82, 0x7E, 0x7B, 0x81, 
	0x85, 0x85, 0x83, 0x83, 0x82, 0x7D, 0x7A, 0x84, 0x88, 0x84, 0x7A, 0x78, 0x7F, 0x82, 0x82, 0x7C, 
	0x7E, 0x85, 0x87, 0x81, 0x77, 0x77, 0x80, 0x83, 0x7D, 0x78, 0x7F, 0x8B, 0x89, 0x7E, 0x7D, 0x83, 
	0x84, 0x7C, 0x77, 0x7E, 0x84, 0x84, 0x7F, 0x80, 0x83, 0x81, 0x7E, 0x7F, 0x7E, 0x7E, 0x7D, 0x80, 
	0x80, 0x7F, 0x7D, 0x82, 0x84, 0x7D, 0x7B, 0x7F, 0x83, 0x82, 0x7D, 0x80, 0x86, 0x84, 0x80, 0x7B, 
	0x7E, 0x7E, 0x7D, 0x7D, 0x82, 0x84, 0x84, 0x81, 0x7E, 0x7F, 0x82, 0x83, 0x82, 0x81, 0x7F, 0x7F, 
	0x80, 0x7E, 0x7E, 0x82, 0x7F, 0x7E, 0x7E, 0x82, 0x86, 0x7F, 0x7D, 0x7F, 0x81, 0x81, 0x7F, 0x82, 
	0x82, 0x81, 0x83, 0x87, 0x85, 0x7C, 0x79, 0x7F, 0x88, 0x82, 0x78, 0x77, 0x81, 0x84, 0x81, 0x7C, 
	0x7E, 0x7F, 0x7D, 0x7D, 0x81, 0x84, 0x84, 0x80, 0x81, 0x81, 0x82, 0x83, 0x81, 0x80, 0x7D, 0x82, 
	0x84, 0x82, 0x7C, 0x7D, 0x7F, 0x83, 0x80, 0x7E, 0x7E, 0x82, 0x80, 0x7E, 0x7F, 0x81, 0x81, 0x7F, 
	0x81, 0x82, 0x82, 0x84, 0x82, 0x7F, 0x7D, 0x7F, 0x83, 0x81, 0x7F, 0x7E, 0x7D, 0x7E, 0x7E, 0x7F, 
	0x82, 0x7F, 0x7C, 0x7D, 0x80, 0x82, 0x81, 0x86, 0x84, 0x7F, 0x7C, 0x80, 0x82, 0x80, 0x7B, 0x7F, 
	0x87, 0x85, 0x7A, 0x77, 0x7E, 0x87, 0x82, 0x7D, 0x80, 0x83, 0x84, 0x80, 0x7C, 0x80, 0x7F, 0x7F, 
	0x81, 0x81, 0x7E, 0x7D, 0x7F, 0x81, 0x83, 0x80, 0x7F, 0x7F, 0x7E, 0x7E, 0x7F, 0x81, 0x7E, 0x7D, 
	0x80, 0x83, 0x81, 0x7E, 0x80, 0x83, 0x85, 0x7F, 0x7C, 0x82, 0x83, 0x7F, 0x7F, 0x81, 0x82, 0x82, 
	0x7E, 0x80, 0x7F, 0x80, 0x7E, 0x81, 0x81, 0x7F, 0x7B, 0x80, 0x82, 0x7F, 0x7E, 0x80, 0x81, 0x83, 
	0x83, 0x85, 0x84, 0x81, 0x80, 0x80, 0x80, 0x7E, 0x7E, 0x82, 0x85, 0x82, 0x79, 0x7B, 0x7E, 0x7E, 
	0x7D, 0x81, 0x86, 0x86, 0x7F, 0x7D, 0x84, 0x87, 0x82, 0x79, 0x7B, 0x81, 0x81, 0x7B, 0x7B, 0x81, 
	0x86, 0x82, 0x7E, 0x7F, 0x81, 0x7F, 0x7C, 0x7F, 0x81, 0x82, 0x83, 0x83, 0x83, 0x80, 0x7E, 0x7F, 
	0x82, 0x7E, 0x7E, 0x7F, 0x83, 0x82, 0x80, 0x7F, 0x7F, 0x7D, 0x7C, 0x7E, 0x83, 0x81, 0x7D, 0x7B, 
	0x80, 0x83, 0x80, 0x7E, 0x82, 0x80, 0x7F, 0x7D, 0x83, 0x85, 0x82, 0x7C, 0x7C, 0x7F, 0x82, 0x81, 
	0x7D, 0x80, 0x84, 0x83, 0x7F, 0x7D, 0x7D, 0x7F, 0x81, 0x82, 0x82, 0x80, 0x7E, 0x81, 0x7F, 0x82, 
	0x82, 0x80, 0x7E, 0x7F, 0x7F, 0x83, 0x80, 0x7E, 0x7F, 0x81, 0x84, 0x80, 0x7D, 0x7D, 0x7E, 0x7E, 
	0x7F, 0x81, 0x80, 0x7E, 0x7D, 0x81, 0x85, 0x86, 0x81, 0x7D, 0x80, 0x82, 0x80, 0x82, 0x81, 0x83, 
	0x82, 0x7E, 0x81, 0x7F, 0x7E, 0x7E, 0x81, 0x84, 0x82, 0x7F, 0x81, 0x85, 0x82, 0x7C, 0x7C, 0x82, 
	0x84, 0x81, 0x7D, 0x7F, 0x81, 0x80, 0x7E, 0x81, 0x82, 0x80, 0x7B, 0x7B, 0x7F, 0x82, 0x82, 0x82, 
	0x7F, 0x7E, 0x7D, 0x7C, 0x7E, 0x7F, 0x7E, 0x7E, 0x81, 0x82, 0x81, 0x80, 0x7D, 0x7E, 0x82, 0x85, 
	0x86, 0x7F, 0x7A, 0x7C, 0x82, 0x83, 0x82, 0x80, 0x81, 0x83, 0x7F, 0x7D, 0x7F, 0x82, 0x82, 0x82, 
	0x81, 0x80, 0x80, 0x80, 0x7E, 0x7D, 0x7F, 0x7F, 0x81, 0x80, 0x7F, 0x7D, 0x7F, 0x85, 0x83, 0x80, 
	0x7A, 0x7D, 0x83, 0x84, 0x80, 0x7C, 0x7D, 0x83, 0x83, 0x82, 0x7F, 0x7E, 0x80, 0x80, 0x81, 0x81, 
	0x7F, 0x7E, 0x81, 0x81, 0x80, 0x7F, 0x7F, 0x7F, 0x7F, 0x81, 0x81, 0x81, 0x7E, 0x7D, 0x81, 0x86, 
	0x83, 0x7D, 0x7D, 0x82, 0x83, 0x80, 0x7D, 0x7F, 0x83, 0x82, 0x81, 0x80, 0x82, 0x7E, 0x7C, 0x7F, 
	0x82, 0x82, 0x7F, 0x7F, 0x81, 0x80, 0x7E, 0x7E, 0x81, 0x81, 0x82, 0x81, 0x81, 0x82, 0x7F, 0x80, 
	0x81, 0x7F, 0x7F, 0x80, 0x80, 0x7F, 0x81, 0x83, 0x80, 0x7D, 0x7C, 0x7E, 0x81, 0x7D, 0x7B, 0x83, 
	0x84, 0x84, 0x7D, 0x7A, 0x80, 0x84, 0x84, 0x81, 0x80, 0x7E, 0x7F, 0x81, 0x83, 0x80, 0x7B, 0x7F, 
	0x83, 0x83, 0x7C, 0x7B, 0x7F, 0x87, 0x85, 0x7F, 0x7E, 0x7E, 0x7E, 0x80, 0x80, 0x81, 0x81, 0x80, 
	0x81, 0x81, 0x7D, 0x7D, 0x7E, 0x84, 0x83, 0x81, 0x7E, 0x7C, 0x7C, 0x80, 0x86, 0x86, 0x80, 0x7B, 
	0x7F, 0x83, 0x82, 0x82, 0x80, 0x82, 0x7E, 0x7D, 0x7D, 0x82, 0x80, 0x7D, 0x80, 0x84, 0x80, 0x7C, 
	0x7E, 0x82, 0x84, 0x7F, 0x7F, 0x80, 0x81, 0x7F, 0x7C, 0x7E, 0x83, 0x83, 0x7F, 0x80, 0x81, 0x82, 
	0x7F, 0x80, 0x80, 0x80, 0x82, 0x83, 0x81, 0x7D, 0x7B, 0x81, 0x83, 0x83, 0x7D, 0x7E, 0x81, 0x82, 
	0x81, 0x7D, 0x7F, 0x7F, 0x80, 0x85, 0x84, 0x7F, 0x7A, 0x7D, 0x83, 0x88, 0x80, 0x7E, 0x7D, 0x80, 
	0x7F, 0x7B, 0x7F, 0x83, 0x82, 0x7F, 0x7A, 0x7D, 0x7F, 0x81, 0x85, 0x81, 0x80, 0x7F, 0x80, 0x7F, 
	0x80, 0x7F, 0x7E, 0x80, 0x7F, 0x83, 0x84, 0x81, 0x7F, 0x7D, 0x7F, 0x83, 0x84, 0x82, 0x80, 0x7E, 
	0x7E, 0x7E, 0x81, 0x81, 0x80, 0x7E, 0x7E, 0x83, 0x81, 0x80, 0x7C, 0x7F, 0x7F, 0x81, 0x80, 0x82, 
	0x84, 0x83, 0x81, 0x7F, 0x80, 0x81, 0x7F, 0x7E, 0x81, 0x80, 0x7D, 0x80, 0x7F, 0x7F, 0x80, 0x81, 
	0x82, 0x84, 0x7F, 0x7F, 0x80, 0x7E, 0x7D, 0x7E, 0x7F, 0x81, 0x83, 0x83, 0x81, 0x7E, 0x7F, 0x7F, 
	0x80, 0x7E, 0x81, 0x83, 0x83, 0x7F, 0x7E, 0x7F, 0x81, 0x81, 0x81, 0x80, 0x82, 0x7D, 0x7E, 0x7E, 
	0x87, 0x85, 0x7F, 0x7B, 0x7E, 0x80, 0x7F, 0x7D, 0x7F, 0x82, 0x82, 0x7E, 0x7F, 0x81, 0x83, 0x80, 
	0x80, 0x80, 0x80, 0x81, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x81, 0x82, 0x80, 0x7E, 0x7F, 0x7D, 0x80, 
	0x81, 0x84, 0x80, 0x7E, 0x7D, 0x80, 0x80, 0x81, 0x81, 0x80, 0x82, 0x80, 0x7E, 0x7E, 0x82, 0x82, 
	0x81, 0x7D, 0x7F, 0x81, 0x83, 0x81, 0x7E, 0x80, 0x80, 0x7F, 0x7F, 0x80, 0x7E, 0x7F, 0x7E, 0x81, 
	0x82, 0x83, 0x81, 0x80, 0x7F, 0x7F, 0x80, 0x80, 0x83, 0x82, 0x7F, 0x80, 0x80, 0x7F, 0x80, 0x80, 
	0x82, 0x81, 0x7E, 0x7D, 0x80, 0x81, 0x80, 0x80, 0x80, 0x82, 0x80, 0x80, 0x80, 0x82, 0x80, 0x7E, 
	0x7F, 0x82, 0x80, 0x7F, 0x7F, 0x82, 0x7F, 0x7D, 0x7E, 0x83, 0x83, 0x7F, 0x7E, 0x7F, 0x81, 0x80, 
	0x7F, 0x80, 0x80, 0x82, 0x7E, 0x7F, 0x80, 0x82, 0x82, 0x82, 0x7E, 0x7D, 0x7F, 0x82, 0x81, 0x7F, 
	0x7F, 0x83, 0x83, 0x80, 0x7D, 0x7D, 0x83, 0x81, 0x7F, 0x7F, 0x81, 0x81, 0x7F, 0x7F, 0x81, 0x81, 
	0x81, 0x7F, 0x80, 0x81, 0x7F, 0x80, 0x81, 0x80, 0x7E, 0x7E, 0x80, 0x80, 0x7F, 0x7D, 0x80, 0x84, 
	0x81, 0x7E, 0x7D, 0x80, 0x83, 0x82, 0x80, 0x81, 0x80, 0x81, 0x80, 0x80, 0x7E, 0x81, 0x80, 0x7F, 
	0x80, 0x7F, 0x82, 0x80, 0x7E, 0x7E, 0x80, 0x81, 0x80, 0x80, 0x81, 0x7F, 0x80, 0x7E, 0x81, 0x82, 
	0x81, 0x81, 0x80, 0x7F, 0x80, 0x81, 0x80, 0x80, 0x81, 0x7F, 0x80, 0x81, 0x84, 0x7E, 0x80, 0x81, 
	0x7F, 0x81, 0x7D, 0x7F, 0x80, 0x7F, 0x80, 0x80, 0x80, 0x80, 0x81, 0x7F, 0x82, 0x80, 0x80, 0x7F, 
	0x80, 0x81, 0x81, 0x7F, 0x81, 0x80, 0x7F, 0x81, 0x80, 0x80, 0x7F, 0x7F, 0x80, 0x81, 0x80, 0x80, 
	0x81, 0x80, 0x7F, 0x80, 0x80, 0x7F, 0x7F, 0x81, 0x80, 0x80, 0x82, 0x7F, 0x7E, 0x80, 0x81, 0x81, 
	0x7F, 0x80, 0x7F, 0x80, 0x80, 0x80, 0x7E, 0x82, 0x81, 0x80, 0x80, 0x7F, 0x81, 0x81, 0x80, 0x7E, 
	0x7E, 0x80, 0x81, 0x82, 0x80, 0x7F, 0x80, 0x81, 0x7E, 0x80, 0x80, 0x81, 0x80, 0x81, 0x7F, 0x7F, 
	0x7E, 0x82, 0x80, 0x81, 0x81, 0x7F, 0x80, 0x81, 0x7F, 0x7F, 0x7F, 0x82, 0x80, 0x7F, 0x80, 0x80, 
	0x82, 0x80, 0x7F, 0x80, 0x80, 0x80, 0x81, 0x7F, 0x80, 0x81, 0x7F, 0x80, 0x80, 0x81, 0x7F, 0x80, 
	0x80, 0x81, 0x7F, 0x81, 
};