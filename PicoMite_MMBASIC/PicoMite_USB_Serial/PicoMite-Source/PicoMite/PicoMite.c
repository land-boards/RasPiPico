/***********************************************************************************************************************
PicoMite MMBasic

Picomite.c

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
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "configuration.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/flash.h"
#include "hardware/adc.h"
#include "hardware/exception.h"
#include "MMBasic_Includes.h"
#include "Hardware_Includes.h"
#include "hardware/structs/systick.h"
#include "hardware/structs/scb.h"
#include "hardware/vreg.h"
#include "pico/unique_id.h"
#include <pico/bootrom.h>
#include "hardware/irq.h"
#include "class/cdc/cdc_device.h" 
#ifdef PICOMITEVGA
#include "Include.h"
#define MES_SIGNON  "\rPicoMiteVGA MMBasic Version " VERSION "\r\n"\
					"Copyright " YEAR " Geoff Graham\r\n"\
					"Copyright " YEAR2 " Peter Mather\r\n\r\n"
#else
#define MES_SIGNON  "\rPicoMite MMBasic Version " VERSION "\r\n"\
					"Copyright " YEAR " Geoff Graham\r\n"\
					"Copyright " YEAR2 " Peter Mather\r\n\r\n"
#endif
#define USBKEEPALIVE 30000
int ListCnt;
int MMCharPos;
int busfault=0;
int ExitMMBasicFlag = false;
volatile int MMAbort = false;
unsigned int _excep_peek;
void CheckAbort(void);
void TryLoadProgram(void);
unsigned char lastchar=0;
unsigned char BreakKey = BREAK_KEY;                                          // defaults to CTRL-C.  Set to zero to disable the break function
volatile char ConsoleRxBuf[CONSOLE_RX_BUF_SIZE]={0};
volatile int ConsoleRxBufHead = 0;
volatile int ConsoleRxBufTail = 0;
volatile char ConsoleTxBuf[CONSOLE_TX_BUF_SIZE]={0};
volatile int ConsoleTxBufHead = 0;
volatile int ConsoleTxBufTail = 0;
volatile unsigned int AHRSTimer = 0;
volatile unsigned int InkeyTimer = 0;
volatile long long int mSecTimer = 0;                               // this is used to count mSec
volatile unsigned int WDTimer = 0;
volatile unsigned int diskchecktimer = DISKCHECKRATE;
volatile unsigned int clocktimer=60*60*1000;
volatile unsigned int PauseTimer = 0;
volatile unsigned int IntPauseTimer = 0;
volatile unsigned int Timer1=0, Timer2=0;		                       //1000Hz decrement timer
volatile unsigned int USBKeepalive=USBKEEPALIVE;
volatile int ds18b20Timer = -1;
volatile unsigned int ScrewUpTimer = 0;
volatile int second = 0;                                            // date/time counters
volatile int minute = 0;
volatile int hour = 0;
volatile int day = 1;
volatile int month = 1;
volatile int year = 2000;
volatile unsigned int GPSTimer = 0;
volatile unsigned int SecondsTimer = 0;
volatile unsigned int I2CTimer = 0;
volatile int day_of_week=1;
unsigned char PulsePin[NBR_PULSE_SLOTS];
unsigned char PulseDirection[NBR_PULSE_SLOTS];
int PulseCnt[NBR_PULSE_SLOTS];
int PulseActive;
const uint8_t *flash_option_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
const uint8_t *SavedVarsFlash = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET +  FLASH_ERASE_SIZE);
const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET + FLASH_ERASE_SIZE + SAVEDVARS_FLASH_SIZE);
const uint8_t *flash_progmemory = (const uint8_t *) (XIP_BASE + PROGSTART);
#ifdef PICOMITEVGA
    uint16_t M_Foreground[16]={
    0x0000,0x000F,0x00f0,0x00ff,0x0f00,0x0f0F,0x0ff0,0x0fff,0xf000,0xf00F,0xf0f0,0xf0ff,0xff00,0xff0F,0xfff0,0xffff
    };
    uint16_t M_Background[16]={
    0xffff,0xfff0,0xff0f,0xff00,0xf0ff,0xf0f0,0xf00f,0xf000,0x0fff,0x0ff0,0x0f0f,0x0f00,0x00ff,0x00f0,0x000f,0x0000
    };
    uint16_t tilefcols[40*30];
    uint16_t tilebcols[40*30];
#endif
int ticks_per_second; 
int InterruptUsed;
int calibrate=0;
char id_out[12];
MMFLOAT VCC=3.3;
int PromptFont, PromptFC=0xFFFFFF, PromptBC=0;                             // the font and colours selected at the prompt
volatile int DISPLAY_TYPE;
volatile int processtick = 1;
unsigned char WatchdogSet = false;
unsigned char IgnorePIN = false;
bool timer_callback(repeating_timer_t *rt);
uint32_t __uninitialized_ram(_excep_code);
unsigned char lastcmd[STRINGSIZE*4];                                           // used to store the last command in case it is needed by the EDIT command
int QVGA_CLKDIV;	// SM divide clock ticks
FATFS fs;                 // Work area (file system object) for logical drive
bool timer_callback(repeating_timer_t *rt);
static uint64_t __not_in_flash_func(uSecFunc)(uint64_t a){
    uint64_t b=time_us_64()+a;
    while(time_us_64()<b){}
    return b;
}
union uFileTable {
    unsigned int com;
    FIL *fptr;
};
extern union uFileTable FileTable[MAXOPENFILES + 1];
//Vector to CFunction routine called every command (ie, from the BASIC interrupt checker)
extern unsigned int CFuncInt1;
//Vector to CFunction routine called by the interrupt 2 handler
extern unsigned int CFuncInt2;
extern unsigned int CFuncmSec;
extern void CallCFuncInt1(void);
extern void CallCFuncInt2(void);
extern volatile int CSubComplete;
static uint64_t __not_in_flash_func(timer)(void){ return time_us_64();}
static int64_t PinReadFunc(int a){return gpio_get(PinDef[a].GPno);}
extern void CallExecuteProgram(char *p);
extern void CallCFuncmSec(void);
#define CFUNCRAM_SIZE   256
int CFuncRam[CFUNCRAM_SIZE/sizeof(int)];
MMFLOAT IntToFloat(long long int a){ return a; }
MMFLOAT FMul(MMFLOAT a, MMFLOAT b){ return a * b; }
MMFLOAT FAdd(MMFLOAT a, MMFLOAT b){ return a + b; }
MMFLOAT FSub(MMFLOAT a, MMFLOAT b){ return a - b; }
MMFLOAT FDiv(MMFLOAT a, MMFLOAT b){ return a / b; }
int   FCmp(MMFLOAT a,MMFLOAT b){if(a>b) return 1;else if(a<b)return -1; else return 0;}
MMFLOAT LoadFloat(unsigned long long c){union ftype{ unsigned long long a; MMFLOAT b;}f;f.a=c;return f.b; }
const void * const CallTable[] __attribute__((section(".text")))  = {	(void *)uSecFunc,	//0x00
																		(void *)putConsole,	//0x04
																		(void *)getConsole,	//0x08
																		(void *)ExtCfg,	//0x0c
																		(void *)ExtSet,	//0x10
																		(void *)ExtInp,	//0x14
																		(void *)PinSetBit,	//0x18
																		(void *)PinReadFunc,	//0x1c
																		(void *)MMPrintString,	//0x20
																		(void *)IntToStr,	//0x24
																		(void *)CheckAbort,	//0x28
																		(void *)GetMemory,	//0x2c
																		(void *)GetTempMemory,	//0x30
																		(void *)FreeMemory, //0x34
																		(void *)&DrawRectangle,	//0x38
																		(void *)&DrawBitmap,	//0x3c
																		(void *)DrawLine,	//0x40
																		(void *)FontTable,	//0x44
																		(void *)&ExtCurrentConfig,	//0x48
																		(void *)&HRes,	//0x4C
																		(void *)&VRes,	//0x50
																		(void *)SoftReset, //0x54
																		(void *)error,	//0x58
																		(void *)&ProgMemory,	//0x5c
																		(void *)&vartbl, //0x60
																		(void *)&varcnt,  //0x64
																		(void *)&DrawBuffer,	//0x68
																		(void *)&ReadBuffer,	//0x6c
																		(void *)&FloatToStr,	//0x70
                                                                        (void *)CallExecuteProgram, //0x74
                                                                        (void *)&CFuncmSec, //0x78
                                                                        (void *)CFuncRam,	//0x7c
                                                                        (void *)&ScrollLCD,	//0x80
																		(void *)IntToFloat, //0x84
																		(void *)FloatToInt64,	//0x88
																		(void *)&Option,	//0x8c
																		(void *)sin,	//0x90
																		(void *)DrawCircle,	//0x94
																		(void *)DrawTriangle,	//0x98
																		(void *)timer,	//0x9c
                                                                        (void *)FMul,//0xa0
                                                                        (void *)FAdd,//0xa4
                                                                        (void *)FSub,//0xa8
                                                                        (void *)FDiv,//0xac
                                                                        (void *)FCmp,//0xb0
                                                                        (void *)&LoadFloat,//0xb4
                                                                        (void *)&CFuncInt1,	//0xb8
                                                                        (void *)&CFuncInt2,	//0xbc
																		(void *)&CSubComplete,	//0xc0
									   	   	   	   	   	   	   	   	   	   };

const struct s_PinDef PinDef[NBRPINS + 1]={
	    { 0, 99, "NULL",  UNUSED  ,99, 99},                                                         // pin 0
	    { 1,  0, "GP0",  DIGITAL_IN | DIGITAL_OUT | SPI0RX | UART0TX  | I2C0SDA | PWM0A,99,0},  	// pin 1
		{ 2,  1, "GP1",  DIGITAL_IN | DIGITAL_OUT | UART0RX | I2C0SCL | PWM0B ,99,128},    		    // pin 2
		{ 3, 99, "GND",  UNUSED  ,99,99},                                                           // pin 3
		{ 4,  2, "GP2",  DIGITAL_IN | DIGITAL_OUT | SPI0SCK | I2C1SDA | PWM1A ,99,1},   		    // pin 4
		{ 5,  3, "GP3",  DIGITAL_IN | DIGITAL_OUT | SPI0TX | I2C1SCL | PWM1B ,99,129},    			// pin 5
		{ 6,  4, "GP4",  DIGITAL_IN | DIGITAL_OUT | SPI0RX| UART1TX  | I2C0SDA | PWM2A ,99,2},  	// pin 6
		{ 7,  5, "GP5",  DIGITAL_IN | DIGITAL_OUT | UART1RX | I2C0SCL | PWM2B	,99,130},    		// pin 7
		{ 8, 99, "GND",  UNUSED  ,99, 99},                                                          // pin 8
		{ 9,  6, "GP6",  DIGITAL_IN | DIGITAL_OUT | SPI0SCK | I2C1SDA | PWM3A	,99, 3},  			// pin 9
		{ 10,  7, "GP7",  DIGITAL_IN | DIGITAL_OUT | SPI0TX | I2C1SCL | PWM3B	,99, 131}, 		    // pin 10
	    { 11,  8, "GP8",  DIGITAL_IN | DIGITAL_OUT | SPI1RX | UART1TX  | I2C0SDA | PWM4A ,99, 4}, 	// pin 11
		{ 12,  9, "GP9",  DIGITAL_IN | DIGITAL_OUT | UART1RX | I2C0SCL | PWM4B	,99, 132},    		// pin 12
		{ 13, 99, "GND",  UNUSED  ,99, 99},                                                         // pin 13
		{ 14, 10, "GP10",  DIGITAL_IN | DIGITAL_OUT | SPI1SCK | I2C1SDA | PWM5A	,99, 5},  			// pin 14
		{ 15, 11, "GP11",  DIGITAL_IN | DIGITAL_OUT | SPI1TX | I2C1SCL | PWM5B	,99, 133},       	// pin 15
		{ 16, 12, "GP12",  DIGITAL_IN | DIGITAL_OUT | SPI1RX | UART0TX | I2C0SDA | PWM6A ,99, 6},   // pin 16
		{ 17, 13, "GP13",  DIGITAL_IN | DIGITAL_OUT | UART0RX | I2C0SCL | PWM6B	,99, 134},    		// pin 17
		{ 18, 99, "GND", UNUSED  ,99, 99},                                                          // pin 18
		{ 19, 14, "GP14",  DIGITAL_IN | DIGITAL_OUT | SPI1SCK | I2C1SDA | PWM7A	,99, 7},    		// pin 19
		{ 20, 15, "GP15",  DIGITAL_IN | DIGITAL_OUT | SPI1TX | I2C1SCL | PWM7B	,99, 135},  		// pin 20

		{ 21, 16, "GP16",  DIGITAL_IN | DIGITAL_OUT | SPI0RX | UART0TX | I2C0SDA | PWM0A ,99, 0},   // pin 21
		{ 22, 17, "GP17",  DIGITAL_IN | DIGITAL_OUT | UART0RX | I2C0SCL | PWM0B	,99, 128},    		// pin 22
		{ 23, 99, "GND",  UNUSED  ,99, 99},                                                         // pin 23
	    { 24, 18, "GP18",  DIGITAL_IN | DIGITAL_OUT | SPI0SCK | I2C1SDA | PWM1A	,99, 1}, 			// pin 24
	    { 25, 19, "GP19",  DIGITAL_IN | DIGITAL_OUT | SPI0TX | I2C1SCL | PWM1B	,99, 129},  		// pin 25
	    { 26, 20, "GP20",  DIGITAL_IN | DIGITAL_OUT | SPI0RX | UART1TX| I2C0SDA | PWM2A	,99, 2},    // pin 26
	    { 27, 21, "GP21",  DIGITAL_IN | DIGITAL_OUT | UART1RX| I2C0SCL | PWM2B	,99, 130},    		// pin 27
		{ 28, 99, "GND",  UNUSED  ,99, 99},                                                         // pin 28
		{ 29, 22, "GP22",  DIGITAL_IN | DIGITAL_OUT | I2C1SDA| PWM3A	,99, 3},    				// pin 29
		{ 30, 99, "RUN",  UNUSED  ,99, 99},                                                         // pin 30
	    { 31, 26, "GP26",  DIGITAL_IN | DIGITAL_OUT	| ANALOG_IN | SPI1SCK| I2C1SDA | PWM5A , 0 , 5},// pin 31
	    { 32, 27, "GP27",  DIGITAL_IN | DIGITAL_OUT	| ANALOG_IN | SPI1TX| I2C1SCL | PWM5B , 1, 133},// pin 32
		{ 33, 99, "AGND", UNUSED  ,99, 99},                                                         // pin 33
		{ 34, 28, "GP28", DIGITAL_IN |DIGITAL_OUT| ANALOG_IN| SPI1RX| UART0TX|I2C0SDA| PWM6A, 2, 6},// pin 34
	    { 35, 99, "VREF", UNUSED  ,99, 99},                                                         // pin 35
		{ 36, 99, "3V3", UNUSED  ,99, 99},                                                          // pin 36
		{ 37, 99, "3V3E", UNUSED  ,99, 99},                                                         // pin 37
		{ 38, 99, "GND", UNUSED  ,99, 99},                                                          // pin 38
		{ 39, 99, "VSYS", UNUSED  ,99, 99},                                                         // pin 39
		{ 40, 99, "VBUS", UNUSED  ,99, 99},                                                         // pin 40
		{ 41, 23, "GP23", DIGITAL_IN | DIGITAL_OUT | SPI0TX | I2C1SCL| PWM3B  ,99 , 131},           // pseudo pin 41
		{ 42, 24, "GP24", DIGITAL_IN | DIGITAL_OUT | SPI1RX | UART1TX | I2C0SDA| PWM4A  ,99 , 4},   // pseudo pin 42
		{ 43, 25, "GP25", DIGITAL_IN | DIGITAL_OUT | UART1RX | I2C0SCL| PWM4B  ,99 , 132},          // pseudo pin 43
		{ 44, 29, "GP29", DIGITAL_IN | DIGITAL_OUT | ANALOG_IN | UART0RX | I2C0SCL | PWM6B, 3, 134},// pseudo pin 44
};
char alive[]="\033[?25h";
const char DaysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
void __not_in_flash_func(routinechecks)(void){
    static int c,when=0;
    if(++when & 7 && CurrentLinePtr) return;
#ifdef PICOMITEVGA
    if(Option.SerialConsole==0){
#else
    if(tud_cdc_connected() && Option.SerialConsole==0){
#endif
        while(( c=tud_cdc_read_char())!=-1){
            ConsoleRxBuf[ConsoleRxBufHead] = c;
            if(BreakKey && ConsoleRxBuf[ConsoleRxBufHead] == BreakKey) {// if the user wants to stop the progran
                MMAbort = true;                                        // set the flag for the interpreter to see
                ConsoleRxBufHead = ConsoleRxBufTail;                    // empty the buffer
            } else if(ConsoleRxBuf[ConsoleRxBufHead] ==keyselect && KeyInterrupt!=NULL){
                Keycomplete=1;
            } else {
                ConsoleRxBufHead = (ConsoleRxBufHead + 1) % CONSOLE_RX_BUF_SIZE;     // advance the head of the queue
                if(ConsoleRxBufHead == ConsoleRxBufTail) {                           // if the buffer has overflowed
                    ConsoleRxBufTail = (ConsoleRxBufTail + 1) % CONSOLE_RX_BUF_SIZE; // throw away the oldest char
                }
            }
        }
    }
	if(GPSchannel)processgps();
    if(diskchecktimer== 0 || CurrentlyPlaying == P_WAV)CheckSDCard();
#ifndef PICOMITEVGA
        if(Ctrl)ProcessTouch();
#endif
        if(tud_cdc_connected() && USBKeepalive==0){
            SSPrintString(alive);
        }
    if(clocktimer==0 && Option.RTC){
        RtcGetTime(0);
        clocktimer=(1000*60*60);
    }
}

int __not_in_flash_func(getConsole)(void) {
    int c=-1;
    CheckAbort();
    if(ConsoleRxBufHead != ConsoleRxBufTail) {                            // if the queue has something in it
        c = ConsoleRxBuf[ConsoleRxBufTail];
        ConsoleRxBufTail = (ConsoleRxBufTail + 1) % CONSOLE_RX_BUF_SIZE;   // advance the head of the queue
	}
    return c;
}

void putConsole(int c, int flush) {
    DisplayPutC(c);
    SerialConsolePutC(c, flush);
}
// put a character out to the serial console
char SerialConsolePutC(char c, int flush) {
    if(Option.SerialConsole==0){
        if(tud_cdc_connected()){
            putc(c,stdout);
            if(flush){
                USBKeepalive=USBKEEPALIVE;
                fflush(stdout);
            }
        }
    } else {
        int empty=uart_is_writable(Option.SerialConsole==1 ? uart0 : uart1);
		while(ConsoleTxBufTail == ((ConsoleTxBufHead + 1) % CONSOLE_TX_BUF_SIZE)); //wait if buffer full
		ConsoleTxBuf[ConsoleTxBufHead] = c;							// add the char
		ConsoleTxBufHead = (ConsoleTxBufHead + 1) % CONSOLE_TX_BUF_SIZE;		   // advance the head of the queue
		if(empty){
	        uart_set_irq_enables(Option.SerialConsole==1 ? uart0 : uart1, true, true);
			irq_set_pending(Option.SerialConsole==1 ? UART0_IRQ : UART1_IRQ);
		}
    }
    return c;
}
char MMputchar(char c, int flush) {
    putConsole(c, flush);
    if(isprint(c)) MMCharPos++;
    if(c == '\r') {
        MMCharPos = 1;
    }
    return c;
}
// returns the number of character waiting in the console input queue
int kbhitConsole(void) {
    int i;
    i = ConsoleRxBufHead - ConsoleRxBufTail;
    if(i < 0) i += CONSOLE_RX_BUF_SIZE;
    return i;
}
// check if there is a keystroke waiting in the buffer and, if so, return with the char
// returns -1 if no char waiting
// the main work is to check for vt100 escape code sequences and map to Maximite codes
int __not_in_flash_func(MMInkey)(void) {
    unsigned int c = -1;                                            // default no character
    unsigned int tc = -1;                                           // default no character
    unsigned int ttc = -1;                                          // default no character
    static unsigned int c1 = -1;
    static unsigned int c2 = -1;
    static unsigned int c3 = -1;
    static unsigned int c4 = -1;

    if(c1 != -1) {                                                  // check if there are discarded chars from a previous sequence
        c = c1; c1 = c2; c2 = c3; c3 = c4; c4 = -1;                 // shuffle the queue down
        return c;                                                   // and return the head of the queue
    }

    c = getConsole();                                               // do discarded chars so get the char
if(!(c==0x1b))return c;
    InkeyTimer = 0;                                             // start the timer
    while((c = getConsole()) == -1 && InkeyTimer < 30);         // get the second char with a delay of 30mS to allow the next char to arrive
    if(c == 'O'){   //support for many linux terminal emulators
        while((c = getConsole()) == -1 && InkeyTimer < 50);        // delay some more to allow the final chars to arrive, even at 1200 baud
        if(c == 'P') return F1;
        if(c == 'Q') return F2;
        if(c == 'R') return F3;
        if(c == 'S') return F4;
        if(c == '2'){
            while((tc = getConsole()) == -1 && InkeyTimer < 70);        // delay some more to allow the final chars to arrive, even at 1200 baud
            if(tc == 'R') return F3 + 0x20;
            c1 = 'O'; c2 = c; c3 = tc; return 0x1b;                 // not a valid 4 char code
        }
        c1 = 'O'; c2 = c; return 0x1b;                 // not a valid 4 char code
    }
    if(c != '[') { c1 = c; return 0x1b; }                       // must be a square bracket
    while((c = getConsole()) == -1 && InkeyTimer < 50);         // get the third char with delay
    if(c == 'A') return UP;                                     // the arrow keys are three chars
    if(c == 'B') return DOWN;
    if(c == 'C') return RIGHT;
    if(c == 'D') return LEFT;
    if(c < '1' && c > '6') { c1 = '['; c2 = c; return 0x1b; }   // the 3rd char must be in this range
    while((tc = getConsole()) == -1 && InkeyTimer < 70);        // delay some more to allow the final chars to arrive, even at 1200 baud
    if(tc == '~') {                                             // all 4 char codes must be terminated with ~
        if(c == '1') return HOME;
        if(c == '2') return INSERT;
        if(c == '3') return DEL;
        if(c == '4') return END;
        if(c == '5') return PUP;
        if(c == '6') return PDOWN;
        c1 = '['; c2 = c; c3 = tc; return 0x1b;                 // not a valid 4 char code
    }
    while((ttc = getConsole()) == -1 && InkeyTimer < 90);       // get the 5th char with delay
    if(ttc == '~') {                                            // must be a ~
        if(c == '1') {
            if(tc >='1' && tc <= '5') return F1 + (tc - '1');   // F1 to F5
            if(tc >='7' && tc <= '9') return F6 + (tc - '7');   // F6 to F8
        }
        if(c == '2') {
            if(tc =='0' || tc == '1') return F9 + (tc - '0');   // F9 and F10
            if(tc =='3' || tc == '4') return F11 + (tc - '3');  // F11 and F12
            if(tc =='5') return F3 + 0x20;                      // SHIFT-F3
        }
    }
    // nothing worked so bomb out
    c1 = '['; c2 = c; c3 = tc; c4 = ttc;
    return 0x1b;
}

// get a line from the keyboard or a serial file handle
// filenbr == 0 means the console input
void MMgetline(int filenbr, char *p) {
	int c, nbrchars = 0;
	char *tp;
    while(1) {
        CheckAbort();												// jump right out if CTRL-C
        if(FileTable[filenbr].com > MAXCOMPORTS && FileEOF(filenbr)) break;
        c = MMfgetc(filenbr);
        if(c <= 0) continue;                                       // keep looping if there are no chars

        // if this is the console, check for a programmed function key and insert the text
        if(filenbr == 0) {
            tp = NULL;
            if(c == F2)  tp = "RUN";
            if(c == F3)  tp = "LIST";
            if(c == F4)  tp = "EDIT";
            if(c == F10) tp = "AUTOSAVE";
            if(c == F11) tp = "XMODEM RECEIVE";
            if(c == F12) tp = "XMODEM SEND";
            if(c == F1) tp = Option.F1key;
            if(c == F5) tp = Option.F5key;
            if(c == F6) tp = Option.F6key;
            if(c == F7) tp = Option.F7key;
            if(c == F8) tp = Option.F8key;
            if(c == F9) tp = Option.F9key;
            if(tp) {
                strcpy(p, tp);
                if(EchoOption) { MMPrintString(tp); MMPrintString("\r\n"); }
                return;
            }
        }

		if(c == '\t') {												// expand tabs to spaces
			 do {
				if(++nbrchars > MAXSTRLEN) error("Line is too long");
				*p++ = ' ';
				if(filenbr == 0 && EchoOption) MMputchar(' ',1);
			} while(nbrchars % 4);
			continue;
		}

		if(c == '\b') {												// handle the backspace
			if(nbrchars) {
				if(filenbr == 0 && EchoOption) MMPrintString("\b \b");
				nbrchars--;
				p--;
			}
			continue;
		}

        if(c == '\n') {                                             // what to do with a newline
                break;                                              // a newline terminates a line (for a file)
        }

        if(c == '\r') {
            if(filenbr == 0 && EchoOption) {
                MMPrintString("\r\n");
                break;                                              // on the console this means the end of the line - stop collecting
            } else
                continue ;                                          // for files loop around looking for the following newline
        }
        
		if(isprint(c)) {
			if(filenbr == 0 && EchoOption) MMputchar(c,1);           // The console requires that chars be echoed
		}
		if(++nbrchars > MAXSTRLEN) error("Line is too long");		// stop collecting if maximum length
		*p++ = c;													// save our char
	}
	*p = 0;
}
// insert a string into the start of the lastcmd buffer.
// the buffer is a sequence of strings separated by a zero byte.
// using the up arrow usere can call up the last few commands executed.
void InsertLastcmd(unsigned char *s) {
int i, slen;
    if(strcmp(lastcmd, s) == 0) return;                             // don't duplicate
    slen = strlen(s);
    if(slen < 1 || slen > STRINGSIZE*4 - 1) return;
    slen++;
    for(i = STRINGSIZE*4 - 1; i >=  slen ; i--)
        lastcmd[i] = lastcmd[i - slen];                             // shift the contents of the buffer up
    strcpy(lastcmd, s);                                             // and insert the new string in the beginning
    for(i = STRINGSIZE*4 - 1; lastcmd[i]; i--) lastcmd[i] = 0;             // zero the end of the buffer
}

void EditInputLine(void) {
    char *p = NULL;
    char buf[MAXKEYLEN + 3];
    int lastcmd_idx, lastcmd_edit;
    int insert, startline, maxchars;
    int CharIndex, BufEdited;
    int c, i, j;
    maxchars = Option.Width;
    if(strlen(inpbuf) >= maxchars) {
        MMPrintString(inpbuf);
        error("Line is too long to edit");
    }
    startline = MMCharPos - 1;                                                          // save the current cursor position
    MMPrintString(inpbuf);                                                              // display the contents of the input buffer (if any)
    CharIndex = strlen(inpbuf);                                                         // get the current cursor position in the line
    insert = false;
//    Cursor = C_STANDARD;
    lastcmd_edit = lastcmd_idx = 0;
    BufEdited = false; //(CharIndex != 0);
    while(1) {
        c = MMgetchar();
        if(c == TAB) {
            strcpy(buf, "        ");
            switch (Option.Tab) {
              case 2:
                buf[2 - (CharIndex % 2)] = 0; break;
              case 3:
                buf[3 - (CharIndex % 3)] = 0; break;
              case 4:
                buf[4 - (CharIndex % 4)] = 0; break;
              case 8:
                buf[8 - (CharIndex % 8)] = 0; break;
            }
        } else {
            buf[0] = c;
            buf[1] = 0;
        }
        do {
            switch(buf[0]) {
                case '\r':
                case '\n':  //if(autoOn && atoi(inpbuf) > 0) autoNext = atoi(inpbuf) + autoIncr;
                            //if(autoOn && !BufEdited) *inpbuf = 0;
                            goto saveline;
                            break;

                case '\b':  if(CharIndex > 0) {
                                BufEdited = true;
                                i = CharIndex - 1;
                                for(p = inpbuf + i; *p; p++) *p = *(p + 1);                 // remove the char from inpbuf
                                while(CharIndex)  { MMputchar('\b',0); CharIndex--; }         // go to the beginning of the line
                                MMPrintString(inpbuf); MMputchar(' ',0); MMputchar('\b',0);     // display the line and erase the last char
                                for(CharIndex = strlen(inpbuf); CharIndex > i; CharIndex--)
                                    MMputchar('\b',0);  
                                fflush(stdout);                                      // return the cursor to the righ position
                            }
                            break;

                case CTRLKEY('S'):
                case LEFT:  if(CharIndex > 0) {
                                if(CharIndex == strlen(inpbuf)) {
                                    insert = true;
      //                              Cursor = C_INSERT;
                                }
                                MMputchar('\b',1);
                                CharIndex--;
                            }
                            break; 

                case CTRLKEY('D'):
                case RIGHT: if(CharIndex < strlen(inpbuf)) {
                                MMputchar(inpbuf[CharIndex],1);
                                CharIndex++;
                            }
                            break;

                case CTRLKEY(']'):
                case DEL:   if(CharIndex < strlen(inpbuf)) {
                                BufEdited = true;
                                i = CharIndex;
                                for(p = inpbuf + i; *p; p++) *p = *(p + 1);                 // remove the char from inpbuf
                                while(CharIndex)  { MMputchar('\b',0); CharIndex--; }         // go to the beginning of the line
                                MMPrintString(inpbuf); MMputchar(' ',0); MMputchar('\b',0);     // display the line and erase the last char
                                for(CharIndex = strlen(inpbuf); CharIndex > i; CharIndex--)
                                    MMputchar('\b',0);   
                                fflush(stdout);                                     // return the cursor to the right position
                            }
                            break;

                case CTRLKEY('N'):
                case INSERT:insert = !insert;
//                            Cursor = C_STANDARD + insert;
                            break;

                case CTRLKEY('U'):
                case HOME:  if(CharIndex > 0) {
                                if(CharIndex == strlen(inpbuf)) {
                                    insert = true;
//                                    Cursor = C_INSERT;
                                }
                                while(CharIndex)  { MMputchar('\b',0); CharIndex--; }
                                fflush(stdout);
                            }
                            break;

                case CTRLKEY('K'):
                case END:   while(CharIndex < strlen(inpbuf))
                                MMputchar(inpbuf[CharIndex++],0);
                                fflush(stdout);
                            break;

/*            if(c == F2)  tp = "RUN";
            if(c == F3)  tp = "LIST";
            if(c == F4)  tp = "EDIT";
            if(c == F10) tp = "AUTOSAVE";
            if(c == F11) tp = "XMODEM RECEIVE";
            if(c == F12) tp = "XMODEM SEND";
            if(c == F5) tp = Option.F5key;
            if(c == F6) tp = Option.F6key;
            if(c == F7) tp = Option.F7key;
            if(c == F8) tp = Option.F8key;
            if(c == F9) tp = Option.F9key;*/
                case 0x91:
                    if(*Option.F1key)strcpy(&buf[1],Option.F1key);
                    break;
                case 0x92:
                    strcpy(&buf[1],"RUN\r\n");
                    break;
                case 0x93:
                    strcpy(&buf[1],"LIST\r\n");
                    break;
                case 0x94:
                    strcpy(&buf[1],"EDIT\r\n");
                    break;
                case 0x95:
                    if(*Option.F5key)strcpy(&buf[1],Option.F5key);
                    break;
                case 0x96:
                    if(*Option.F6key)strcpy(&buf[1],Option.F6key);
                    break;
                case 0x97:
                    if(*Option.F7key)strcpy(&buf[1],Option.F7key);
                    break;
                case 0x98:
                    if(*Option.F8key)strcpy(&buf[1],Option.F8key);
                    break;
                case 0x99:
                    if(*Option.F9key)strcpy(&buf[1],Option.F9key);
                    break;
                case 0x9a:
                    strcpy(&buf[1],"AUTOSAVE\r\n");
                    break;
                case 0x9b:
                    strcpy(&buf[1],"XMODEM RECEIVE\r\n");
                    break;
                 case 0x9c:
                    strcpy(&buf[1],"XMODEM SEND\r\n");
                    break;
                case CTRLKEY('E'):
                case UP:    if(!(BufEdited /*|| autoOn || CurrentLineNbr */)) {
                                while(CharIndex)  { MMputchar('\b',0); CharIndex--; } 
                                fflush(stdout);       // go to the beginning of line
                                if(lastcmd_edit) {
                                    i = lastcmd_idx + strlen(&lastcmd[lastcmd_idx]) + 1;    // find the next command
                                    if(lastcmd[i] != 0 && i < STRINGSIZE*4 - 1) lastcmd_idx = i;  // and point to it for the next time around
                                } else
                                    lastcmd_edit = true;
                                strcpy(inpbuf, &lastcmd[lastcmd_idx]);                      // get the command into the buffer for editing
                                goto insert_lastcmd;
                            }
                            break;


                case CTRLKEY('X'):
                case DOWN:  if(!(BufEdited /*|| autoOn || CurrentLineNbr */)) {
                                while(CharIndex)  { MMputchar('\b',0); CharIndex--; }   
                                fflush(stdout);      // go to the beginning of line
                                if(lastcmd_idx == 0)
                                    *inpbuf = lastcmd_edit = 0;
                                else {
                                    for(i = lastcmd_idx - 2; i > 0 && lastcmd[i - 1] != 0; i--);// find the start of the previous command
                                    lastcmd_idx = i;                                        // and point to it for the next time around
                                    strcpy(inpbuf, &lastcmd[i]);                            // get the command into the buffer for editing
                                }
                                goto insert_lastcmd;                                        // gotos are bad, I know, I know
                            }
                            break;

                insert_lastcmd:                                                             // goto here if we are just editing a command
                            if(strlen(inpbuf) + startline >= maxchars) {                    // if the line is too long
                                while(CharIndex)  { MMputchar('\b',0); CharIndex--; }         // go to the start of the line
                                MMPrintString(inpbuf);                                      // display the offending line
                                error("Line is too long to edit");
                            }
                            MMPrintString(inpbuf);                                          // display the line
                            CharIndex = strlen(inpbuf);                                     // get the current cursor position in the line
                            for(i = 1; i <= maxchars - strlen(inpbuf) - startline; i++) {
                                MMputchar(' ',0);                                             // erase the rest of the line
                                CharIndex++;
                            }
                            while(CharIndex > strlen(inpbuf)) { MMputchar('\b',0); CharIndex--; } // return the cursor to the right position

                            SSPrintString("\033[0K");
                            break;

                default:    if(buf[0] >= ' ' && buf[0] < 0x7f) {
                                BufEdited = true;                                           // this means that something was typed
                                i = CharIndex;
                                j = strlen(inpbuf);
                                if(insert) {
                                    if(strlen(inpbuf) >= maxchars - 1) break;               // sorry, line full
                                    for(p = inpbuf + strlen(inpbuf); j >= CharIndex; p--, j--) *(p + 1) = *p;
                                    inpbuf[CharIndex] = buf[0];                             // insert the char
                                    MMPrintString(&inpbuf[CharIndex]);                      // display new part of the line
                                    CharIndex++;
                                    for(j = strlen(inpbuf); j > CharIndex; j--)
                                        MMputchar('\b',0); 
                                        fflush(stdout);                                   // return the cursor to the right position
                                } else {
                                    inpbuf[strlen(inpbuf) + 1] = 0;                         // incase we are adding to the end of the string
                                    inpbuf[CharIndex++] = buf[0];                           // overwrite the char
                                    MMputchar(buf[0],1);                                      // display it
                                    if(CharIndex + startline >= maxchars) {                 // has the input gone beyond the end of the line?
                                        MMgetline(0, inpbuf);                               // use the old fashioned way of getting the line
                                        //if(autoOn && atoi(inpbuf) > 0) autoNext = atoi(inpbuf) + autoIncr;
                                        goto saveline;
                                    }
                                }
                            }
                            break;
            }
            for(i = 0; i < MAXKEYLEN + 1; i++) buf[i] = buf[i + 1];                             // shuffle down the buffer to get the next char
        } while(*buf);
    if(CharIndex == strlen(inpbuf)) {
        insert = false;
//        Cursor = C_STANDARD;
        }
    }

    saveline:
//    Cursor = C_STANDARD;
    MMPrintString("\r\n");
}
// get a keystroke.  Will wait forever for input
// if the unsigned char is a cr then replace it with a newline (lf)
int MMgetchar(void) {
	int c;
	do {
		ShowCursor(1);
        CheckKeyboard();
		c=MMInkey();
	} while(c == -1);
	ShowCursor(0);
	return c;
}
// print a string to the console interfaces
void MMPrintString(char* s) {
    while(*s) {
        MMputchar(*s,0);
        s++;
    }
    fflush(stdout);
    USBKeepalive=USBKEEPALIVE;
}
void SSPrintString(char* s) {
    while(*s) {
        SerialConsolePutC(*s,0);
        s++;
    }
    fflush(stdout);
    USBKeepalive=USBKEEPALIVE;
}

/*void myprintf(char *s){
   fputs(s,stdout);
     fflush(stdout);
}*/

void mT4IntEnable(int status){
	if(status){
		processtick=1;
	} else{
		processtick=0;
	}
}

/*void init_systick()
{ 
	systick_hw->csr = 0; 	    //Disable 
	systick_hw->rvr = 249999; //Standard System clock (125Mhz)/ (rvr value + 1) = 1ms 
    systick_hw->csr = 0x7;      //Enable Systic, Enable Exceptions	
}*/
bool __not_in_flash_func(timer_callback)(repeating_timer_t *rt)
{
    mSecTimer++;                                                      // used by the TIMER function
    if(processtick){
        static int IrTimeout, IrTick, NextIrTick;
        int ElapsedMicroSec, IrDevTmp, IrCmdTmp;
        AHRSTimer++;
        InkeyTimer++;                                                     // used to delay on an escape character
        mSecTimer++;                                                      // used by the TIMER function
        PauseTimer++;													// used by the PAUSE command
        IntPauseTimer++;												// used by the PAUSE command inside an interrupt
        ds18b20Timer++;
		GPSTimer++;
        I2CTimer++;
        if(clocktimer)clocktimer--;
        if(Timer2)Timer2--;
        if(Timer1)Timer1--;
        if(USBKeepalive)USBKeepalive--;
        if(diskchecktimer && Option.SD_CS)diskchecktimer--;
	    if(++CursorTimer > CURSOR_OFF + CURSOR_ON) CursorTimer = 0;		// used to control cursor blink rate
        if(CFuncmSec) CallCFuncmSec();                                  // the 1mS tick for CFunctions (see CFunction.c)
        if(InterruptUsed) {
            int i;
            for(i = 0; i < NBRSETTICKS; i++) if(TickActive[i])TickTimer[i]++;			// used in the interrupt tick
         }
        if(WDTimer) {
            if(--WDTimer == 0) {
                _excep_code = WATCHDOG_TIMEOUT;
                SoftReset();                                            // crude way of implementing a watchdog timer.
            }
        }
        if (ScrewUpTimer) {
            if (--ScrewUpTimer == 0) {
                _excep_code = SCREWUP_TIMEOUT;
                SoftReset();                                            // crude way of implementing a watchdog timer.
            }
        }
        if(PulseActive) {
            int i;
            for(PulseActive = i = 0; i < NBR_PULSE_SLOTS; i++) {
                if(PulseCnt[i] > 0) {                                   // if the pulse timer is running
                    PulseCnt[i]--;                                      // and decrement our count
                    if(PulseCnt[i] == 0)                                // if this is the last count reset the pulse
                        PinSetBit(PulsePin[i], LATINV);
                    else
                        PulseActive = true;                             // there is at least one pulse still active
                }
            }
        }
        ElapsedMicroSec = readIRclock();
        if(IrState > IR_WAIT_START && ElapsedMicroSec > 15000) IrReset();
        IrCmdTmp = -1;
        
        // check for any Sony IR receive activity
        if(IrState == SONY_WAIT_BIT_START && ElapsedMicroSec > 2800 && (IrCount == 12 || IrCount == 15 || IrCount == 20)) {
            IrDevTmp = ((IrBits >> 7) & 0b11111);
            IrCmdTmp = (IrBits & 0b1111111) | ((IrBits >> 5) & ~0b1111111);
        }
        
        // check for any NEC IR receive activity
        if(IrState == NEC_WAIT_BIT_END && IrCount == 32) {
            // check if it is a NON extended address and adjust if it is
            if((IrBits >> 24) == ~((IrBits >> 16) & 0xff)) IrBits = (IrBits & 0x0000ffff) | ((IrBits >> 8) & 0x00ff0000);
            IrDevTmp = ((IrBits >> 16) & 0xffff);
            IrCmdTmp = ((IrBits >> 8) & 0xff);
        }
#ifndef PICOMITEVGA
    // check on the touch panel, is the pen down?

    TouchTimer++;
    if(CheckGuiFlag) CheckGuiTimeouts();                            // are blinking LEDs in use?  If so count down their timers

    if(TouchIrqPortAddr && TOUCH_GETIRQTRIS){                       // is touch enabled and the PEN IRQ pin an input?
        if(TOUCH_DOWN) {                                            // is the pen down
            if(!TouchState) {                                       // yes, it is.  If we have not reported this before
                TouchState = TouchDown = true;                      // set the flags
//                TouchUp = false;
            }
        } else {
            if(TouchState) {                                        // the pen is not down.  If we have not reported this before
                TouchState/* = TouchDown*/ = false;                     // set the flags
                TouchUp = true;
            }
        }
    }

    if(ClickTimer) {
        ClickTimer--;
        if(Option.TOUCH_Click) PinSetBit(Option.TOUCH_Click, ClickTimer ? LATSET : LATCLR);
    }
#endif
    // now process the IR message, this includes handling auto repeat while the key is held down
    // IrTick counts how many mS since the key was first pressed
    // NextIrTick is used to time the auto repeat
    // IrTimeout is used to detect when the key is released
    // IrGotMsg is a signal to the interrupt handler that an interrupt is required
    if(IrCmdTmp != -1) {
        if(IrTick > IrTimeout) {
            // this is a new keypress
            IrTick = 0;
            NextIrTick = 650;
        }
        if(IrTick == 0 || IrTick > NextIrTick) {
            if(IrVarType & 0b01)
                *(MMFLOAT *)IrDev = IrDevTmp;
            else
                *(long long int *)IrDev = IrDevTmp;
            if(IrVarType & 0b10)
                *(MMFLOAT *)IrCmd = IrCmdTmp;
            else
                *(long long int *)IrCmd = IrCmdTmp;
            IrGotMsg = true;
            NextIrTick += 250;
        }
        IrTimeout = IrTick + 150;
        IrReset();
    }
    IrTick++;
	if(ExtCurrentConfig[Option.INT1pin] == EXT_PER_IN) INT1Count++;
	if(ExtCurrentConfig[Option.INT2pin] == EXT_PER_IN) INT2Count++;
	if(ExtCurrentConfig[Option.INT3pin] == EXT_PER_IN) INT3Count++;
	if(ExtCurrentConfig[Option.INT4pin] == EXT_PER_IN) INT4Count++;
    if(ExtCurrentConfig[Option.INT1pin] == EXT_FREQ_IN && --INT1Timer <= 0) { INT1Value = INT1Count; INT1Count = 0; INT1Timer = INT1InitTimer; }
    if(ExtCurrentConfig[Option.INT2pin] == EXT_FREQ_IN && --INT2Timer <= 0) { INT2Value = INT2Count; INT2Count = 0; INT2Timer = INT2InitTimer; }
    if(ExtCurrentConfig[Option.INT3pin] == EXT_FREQ_IN && --INT3Timer <= 0) { INT3Value = INT3Count; INT3Count = 0; INT3Timer = INT3InitTimer; }
    if(ExtCurrentConfig[Option.INT4pin] == EXT_FREQ_IN && --INT4Timer <= 0) { INT4Value = INT4Count; INT4Count = 0; INT4Timer = INT4InitTimer; }

    ////////////////////////////////// this code runs once a second /////////////////////////////////
    if(++SecondsTimer >= 1000) {
        SecondsTimer -= 1000; 
        if(ExtCurrentConfig[PinDef[HEARTBEATpin].pin]==EXT_HEARTBEAT)gpio_xor_mask(1<<PICO_DEFAULT_LED_PIN);
            // keep track of the time and date
        if(++second >= 60) {
            second = 0 ;
            if(++minute >= 60) {
                minute = 0;
                if(++hour >= 24) {
                    hour = 0;
                    if(++day > DaysInMonth[month] + ((month == 2 && (year % 4) == 0)?1:0)) {
                        day = 1;
                        if(++month > 12) {
                            month = 1;
                            year++;
                        }
                    }
                }
            }
        }
    }
    }
  return 1;
}
void __not_in_flash_func(CheckAbort)(void) {
    routinechecks();
    if(MMAbort) {
        WDTimer = 0;                                                // turn off the watchdog timer
        calibrate=0;
        memset(inpbuf,0,STRINGSIZE);
        ShowCursor(false);
        longjmp(mark, 1);                                           // jump back to the input prompt
    }
}
void PRet(void){
    MMPrintString("\r\n");
}
void SRet(void){
    SSPrintString("\r\n");
}

