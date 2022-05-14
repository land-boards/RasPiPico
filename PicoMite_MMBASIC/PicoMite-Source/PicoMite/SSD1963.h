/***********************************************************************************************************************
PicoMite MMBasic

SSD1963.h

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

#ifndef _SSD1963_H
    #define _SSD1963_H

    // parameters of the display panel (refer to the glass data sheet)
    extern int SSD1963HorizPulseWidth, SSD1963HorizBackPorch, SSD1963HorizFrontPorch;
    extern int SSD1963VertPulseWidth, SSD1963VertBackPorch, SSD1963VertFrontPorch;
    extern int SSD1963PClock1, SSD1963PClock2, SSD1963PClock3;
    extern int SSD1963Mode1, SSD1963Mode2;

    // define global functions
    extern void InitSSD1963(void);
    extern void SetBacklightSSD1963(int intensity);
    extern void SetTearingCfg(int state, int mode);
    extern void DrawBitmapSSD1963(int x1, int y1, int width, int height, int scale, int fg, int bg, unsigned char *bitmap);

    extern void DisplayPutC(char c);
    extern void DisplayPutS(char *);
    extern void DrawBufferSSD1963(int x1, int y1, int x2, int y2, unsigned char* p);
    extern void ReadBufferSSD1963(int x1, int y1, int x2, int y2, unsigned char* p);

    // cursor definition
    extern void ShowCursor(int show);
    extern volatile int CursorTimer;                                // used to time the flashing cursor
    extern volatile int ClickTimer;                                 // used to time the click when touch occurs
    extern volatile int TouchTimer;                                 // used to time the response to touch

    #define GPIO3       3
    #define GPIO2       2
    #define GPIO1       1
    #define GPIO0       0
    #define LCD_RESET   (1<<GPIO0)                                  // LCD Reset signal (Reset for display panel, NOT ssd1963)
    #define LCD_SPENA   0
    #define LCD_SPCLK   0
    #define LCD_SPDAT   0

    #define SSD1963_LANDSCAPE       0b00
    #define SSD1963_PORTRAIT        ((1 << 7) | (1 << 5))
    #define SSD1963_RLANDSCAPE      (SSD1963_LANDSCAPE | 0b11)
    #define SSD1963_RPORTRAIT       (SSD1963_PORTRAIT | 0b11)

    // SSD1963 command table
    #define CMD_NOP               0x00    //No operation
    #define CMD_SOFT_RESET        0x01    //Software reset
    #define CMD_GET_PWR_MODE      0x0A    //Get the current power mode
    #define CMD_GET_ADDR_MODE     0x0B    //Get the frame memory to the display panel read order
    #define CMD_GET_PIXEL_FORMAT  0x0C    //Get the current pixel format
    #define CMD_GET_DISPLAY_MODE  0x0D    //Returns the display mode
    #define CMD_GET_SIGNAL_MODE   0x0E    //
    #define CMD_GET_DIAGNOSTIC    0x0F
    #define CMD_ENT_SLEEP         0x10
    #define CMD_EXIT_SLEEP        0x11
    #define CMD_ENT_PARTIAL_MODE  0x12
    #define CMD_ENT_NORMAL_MODE   0x13
    #define CMD_EXIT_INVERT_MODE  0x20
    #define CMD_ENT_INVERT_MODE   0x21
    #define CMD_SET_GAMMA         0x26
    #define CMD_BLANK_DISPLAY     0x28
    #define CMD_ON_DISPLAY        0x29
    #define CMD_SET_COLUMN        0x2A
    #define CMD_SET_PAGE          0x2B
    #define CMD_WR_MEMSTART       0x2C
    #define CMD_RD_MEMSTART       0x2E
    #define CMD_SET_PARTIAL_AREA  0x30
    #define CMD_SET_SCROLL_AREA   0x33
    #define CMD_SET_TEAR_OFF      0x34    //synchronization information is not sent from the display
    #define CMD_SET_TEAR_ON       0x35    //sync. information is sent from the display
    #define CMD_SET_ADDR_MODE     0x36    //set fram buffer read order to the display panel
    #define CMD_SET_SCROLL_START  0x37
    #define CMD_EXIT_IDLE_MODE    0x38
    #define CMD_ENT_IDLE_MODE     0x39
    #define CMD_SET_PIXEL_FORMAT  0x3A    //defines how many bits per pixel is used
    #define CMD_WR_MEM_AUTO       0x3C
    #define CMD_RD_MEM_AUTO       0x3E
    #define CMD_SET_TEAR_SCANLINE 0x44
    #define CMD_GET_SCANLINE      0x45
    #define CMD_RD_DDB_START      0xA1
    #define CMD_RD_DDB_AUTO       0xA8
    #define CMD_SET_PANEL_MODE    0xB0
    #define CMD_GET_PANEL_MODE    0xB1
    #define CMD_SET_HOR_PERIOD    0xB4
    #define CMD_GET_HOR_PERIOD    0xB5
    #define CMD_SET_VER_PERIOD    0xB6
    #define CMD_GET_VER_PERIOD    0xB7
    #define CMD_SET_GPIO_CONF     0xB8
    #define CMD_GET_GPIO_CONF     0xB9
    #define CMD_SET_GPIO_VAL      0xBA
    #define CMD_GET_GPIO_STATUS   0xBB
    #define CMD_SET_POST_PROC     0xBC
    #define CMD_GET_POST_PROC     0xBD
    #define CMD_SET_PWM_CONF      0xBE
    #define CMD_GET_PWM_CONF      0xBF
    #define CMD_SET_LCD_GEN0      0xC0
    #define CMD_GET_LCD_GEN0      0xC1
    #define CMD_SET_LCD_GEN1      0xC2
    #define CMD_GET_LCD_GEN1      0xC3
    #define CMD_SET_LCD_GEN2      0xC4
    #define CMD_GET_LCD_GEN2      0xC5
    #define CMD_SET_LCD_GEN3      0xC6
    #define CMD_GET_LCD_GEN3      0xC7
    #define CMD_SET_GPIO0_ROP     0xC8
    #define CMD_GET_GPIO0_ROP     0xC9
    #define CMD_SET_GPIO1_ROP     0xCA
    #define CMD_GET_GPIO1_ROP     0xCB
    #define CMD_SET_GPIO2_ROP     0xCC
    #define CMD_GET_GPIO2_ROP     0xCD
    #define CMD_SET_GPIO3_ROP     0xCE
    #define CMD_GET_GPIO3_ROP     0xCF
    #define CMD_SET_ABC_DBC_CONF  0xD0
    #define CMD_GET_ABC_DBC_CONF  0xD1
    #define CMD_SET_DBC_HISTO_PTR 0xD2
    #define CMD_GET_DBC_HISTO_PTR 0xD3
    #define CMD_SET_DBC_THRES     0xD4
    #define CMD_GET_DBC_THRES     0xD5
    #define CMD_SET_ABM_TMR       0xD6
    #define CMD_GET_ABM_TMR       0xD7
    #define CMD_SET_AMB_LVL0      0xD8
    #define CMD_GET_AMB_LVL0      0xD9
    #define CMD_SET_AMB_LVL1      0xDA
    #define CMD_GET_AMB_LVL1      0xDB
    #define CMD_SET_AMB_LVL2      0xDC
    #define CMD_GET_AMB_LVL2      0xDD
    #define CMD_SET_AMB_LVL3      0xDE
    #define CMD_GET_AMB_LVL3      0xDF
    #define CMD_PLL_START         0xE0    //start the PLL
    #define CMD_PLL_STOP          0xE1    //disable the PLL
    #define CMD_SET_PLL_MN        0xE2
    #define CMD_GET_PLL_MN        0xE3
    #define CMD_GET_PLL_STATUS    0xE4    //get the current PLL status
    #define CMD_ENT_DEEP_SLEEP    0xE5
    #define CMD_SET_PCLK          0xE6    //set pixel clock (LSHIFT signal) frequency
    #define CMD_GET_PCLK          0xE7    //get pixel clock (LSHIFT signal) freq. settings
    #define CMD_SET_DATA_INTERFACE    0xF0
    #define CMD_GET_DATA_INTERFACE    0xF1
    #define SSD1963_DC_PIN  17
    #define SSD1963_WR_PIN  19
    #define SSD1963_RD_PIN  20
    #define SSD1963_RESET_PIN 21
    #define SSD1963_DAT1    1
    #define SSD1963_DAT2    2
    #define SSD1963_DAT3    4
    #define SSD1963_DAT4    5
    #define SSD1963_DAT5    6
    #define SSD1963_DAT6    7
    #define SSD1963_DAT7    9
    #define SSD1963_DAT8    10
    #define SSD1963_DC_GPPIN  13
    #define SSD1963_WR_GPPIN  14
    #define SSD1963_RD_GPPIN  15
    #define SSD1963_RESET_GPPIN 16
    #define SSD1963_GPDAT1    0
    #define SSD1963_GPDAT2    1
    #define SSD1963_GPDAT3    2
    #define SSD1963_GPDAT4    3
    #define SSD1963_GPDAT5    4
    #define SSD1963_GPDAT6    5
    #define SSD1963_GPDAT7    6
    #define SSD1963_GPDAT8    7

    #define nop asm("NOP")


#endif