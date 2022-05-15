/***********************************************************************************************************************
PicoMite MMBasic

Hardware_Includes.h

<COPYRIGHT HOLDERS>  Geoff Graham, Peter Mather
Copyright (c) 2021, <COPYRIGHT HOLDERS> All rights reserved. 
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
1.	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2.	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
    in the documentation and/or other materials provided with the distribution.
3.	The name MMBasic be used when referring to the interpreter in any documentation and promotional material and the original copyright message be displayed 
    on the console at startup (additional copyright messages may be added).
4.	All advertising materials mentioning features or use of this software must display the following acknowledgement: This product includes software developed 
    by the <copyright holder>.
5.	Neither the name of the <copyright holder> nor the names of its contributors may be used to endorse or promote products derived from this software 
    without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDERS> AS IS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDERS> BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

************************************************************************************************************************/

#include "Memory.h"
#include "hardware/watchdog.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#ifdef PICOMITEVGA
#include "pico/multicore.h"
#endif



#if !defined(INCLUDE_COMMAND_TABLE) && !defined(INCLUDE_TOKEN_TABLE)
#include <stdio.h>
#include <stddef.h>
#include "Version.h"
#include "configuration.h"
#include "FileIO.h"
#include "ff.h"
void cmd_open(void);
void cmd_close(void);
void cmd_files(void);
void cmd_mkdir(void);
void cmd_rmdir(void);
void cmd_chdir(void);
void cmd_kill(void);
void cmd_copy(void);
void cmd_name(void);
void cmd_exitmmb(void);
void cmd_pause(void);
void cmd_timer(void);
void cmd_copyright(void);
void cmd_seek(void);
void cmd_library(void);

void fun_eof(void);
void fun_loc(void);
void fun_lof(void);
void fun_cwd(void);
void fun_inputstr(void);
void fun_mmfname(void);
void fun_dir(void);
void fun_date(void);
void fun_time(void);
void fun_timer(void);
void fun_device(void);
void cmd_date(void);
void cmd_time(void);
void cmd_flash(void);
void cmd_var(void);
void cmd_flush(void);
// global variables used in MMBasic but must be maintained outside of the interpreter
extern int MMerrno;
extern int ListCnt;
extern int MMCharPos;
extern unsigned char *StartEditPoint;
extern int StartEditChar;
extern int OptionErrorSkip;
extern int ExitMMBasicFlag;
extern unsigned char *InterruptReturn;
extern unsigned int _excep_peek;
extern volatile long long int mSecTimer;
extern volatile unsigned int PauseTimer;
extern volatile unsigned int IntPauseTimer;
extern volatile unsigned int Timer1, Timer2;		                       //1000Hz decrement timer
extern volatile unsigned int diskchecktimer;
extern volatile int ds18b20Timer;
extern volatile int CursorTimer;
extern volatile unsigned int I2CTimer;
extern volatile int second;
extern volatile int minute;
extern volatile int hour;
extern volatile int day;
extern volatile int month;
extern volatile int year;
extern volatile unsigned int SecondsTimer;
extern volatile int day_of_week;
extern unsigned char WatchdogSet;
extern unsigned char IgnorePIN;
extern MMFLOAT VCC;
extern volatile unsigned int WDTimer;                               // used for the watchdog timer
extern unsigned char PulsePin[];
extern unsigned char PulseDirection[];
extern int PulseCnt[];
extern int PulseActive;
extern volatile int ClickTimer;
extern int calibrate;
extern volatile unsigned int InkeyTimer;                            // used to delay on an escape character
extern volatile int DISPLAY_TYPE;

extern volatile char ConsoleRxBuf[CONSOLE_RX_BUF_SIZE];
extern volatile int ConsoleRxBufHead;
extern volatile int ConsoleRxBufTail;
extern volatile char ConsoleTxBuf[CONSOLE_TX_BUF_SIZE];
extern volatile int ConsoleTxBufHead;
extern volatile int ConsoleTxBufTail;
extern datetime_t rtc_t;
extern uint16_t tilefcols[40*30];
extern uint16_t tilebcols[40*30];
extern void __not_in_flash_func(QVgaCore)(void);
extern uint32_t core1stack[64];
extern int QVGA_CLKDIV;
struct s_PinDef {
	int pin;
	int GPno;
    char pinname[5];
    uint64_t mode;
    unsigned char ADCpin;
	unsigned char slice;
};
extern uint32_t _excep_code;
extern const struct s_PinDef PinDef[NBRPINS + 1];
#define VCHARS  25					// nbr of lines in the DOS box (used in LIST)