void PInt(int64_t n) {
    char s[20];
    IntToStr(s, (int64_t)n, 10);
    MMPrintString(s);
}
void SInt(int64_t n) {
    char s[20];
    IntToStr(s, (int64_t)n, 10);
    SSPrintString(s);
}

void SIntComma(int64_t n) {
    SSPrintString(", "); SInt(n);
}

void PIntComma(int64_t n) {
    MMPrintString(", "); PInt(n);
}

void PIntH(unsigned long long int n) {
    char s[20];
    IntToStr(s, (int64_t)n, 16);
    MMPrintString(s);
}
void PIntHC(unsigned long long int n) {
    MMPrintString(", "); PIntH(n);
}

void PFlt(MMFLOAT flt){
	   char s[20];
	   FloatToStr(s, flt, 4,4, ' ');
	    MMPrintString(s);
}
void PFltComma(MMFLOAT n) {
    MMPrintString(", "); PFlt(n);
}
void sigbus(void){
    MMPrintString("Error: Invalid address - resetting\r\n");
	uSec(250000);
	disable_interrupts();
	flash_range_erase(PROGSTART, MAX_PROG_SIZE);
	enable_interrupts();
    memset(inpbuf,0,STRINGSIZE);
    SoftReset();
}

#ifdef PICOMITEVGA
// ****************************************************************************
//
//                                  QVGA
//
// ****************************************************************************
// VGA resolution:
// - 640x480 pixels
// - vertical frequency 60 Hz
// - horizontal frequency 31.4685 kHz
// - pixel clock 25.175 MHz
//
// QVGA resolution:
// - 320x240 pixels
// - vertical double image scanlines
// - vertical frequency 60 Hz
// - horizontal frequency 31.4685 kHz
// - pixel clock 12.5875 MHz
//
// VGA vertical timings:
// - 525 scanlines total
// - line 1,2: (2) vertical sync
// - line 3..35: (33) dark
// - line 36..515: (480) image lines 0..479
// - line 516..525: (10) dark
//
// VGA horizontal timings:
// - 31.77781 total scanline in [us] (800 pixels, QVGA 400 pixels)
// - 0.63556 H front porch (after image, before HSYNC) in [us] (16 pixels, QVGA 8 pixels)
// - 3.81334 H sync pulse in [us] (96 pixels, QVGA 48 pixels)
// - 1.90667 H back porch (after HSYNC, before image) in [us] (48 pixels, QVGA 24 pixels)
// - 25.42224 H full visible in [us] (640 pixels, QVGA 320 pixels)
// - 0.0397222625 us per pixel at VGA, 0.079444525 us per pixel at QVGA
//
// We want reach 25.175 pixel clock (at 640x480). Default system clock is 125 MHz, which is
// approx. 5x pixel clock. We need 25.175*5 = 125.875 MHz. We use nearest frequency 126 MHz.
//	126000, 1512000, 126, 6, 2,     // 126.00MHz, VC0=1512MHz, FBDIV=126, PD1=6, PD2=2
//	126000, 504000, 42, 4, 1,       // 126.00MHz, VC0=504MHz, FBDIV=42, PD1=4, PD2=1
//	sysclk=126.000000 MHz, vco=504 MHz, fbdiv=42, pd1=4, pd2=1
//	sysclk=126.000000 MHz, vco=504 MHz, fbdiv=42, pd1=2, pd2=2
//	sysclk=126.000000 MHz, vco=756 MHz, fbdiv=63, pd1=6, pd2=1
//	sysclk=126.000000 MHz, vco=756 MHz, fbdiv=63, pd1=3, pd2=2
//	sysclk=126.000000 MHz, vco=1008 MHz, fbdiv=84, pd1=4, pd2=2 !!!!!
//	sysclk=126.000000 MHz, vco=1260 MHz, fbdiv=105, pd1=5, pd2=2
//	sysclk=126.000000 MHz, vco=1512 MHz, fbdiv=126, pd1=6, pd2=2
//	sysclk=126.000000 MHz, vco=1512 MHz, fbdiv=126, pd1=4, pd2=3
// Pixel clock is now:
//      5 system clock ticks per pixel at VGA ... pixel clock = 25.2 MHz, 0.039683 us per pixel
//     10 system clock ticks per pixel at QVGA ... pixel clock = 12.6 MHz, 0.079365 us per pixel
//
// - active image is 640*5=3200 clock ticks = 25.3968 us (QVGA: 1600 clock ticks)
// - total scanline is 126*31.77781=4004 clock ticks (QVGA: 2002 clock ticks)
// - H front porch = 82 clock ticks (QVGA: 41 clock ticks)
// - H sync pulse = 480 clock ticks (QVGA: 240 clock ticks)
// - H back porch = 242 clock ticks (QVGA: 121 clock ticks)

