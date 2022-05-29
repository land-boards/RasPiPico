/***********************************************************************************************************************
PicoMite MMBasic

FileIO.h

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

************************************************************************************************************************/#ifndef __FILEIO_H
#define __FILEIO_H

#ifdef __cplusplus
extern "C" {
#endif

// File related I/O
unsigned char MMfputc(unsigned char c, int fnbr);
int MMfgetc(int filenbr);
void MMfopen(unsigned char *fname, unsigned char *mode, int fnbr);
int MMfeof(int filenbr);
void MMfclose(int fnbr);
int FindFreeFileNbr(void);
void CloseAllFiles(void);
void MMgetline(int filenbr, char *p);
void MMPrintString(char *s);
void CheckAbort(void);
char FileGetChar(int fnbr);
char FilePutChar(char c, int fnbr);
void CheckSDCard(void);
void LoadOptions(void);
void CrunchData(unsigned char **p, int c);
int FileEOF(int fnbr);
void ClearSavedVars(void);
int FileLoadProgram(unsigned char *fname);
void SaveOptions(void);
void ResetAllFlash(void);
void disable_interrupts(void);
void enable_interrupts(void);
void ErrorCheck(int fnbr);
extern int OptionFileErrorAbort;
struct option_s {
    int  Magic;
    char Autorun;
    char Tab;
    char Invert;
    char Listcase; //4
  //
    unsigned int PROG_FLASH_SIZE;
    unsigned int HEAP_SIZE;
    char Height;
    char Width;
    char DISPLAY_TYPE;
    char DISPLAY_ORIENTATION; //8
//
    int  PIN;
    int  Baudrate;
    int  ColourCode;
    int CPU_Speed; 
    unsigned int Dummyint;    // used to store the size of the program flash (also start of the LIBRARY code)
    int DefaultFC, DefaultBC;      // the default colours
    int DefaultBrightness;         // default backlight brightness //40
    uint16_t VGAFC, VGABC;      // the default colours
//
    // display related
    unsigned char DefaultFont;
    unsigned char KeyboardConfig;
    unsigned char RTC_Clock, RTC_Data; //44
//
    int MaxCtrls;                // maximum number of controls allowed //48
    // for the SPI LCDs
    unsigned char LCD_CD;
    unsigned char LCD_CS;
    unsigned char LCD_Reset;
    // touch related
    unsigned char TOUCH_CS;
    unsigned char TOUCH_IRQ;
    char TOUCH_SWAPXY; 
    unsigned char repeat;
    char dummy;//56
    int  TOUCH_XZERO;
    int  TOUCH_YZERO;
    float TOUCH_XSCALE;
    float TOUCH_YSCALE; //72
    unsigned int fullrefresh;
 
     // these are only used in the MX470 version
    unsigned char SD_CS;
    unsigned char SYSTEM_MOSI;
    unsigned char SYSTEM_MISO;
    unsigned char SYSTEM_CLK;
    unsigned char DISPLAY_BL;
    unsigned char DISPLAY_CONSOLE;
    unsigned char TOUCH_Click;
    char LCD_RD;                   // used for the RD pin for SSD1963  //78
    unsigned char AUDIO_L;
    unsigned char AUDIO_R;
    unsigned char AUDIO_SLICE;
    unsigned char pins[8];                  // general use storage for CFunctions written by PeterM //86
    unsigned char SDspeed;
    char LCDVOP;
    char I2Coffset;
    unsigned char E_INKbusy;
    char Refresh;
    unsigned char SYSTEM_I2C_SDA;
    unsigned char SYSTEM_I2C_SCL;
    unsigned char RTC;
    char PWM;
    unsigned char INT1pin;
    unsigned char INT2pin;
    unsigned char INT3pin;
    unsigned char INT4pin;
    unsigned char SD_CLK_PIN;
    unsigned char SD_MOSI_PIN;
    unsigned char SD_MISO_PIN;
    unsigned char SerialConsole;
    unsigned char SerialTX;
    unsigned char SerialRX;
    unsigned char numlock;
    unsigned char capslock;
    unsigned char F1key[MAXKEYLEN];
    unsigned char F5key[MAXKEYLEN];
    unsigned char F6key[MAXKEYLEN];
    unsigned char F7key[MAXKEYLEN];
    unsigned char F8key[MAXKEYLEN];
    unsigned char F9key[MAXKEYLEN];
    // To enable older CFunctions to run any new options *MUST* be added at the end of the list
} __attribute__((packed));
extern unsigned char *CFunctionFlash, *CFunctionLibrary;
extern struct option_s Option;
extern int FlashLoad;
extern void ResetOptions(void);
extern void FlashWriteBlock(void);
extern void FlashWriteWord(unsigned int i);
extern void FlashWriteByte(unsigned char b);
extern void FlashWriteAlign(void);
extern void FlashWriteClose(void);
extern void FlashWriteInit(int region);
extern volatile uint32_t realflashpointer;
#define SAVED_OPTIONS_FLASH 4
#define SAVED_VARS_FLASH 2
#define PROGRAM_FLASH 1

#ifdef __cplusplus
}
#endif
#endif /* __FILEIO_H */