#define FILENAME_LENGTH 12

//extern unsigned char *ModuleTable[MAXMODULES];           // list of pointers to modules loaded in memory;
//extern int NbrModules;                          // the number of modules currently loaded
extern void mT4IntEnable(int status);
extern int BasicFileOpen(char *fname, int fnbr, int mode);
extern int kbhitConsole(void);
extern int InitSDCard(void);
extern void FileClose(int fnbr);
#define NBRERRMSG 17				// number of file error messages
extern void PRet(void);
extern void PInt(int64_t n);
extern void PIntComma(int64_t n);
extern void SRet(void);
extern void SInt(int64_t n);
extern void SIntComma(int64_t n);
extern void PIntH(unsigned long long int n);
extern void PIntHC(unsigned long long int n) ;
extern void PFlt(MMFLOAT flt);
extern void PFltComma(MMFLOAT n) ;
extern void putConsole(int c, int flush);
extern void MMPrintString(char* s);
extern void SSPrintString(char* s);
extern void myprintf(char *s);
extern int getConsole(void);
extern void InitReservedIO(void);
extern char SerialConsolePutC(char c, int flush);
extern long long int *GetReceiveDataBuffer(unsigned char *p, unsigned int *nbr);
extern int ticks_per_second;
extern volatile unsigned int GPSTimer;
extern int AUDIO_L_PIN, AUDIO_R_PIN, AUDIO_SLICE;
extern uint16_t AUDIO_WRAP;
extern int PromptFont, PromptFC, PromptBC;                             // the font and colours selected at the prompt
extern const uint8_t *flash_progmemory;
// console related I/O
int __not_in_flash_func(MMInkey)(void);
int MMgetchar(void);
char MMputchar(char c, int flush);
void SaveProgramToFlash(unsigned char *pm, int msg);

void CheckAbort(void);
void EditInputLine(void);
// empty functions used in MMBasic but must be maintained outside of the interpreter
void UnloadFont(int);
#define NBRFONTS 0

#if defined(MSVCC)
#define mkdir _mkdir
#define rmdir _rmdir
#define chdir _chdir
#define getcwd _getcwd
#define kbhit _kbhit
#define getch _getch
#define ungetch _ungetch
#define putch _putch
#endif
#endif
#define CURSOR_OFF        350              // cursor off time in mS
#define CURSOR_ON     650                  // cursor on time in mS

#define dp(...) {unsigned char s[140];sprintf((char *)s,  __VA_ARGS__); MMPrintString((char *)s); MMPrintString((char *)"\r\n");}

#define TAB     	0x9
#define BKSP    	0x8
#define ENTER   	0xd
#define ESC     	0x1b

// the values returned by the function keys
#define F1      	0x91
#define F2      	0x92
#define F3      	0x93
#define F4      	0x94
#define F5      	0x95
#define F6      	0x96
#define F7      	0x97
#define F8      	0x98
#define F9      	0x99
#define F10     	0x9a
#define F11     	0x9b
#define F12     	0x9c

// the values returned by special control keys
#define UP			0x80
#define DOWN		0x81
#define LEFT		0x82
#define RIGHT		0x83
#define DOWNSEL     0xA1
#define RIGHTSEL    0xA3
#define INSERT		0x84
#define DEL			0x7f
#define HOME		0x86
#define END			0x87
#define PUP			0x88
#define PDOWN		0x89
#define NUM_ENT		ENTER
#define SLOCK		0x8c
#define ALT			0x8b
#define	SHIFT_TAB 	0x9F
#define SHIFT_DEL   0xa0
#define CTRLKEY(a) (a & 0x1f)
#define DISPLAY_CLS             1
#define REVERSE_VIDEO           3
#define CLEAR_TO_EOL            4
#define CLEAR_TO_EOS            5
#define SCROLL_DOWN             6
#define DRAW_LINE               7
#define CONFIG_TAB2		0b111
#define CONFIG_TAB4		0b001
#define CONFIG_TAB8		0b010
#define WPN 65   //Framebuffer page no.
#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0
#define SD_SLOW_SPI_SPEED 0
#define SD_FAST_SPI_SPEED 1
#define NONE_SPI_SPEED 4