// in case of system clock = 125 MHz
// - PIO clock = system clock / 2
// - 5 PIO clocks per pixel = 10 system clocks per pixel
// - PIO clock = 62.5 MHz
// - pixel clock = 12.5 MHz
// - active image (320 pixels): 320*5 = 1600 PIO clocks = 3200 system ticks = 25.6 us (2.2 pixels stays invisible)
// - total scanline: 125*31.77781 = 3972 system clocks = 1986 PIO clocks
// - H front porch = 34 PIO clk
// - H sync = 238 PIO clk
// - H back = 114 PIO clk


// PIO command (jmp=program address, num=loop counter)
#define QVGACMD(jmp, num) ( ((uint32_t)((jmp)+QVGAOff)<<27) | (uint32_t)(num))

// display frame buffer

// pointer to current frame buffer
uint QVGAOff;	// offset of QVGA PIO program

// Scanline data buffers (commands sent to PIO)
uint32_t ScanLineImg[3];	// image: HSYNC ... back porch ... image command
uint32_t ScanLineFp;		// front porch
uint32_t ScanLineDark[2];	// dark: HSYNC ... back porch + dark + front porch
uint32_t ScanLineSync[2];	// vertical sync: VHSYNC ... VSYNC(back porch + dark + front porch)

// Scanline control buffers
#define CB_MAX 8	// size of one scanline control buffer (1 link to data buffer requires 2x uint32_t)
uint32_t ScanLineCB[2*CB_MAX]; // 2 control buffers
int QVgaBufInx;		// current running control buffer
uint32_t* ScanLineCBNext;	// next control buffer

