/***********************************************************************************************************************
PicoMite MMBasic

External.h

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



/**********************************************************************************
 the C language function associated with commands, functions or operators should be
 declared here
**********************************************************************************/
#if !defined(INCLUDE_COMMAND_TABLE) && !defined(INCLUDE_TOKEN_TABLE)
// format:
//      void cmd_???(void)
//      void fun_???(void)
//      void op_???(void)

int getinttblpos(int tbl[],int pin);
void cmd_setpin(void);
void cmd_pulse(void);
void cmd_pwm(void);
void cmd_pin(void);
void fun_pin(void);

void cmd_port(void);
void fun_port(void);
void cmd_adc(void);
void cmd_ir(void);
void cmd_lcd(unsigned char *p);
void cmd_keypad(void);
void fun_distance(void);
void fun_pulsin(void);
void cmd_backlight(void);
void cmd_bitbang(void);
#endif



/**********************************************************************************
 All command tokens tokens (eg, PRINT, FOR, etc) should be inserted in this table
**********************************************************************************/
#ifdef INCLUDE_COMMAND_TABLE
// the format is:
//    TEXT      	TYPE                P  FUNCTION TO CALL
// where type is always T_CMD
// and P is the precedence (which is only used for operators and not commands)

  { (unsigned char *)"ADC",		T_CMD,			0, cmd_adc        },
  { (unsigned char *)"Pin(",		T_CMD | T_FUN,		0, cmd_pin          },
	{ (unsigned char *)"SetPin",		T_CMD,			0, cmd_setpin       },
	{ (unsigned char *)"Pulse",		T_CMD,			0, cmd_pulse        },
	{ (unsigned char *)"Port(",		T_CMD | T_FUN,		0, cmd_port	    },
	{ (unsigned char *)"IR",                 T_CMD,			0, cmd_ir           },
	{ (unsigned char *)"KeyPad",             T_CMD,			0, cmd_keypad       },
	{ (unsigned char *)"Bitbang",              T_CMD,			0, cmd_bitbang        },
	{ (unsigned char *)"PWM",		T_CMD,		0, cmd_pwm		},
#ifndef PICOMITEVGA
	{ (unsigned char *)"Backlight",		T_CMD,		0, cmd_backlight		},
#endif
#endif


/**********************************************************************************
 All other tokens (keywords, functions, operators) should be inserted in this table
**********************************************************************************/
#ifdef INCLUDE_TOKEN_TABLE
// the format is:
//    TEXT      	TYPE                P  FUNCTION TO CALL
// where type is T_NA, T_FUN, T_FNA or T_OPER argumented by the types T_STR and/or T_NBR
// and P is the precedence (which is only used for operators)
	{ (unsigned char *)"Pin(",		T_FUN | T_NBR | T_INT,	0, fun_pin		},
	{ (unsigned char *)"Port(",		T_FUN | T_INT,		0, fun_port		},
	{ (unsigned char *)"Distance(",		T_FUN | T_NBR,		0, fun_distance		},
	{ (unsigned char *)"Pulsin(",		T_FUN | T_INT,		0, fun_pulsin		},

#endif


#if !defined(INCLUDE_COMMAND_TABLE) && !defined(INCLUDE_TOKEN_TABLE)
// General definitions used by other modules

#ifndef EXTERNAL_HEADER
#define EXTERNAL_HEADER

#define NBR_PULSE_SLOTS     5                       // number of concurrent pulse commands, each entry is 8 bytes


extern unsigned char *InterruptReturn;
extern int check_interrupt(void);
extern void ClearExternalIO(void);


/****************************************************************************************************************************
New, more portable, method of manipulating an I/O pin
*****************************************************************************************************************************/

// the basic functions
// set or clear bits in the pin's sfr registers

// some useful defines
////#define PinOpenCollectorOn(x)   PinSetBit(x, ODCSET)
#define PinOpenCollectorOff(x)  PinSetBit(x, ODCCLR)
#define PinHigh(x)              PinSetBit(x, LATSET)
#define PinLow(x)               PinSetBit(x, LATCLR)
#define PinSetOutput(x)         PinSetBit(x, TRISCLR)
#define PinSetInput(x)          PinSetBit(x, TRISSET)

// Define the offsets from the PORT address
#define ANSEL               -8
#define ANSELCLR            -7
#define ANSELSET            -6
#define ANSELINV            -5
#define TRIS                -4
#define TRISCLR             -3
#define TRISSET             -2
#define TRISINV             -1
//#define PORT                0