#define RESET_COMMAND       9999                                    // indicates that the reset was caused by the RESET command
#define WATCHDOG_TIMEOUT    9998                                    // reset caused by the watchdog timer
#define PIN_RESTART         9997                                    // reset caused by entering 0 at the PIN prompt
#define RESTART_NOAUTORUN   9996                                    // reset required after changing the LCD or touch config
#define RESTART_DOAUTORUN   9995                                    // reset required by OPTION SET (ie, re runs the program)
#define uSec(a) busy_wait_us(a)
#define KEYBOARD_CLOCK 11
#define KEYBOARD_DATA 12

/**********************************************************************************
 All command tokens tokens (eg, PRINT, FOR, etc) should be inserted in this table
**********************************************************************************/
#ifdef INCLUDE_COMMAND_TABLE
// the format is:
//    TEXT      	TYPE                P  FUNCTION TO CALL
// where type is always T_CMD
// and P is the precedence (which is only used for operators and not commands)


	{ (unsigned char *)"Open",		T_CMD,				0, cmd_open		},
	{ (unsigned char *)"Close",		T_CMD,				0, cmd_close	},
	{ (unsigned char *)"Kill",		T_CMD,				0, cmd_kill		},
	{ (unsigned char *)"Rmdir",		T_CMD,				0, cmd_rmdir	},
	{ (unsigned char *)"Chdir",		T_CMD,				0, cmd_chdir	},
	{ (unsigned char *)"Mkdir",		T_CMD,				0, cmd_mkdir	},
	{ (unsigned char *)"Copy",		T_CMD,				0, cmd_copy		},
	{ (unsigned char *)"Rename",	T_CMD,				0, cmd_name		},
	{ (unsigned char *)"Seek",		T_CMD,				0, cmd_seek     },
	{ (unsigned char *)"Files",		T_CMD,				0, cmd_files    },
	{ (unsigned char *)"Flash",		T_CMD,				0, cmd_flash    },
	{ (unsigned char *)"VAR",		T_CMD,				0, cmd_var     	},
	{ (unsigned char *)"Flush",		T_CMD,				0, cmd_flush    },


#endif


/**********************************************************************************
 All other tokens (keywords, functions, operators) should be inserted in this table
**********************************************************************************/
#ifdef INCLUDE_TOKEN_TABLE
// the format is:
//    TEXT      	TYPE                P  FUNCTION TO CALL
// where type is T_NA, T_FUN, T_FNA or T_OPER argumented by the types T_STR and/or T_NBR
// and P is the precedence (which is only used for operators)
  	{ (unsigned char *)"Eof(",   T_FUN | T_INT,      0, fun_eof      },
  	{ (unsigned char *)"Loc(",   T_FUN | T_INT,      0, fun_loc      },
  	{ (unsigned char *)"Lof(",   T_FUN | T_INT,      0, fun_lof      },
	{ (unsigned char *)"Cwd$",		T_FNA | T_STR,		0, fun_cwd		},
	{ (unsigned char *)"As",			T_NA,			0, op_invalid	},
	{ (unsigned char *)"Input$(",	T_FUN | T_STR,		0, fun_inputstr	},
	{ (unsigned char *)"Dir$(",		T_FUN | T_STR,		0, fun_dir		},


#endif
#include "External.h"
#include "MM_Misc.h"
#include "Editor.h"
#include "Draw.h"
#include "XModem.h"
#include "MATHS.h"
#include "Onewire.h"
#include "I2C.h"
#include "SPI.h"
#include "Serial.h"
#include "SPI-LCD.h"
#ifndef PICOMITEVGA
	#include "Touch.h"
	#include "GUI.h"
	#include "SSD1963.h"
#endif
#include "GPS.h"
#include "Audio.h"
#include "PS2Keyboard.h"