// handler variables
volatile int QVgaScanLine; // current processed scan line 0... (next displayed scan line)
volatile uint32_t QVgaFrame;	// frame counter
uint16_t fbuff[2][160]={0};

// saved integer divider state
// VGA DMA handler - called on end of every scanline
void __not_in_flash_func(QVgaLine0)()
{
    static int nextbuf=0,nowbuf=1,i,line,bufinx;
	// Clear the interrupt request for DMA control channel
	dma_hw->ints0 = (1u << QVGA_DMA_PIO);

	// update DMA control channel and run it
	dma_channel_set_read_addr(QVGA_DMA_CB, ScanLineCBNext, true);

	// save integer divider state
//	hw_divider_save_state(&SaveDividerState);

	// switch current buffer index (bufinx = current preparing buffer, MiniVgaBufInx = current running buffer)
	bufinx = QVgaBufInx;
	QVgaBufInx = bufinx ^ 1;

	// prepare control buffer to be processed
	uint32_t* cb = &ScanLineCB[bufinx*CB_MAX];
	ScanLineCBNext = cb;

	// increment scanline (1..)
	line = QVgaScanLine; // current scanline
	line++; 		// new current scanline
	if (line >= QVGA_VTOT) // last scanline?
	{
		QVgaFrame++;	// increment frame counter
		line = 0; 	// restart scanline
	}
	QVgaScanLine = line;	// store new scanline

	// check scanline
	line -= QVGA_VSYNC;
	if (line < 0)
	{
		// VSYNC
		*cb++ = 2;
		*cb++ = (uint32_t)&ScanLineSync[0];
	}
	else
	{
		// front porch and back porch
		line -= QVGA_VBACK;
		if ((line < 0) || (line >= QVGA_VACT))
		{
			// dark line
			*cb++ = 2;
			*cb++ = (uint32_t)&ScanLineDark[0];
		}

		// image scanlines
		else
		{
			// prepare image line
            if(DISPLAY_TYPE==MONOVGA){
            	int ytile=line>>4;
                for(int i=0,j=0;i<80;i++,j+=2){
                    int xtile=i>>1;
                    int low= FrameBuf[line * 80 + i] & 0xF;
                    int high=FrameBuf[line * 80 + i] >>4;
                    int pos=ytile*40+xtile;
                    fbuff[nextbuf][j]=(M_Foreground[low] & tilefcols[pos]) | (M_Background[low] & tilebcols[pos]) ;
                    fbuff[nextbuf][j+1]=(M_Foreground[high]& tilefcols[pos]) | (M_Background[high] & tilebcols[pos]) ;
                }
            } else {
                line>>=1;
                for(int i=0;i<160;i++){
                    int low= FrameBuf[line * 160 + i] & 0xF;
                    int high=FrameBuf[line * 160 + i] >>4;
                    fbuff[nextbuf][i]=(low | (low<<4) | (high<<8) | (high<<12));
                }
            }
            if(nextbuf){
                nextbuf=0;nowbuf=1;
            } else {
                nextbuf=1;nowbuf=0;
            }

			// HSYNC ... back porch ... image command
			*cb++ = 3;
			*cb++ = (uint32_t)&ScanLineImg[0];

			// image data
			*cb++ = 80;
			*cb++ = (uint32_t)fbuff[nowbuf];

			// front porch
			*cb++ = 1;
			*cb++ = (uint32_t)&ScanLineFp;
		}
	}

	// end mark
	*cb++ = 0;
	*cb = 0;

	// restore integer divider state
//	hw_divider_restore_state(&SaveDividerState);
}