#define PORTCLR             1
#define PORTSET             2
#define PORTINV             3
#define LAT                 4
#define LATCLR              5
#define LATSET              6
#define LATINV              7
#define ODC                 8
#define ODCCLR              9
#define ODCSET              10
#define ODCINV              11
#define CNPU                12
#define CNPUCLR             13
#define CNPUSET             14
#define CNPUINV             15
#define CNPD                16
#define CNPDCLR             17
#define CNPDSET             18
#define CNPDINV             19
#define CNCON               20
#define CNCONCLR            21
#define CNCONSET            22
#define CNCONINV            23
#define CNEN                24
#define CNENCLR             25
#define CNENSET             26
#define CNENINV             27
#define CNSTAT              28
#define CNSTATCLR           29
#define CNSTATSET           30
#define CNSTATINV           31

#define EXT_NOT_CONFIG          0
#define EXT_ANA_IN				1
#define EXT_DIG_IN				2
#define EXT_FREQ_IN				3
#define EXT_PER_IN				4
#define EXT_CNT_IN				5
#define EXT_INT_HI				6
#define EXT_INT_LO				7
#define EXT_DIG_OUT				8
#define EXT_HEARTBEAT			9
#define EXT_INT_BOTH	  10
#define EXT_UART0TX			11
#define EXT_UART0RX			12
#define EXT_UART1TX			13
#define EXT_UART1RX			14
#define EXT_I2C0SDA			15
#define EXT_I2C0SCL			16
#define EXT_I2C1SDA			17
#define EXT_I2C1SCL			18
#define EXT_SPI0RX			19
#define EXT_SPI0TX			20
#define EXT_SPI0SCK			21
#define EXT_SPI1RX			22
#define EXT_SPI1TX			23
#define EXT_SPI1SCK			24
#define EXT_IR          25
#define EXT_INT1        26
#define EXT_INT2        27
#define EXT_INT3        28
#define EXT_INT4        29
#define EXT_PWM0A        30
#define EXT_PWM0B        31
#define EXT_PWM1A        32
#define EXT_PWM1B        33
#define EXT_PWM2A        34
#define EXT_PWM2B        35
#define EXT_PWM3A        36
#define EXT_PWM3B        37
#define EXT_PWM4A        38
#define EXT_PWM4B        39
#define EXT_PWM5A        40
#define EXT_PWM5B        41
#define EXT_PWM6A        42
#define EXT_PWM6B        43
#define EXT_PWM7A        44
#define EXT_PWM7B        45
#define EXT_PIO0_OUT      46
#define EXT_PIO1_OUT      47
#define EXT_DS18B20_RESERVED    0x100                 // this pin is reserved for DS18B20 and cannot be used
#define EXT_COM_RESERVED        0x200                 // this pin is reserved and SETPIN and PIN cannot be used
#define EXT_BOOT_RESERVED       0x400                 // this pin is reserved at bootup and cannot be used
extern const char *PinFunction[];
extern volatile int ExtCurrentConfig[NBRPINS + 1];
extern volatile int INT0Count, INT0Value, INT0InitTimer, INT0Timer;
extern volatile int INT1Count, INT1Value, INT1InitTimer, INT1Timer;
extern volatile int INT2Count, INT2Value, INT2InitTimer, INT2Timer;
extern volatile int INT3Count, INT3Value, INT3InitTimer, INT3Timer;
extern volatile int INT4Count, INT4Value, INT4InitTimer, INT4Timer;
extern volatile uint64_t INT5Count, INT5Value, INT5InitTimer, INT5Timer;
extern void PinSetBit(int pin, unsigned int offset);
extern int PinRead(int pin);
extern volatile unsigned int GetPinStatus(int pin);
extern int GetPinBit(int pin);
extern void WriteCoreTimer(unsigned long timeset);
extern unsigned long ReadCoreTimer(void);
extern uint64_t readusclock(void);
extern void writeusclock(uint64_t timeset);
extern uint64_t readIRclock(void);
extern void writeIRclock(uint64_t timeset);
extern void initExtIO(void);
extern void ExtCfg(int pin, int cfg, int option) ;
extern void ExtSet(int pin, int val);
extern int64_t ExtInp(int pin);
extern int IsInvalidPin(int pin);
extern int p100interrupts[];
extern unsigned long ReadCount5(void);
extern void WriteCount5(unsigned long timeset);
extern void SoftReset(void);
extern volatile uint64_t IRoffset;
extern int BacklightSlice,BacklightChannel;