// initialize QVGA PIO
void QVgaPioInit()
{
	int i;

	// load PIO program
	QVGAOff = pio_add_program(QVGA_PIO, &qvga_program);


	// configure GPIOs for use by PIO
	for (i = QVGA_GPIO_FIRST; i <= QVGA_GPIO_LAST; i++) pio_gpio_init(QVGA_PIO, i);
	pio_gpio_init(QVGA_PIO, QVGA_GPIO_HSYNC);
	pio_gpio_init(QVGA_PIO, QVGA_GPIO_VSYNC);

	// set pin direction to output
	pio_sm_set_consecutive_pindirs(QVGA_PIO, QVGA_SM, QVGA_GPIO_FIRST, QVGA_GPIO_NUM, true);
	pio_sm_set_consecutive_pindirs(QVGA_PIO, QVGA_SM, QVGA_GPIO_HSYNC, 2, true);

	// negate HSYNC and VSYNC output
	gpio_set_outover(QVGA_GPIO_HSYNC, GPIO_OVERRIDE_INVERT);
	gpio_set_outover(QVGA_GPIO_VSYNC, GPIO_OVERRIDE_INVERT);

	// prepare default PIO program config
	pio_sm_config cfg = qvga_program_get_default_config(QVGAOff);

	// map state machine's OUT and MOV pins	
	sm_config_set_out_pins(&cfg, QVGA_GPIO_FIRST, QVGA_GPIO_NUM);

	// set sideset pins (HSYNC and VSYNC)
	sm_config_set_sideset_pins(&cfg, QVGA_GPIO_HSYNC);

	// join FIFO to send only
	sm_config_set_fifo_join(&cfg, PIO_FIFO_JOIN_TX);

	// PIO clock divider
	sm_config_set_clkdiv(&cfg, QVGA_CLKDIV);

	// shift right, autopull, pull threshold
	sm_config_set_out_shift(&cfg, true, true, 32);

	// initialize state machine
	pio_sm_init(QVGA_PIO, QVGA_SM, QVGAOff+qvga_offset_entry, &cfg);
}

// initialize scanline buffers
void QVgaBufInit()
{
	// image scanline data buffer: HSYNC ... back porch ... image command
	ScanLineImg[0] = QVGACMD(qvga_offset_hsync, QVGA_HSYNC-3); // HSYNC
	ScanLineImg[1] = QVGACMD(qvga_offset_dark, QVGA_BP-4); // back porch
	ScanLineImg[2] = QVGACMD(qvga_offset_output, 640-2); // image

	// front porch
	ScanLineFp = QVGACMD(qvga_offset_dark, QVGA_FP-4); // front porch

	// dark scanline: HSYNC ... back porch + dark + front porch
	ScanLineDark[0] = QVGACMD(qvga_offset_hsync, QVGA_HSYNC-3); // HSYNC
	ScanLineDark[1] = QVGACMD(qvga_offset_dark, QVGA_TOTAL-QVGA_HSYNC-4); // back porch + dark + front porch

	// vertical sync: VHSYNC ... VSYNC(back porch + dark + front porch)
	ScanLineSync[0] = QVGACMD(qvga_offset_vhsync, QVGA_HSYNC-3); // VHSYNC
	ScanLineSync[1] = QVGACMD(qvga_offset_vsync, QVGA_TOTAL-QVGA_HSYNC-3); // VSYNC(back porch + dark + front porch)

	// control buffer 1 - initialize to VSYNC
	ScanLineCB[0] = 2; // send 2x uint32_t (send ScanLineSync)
	ScanLineCB[1] = (uint32_t)&ScanLineSync[0]; // VSYNC data buffer
	ScanLineCB[2] = 0; // stop mark
	ScanLineCB[3] = 0; // stop mark

	// control buffer 1 - initialize to VSYNC
	ScanLineCB[CB_MAX+0] = 2; // send 2x uint32_t (send ScanLineSync)
	ScanLineCB[CB_MAX+1] = (uint32_t)&ScanLineSync[0]; // VSYNC data buffer
	ScanLineCB[CB_MAX+2] = 0; // stop mark
	ScanLineCB[CB_MAX+3] = 0; // stop mark
}

// initialize QVGA DMA
//   control blocks aliases:
//                  +0x0        +0x4          +0x8          +0xC (Trigger)
// 0x00 (alias 0):  READ_ADDR   WRITE_ADDR    TRANS_COUNT   CTRL_TRIG
// 0x10 (alias 1):  CTRL        READ_ADDR     WRITE_ADDR    TRANS_COUNT_TRIG
// 0x20 (alias 2):  CTRL        TRANS_COUNT   READ_ADDR     WRITE_ADDR_TRIG
// 0x30 (alias 3):  CTRL        WRITE_ADDR    TRANS_COUNT   READ_ADDR_TRIG ... we use this!
void QVgaDmaInit()
{

// ==== prepare DMA control channel
    dma_channel_claim (QVGA_DMA_CB);
	// prepare DMA default config
	dma_channel_config cfg = dma_channel_get_default_config(QVGA_DMA_CB);

	// increment address on read from memory
	channel_config_set_read_increment(&cfg, true);

	// increment address on write to DMA port
	channel_config_set_write_increment(&cfg, true);

	// each DMA transfered entry is 32-bits
	channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32);

	// write ring - wrap to 8-byte boundary (TRANS_COUNT and READ_ADDR_TRIG of data DMA)
	channel_config_set_ring(&cfg, true, 3);

	// DMA configure
	dma_channel_configure(
		QVGA_DMA_CB,		// channel
		&cfg,			// configuration
		&dma_hw->ch[QVGA_DMA_PIO].al3_transfer_count, // write address
		&ScanLineCB[0],		// read address - as first, control buffer 1 will be sent out
		2,			// number of transfers in uint32_t (number of transfers per one request from data DMA)
		false			// do not start yet
	);

// ==== prepare DMA data channel

	// prepare DMA default config
	cfg = dma_channel_get_default_config(QVGA_DMA_PIO);

	// increment address on read from memory
	channel_config_set_read_increment(&cfg, true);

	// do not increment address on write to PIO
	channel_config_set_write_increment(&cfg, false);

	// each DMA transfered entry is 32-bits
	channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32);

	// DMA data request for sending data to PIO
	channel_config_set_dreq(&cfg, pio_get_dreq(QVGA_PIO, QVGA_SM, true));

	// chain channel to DMA control block
	channel_config_set_chain_to(&cfg, QVGA_DMA_CB);

	// raise the IRQ flag when 0 is written to a trigger register (end of chain)
	channel_config_set_irq_quiet(&cfg, true);

	// set high priority
	cfg.ctrl |= DMA_CH0_CTRL_TRIG_HIGH_PRIORITY_BITS;

	// DMA configure
	dma_channel_configure(
		QVGA_DMA_PIO,		// channel
		&cfg,			// configuration
		&QVGA_PIO->txf[QVGA_SM], // write address
		NULL,			// read address
		0,			// number of transfers in uint32_t
		false			// do not start immediately
	);

// ==== initialize IRQ0, raised from DMA data channel

	// enable DMA channel IRQ0
	dma_channel_set_irq0_enabled(QVGA_DMA_PIO, true);

	// set DMA IRQ handler
    irq_set_exclusive_handler(DMA_IRQ_0, QVgaLine0);
	// set highest IRQ priority
	irq_set_priority(DMA_IRQ_0, 0);
}

// initialize QVGA (can change system clock)
void QVgaInit()
{
	// initialize PIO
	QVgaPioInit();

	// initialize scanline buffers
	QVgaBufInit();

	// initialize DMA
	QVgaDmaInit();

	// initialize parameters
	QVgaScanLine = 0; // currently processed scanline
	QVgaBufInx = 0; // at first, control buffer 1 will be sent out
	QVgaFrame = 0; // current frame
	ScanLineCBNext = &ScanLineCB[CB_MAX]; // send control buffer 2 next

	// enable DMA IRQ
	irq_set_enabled(DMA_IRQ_0, true);

	// start DMA
	dma_channel_start(QVGA_DMA_CB);

	// run state machine
	pio_sm_set_enabled(QVGA_PIO, QVGA_SM, true);
}

void (* volatile Core1Fnc)() = NULL; // core 1 remote function

// QVGA core
void __not_in_flash_func(QVgaCore)()
{
	// initialize QVGA
	QVgaInit();

	// infinite loop
	while (true)
	{
		// data memory barrier
		__dmb();
        if (multicore_fifo_rvalid()) {
            int command=multicore_fifo_pop_blocking();
            if(command==0x5555){
                irq_set_enabled(DMA_IRQ_0, false);
            };
            if(command==0xAAAA){
                irq_set_enabled(DMA_IRQ_0, true);
            }
        } 
    }
}
uint32_t core1stack[64];
#endif
int main(){
    static int ErrorInPrompt;
    repeating_timer_t timer;
    int i;
    LoadOptions();
    if(  Option.Baudrate == 0 ||
        !(Option.Tab==2 || Option.Tab==3 || Option.Tab==4 ||Option.Tab==8) ||
        !(Option.Autorun>=0 && Option.Autorun<=MAXFLASHSLOTS+1) ||
        !(Option.Magic==MagicKey)
        ){
        ResetAllFlash();              // init the options if this is the very first startup
        _excep_code=0;
        SoftReset();
    }
    m_alloc(M_PROG);                                           // init the variables for program memory
    busy_wait_ms(100);
    if(Option.CPU_Speed>200000)vreg_set_voltage(VREG_VOLTAGE_1_20);
    busy_wait_ms(100);
    set_sys_clock_khz(Option.CPU_Speed, true);
    pico_get_unique_board_id_string (id_out,12);
    clock_configure(
        clk_peri,
        0,                                                // No glitchless mux
        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, // System PLL on AUX mux
        Option.CPU_Speed * 1000,                               // Input frequency
        Option.CPU_Speed * 1000                                // Output (must be same as no divider)
    );
    systick_hw->csr = 0x5;
    systick_hw->rvr = 0x00FFFFFF;
    busy_wait_ms(100);
    if(Option.CPU_Speed==252000)QVGA_CLKDIV=(Option.DISPLAY_TYPE == COLOURVGA ? 2	: 2);
    else QVGA_CLKDIV=(Option.DISPLAY_TYPE  == COLOURVGA ? 1	: 1);
    ticks_per_second = Option.CPU_Speed*1000;
    // The serial clock won't vary from this point onward, so we can configure
    // the UART etc.
    stdio_set_translate_crlf(&stdio_usb, false);
    LoadOptions();
	stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(true);
    add_repeating_timer_us(-1000, timer_callback, NULL, &timer);
    if(Option.SerialConsole==0) while (!tud_cdc_connected() && mSecTimer<5000) {}
    InitReservedIO();
    initKeyboard();
    ClearExternalIO();
    ConsoleRxBufHead = 0;
    ConsoleRxBufTail = 0;
    ConsoleTxBufHead = 0;
    ConsoleTxBufTail = 0;
    InitHeap();              										// initilise memory allocation
    uSecFunc(10);
    DISPLAY_TYPE = Option.DISPLAY_TYPE;
    // negative timeout means exact delay (rather than delay between callbacks)
	OptionErrorSkip = false;
	InitBasic();
#ifndef PICOMITEVGA
    InitDisplaySSD();
    InitDisplaySPI(0);
    InitDisplayI2C(0);
    InitTouch();
#endif
    ResetDisplay();
    ErrorInPrompt = false;
    exception_set_exclusive_handler(HARDFAULT_EXCEPTION,sigbus);
    while((i=getConsole())!=-1){}
#ifdef PICOMITEVGA
    multicore_launch_core1_with_stack(QVgaCore,core1stack,256);
	memset(FrameBuf, 0, 38400);
    if(Option.DISPLAY_TYPE!=MONOVGA)ClearScreen(Option.DefaultBC);
#endif
    if(!(_excep_code == RESTART_NOAUTORUN || _excep_code == WATCHDOG_TIMEOUT)){
        if(Option.Autorun==0 ){
            if(!(_excep_code == RESET_COMMAND))MMPrintString(MES_SIGNON); //MMPrintString(b);                                 // print sign on message
        } else {
            if(Option.Autorun!=MAXFLASHSLOTS+1){
                ProgMemory=(char *)(flash_target_contents+(Option.Autorun-1)*MAX_PROG_SIZE);
            }
            if(*ProgMemory != 0x01 ) MMPrintString(MES_SIGNON); 
        }
    }
    if(_excep_code == WATCHDOG_TIMEOUT) {
        WatchdogSet = true;                                 // remember if it was a watchdog timeout
        MMPrintString("\r\n\nWatchdog timeout\r\n");
    }
    if(noRTC){
        noRTC=0;
        Option.RTC=0;
        SaveOptions();
        MMPrintString("RTC not found, OPTION RTC AUTO disabled\r\n");
    }
 	*tknbuf = 0;
     ContinuePoint = nextstmt;                               // in case the user wants to use the continue command
	if(setjmp(mark) != 0) {
     // we got here via a long jump which means an error or CTRL-C or the program wants to exit to the command prompt
        ScrewUpTimer = 0;
        ProgMemory=(uint8_t *)flash_progmemory;
        ContinuePoint = nextstmt;                               // in case the user wants to use the continue command
		*tknbuf = 0;											// we do not want to run whatever is in the token buffer
    } else {
        if(*ProgMemory == 0x01 ) ClearVars(0);
        else {
            ClearProgram();
        }
        PrepareProgram(true);
        if(FindSubFun("MM.STARTUP", 0) >= 0) ExecuteProgram("MM.STARTUP\0");
        if(Option.Autorun && _excep_code != RESTART_DOAUTORUN) {
            ClearRuntime();
            PrepareProgram(true);
            if(*ProgMemory == 0x01 ){
                ExecuteProgram(ProgMemory);  
            }  else {
                Option.Autorun=0;
                SaveOptions();
            }       
        }
    }
    while(1) {
    if(Option.DISPLAY_CONSOLE) {
        SetFont(PromptFont);
        gui_fcolour = PromptFC;
        gui_bcolour = PromptBC;
        if(CurrentX != 0) MMPrintString("\r\n");                    // prompt should be on a new line
    }
        MMAbort = false;
        BreakKey = BREAK_KEY;
        EchoOption = true;
        LocalIndex = 0;                                             // this should not be needed but it ensures that all space will be cleared
        ClearTempMemory();                                          // clear temp string space (might have been used by the prompt)
        CurrentLinePtr = NULL;                                      // do not use the line number in error reporting
        if(MMCharPos > 1) MMPrintString("\r\n");                    // prompt should be on a new line
        while(Option.PIN && !IgnorePIN) {
            _excep_code = PIN_RESTART;
            if(Option.PIN == 99999999)                              // 99999999 is permanent lockdown
                MMPrintString("Console locked, press enter to restart: ");
            else
                MMPrintString("Enter PIN or 0 to restart: ");
            MMgetline(0, inpbuf);
            if(Option.PIN == 99999999) SoftReset();
            if(*inpbuf != 0) {
                uSec(3000000);
                i = atoi(inpbuf);
                if(i == 0) SoftReset();
                if(i == Option.PIN) {
                    IgnorePIN = true;
                    break;
                }
            }
        }
        _excep_code = 0;
        PrepareProgram(false);
        if(!ErrorInPrompt && FindSubFun("MM.PROMPT", 0) >= 0) {
            ErrorInPrompt = true;
            ExecuteProgram("MM.PROMPT\0");
        } else
            MMPrintString("> ");                                    // print the prompt
        ErrorInPrompt = false;
        EditInputLine();
        InsertLastcmd(inpbuf);                                  // save in case we want to edit it later
//        MMgetline(0, inpbuf);                                       // get the input
        if(!*inpbuf) continue;                                      // ignore an empty line
	  char *p=inpbuf;
	  skipspace(p);
	  if(*p=='*'){ //shortform RUN command so convert to a normal version
		  memmove(&p[4],&p[0],strlen(p)+1);
		  p[0]='R';p[1]='U';p[2]='N';p[3]='$';p[4]=34;
		  char  *q;
		  if((q=strchr(p,' ')) != 0){ //command line after the filename
			  *q=','; //chop the command at the first space character
			  memmove(&q[1],&q[0],strlen(q)+1);
			  q[0]=34;
		  } else strcat(p,"\"");
		  p[3]=' ';
//		  PRet();MMPrintString(inpbuf);PRet();
	  }
        tokenise(true);                                             // turn into executable code
        if (setjmp(jmprun) != 0) {
            PrepareProgram(false);
            CurrentLinePtr = 0;
        }
        ExecuteProgram(tknbuf);                                     // execute the line straight away
        memset(inpbuf,0,STRINGSIZE);
	}
}