void gpio_callback(uint gpio, uint32_t events);
// for CheckPin() action can be set to:
#define CP_CHECKALL          0b0000     // abort with an error if invalid, in use or reserved
#define CP_NOABORT           0b0001     // the function will not abort with an error
#define CP_IGNORE_INUSE      0b0010     // the function will ignore pins that are in use (but not including reserved pins)
#define CP_IGNORE_RESERVED   0b0100     // the function will ignore reserved pins (EXT_COM_RESERVED and EXT_BOOT_RESERVED)
#define CP_IGNORE_BOOTRES    0b1000     // the function will ignore the boot reserved pins (EXT_BOOT_RESERVED)
#define setuptime (12-(Option.CPU_Speed-250000)/50000)
#define shortpause(a){systick_hw->cvr=0;while(systick_hw->cvr>a){};}
extern int CheckPin(int pin, int action);

extern int InterruptUsed;
typedef enum
{
  GPIO_Mode_IN   = 0x00, /*!< GPIO Input Mode */
  GPIO_Mode_OUT  = 0x01, /*!< GPIO Output Mode */
  GPIO_Mode_AF   = 0x02, /*!< GPIO Alternate function Mode */
  GPIO_Mode_AN   = 0x03  /*!< GPIO Analog Mode */
}GPIOMode_TypeDef;
typedef enum
{
  GPIO_PuPd_NOPULL = 0x00,
  GPIO_PuPd_UP     = 0x01,
  GPIO_PuPd_DOWN   = 0x02
}GPIOPuPd_TypeDef;
typedef enum
{
  GPIO_OType_PP = 0x0,
  GPIO_OType_OD = 0x1
}GPIOOType_TypeDef;
// IR related stuff
extern void *IrDev, *IrCmd;
extern volatile char IrVarType, IrState, IrGotMsg;
extern int IrBits, IrCount;
extern unsigned char *IrInterrupt;
extern int codemap(int pin);
extern int codecheck(unsigned char *line);
extern int PWM0Apin;
extern int PWM1Apin;
extern int PWM2Apin;
extern int PWM3Apin;
extern int PWM4Apin;
extern int PWM5Apin;
extern int PWM6Apin;
extern int PWM7Apin;
extern int PWM0Bpin;
extern int PWM1Bpin;
extern int PWM2Bpin;
extern int PWM3Bpin;
extern int PWM4Bpin;
extern int PWM5Bpin;
extern int PWM6Bpin;
extern int PWM7Bpin;
extern int UART1RXpin;
extern int UART1TXpin;
extern int UART0TXpin;
extern int UART0RXpin;
extern int SPI1TXpin;
extern int SPI1RXpin;
extern int SPI1SCKpin;
extern int SPI0TXpin;
extern int SPI0RXpin;
extern int SPI0SCKpin;
extern int I2C1SDApin;
extern int I2C1SCLpin;
extern int I2C0SDApin;
extern int I2C0SCLpin;
extern int slice0,slice1,slice2,slice3,slice4,slice5,slice6,slice7;
extern int SPI0locked;
extern int SPI1locked;
extern int I2C0locked;
extern int I2C1locked;
extern int CallBackEnabled;
extern int ADCopen;
extern volatile MMFLOAT * volatile a1float, * volatile a2float, * volatile a3float, * volatile a4float;
extern int ADCmax;
extern int dmarunning;
extern char *ADCInterrupt;
extern uint dma_chan;
extern short *ADCbuffer;
void IrInit(void);
void IrReset(void);
void IRSendSignal(int pin, int half_cycles);
void TM_EXTI_Handler_5(char *buf, uint32_t events);
// numpad declares
extern unsigned char *KeypadInterrupt;
int KeypadCheck(void);

#define IR_CLOSED                   0
#define IR_WAIT_START               1
#define IR_WAIT_START_END           2
#define SONY_WAIT_BIT_START         3
#define SONY_WAIT_BIT_END           4
#define NEC_WAIT_FIRST_BIT_START    5
#define NEC_WAIT_BIT_START          7
#define NEC_WAIT_BIT_END            8
#define elapsed             readusclock()


#endif
#endif