// takes a pointer to RAM containing a program (in clear text) and writes it to memory in tokenised format
void SaveProgramToFlash(unsigned char *pm, int msg) {
    unsigned char *p, endtoken, fontnbr, prevchar = 0, buf[STRINGSIZE];
    int nbr, i, j, n, SaveSizeAddr;
    uint32_t storedupdates[MAXCFUNCTION], updatecount=0, realflashsave, retvalue;
    memcpy(buf, tknbuf, STRINGSIZE);                                // save the token buffer because we are going to use it
    FlashWriteInit(PROGRAM_FLASH);
    flash_range_erase(realflashpointer, MAX_PROG_SIZE);
    j=MAX_PROG_SIZE/4;
    int *pp=(int *)(flash_progmemory);
        while(j--)if(*pp++ != 0xFFFFFFFF){
            enable_interrupts();
            error("Flash erase problem");
        }
    nbr = 0;
    // this is used to count the number of bytes written to flash
    while(*pm) {
        p = inpbuf;
        while(!(*pm == 0 || *pm == '\r' || (*pm == '\n' && prevchar != '\r'))) {
            if(*pm == TAB) {
                do {*p++ = ' ';
                    if((p - inpbuf) >= MAXSTRLEN) goto exiterror;
                } while((p - inpbuf) % 2);
            } else {
                if(isprint((uint8_t)*pm)) {
                    *p++ = *pm;
                    if((p - inpbuf) >= MAXSTRLEN) goto exiterror;
                }
            }
            prevchar = *pm++;
        }
        if(*pm) prevchar = *pm++;                                   // step over the end of line char but not the terminating zero
        *p = 0;                                                     // terminate the string in inpbuf

        if(*inpbuf == 0 && (*pm == 0 || (!isprint((uint8_t)*pm) && pm[1] == 0))) break; // don't save a trailing newline

        tokenise(false);                                            // turn into executable code
        p = tknbuf;
        while(!(p[0] == 0 && p[1] == 0)) {
            FlashWriteByte(*p++); nbr++;

            if((int)((char *)realflashpointer - (uint32_t)PROGSTART) >= Option.PROG_FLASH_SIZE - 5)  goto exiterror;
        }
        FlashWriteByte(0); nbr++;                              // terminate that line in flash
    }
    FlashWriteByte(0);
    FlashWriteAlign();                                            // this will flush the buffer and step the flash write pointer to the next word boundary
    // now we must scan the program looking for CFUNCTION/CSUB/DEFINEFONT statements, extract their data and program it into the flash used by  CFUNCTIONs
     // programs are terminated with two zero bytes and one or more bytes of 0xff.  The CFunction area starts immediately after that.
     // the format of a CFunction/CSub/Font in flash is:
     //   Unsigned Int - Address of the CFunction/CSub in program memory (points to the token representing the "CFunction" keyword) or NULL if it is a font
     //   Unsigned Int - The length of the CFunction/CSub/Font in bytes including the Offset (see below)
     //   Unsigned Int - The Offset (in words) to the main() function (ie, the entry point to the CFunction/CSub).  Omitted in a font.
     //   word1..wordN - The CFunction/CSub/Font code
     // The next CFunction/CSub/Font starts immediately following the last word of the previous CFunction/CSub/Font
    int firsthex=1;
    realflashsave= realflashpointer;
    p = (char *)flash_progmemory;                                              // start scanning program memory
    while(*p != 0xff) {
    	nbr++;
        if(*p == 0) p++;                                            // if it is at the end of an element skip the zero marker
        if(*p == 0) break;                                          // end of the program
        if(*p == T_NEWLINE) {
            CurrentLinePtr = p;
            p++;                                                    // skip the newline token
        }
        if(*p == T_LINENBR) p += 3;                                 // step over the line number

        skipspace(p);
        if(*p == T_LABEL) {
            p += p[1] + 2;                                          // skip over the label
            skipspace(p);                                           // and any following spaces
        }
        if(*p == cmdCSUB || *p == GetCommandValue("DefineFont")) {      // found a CFUNCTION, CSUB or DEFINEFONT token
            if(*p == GetCommandValue("DefineFont")) {
             endtoken = GetCommandValue("End DefineFont");
             p++;                                                // step over the token
             skipspace(p);
             if(*p == '#') p++;
             fontnbr = getint(p, 1, FONT_TABLE_SIZE);
                                                 // font 6 has some special characters, some of which depend on font 1
             if(fontnbr == 1 || fontnbr == 6 || fontnbr == 7) {
                enable_interrupts();
                error("Cannot redefine fonts 1, 6 or 7");
             }
             realflashpointer+=4;
             skipelement(p);                                     // go to the end of the command
             p--;
            } else {
                endtoken = GetCommandValue("End CSub");
                realflashpointer+=4;
                fontnbr = 0;
                firsthex=0;
            }
             SaveSizeAddr = realflashpointer;                                // save where we are so that we can write the CFun size in here
             realflashpointer+=4;
             p++;
             skipspace(p);
             if(!fontnbr) {
                 if(!isnamestart((uint8_t)*p)){
                    error("Function name");
                    enable_interrupts();
                 }  
                 do { p++; } while(isnamechar((uint8_t)*p));
                 skipspace(p);
                 if(!(isxdigit((uint8_t)p[0]) && isxdigit((uint8_t)p[1]) && isxdigit((uint8_t)p[2]))) {
                     skipelement(p);
                     p++;
                    if(*p == T_NEWLINE) {
                        CurrentLinePtr = p;
                        p++;                                        // skip the newline token
                    }
                    if(*p == T_LINENBR) p += 3;                     // skip over a line number
                 }
             }
             do {
                 while(*p && *p != '\'') {
                     skipspace(p);
                     n = 0;
                     for(i = 0; i < 8; i++) {
                         if(!isxdigit((uint8_t)*p)) {
                            enable_interrupts();
                            error("Invalid hex word");
                         }
                         if((int)((char *)realflashpointer - (uint32_t)PROGSTART) >= Option.PROG_FLASH_SIZE - 5) goto exiterror;
                         n = n << 4;
                         if(*p <= '9')
                             n |= (*p - '0');
                         else
                             n |= (toupper(*p) - 'A' + 10);
                         p++;
                     }
                     realflashpointer+=4;
                     skipspace(p);
                     if(firsthex){
                    	 firsthex=0;
                    	 if(((n>>16) & 0xff) < 0x20){
                            enable_interrupts();
                            error("Can't define non-printing characters");
                         }
                     }
                 }
                 // we are at the end of a embedded code line
                 while(*p) p++;                                      // make sure that we move to the end of the line
                 p++;                                                // step to the start of the next line
                 if(*p == 0) {
                     enable_interrupts();
                     error("Missing END declaration");
                 }
                 if(*p == T_NEWLINE) {
                     CurrentLinePtr = p;
                     p++;                                            // skip the newline token
                 }
                 if(*p == T_LINENBR) p += 3;                         // skip over the line number
                 skipspace(p);
             } while(*p != endtoken);
             storedupdates[updatecount++]=realflashpointer - SaveSizeAddr - 4;
         }
         while(*p) p++;                                              // look for the zero marking the start of the next element
     }
    realflashpointer = realflashsave ;
    updatecount=0;
    p = (char *)flash_progmemory;                                              // start scanning program memory
     while(*p != 0xff) {
     	nbr++;
         if(*p == 0) p++;                                            // if it is at the end of an element skip the zero marker
         if(*p == 0) break;                                          // end of the program
         if(*p == T_NEWLINE) {
             CurrentLinePtr = p;
             p++;                                                    // skip the newline token
         }
         if(*p == T_LINENBR) p += 3;                                 // step over the line number

         skipspace(p);
         if(*p == T_LABEL) {
             p += p[1] + 2;                                          // skip over the label
             skipspace(p);                                           // and any following spaces
         }
         if(*p == cmdCSUB || *p == GetCommandValue("DefineFont")) {      // found a CFUNCTION, CSUB or DEFINEFONT token
         if(*p == GetCommandValue("DefineFont")) {      // found a CFUNCTION, CSUB or DEFINEFONT token
             endtoken = GetCommandValue("End DefineFont");
             p++;                                                // step over the token
             skipspace(p);
             if(*p == '#') p++;
             fontnbr = getint(p, 1, FONT_TABLE_SIZE);
                                                 // font 6 has some special characters, some of which depend on font 1
             if(fontnbr == 1 || fontnbr == 6 || fontnbr == 7) {
                 enable_interrupts();
                 error("Cannot redefine fonts 1, 6, or 7");
             }

             FlashWriteWord(fontnbr - 1);             // a low number (< FONT_TABLE_SIZE) marks the entry as a font
             skipelement(p);                                     // go to the end of the command
             p--;
         } else {
             endtoken = GetCommandValue("End CSub");
             FlashWriteWord((unsigned int)(p-flash_progmemory));               // if a CFunction/CSub save a relative pointer to the declaration
             fontnbr = 0;
         }
            SaveSizeAddr = realflashpointer;                                // save where we are so that we can write the CFun size in here
             FlashWriteWord(storedupdates[updatecount++]);                        // leave this blank so that we can later do the write
             p++;
             skipspace(p);
             if(!fontnbr) {
                 if(!isnamestart((uint8_t)*p))  {
                     enable_interrupts();
                     error("Function name");
                 }
                 do { p++; } while(isnamechar(*p));
                 skipspace(p);
                 if(!(isxdigit(p[0]) && isxdigit(p[1]) && isxdigit(p[2]))) {
                     skipelement(p);
                     p++;
                    if(*p == T_NEWLINE) {
                        CurrentLinePtr = p;
                        p++;                                        // skip the newline token
                    }
                    if(*p == T_LINENBR) p += 3;                     // skip over a line number
                 }
             }
             do {
                 while(*p && *p != '\'') {
                     skipspace(p);
                     n = 0;
                     for(i = 0; i < 8; i++) {
                         if(!isxdigit(*p)) {
                            enable_interrupts();
                            error("Invalid hex word");
                         }
                         if((int)((char *)realflashpointer - (uint32_t)PROGSTART) >= Option.PROG_FLASH_SIZE - 5) goto exiterror;
                         n = n << 4;
                         if(*p <= '9')
                             n |= (*p - '0');
                         else
                             n |= (toupper(*p) - 'A' + 10);
                         p++;
                     }

                     FlashWriteWord(n);
                     skipspace(p);
                 }
                 // we are at the end of a embedded code line
                 while(*p) p++;                                      // make sure that we move to the end of the line
                 p++;                                                // step to the start of the next line
                 if(*p == 0) {
                    enable_interrupts();
                    error("Missing END declaration");
                 }
                 if(*p == T_NEWLINE) {
                    CurrentLinePtr = p;
                    p++;                                        // skip the newline token
                 }
                 if(*p == T_LINENBR) p += 3;                     // skip over a line number
                 skipspace(p);
             } while(*p != endtoken);
         }
         while(*p) p++;                                              // look for the zero marking the start of the next element
     }
     FlashWriteWord(0xffffffff);                                // make sure that the end of the CFunctions is terminated with an erased word
     FlashWriteClose();                                              // this will flush the buffer and step the flash write pointer to the next word boundary
    if(msg) {                                                       // if requested by the caller, print an informative message
        if(MMCharPos > 1) MMPrintString("\r\n");                    // message should be on a new line
        MMPrintString("Saved ");
        IntToStr(tknbuf, nbr + 3, 10);
        MMPrintString(tknbuf);
        MMPrintString(" bytes\r\n");
    }
    memcpy(tknbuf, buf, STRINGSIZE);                                // restore the token buffer in case there are other commands in it
//    initConsole();
    enable_interrupts();
    return;

    // we only get here in an error situation while writing the program to flash
    exiterror:
        FlashWriteByte(0); FlashWriteByte(0); FlashWriteByte(0);    // terminate the program in flash
        FlashWriteClose();
        error("Not enough memory");
}


#ifdef __cplusplus
}
#endif

/// \end:uart_advanced[]
