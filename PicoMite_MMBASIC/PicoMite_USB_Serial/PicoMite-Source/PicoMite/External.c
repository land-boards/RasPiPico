/***********************************************************************************************************************
PicoMite MMBasic

External.c

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

************************************************************************************************************************/#include "MMBasic_Includes.h"
#include "Hardware_Includes.h"
#include "hardware/watchdog.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"
//#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/structs/systick.h"
#include "hardware/structs/adc.h"
#include "hardware/dma.h"
extern MMFLOAT FDiv(MMFLOAT a, MMFLOAT b);
extern MMFLOAT FMul(MMFLOAT a, MMFLOAT b);
extern MMFLOAT FSub(MMFLOAT a, MMFLOAT b);
const char *PinFunction[] = {	
        "OFF",
		"AIN",
		"DIN",
		"FIN",
		"PER",
		"CIN",
		"INTH",
		"INTL",
		"DOUT",
        "HEARTBEAT",
		"INTB",
		"UART0TX",
		"UART0RX",
		"UART1TX",
		"UART1RX",
		"I2C0SDA",
		"I2C0SCL",
		"I2C1SDA",
		"I2C1SCL",
		"SPI0RX",
		"SPI0TX",
		"SPI0SCK",
		"SPI1RX",
		"SPI1TX",
		"SPI1SCK",
        "IR",
        "INT1",
        "INT2",
        "INT3",
        "INT4",
        "PWM0A",
        "PWM0B",
        "PWM1A",
        "PWM1B",
        "PWM2A",
        "PWM2B",
        "PWM3A",
        "PWM3B",
        "PWM4A",
        "PWM4B",
        "PWM5A",
        "PWM5B",
        "PWM6A",
        "PWM6B",
        "PWM7A",
        "PWM7B"
};
;
extern struct s_vartbl {                               // structure of the variable table
	unsigned char name[MAXVARLEN];                       // variable's name
	unsigned char type;                                  // its type (T_NUM, T_INT or T_STR)
	unsigned char level;                                 // its subroutine or function level (used to track local variables)
    unsigned char size;                         // the number of chars to allocate for each element in a string array
    unsigned char dummy;
    int __attribute__ ((aligned (4))) dims[MAXDIM];                     // the dimensions. it is an array if the first dimension is NOT zero
    union u_val{
        MMFLOAT f;                              // the value if it is a float
        long long int i;                        // the value if it is an integer
        MMFLOAT *fa;                            // pointer to the allocated memory if it is an array of floats
        long long int *ia;                      // pointer to the allocated memory if it is an array of integers
        unsigned char *s;                                // pointer to the allocated memory if it is a string
    }  __attribute__ ((aligned (8))) val;
} __attribute__ ((aligned (8))) s_vartbl_val;
volatile int ExtCurrentConfig[NBRPINS + 1];
volatile int INT1Count, INT1Value, INT1InitTimer, INT1Timer;
volatile int INT2Count, INT2Value, INT2InitTimer, INT2Timer;
volatile int INT3Count, INT3Value, INT3InitTimer, INT3Timer;
volatile int INT4Count, INT4Value, INT4InitTimer, INT4Timer;
uint64_t uSecoffset=0;
volatile uint64_t IRoffset=0;
void *IrDev, *IrCmd;
volatile char IrVarType;
volatile char IrState, IrGotMsg;
int IrBits, IrCount;
unsigned char *IrInterrupt;
int last_adc=99;
int CallBackEnabled=0;
int IRpin=99;
int PWM0Apin=99;
int PWM1Apin=99;
int PWM2Apin=99;
int PWM3Apin=99;
int PWM4Apin=99;
int PWM5Apin=99;
int PWM6Apin=99;
int PWM7Apin=99;
int PWM0Bpin=99;
int PWM1Bpin=99;
int PWM2Bpin=99;
int PWM3Bpin=99;
int PWM4Bpin=99;
int PWM5Bpin=99;
int PWM6Bpin=99;
int PWM7Bpin=99;
int UART1RXpin=99;
int UART1TXpin=99;
int UART0TXpin=99;
int UART0RXpin=99;
int SPI1TXpin=99;
int SPI1RXpin=99;
int SPI1SCKpin=99;
int SPI0TXpin=99;
int SPI0RXpin=99;
int SPI0SCKpin=99;
int I2C1SDApin=99;
int I2C1SCLpin=99;
int I2C0SDApin=99;
int I2C0SCLpin=99;
int slice0=0,slice1=0,slice2=0,slice3=0,slice4=0,slice5=0,slice6=0,slice7=0;
char *ADCInterrupt;
int ADCopen=0;
volatile MMFLOAT * volatile a1float=NULL, * volatile a2float=NULL, * volatile a3float=NULL, * volatile a4float=NULL;
int ADCmax=0;
volatile int ADCpos=0;
float frequency;
int dmarunning=0;
uint dma_chan;
short *ADCbuffer=NULL;
void PWMoff(int slice);
//Vector to CFunction routine called every command (ie, from the BASIC interrupt checker)
extern unsigned int CFuncInt1;
//Vector to CFunction routine called by the interrupt 2 handler
extern unsigned int CFuncInt2;
extern void CallCFuncInt1(void);
extern void CallCFuncInt2(void);

uint64_t readusclock(void){
    return time_us_64()-uSecoffset;
}
void writeusclock(uint64_t timeset){
  uSecoffset=time_us_64()-(uint64_t)timeset;
}
uint64_t readIRclock(void){
    return time_us_64()-IRoffset;
}
void writeIRclock(uint64_t timeset){
  IRoffset=time_us_64()-(uint64_t)timeset;
}

const uint8_t PINMAP[30]={1,2,4,5,6,7,9,10,11,12,14,15,16,17,19,20,21,22,24,25,26,27,29,41,42,43,31,32,34,44};
int codemap(int pin){
			if(pin>29 || pin<0) error("Invalid GPIO");
			return (int)PINMAP[pin];
	return 0;
}
int codecheck(unsigned char *line){
	if((line[0]=='G' || line[0]=='g') && (line[1]=='P' || line[1]=='p')){
		line+=2;
		if(isnamestart(*line) || *line=='.') return 1;

		if(isdigit(*line) && !isnamechar(line[1])) {
			return 0;
		}
		line++;
		
		if(!(isdigit(*line))) return 2;
		line++;
		if(isnamechar(*line)) return 3;
	} else return 4;
	return 0;
}
void SoftReset(void){
	watchdog_enable(1, 1);
	while(1);
}
void PinSetBit(int pin, unsigned int offset) {
	switch (offset){
	case LATCLR:
		gpio_set_pulls(PinDef[pin].GPno,false,false);
		gpio_pull_down(PinDef[pin].GPno);
		gpio_put(PinDef[pin].GPno,GPIO_PIN_RESET);
		return;
	case LATSET:
		gpio_set_pulls(PinDef[pin].GPno,false,false);
		gpio_pull_up(PinDef[pin].GPno);
		gpio_put(PinDef[pin].GPno,GPIO_PIN_SET);
		return;
	case LATINV:
        gpio_xor_mask(1<<PinDef[pin].GPno);
		return;
	case TRISSET:
        gpio_set_dir(PinDef[pin].GPno, GPIO_IN);
        uSec(2);
        return;
	case TRISCLR:
        gpio_set_dir(PinDef[pin].GPno, GPIO_OUT);
        uSec(2);
        return;
	case CNPUSET:
		gpio_set_pulls(PinDef[pin].GPno,true,false);
	    return;
	case CNPDSET:
		gpio_set_pulls(PinDef[pin].GPno,false,true);
	    return;
	case CNPUCLR:
	case CNPDCLR:
		gpio_set_pulls(PinDef[pin].GPno,false,false);
		return;
	case ODCCLR:
        gpio_set_dir(PinDef[pin].GPno, GPIO_OUT);
		gpio_put(PinDef[pin].GPno,GPIO_PIN_RESET);
        uSec(2);
		return;
	case ODCSET:
		gpio_set_pulls(PinDef[pin].GPno,true,false);
        gpio_set_dir(PinDef[pin].GPno, GPIO_IN);
        uSec(2);
		return;
    case ANSELCLR:
        gpio_set_function(PinDef[pin].GPno, GPIO_FUNC_SIO);
        gpio_set_dir(PinDef[pin].GPno, GPIO_IN);
        return;
    case ANSELSET:
        gpio_set_dir(PinDef[pin].GPno, GPIO_IN);
        gpio_disable_pulls(PinDef[pin].GPno);
        gpio_set_input_enabled(PinDef[pin].GPno, false);
        adc_select_input(PinDef[pin].ADCpin);
        return;
	default: error("Unknown PinSetBit command");
	}
}

int IsInvalidPin(int pin) {
    if(pin < 1 || pin > NBRPINS) return true;
    if(PinDef[pin].mode & UNUSED) return true;
    return false;
}
void __not_in_flash_func(ExtSet)(int pin, int val){

    if(ExtCurrentConfig[pin] == EXT_NOT_CONFIG || ExtCurrentConfig[pin] == EXT_DIG_OUT/* || ExtCurrentConfig[pin] == EXT_OC_OUT*/) {
        PinSetBit(pin, val ? LATSET : LATCLR);
//        INTEnableInterrupts();
    }
    else if(ExtCurrentConfig[pin] == EXT_CNT_IN){ //allow the user to zero the count
        if(pin == Option.INT1pin) INT1Count=val;
        if(pin == Option.INT2pin) INT2Count=val;
        if(pin == Option.INT3pin) INT3Count=val;
        if(pin == Option.INT4pin) INT4Count=val;

    }
    else
        error("Pin % is not an output",pin);
}


// this is invoked as a command (ie, pin(3) = 1)
// first get the argument then step over the closing bracket.  Search through the rest of the command line looking
// for the equals sign and step over it, evaluate the rest of the command and set the pin accordingly
void __not_in_flash_func(cmd_pin)(void) {
	int pin, value;
	unsigned char code;
	if(!(code=codecheck(cmdline)))cmdline+=2;
	pin = getinteger(cmdline);
	if(!code)pin=codemap(pin);
	if(IsInvalidPin(pin)) error("Invalid pin");
	while(*cmdline && tokenfunction(*cmdline) != op_equal) cmdline++;
	if(!*cmdline) error("Invalid syntax");
	++cmdline;
	if(!*cmdline) error("Invalid syntax");
	value = getinteger(cmdline);
	ExtSet(pin, value);
}
void ClearPin(int pin){
    if(pin==IRpin)IRpin=99;
    if(pin==PWM0Apin)PWM0Apin=99;
    if(pin==PWM1Apin)PWM1Apin=99;
    if(pin==PWM2Apin)PWM2Apin=99;
    if(pin==PWM3Apin)PWM3Apin=99;
    if(pin==PWM4Apin)PWM4Apin=99;
    if(pin==PWM5Apin)PWM5Apin=99;
    if(pin==PWM6Apin)PWM6Apin=99;
    if(pin==PWM7Apin)PWM7Apin=99;
    if(pin==PWM0Bpin)PWM0Bpin=99;
    if(pin==PWM1Bpin)PWM1Bpin=99;
    if(pin==PWM2Bpin)PWM2Bpin=99;
    if(pin==PWM3Bpin)PWM3Bpin=99;
    if(pin==PWM4Bpin)PWM4Bpin=99;
    if(pin==PWM5Bpin)PWM5Bpin=99;
    if(pin==PWM6Bpin)PWM6Bpin=99;
    if(pin==PWM7Bpin)PWM7Bpin=99;
    if(pin==UART0TXpin)UART0TXpin=99;
    if(pin==UART0RXpin)UART0RXpin=99;
    if(pin==UART1RXpin)UART1RXpin=99;
    if(pin==UART1TXpin)UART1TXpin=99;
    if(pin==SPI1TXpin)SPI1TXpin=99;
    if(pin==SPI1RXpin)SPI1RXpin=99;
    if(pin==SPI1SCKpin)SPI1SCKpin=99;
    if(pin==SPI0TXpin)SPI0TXpin=99;
    if(pin==SPI0RXpin)SPI0RXpin=99;
    if(pin==SPI0SCKpin)SPI0SCKpin=99;
    if(pin==I2C1SDApin)I2C1SDApin=99;
    if(pin==I2C1SCLpin)I2C1SCLpin=99;
    if(pin==I2C0SDApin)I2C0SDApin=99;
    if(pin==I2C0SCLpin)I2C0SCLpin=99;
}
/****************************************************************************************************************************
Configure an I/O pin
*****************************************************************************************************************************/
void ExtCfg(int pin, int cfg, int option) {
  int i, tris, ana, oc, edge;

    CheckPin(pin, CP_IGNORE_INUSE | CP_IGNORE_RESERVED);

    if(cfg >= EXT_DS18B20_RESERVED) {
        ExtCurrentConfig[pin] |= cfg;                                // don't do anything except set the config type
        return;
    }
    ClearPin(pin);  //disable the link to any special functions
    if(pin == Option.INT1pin) {
        if(CallBackEnabled==2) gpio_set_irq_enabled_with_callback(PinDef[pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &gpio_callback);
        else gpio_set_irq_enabled(PinDef[pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
        CallBackEnabled &= (~2);
    }
    if(pin == Option.INT2pin) {
        if(CallBackEnabled==4) gpio_set_irq_enabled_with_callback(PinDef[pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &gpio_callback);
        else gpio_set_irq_enabled(PinDef[pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
        CallBackEnabled &= (~4);
    }
    if(pin == Option.INT3pin) {
        if(CallBackEnabled==8) gpio_set_irq_enabled_with_callback(PinDef[pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &gpio_callback);
        else gpio_set_irq_enabled(PinDef[pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
        CallBackEnabled &= (~8);
    }
    if(pin == Option.INT4pin) {
        if(CallBackEnabled==16) gpio_set_irq_enabled_with_callback(PinDef[pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &gpio_callback);
        else gpio_set_irq_enabled(PinDef[pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
        CallBackEnabled &= (~16);
    }
 

    // make sure any pullups/pulldowns are removed in case we are changing from a digital input
    gpio_disable_pulls(PinDef[pin].GPno);
    // disable ADC if we are changing from a analogue input
    if(ExtCurrentConfig[pin]==EXT_ANA_IN)PinSetBit(pin, ANSELCLR);

    for(i = 0; i < NBRINTERRUPTS; i++)
        if(inttbl[i].pin == pin)
            inttbl[i].pin = 0;                                      // start off by disable a software interrupt (if set) on this pin
    gpio_init(PinDef[pin].GPno); 
    gpio_set_input_hysteresis_enabled(PinDef[pin].GPno,true);

    switch(cfg) {
        case EXT_NOT_CONFIG:    tris = 1; ana = 1; oc = 0;
//                                gpio_init(PinDef[pin].GPno); 
//		                        gpio_set_input_hysteresis_enabled(PinDef[pin].GPno,true);
                                switch(ExtCurrentConfig[pin]){      //Disable the pin numbers used by the special function code
                                     case EXT_IR:
				                        IRpin=99;
					                    break;
                                     case EXT_PWM0A:
				                        PWM0Apin=99;
					                    break;
                                     case EXT_PWM1A:
				                        PWM1Apin=99;
					                    break;
                                     case EXT_PWM2A:
				                        PWM2Apin=99;
					                    break;
                                     case EXT_PWM3A:
				                        PWM3Apin=99;
					                    break;
                                     case EXT_PWM4A:
				                        PWM4Apin=99;
					                    break;
                                     case EXT_PWM5A:
				                        PWM5Apin=99;
					                    break;
                                     case EXT_PWM6A:
				                        PWM6Apin=99;
					                    break;
                                     case EXT_PWM7A:
				                        PWM7Apin=99;
					                    break;
                                     case EXT_PWM0B:
				                        PWM0Bpin=99;
					                    break;
                                     case EXT_PWM1B:
				                        PWM1Bpin=99;
					                    break;
                                     case EXT_PWM2B:
				                        PWM2Bpin=99;
					                    break;
                                     case EXT_PWM3B:
				                        PWM3Bpin=99;
					                    break;
                                     case EXT_PWM4B:
				                        PWM4Bpin=99;
					                    break;
                                     case EXT_PWM5B:
				                        PWM5Bpin=99;
					                    break;
                                     case EXT_PWM6B:
				                        PWM6Bpin=99;
					                    break;
                                     case EXT_PWM7B:
				                        PWM7Bpin=99;
					                    break;
                                     case EXT_UART0TX:
				                        UART0TXpin=99;
					                    break;
                                    case EXT_UART0RX:
				                        UART0RXpin=99;
					                    break;
                                    case EXT_UART1TX:
				                        UART1TXpin=99;
					                    break;
                                    case EXT_UART1RX:
				                        UART1RXpin=99;
					                    break;
                                    case EXT_I2C0SDA:
				                        I2C0SDApin=99;
					                    break;
                                    case EXT_I2C0SCL:
				                        I2C0SCLpin=99;
					                    break;
                                    case EXT_I2C1SDA:
				                        I2C1SDApin=99;
					                    break;
                                    case EXT_I2C1SCL:
				                        I2C1SCLpin=99;
					                    break;
                                    case EXT_SPI0RX:
				                        SPI0RXpin=99;
					                    break;
                                    case EXT_SPI0TX:
				                        SPI0TXpin=99;
					                    break;
                                    case EXT_SPI0SCK:
				                        SPI0SCKpin=99;
					                    break;
                                    case EXT_SPI1RX:
				                        SPI1RXpin=99;
					                    break;
                                    case EXT_SPI1TX:
				                        SPI1TXpin=99;
					                    break;
                                    case EXT_SPI1SCK:
				                        SPI1SCKpin=99;
					                    break;
                                }
                                break;

        case EXT_ANA_IN:        if(!(PinDef[pin].mode & ANALOG_IN)) error("Invalid configuration");
                                tris = 1; ana = 0; oc = 0;
                                break;

        case EXT_CNT_IN:        
        case EXT_FREQ_IN:   // same as counting, so fall through
        case EXT_PER_IN:        // same as counting, so fall through
                                    edge = GPIO_IRQ_EDGE_RISE;
                                    if(cfg==EXT_CNT_IN && option==2)edge = GPIO_IRQ_EDGE_FALL;
                                    if(cfg==EXT_CNT_IN && option>=3)edge = GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE;
                                    if(option==1 || option==4)gpio_pull_down (PinDef[pin].GPno);
                                    if(option==2 || option==5)gpio_pull_up (PinDef[pin].GPno);
                                    if(pin == Option.INT1pin) {
                                    if(!CallBackEnabled){
                                        gpio_set_irq_enabled_with_callback(PinDef[pin].GPno, edge , true, &gpio_callback);
                                        CallBackEnabled=2;
                                    } else {
                                        gpio_set_irq_enabled(PinDef[pin].GPno, edge, true);
                                        CallBackEnabled|=2;
                                    }
                                    INT1Count = INT1Value = 0;
                                    INT1Timer = INT1InitTimer = option;  // only used for frequency and period measurement
                                    tris = 1; ana = 1; oc = 0;
		                            gpio_set_input_hysteresis_enabled(PinDef[pin].GPno,true);
                                    break;
                                }
                                if(pin == Option.INT2pin) {
                                    if(!CallBackEnabled){
                                        gpio_set_irq_enabled_with_callback(PinDef[pin].GPno, edge , true, &gpio_callback);
                                        CallBackEnabled=4;
                                    } else {
                                        gpio_set_irq_enabled(PinDef[pin].GPno, edge, true);
                                        CallBackEnabled|=4;
                                    }
                                    INT2Count = INT2Value = 0;
                                    INT2Timer = INT2InitTimer = option;  // only used for frequency and period measurement
                                    tris = 1; ana = 1; oc = 0;
		                            gpio_set_input_hysteresis_enabled(PinDef[pin].GPno,true);
                                    break;
                                }
                                if(pin == Option.INT3pin) {
                                    if(!CallBackEnabled){
                                        gpio_set_irq_enabled_with_callback(PinDef[pin].GPno, edge , true, &gpio_callback);
                                        CallBackEnabled=8;
                                    } else {
                                        gpio_set_irq_enabled(PinDef[pin].GPno, edge, true);
                                        CallBackEnabled|=8;
                                    }
                                    INT3Count = INT3Value = 0;
                                    INT3Timer = INT3InitTimer = option;  // only used for frequency and period measurement
                                    tris = 1; ana = 1; oc = 0;
		                            gpio_set_input_hysteresis_enabled(PinDef[pin].GPno,true);
                                    break;
                                }
                                if(pin == Option.INT4pin) {
                                    if(!CallBackEnabled){
                                        gpio_set_irq_enabled_with_callback(PinDef[pin].GPno, edge , true, &gpio_callback);
                                        CallBackEnabled=16;
                                    } else {
                                        gpio_set_irq_enabled(PinDef[pin].GPno, edge, true);
                                        CallBackEnabled|=16;
                                    }
                                    INT4Count = INT4Value = 0;
                                    INT4Timer = INT4InitTimer = option;  // only used for frequency and period measurement
                                    tris = 1; ana = 1; oc = 0;
		                            gpio_set_input_hysteresis_enabled(PinDef[pin].GPno,true);
                                    break;
                                }
                                error("Invalid configuration");       // not an interrupt enabled pin
                                return;

        case EXT_INT_LO:                                              // same as digital input, so fall through
        case EXT_INT_HI:                                              // same as digital input, so fall through
        case EXT_INT_BOTH:                                            // same as digital input, so fall through
        case EXT_DIG_IN:        if(!(PinDef[pin].mode & DIGITAL_IN)) error("Invalid configuration");
                                if(option) PinSetBit(pin, option);
                                tris = 1; ana = 1; oc = 0;
		                        gpio_set_input_hysteresis_enabled(PinDef[pin].GPno,true);
                                break;

        case EXT_PIO0_OUT:       
        case EXT_PIO1_OUT:       
        case EXT_DIG_OUT:       if(!(PinDef[pin].mode & DIGITAL_OUT)) error("Invalid configuration");
                                tris = 0; ana = 1; oc = 0;
                                break;
        case EXT_HEARTBEAT:     if(!(pin==43)) error("Invalid configuration");
                                tris = 0; ana = 1; oc = 0;
                                break;
        case EXT_UART0TX:       if(!(PinDef[pin].mode & UART0TX)) error("Invalid configuration");
                                if((UART0TXpin!=99)) error("Already Set to pin %",UART0TXpin);
                                UART0TXpin=pin;
                                break;
        case EXT_UART0RX:       if(!(PinDef[pin].mode & UART0RX)) error("Invalid configuration");
                                if((UART0RXpin!=99)) error("Already Set to pin %",UART0RXpin);
                                UART0RXpin=pin;
                                break;
        case EXT_UART1TX:       if(!(PinDef[pin].mode & UART1TX)) error("Invalid configuration");
                                if((UART1TXpin!=99)) error("Already Set to pin %",UART1TXpin);
                                UART1TXpin=pin;
                                break;
        case EXT_UART1RX:       if(!(PinDef[pin].mode & UART1RX)) error("Invalid configuration");
                                if((UART1RXpin!=99)) error("Already Set to pin %",UART1RXpin);
                                UART1RXpin=pin;
                                break;
        case EXT_SPI0TX:        if(!(PinDef[pin].mode & SPI0TX)) error("Invalid configuration");
                                if(SPI0locked)error("SPI in use for SYSTEM SPI");
                                if((SPI0TXpin!=99 && SPI0TXpin!=pin)) error("Already Set to pin %",SPI0TXpin);
                                SPI0TXpin=pin;
                                break;
        case EXT_SPI0RX:        if(!(PinDef[pin].mode & SPI0RX)) error("Invalid configuration");
                                if(SPI0locked)error("SPI in use for SYSTEM SPI");
                                if((SPI0RXpin!=99 && SPI0RXpin!=pin)) error("Already Set to pin %",SPI0RXpin);
                                SPI0RXpin=pin;
                                break;
        case EXT_SPI0SCK:       if(!(PinDef[pin].mode & SPI0SCK)) error("Invalid configuration");
                                 if(SPI0locked)error("SPI in use for SYSTEM SPI");
                               if((SPI0SCKpin!=99 && SPI0SCKpin!=pin)) error("Already Set to pin %",SPI0SCKpin);
                                SPI0SCKpin=pin;
                                break;
        case EXT_SPI1TX:        if(!(PinDef[pin].mode & SPI1TX)) error("Invalid configuration");
                                if(SPI1locked)error("SPI2 in use for SYSTEM SPI");
                                if((SPI1TXpin!=99 && SPI1TXpin!=pin)) error("Already Set to pin %",SPI1TXpin);
                                SPI1TXpin=pin;
                                break;
        case EXT_SPI1RX:        if(!(PinDef[pin].mode & SPI1RX)) error("Invalid configuration");
                                if(SPI1locked)error("SPI2 in use for SYSTEM SPI");
                                if((SPI1RXpin!=99 && SPI1RXpin!=pin)) error("Already Set to pin %",SPI1RXpin);
                                SPI1RXpin=pin;
                                break;
        case EXT_SPI1SCK:       if(!(PinDef[pin].mode & SPI1SCK)) error("Invalid configuration");
                                if(SPI1locked)error("SPI2 in use for SYSTEM SPI");
                                if((SPI1SCKpin!=99 && SPI1SCKpin!=pin)) error("Already Set to pin %",SPI1SCKpin);
                                SPI1SCKpin=pin;
                                break;
        case EXT_IR:            if((IRpin!=99)) error("Already Set to pin %",IRpin);
                                IRpin=pin;
                                break;
        case EXT_PWM0A:         if(!(PinDef[pin].mode & PWM0A)) error("Invalid configuration");
                                if((PWM0Apin!=99 && PWM0Apin!=pin)) error("Already Set to pin %",PWM0Apin);
                                PWM0Apin=pin;
                                break;
        case EXT_PWM1A:         if(!(PinDef[pin].mode & PWM1A)) error("Invalid configuration");
                                if((PWM1Apin!=99 && PWM1Apin!=pin)) error("Already Set to pin %",PWM1Apin);
                                PWM1Apin=pin;
                                break;
        case EXT_PWM2A:         if(!(PinDef[pin].mode & PWM2A)) error("Invalid configuration");
                                if((PWM2Apin!=99 && PWM2Apin!=pin)) error("Already Set to pin %",PWM2Apin);
                                PWM2Apin=pin;
                                break;
        case EXT_PWM3A:         if(!(PinDef[pin].mode & PWM3A)) error("Invalid configuration");
                                if((PWM3Apin!=99 && PWM3Apin!=pin)) error("Already Set to pin %",PWM3Apin);
                                PWM3Apin=pin;
                                break;
        case EXT_PWM4A:         if(!(PinDef[pin].mode & PWM4A)) error("Invalid configuration");
                                if((PWM4Apin!=99 && PWM4Apin!=pin)) error("Already Set to pin %",PWM4Apin);
                                PWM4Apin=pin;
                                break;
        case EXT_PWM5A:         if(!(PinDef[pin].mode & PWM5A)) error("Invalid configuration");
                                if((PWM5Apin!=99 && PWM5Apin!=pin)) error("Already Set to pin %",PWM5Apin);
                                PWM5Apin=pin;
                                break;
        case EXT_PWM6A:         if(!(PinDef[pin].mode & PWM6A)) error("Invalid configuration");
                                if((PWM6Apin!=99 && PWM6Apin!=pin)) error("Already Set to pin %",PWM6Apin);
                                PWM6Apin=pin;
                                break;
        case EXT_PWM7A:         if(!(PinDef[pin].mode & PWM7A)) error("Invalid configuration");
                                if((PWM7Apin!=99 && PWM7Apin!=pin)) error("Already Set to pin %",PWM7Apin);
                                PWM7Apin=pin;
                                break;
        case EXT_PWM0B:         if(!(PinDef[pin].mode & PWM0B)) error("Invalid configuration");
                                if((PWM0Bpin!=99 && PWM0Bpin!=pin)) error("Already Set to pin %",PWM0Bpin);
                                PWM0Bpin=pin;
                                break;
        case EXT_PWM1B:         if(!(PinDef[pin].mode & PWM1B)) error("Invalid configuration");
                                if((PWM1Bpin!=99 && PWM1Bpin!=pin)) error("Already Set to pin %",PWM1Bpin);
                                PWM1Bpin=pin;
                                break;
        case EXT_PWM2B:         if(!(PinDef[pin].mode & PWM2B)) error("Invalid configuration");
                                if((PWM2Bpin!=99 && PWM2Bpin!=pin)) error("Already Set to pin %",PWM2Bpin);
                                PWM2Bpin=pin;
                                break;
        case EXT_PWM3B:         if(!(PinDef[pin].mode & PWM3B)) error("Invalid configuration");
                                if((PWM3Bpin!=99 && PWM3Bpin!=pin)) error("Already Set to pin %",PWM3Bpin);
                                PWM3Bpin=pin;
                                break;
        case EXT_PWM4B:         if(!(PinDef[pin].mode & PWM4B)) error("Invalid configuration");
                                if((PWM4Bpin!=99 && PWM4Bpin!=pin)) error("Already Set to pin %",PWM4Bpin);
                                PWM4Bpin=pin;
                                break;
        case EXT_PWM5B:         if(!(PinDef[pin].mode & PWM5B)) error("Invalid configuration");
                                if((PWM5Bpin!=99 && PWM5Bpin!=pin)) error("Already Set to pin %",PWM5Bpin);
                                PWM5Bpin=pin;
                                break;
        case EXT_PWM6B:         if(!(PinDef[pin].mode & PWM6B)) error("Invalid configuration");
                                if((PWM6Bpin!=99 && PWM6Bpin!=pin)) error("Already Set to pin %",PWM6Bpin);
                                PWM6Bpin=pin;
                                break;
        case EXT_PWM7B:         if(!(PinDef[pin].mode & PWM7B)) error("Invalid configuration");
                                if((PWM7Bpin!=99 && PWM7Bpin!=pin)) error("Already Set to pin %",PWM7Bpin);
                                PWM7Bpin=pin;
                                break;
        case EXT_I2C0SDA:       if(!(PinDef[pin].mode & I2C0SDA)) error("Invalid configuration");
                                if(I2C0locked)error("I2C in use for SYSTEM I2C");
                                if((I2C0SDApin!=99 && I2C0SDApin!=pin)) error("Already Set to pin %",I2C0SDApin);
                                I2C0SDApin=pin;
                                break;
        case EXT_I2C0SCL:       if(!(PinDef[pin].mode & I2C0SCL)) error("Invalid configuration");
                                if(I2C0locked)error("I2C in use for SYSTEM I2C");
                                 if((I2C0SCLpin!=99 && I2C0SCLpin!=pin)) error("Already Set to pin %",I2C0SCLpin);
                                I2C0SCLpin=pin;
                                break;
        case EXT_I2C1SDA:       if(!(PinDef[pin].mode & I2C1SDA)) error("Invalid configuration");
                                if(I2C1locked)error("I2C2 in use for SYSTEM I2C");
                                 if((I2C1SDApin!=99) && I2C1SDApin!=pin) error("Already Set to pin %",I2C1SDApin);
                                I2C1SDApin=pin;
                                break;
        case EXT_I2C1SCL:       if(!(PinDef[pin].mode & I2C1SCL)) error("Invalid configuration");
                                if(I2C1locked)error("I2C2 in use for SYSTEM I2C");
                                 if((I2C1SCLpin!=99 && I2C1SCLpin!=pin)) error("Already Set to pin %",I2C1SCLpin);
                                I2C1SCLpin=pin;
                                break;
        default:                error("Invalid configuration");
                              return;
  }
    ExtCurrentConfig[pin] = cfg;
    if(cfg<=EXT_INT_BOTH){
        //    *GetPortAddr(pin, ana ? ANSELCLR : ANSELSET) = (1 << GetPinBit(pin));// if ana = 1 then it is a digital I/O
        PinSetBit(pin, tris ? TRISSET : TRISCLR);                         // if tris = 1 then it is an input
        if(cfg == EXT_NOT_CONFIG) ExtSet(pin, 0);                         // set the default output to low
        if(ana==0)PinSetBit(pin, ANSELSET);
    }
    else if(cfg>=EXT_UART0TX && cfg<=EXT_UART1RX){
        gpio_set_function(PinDef[pin].GPno, GPIO_FUNC_UART);
        if(cfg==EXT_UART0RX || cfg==EXT_UART1RX)gpio_set_pulls(PinDef[pin].GPno,true,false);
    }
    else if(cfg>=EXT_I2C0SDA && cfg<=EXT_I2C1SCL)gpio_set_function(PinDef[pin].GPno, GPIO_FUNC_I2C);
    else if(cfg>=EXT_SPI0RX && cfg<=EXT_SPI1SCK)gpio_set_function(PinDef[pin].GPno, GPIO_FUNC_SPI);
    else if(cfg>=EXT_PWM0A && cfg<=EXT_PWM7B)gpio_set_function(PinDef[pin].GPno, GPIO_FUNC_PWM);
    else if(cfg==EXT_PIO0_OUT)gpio_set_function(PinDef[pin].GPno, GPIO_FUNC_PIO0);
    else if(cfg==EXT_PIO1_OUT)gpio_set_function(PinDef[pin].GPno, GPIO_FUNC_PIO1);
    uSec(2);
}
int64_t __not_in_flash_func(ExtInp)(int pin){
    if(ExtCurrentConfig[pin]==EXT_ANA_IN){
        if(last_adc!=pin){
            last_adc=pin;
            adc_select_input(PinDef[pin].ADCpin);
        }
        return adc_read();          
    } else if(ExtCurrentConfig[pin] == EXT_FREQ_IN || ExtCurrentConfig[pin] == EXT_PER_IN) {
      // select input channel
        if(pin == Option.INT1pin) return INT1Value;
        if(pin == Option.INT2pin) return INT2Value;
        if(pin == Option.INT3pin) return INT3Value;
        if(pin == Option.INT4pin) return INT4Value;
    }  else if(ExtCurrentConfig[pin] == EXT_CNT_IN) {
        // select input channel
            if(pin == Option.INT1pin) return INT1Count;
            if(pin == Option.INT2pin) return INT2Count;
            if(pin == Option.INT3pin) return INT3Count;
            if(pin == Option.INT4pin) return INT4Count;
    } else return  gpio_get(PinDef[pin].GPno);
}
void cmd_setpin(void) {
	int i, pin, pin2=0, pin3=0, value, value2=0, value3=0, option = 0;
	getargs(&cmdline, 7, ",");
	if(argc%2 == 0 || argc < 3) error("Argument count");
	char code;
	if(!(code=codecheck(argv[0])))argv[0]+=2;
	pin = getinteger(argv[0]);
	if(!code)pin=codemap(pin);

    if(checkstring(argv[2], "OFF") || checkstring(argv[2], "0"))
        value = EXT_NOT_CONFIG;
    else if(checkstring(argv[2], "AIN"))
        value = EXT_ANA_IN;
    else if(checkstring(argv[2], "DIN"))
        value = EXT_DIG_IN;
    else if(checkstring(argv[2], "FIN"))
        value = EXT_FREQ_IN;
    else if(checkstring(argv[2], "PIN"))
        value = EXT_PER_IN;
    else if(checkstring(argv[2], "CIN"))
        value = EXT_CNT_IN;
    else if(checkstring(argv[2], "INTH"))
        value = EXT_INT_HI;
    else if(checkstring(argv[2], "INTL"))
        value = EXT_INT_LO;
    else if(checkstring(argv[2], "DOUT"))
        value = EXT_DIG_OUT;
    else if(checkstring(argv[2], "HEARTBEAT"))
        value = EXT_HEARTBEAT;
    else if(checkstring(argv[2], "INTB"))
        value = EXT_INT_BOTH;
/*    else if(checkstring(argv[2], "UART0TX"))
        value = EXT_UART0TX;
    else if(checkstring(argv[2], "UART0RX"))
        value = EXT_UART0RX;
    else if(checkstring(argv[2], "UART1TX"))
        value = EXT_UART1TX;
    else if(checkstring(argv[2], "UART1RX"))
        value = EXT_UART1RX;
    else if(checkstring(argv[2], "I2C0SDA"))
        value = EXT_I2C0SDA;
    else if(checkstring(argv[2], "I2C0SCL"))
        value = EXT_I2C0SCL;
    else if(checkstring(argv[2], "I2C1SDA"))
        value = EXT_I2C1SDA;
    else if(checkstring(argv[2], "I2C1SCL"))
        value = EXT_I2C1SCL;
    else if(checkstring(argv[2], "SPI0TX"))
        value = EXT_SPI0TX;
    else if(checkstring(argv[2], "SPI0RX"))
        value = EXT_SPI0RX;
    else if(checkstring(argv[2], "SPI0SCK"))
        value = EXT_SPI0SCK;
    else if(checkstring(argv[2], "SPI1TX"))
        value = EXT_SPI1TX;
    else if(checkstring(argv[2], "SPI1RX"))
        value = EXT_SPI1RX;
    else if(checkstring(argv[2], "SPI1SCK"))
        value = EXT_SPI1SCK;
    else if(checkstring(argv[2], "INT1"))
        value = EXT_INT1;
    else if(checkstring(argv[2], "INT2"))
        value = EXT_INT2;
    else if(checkstring(argv[2], "INT3"))
        value = EXT_INT3;
    else if(checkstring(argv[2], "INT4"))
        value = EXT_INT4;*/
    else if(checkstring(argv[2], "IR"))
        value = EXT_IR;
    else if(checkstring(argv[2], "PWM0A"))
        value = EXT_PWM0A;
    else if(checkstring(argv[2], "PWM1A"))
        value = EXT_PWM1A;
    else if(checkstring(argv[2], "PWM2A"))
        value = EXT_PWM2A;
    else if(checkstring(argv[2], "PWM3A"))
        value = EXT_PWM3A;
    else if(checkstring(argv[2], "PWM4A"))
        value = EXT_PWM4A;
    else if(checkstring(argv[2], "PWM5A"))
        value = EXT_PWM5A;
    else if(checkstring(argv[2], "PWM6A"))
        value = EXT_PWM6A;
    else if(checkstring(argv[2], "PWM7A"))
        value = EXT_PWM7A;
    else if(checkstring(argv[2], "PWM0B"))
        value = EXT_PWM0B;
    else if(checkstring(argv[2], "PWM1B"))
        value = EXT_PWM1B;
    else if(checkstring(argv[2], "PWM2B"))
        value = EXT_PWM2B;
    else if(checkstring(argv[2], "PWM3B"))
        value = EXT_PWM3B;
    else if(checkstring(argv[2], "PWM4B"))
        value = EXT_PWM4B;
    else if(checkstring(argv[2], "PWM5B"))
        value = EXT_PWM5B;
    else if(checkstring(argv[2], "PWM6B"))
        value = EXT_PWM6B;
    else if(checkstring(argv[2], "PWM7B"))
        value = EXT_PWM7B;
    else if(checkstring(argv[2], "PIO0"))
        value = EXT_PIO0_OUT;
    else if(checkstring(argv[2], "PIO1"))
        value = EXT_PIO1_OUT;
    else if(checkstring(argv[4],"COM1")){
        if(!(code=codecheck(argv[2])))argv[2]+=2;
        pin2 = getinteger(argv[2]);
        if(!code)pin2=codemap(pin2);
        if(PinDef[pin].mode & UART0TX)value = EXT_UART0TX;
        else if(PinDef[pin].mode & UART0RX)value = EXT_UART0RX;
        else error("Invalid configuration");
        if(PinDef[pin2].mode & UART0TX)value2 = EXT_UART0TX;
        else if(PinDef[pin2].mode & UART0RX)value2 = EXT_UART0RX;
        else error("Invalid configuration");
        if(value==value2)error("Invalid configuration");
    } else if(checkstring(argv[4],"COM2")){
        if(!(code=codecheck(argv[2])))argv[2]+=2;
        pin2 = getinteger(argv[2]);
        if(!code)pin2=codemap(pin2);
        if(PinDef[pin].mode & UART1TX)value = EXT_UART1TX;
        else if(PinDef[pin].mode & UART1RX)value = EXT_UART1RX;
        else error("Invalid configuration");
        if(PinDef[pin2].mode & UART1TX)value2 = EXT_UART1TX;
        else if(PinDef[pin2].mode & UART1RX)value2 = EXT_UART1RX;
        else error("Invalid configuration");
        if(value==value2)error("Invalid configuration");
    }  else if(checkstring(argv[4],"I2C")){
        if(!(code=codecheck(argv[2])))argv[2]+=2;
        pin2 = getinteger(argv[2]);
        if(!code)pin2=codemap(pin2);
        if(PinDef[pin].mode & I2C0SCL)value = EXT_I2C0SCL;
        else if(PinDef[pin].mode & I2C0SDA)value = EXT_I2C0SDA;
        else error("Invalid configuration");
        if(PinDef[pin2].mode & I2C0SCL)value2 = EXT_I2C0SCL;
        else if(PinDef[pin2].mode & I2C0SDA)value2 = EXT_I2C0SDA;
        else error("Invalid configuration");
        if(value==value2)error("Invalid configuration");
    }  else if(checkstring(argv[4],"I2C2")){
        if(!(code=codecheck(argv[2])))argv[2]+=2;
        pin2 = getinteger(argv[2]);
        if(!code)pin2=codemap(pin2);
        if(PinDef[pin].mode & I2C1SCL)value = EXT_I2C1SCL;
        else if(PinDef[pin].mode & I2C1SDA)value = EXT_I2C1SDA;
        else error("Invalid configuration");
        if(PinDef[pin2].mode & I2C1SCL)value2 = EXT_I2C1SCL;
        else if(PinDef[pin2].mode & I2C1SDA)value2 = EXT_I2C1SDA;
        else error("Invalid configuration");
        if(value==value2)error("Invalid configuration");
    }  else if(checkstring(argv[6],"SPI")){
        if(!(code=codecheck(argv[2])))argv[2]+=2;
        pin2 = getinteger(argv[2]);
        if(!code)pin2=codemap(pin2);
        if(!(code=codecheck(argv[4])))argv[4]+=2;
        pin3 = getinteger(argv[4]);
        if(!code)pin3=codemap(pin3);
        if(PinDef[pin].mode & SPI0RX)value = EXT_SPI0RX;
        else if(PinDef[pin].mode & SPI0TX)value = EXT_SPI0TX;
        else if(PinDef[pin].mode & SPI0SCK)value = EXT_SPI0SCK;
        else error("Invalid configuration");
        if(PinDef[pin2].mode & SPI0RX)value2 = EXT_SPI0RX;
        else if(PinDef[pin2].mode & SPI0TX)value2 = EXT_SPI0TX;
        else if(PinDef[pin2].mode & SPI0SCK)value2 = EXT_SPI0SCK;
        else error("Invalid configuration");
        if(PinDef[pin3].mode & SPI0RX)value3 = EXT_SPI0RX;
        else if(PinDef[pin3].mode & SPI0TX)value3 = EXT_SPI0TX;
        else if(PinDef[pin3].mode & SPI0SCK)value3 = EXT_SPI0SCK;
        else error("Invalid configuration");
        if(value==value2 || value==value3 || value2==value3)error("Invalid configuration");
    }  else if(checkstring(argv[6],"SPI2")){
        if(!(code=codecheck(argv[2])))argv[2]+=2;
        pin2 = getinteger(argv[2]);
        if(!code)pin2=codemap(pin2);
        if(!(code=codecheck(argv[4])))argv[4]+=2;
        pin3 = getinteger(argv[4]);
        if(!code)pin3=codemap(pin3);
        if(PinDef[pin].mode & SPI1RX)value = EXT_SPI1RX;
        else if(PinDef[pin].mode & SPI1TX)value = EXT_SPI1TX;
        else if(PinDef[pin].mode & SPI1SCK)value = EXT_SPI1SCK;
        else error("Invalid configuration");
        if(PinDef[pin2].mode & SPI1RX)value2 = EXT_SPI1RX;
        else if(PinDef[pin2].mode & SPI1TX)value2 = EXT_SPI1TX;
        else if(PinDef[pin2].mode & SPI1SCK)value2 = EXT_SPI1SCK;
        else error("Invalid configuration");
        if(PinDef[pin3].mode & SPI1RX)value3 = EXT_SPI1RX;
        else if(PinDef[pin3].mode & SPI1TX)value3 = EXT_SPI1TX;
        else if(PinDef[pin3].mode & SPI1SCK)value3 = EXT_SPI1SCK;
        else error("Invalid configuration");
        if(value==value2 || value==value3 || value2==value3)error("Invalid configuration");
    }  else if(checkstring(argv[2],"PWM")){
        if(PinDef[pin].mode & PWM0A)value = EXT_PWM0A;
        else if(PinDef[pin].mode & PWM0B)value = EXT_PWM0B;
        else if(PinDef[pin].mode & PWM1A)value = EXT_PWM1A;
        else if(PinDef[pin].mode & PWM1B)value = EXT_PWM1B;
        else if(PinDef[pin].mode & PWM2A)value = EXT_PWM2A;
        else if(PinDef[pin].mode & PWM2B)value = EXT_PWM2B;
        else if(PinDef[pin].mode & PWM3A)value = EXT_PWM3A;
        else if(PinDef[pin].mode & PWM3B)value = EXT_PWM3B;
        else if(PinDef[pin].mode & PWM4A)value = EXT_PWM4A;
        else if(PinDef[pin].mode & PWM4B)value = EXT_PWM4B;
        else if(PinDef[pin].mode & PWM5A)value = EXT_PWM5A;
        else if(PinDef[pin].mode & PWM5B)value = EXT_PWM5B;
        else if(PinDef[pin].mode & PWM6A)value = EXT_PWM6A;
        else if(PinDef[pin].mode & PWM6B)value = EXT_PWM6B;
        else if(PinDef[pin].mode & PWM7A)value = EXT_PWM7A;
        else if(PinDef[pin].mode & PWM7B)value = EXT_PWM7B;
        else error("Invalid configuration");
    }  else if(checkstring(argv[2],"INT")){
        if(pin==Option.INT1pin)value = EXT_INT1;
        else if(pin==Option.INT2pin)value = EXT_INT2;
        else if(pin==Option.INT3pin)value = EXT_INT3;
        else if(pin==Option.INT4pin)value = EXT_INT4;
        else error("Invalid configuration");
    } else error("Syntax");
//        value = getint(argv[2], 1, 9);

    // check for any options
    switch(value) {
    	case EXT_ANA_IN:if(argc == 5) {
    						option = getint((argv[4]), 8, 12);
    						if(option & 1)error("Invalid bit count");
        					} else
        					option = 12;
        break;

        case EXT_DIG_IN:    if(argc == 5) {
                                if(checkstring(argv[4], "PULLUP")) option = CNPUSET;
                                else if(checkstring(argv[4], "PULLDOWN")) option = CNPDSET;
                                else error("Invalid configuration");
                            } else
                                option = 0;
                            break;
        case EXT_INT_HI:
        case EXT_INT_LO:
        case EXT_INT_BOTH:  if(argc == 7) {
                                if(checkstring(argv[6], "PULLUP")) option = CNPUSET;
                                else if(checkstring(argv[6], "PULLDOWN")) option = CNPDSET;
                                else error("Invalid configuration");
                            } else
                                option = 0;
                            break;
        case EXT_FREQ_IN:   if(argc == 5)
                                option = getint((argv[4]), 10, 100000);
                            else
                                option = 1000;
                            break;
        case EXT_PER_IN:   if(argc == 5)
                                option = getint((argv[4]), 1, 10000);
                            else
                                option = 1;
                            break;
        case EXT_CNT_IN:   if(argc == 5)
                                option = getint((argv[4]), 1, 5);
                            else
                                option = 1;
                            break;
        case EXT_DIG_OUT:
        case EXT_HEARTBEAT:   
                            option=0;
                            break;
        default:            if(argc > 3 && !value2) error("Unexpected text");
    }
    // this allows the user to set a software interrupt on the touch IRQ pin if the GUI environment is not enabled
    if(pin == Option.TOUCH_IRQ && Option.MaxCtrls==0) {
        if(value == EXT_INT_HI || value == EXT_INT_LO || value == EXT_INT_BOTH)
            ExtCurrentConfig[pin] = value;
        else if(value == EXT_NOT_CONFIG) {
            ExtCurrentConfig[pin] = EXT_BOOT_RESERVED;
            for(i = 0; i < NBRINTERRUPTS; i++)
                if(inttbl[i].pin == pin)
                    inttbl[i].pin = 0;                              // disable the software interrupt on this pin
        }
        else
            error("Pin % is reserved on startup", pin);
    } else

    {
        CheckPin(pin, CP_IGNORE_INUSE);
        ExtCfg(pin, value, option);
    }
    if(value2)    {
        CheckPin(pin2, CP_IGNORE_INUSE);
        ExtCfg(pin2, value2, option);
    }
    if(value3)    {
        CheckPin(pin3, CP_IGNORE_INUSE);
        ExtCfg(pin3, value3, option);
    }


	if(value == EXT_INT_HI || value == EXT_INT_LO || value == EXT_INT_BOTH) {
		// we need to set up a software interrupt
		if(argc < 5) error("Argument count");
        for(i = 0; i < NBRINTERRUPTS; i++) if(inttbl[i].pin == 0) break;
        if(i >= NBRINTERRUPTS) error("Too many interrupts");
        inttbl[i].pin = pin;
		inttbl[i].intp = GetIntAddress(argv[4]);					// get the interrupt routine's location
		inttbl[i].last = ExtInp(pin);								// save the current pin value for the first test
        switch(value) {                                             // and set trigger polarity
            case EXT_INT_HI:    inttbl[i].lohi = T_LOHI; break;
            case EXT_INT_LO:    inttbl[i].lohi = T_HILO; break;
            case EXT_INT_BOTH:  inttbl[i].lohi = T_BOTH; break;
        }
		InterruptUsed = true;
	}
}

void fun_pin(void) {
  #define ANA_AVERAGE     10
  #define ANA_DISCARD     2
	char code;
    int pin, i, j, b[ANA_AVERAGE];
    MMFLOAT t;
	if(checkstring(ep, "TEMP")){
        adc_select_input(4);
        last_adc=4;
        t=(MMFLOAT)adc_read()/4095.0*VCC;
        fret=(27.0-(t-0.706)/0.001721);
        targ=T_NBR;
        return;
    }
	if(!(code=codecheck(ep)))ep+=2;
	pin = getinteger(ep);
	if(!code)pin=codemap(pin);
    if(IsInvalidPin(pin)) error("Invalid pin");
    switch(ExtCurrentConfig[pin]) {
        case EXT_DIG_IN:
        case EXT_CNT_IN:
        case EXT_INT_HI:
        case EXT_INT_LO:
        case EXT_INT_BOTH:
        case EXT_DIG_OUT:
                            iret = ExtInp(pin);
                            targ = T_INT;
                            return;
        case EXT_PER_IN:	// if period measurement get the count and average it over the number of cycles
                            if(pin == Option.INT1pin) fret = (MMFLOAT)ExtInp(pin) / (MMFLOAT)INT1InitTimer;
                            else if(pin == Option.INT2pin)  fret = (MMFLOAT)ExtInp(pin) / (MMFLOAT)INT2InitTimer;
                            else if(pin == Option.INT3pin)  fret = (MMFLOAT)ExtInp(pin) / (MMFLOAT)INT3InitTimer;
                            else if(pin == Option.INT4pin)  fret = (MMFLOAT)ExtInp(pin) / (MMFLOAT)INT4InitTimer;
                            targ = T_NBR;
                            return;
        case EXT_FREQ_IN:	// if frequency measurement get the count and scale the reading
                            if(pin == Option.INT1pin) fret = (MMFLOAT)(ExtInp(pin)) * (MMFLOAT)1000.0 / (MMFLOAT)INT1InitTimer;
                            else if(pin == Option.INT2pin)  fret = (MMFLOAT)(ExtInp(pin)) * (MMFLOAT)1000.0 / (MMFLOAT)INT2InitTimer;
                            else if(pin == Option.INT3pin)  fret = (MMFLOAT)(ExtInp(pin)) * (MMFLOAT)1000.0 / (MMFLOAT)INT3InitTimer;
                            else if(pin == Option.INT4pin)  fret = (MMFLOAT)(ExtInp(pin)) * (MMFLOAT)1000.0 / (MMFLOAT)INT4InitTimer;
                            targ = T_NBR;
                            return;
        case EXT_ANA_IN:    
                            for(i = 0; i < ANA_AVERAGE; i++) {
                                b[i] = ExtInp(pin);                 // get the value
                                for(j = i; j > 0; j--) {            // and sort into position
                                    if(b[j - 1] < b[j]) {
                                        t = b[j - 1];
                                        b[j - 1] = b[j];
                                        b[j] = t;
                                    }
                                    else
                                        break;
                                }
                            }
                            // we then discard the top ANA_DISCARD samples and the bottom ANA_DISCARD samples and add up the remainder
                            for(j = 0, i = ANA_DISCARD; i < ANA_AVERAGE - ANA_DISCARD; i++) j += b[i];

                            // the total is averaged and scaled
                            fret = FMul((MMFLOAT)j , VCC) / (MMFLOAT)(4095 * (ANA_AVERAGE - ANA_DISCARD*2));
                            targ = T_NBR;
                            return;

        default:            error("Pin % is not an input",pin);
    }
}

int CheckPin(int pin, int action) {


    if(pin < 1 || pin > NBRPINS || (PinDef[pin].mode & UNUSED)) {
        if(!(action & CP_NOABORT)) error("Pin % is invalid", pin);
        return false;
    }

    if(!(action & CP_IGNORE_INUSE) && ExtCurrentConfig[pin] >= EXT_DS18B20_RESERVED && ExtCurrentConfig[pin] < EXT_COM_RESERVED) {
        if(!(action & CP_NOABORT)) error("Pin % is in use", pin);
        return false;
    }

    if(!(action & CP_IGNORE_BOOTRES) && ExtCurrentConfig[pin] >= EXT_BOOT_RESERVED) {
        if(!(action & CP_NOABORT)) {
            error("Pin % is reserved on startup", pin);
            uSec(1000000);
        }
        return false;
    }

    if(!(action & CP_IGNORE_RESERVED) && ExtCurrentConfig[pin] >= EXT_DS18B20_RESERVED) {
        if(!(action & CP_NOABORT)) error("Pin % is in use", pin);
        return false;
    }

    return true;
}
// this is invoked as a command (ie, port(3, 8) = Value)
// first get the arguments then step over the closing bracket.  Search through the rest of the command line looking
// for the equals sign and step over it, evaluate the rest of the command and set the pins accordingly
void cmd_port(void) {
	int pin, nbr, value, code, pincode;
    int i;
	getargs(&cmdline, NBRPINS * 4, ",");

	if((argc & 0b11) != 0b11) error("Invalid syntax");

    // step over the equals sign and get the value for the assignment
	while(*cmdline && tokenfunction(*cmdline) != op_equal) cmdline++;
	if(!*cmdline) error("Invalid syntax");
	++cmdline;
	if(!*cmdline) error("Invalid syntax");
	value = getinteger(cmdline);

    for(i = 0; i < argc; i += 4) {
    	code=0;
    	if(!(code=codecheck(argv[i])))argv[i]+=2;
    	pincode = getinteger(argv[i]);
        nbr = getinteger(argv[i + 2]);
        if(nbr < 0 || (pincode == 0 && code!=0) || (pincode<0)) error("Invalid argument");

        while(nbr) {
        	if(!code)pin=codemap(pincode);
        	else pin=pincode;
//        	PIntComma(pin);
            if(IsInvalidPin(pin) || !(ExtCurrentConfig[pin] == EXT_DIG_OUT )) error("Invalid output pin");
            ExtSet(pin, value & 1);
            value >>= 1;
            nbr--;
            pincode++;
        }
    } //MMPrintString("\r\n");
}


void fun_distance(void) {
    int trig, echo,techo;

	getargs(&ep, 3, ",");
	if((argc &1) != 1) error("Invalid syntax");
	char code;
	if(!(code=codecheck(argv[0])))argv[0]+=2;
	trig = getinteger(argv[0]);
	if(!code)trig=codemap(trig);
    if(argc == 3){
    	if(!(code=codecheck(argv[2])))argv[2]+=2;
    	echo = getinteger(argv[2]);
    	if(!code)echo=codemap(echo);
    }
    else
        echo = trig;                                                // they are the same if it is a 3-pin device
    if(IsInvalidPin(trig) || IsInvalidPin(echo)) error("Invalid pin |",echo);
    if(ExtCurrentConfig[trig] >= EXT_COM_RESERVED || ExtCurrentConfig[echo] >= EXT_COM_RESERVED)  error("Pin | is in use",trig);
    ExtCfg(echo, EXT_DIG_IN, CNPUSET);                              // setup the echo input
    PinSetBit(trig, LATCLR);                                        // trigger output must start low
    ExtCfg(trig, EXT_DIG_OUT, 0);                                   // setup the trigger output
    PinSetBit(trig, LATSET); uSec(20); PinSetBit(trig, LATCLR);     // pulse the trigger
    uSec(50);
    ExtCfg(echo, EXT_DIG_IN, CNPUSET);                              // this is in case the sensor is a 3-pin type
    uSec(50);
    PauseTimer = 0;                                                 // this is our timeout
    while(PinRead(echo)) if(PauseTimer > 50) { fret = -2; return; } // wait for the acknowledgement pulse start
    while(!PinRead(echo)) if(PauseTimer > 100) { fret = -2; return;}// then its end
    PauseTimer = 0;
    writeusclock(0);
    while(PinRead(echo)) {                                          // now wait for the echo pulse
        if(PauseTimer > 38) {                                       // timeout is 38mS
            fret = -1;
            return;
        }
    }
    techo=readusclock();
    // we have the echo, convert the time to centimeters
    fret = FDiv((MMFLOAT)techo,58.0);  //200 ticks per us, 58 us per cm
    targ = T_NBR;
}



// this is invoked as a function (ie, x = port(10,8) )
void fun_port(void) {
	int pin, nbr, i, value = 0, code, pincode;

	getargs(&ep, NBRPINS * 4, ",");
	if((argc & 0b11) != 0b11) error("Invalid syntax");

    for(i = argc - 3; i >= 0; i -= 4) {
    	code=0;
    	if(!(code=codecheck(argv[i])))argv[i]+=2;
        pincode = getinteger(argv[i]);
        nbr = getinteger(argv[i + 2]);
        if(nbr < 0 || (pincode == 0 && code!=0) || (pincode<0)) error("Invalid argument");
        pincode += nbr - 1;                                             // we start by reading the most significant bit

        while(nbr) {
        	if(!code)pin=codemap(pincode);
        	else pin=pincode;
            if(IsInvalidPin(pin) || !(ExtCurrentConfig[pin] == EXT_DIG_IN || ExtCurrentConfig[pin] == EXT_INT_HI || ExtCurrentConfig[pin] == EXT_INT_LO || ExtCurrentConfig[pin] == EXT_INT_BOTH)) error("Invalid input pin");
            value <<= 1;
            value |= PinRead(pin);
            nbr--;
            pincode--;
        }
    }

    iret = value;
    targ = T_INT;
}


void cmd_pulse(void) {
    int pin, i, x, y;
    MMFLOAT f;

	getargs(&cmdline, 3, ",");
	if(argc != 3) error("Invalid syntax");
	char code;
	if(!(code=codecheck(argv[0])))argv[0]+=2;
	pin = getinteger(argv[0]);
	if(!code)pin=codemap(pin);
	if(!(ExtCurrentConfig[pin] == EXT_DIG_OUT)) error("Pin | is not an output", pin);

    f = getnumber(argv[2]);                                         // get the pulse width
    if(f < 0) error("Number out of bounds");
    x = f;                                                          // get the integer portion (in mSec)
    y = (int)(FSub(f , (MMFLOAT)x) * 1000.0);                             // get the fractional portion (in uSec)

    for(i = 0; i < NBR_PULSE_SLOTS; i++)                            // search looking to see if the pin is in use
        if(PulseCnt[i] != 0 && PulsePin[i] == pin) {
            mT4IntEnable(0);       									// disable the timer interrupt to prevent any conflicts while updating
            PulseCnt[i] = x;                                        // and if the pin is in use, set its time to the new setting or reset if the user wants to terminate
            mT4IntEnable(1);
            if(x == 0) PinSetBit(PulsePin[i], LATINV);
            return;
        }

    if(x == 0 && y == 0) return;                                    // silently ignore a zero pulse width

    if(x < 3) {                                                     // if this is under 3 milliseconds just do it now
        PinSetBit(pin, LATINV);                    // starting edge of the pulse
        uSec(x * 1000 + y);
        PinSetBit(pin, LATINV);                    // finishing edge
        return;
    }

    for(i = 0; i < NBR_PULSE_SLOTS; i++)
        if(PulseCnt[i] == 0) break;                                 // find a spare slot

    if(i >= NBR_PULSE_SLOTS) error("Too many concurrent PULSE commands");

    PinSetBit(pin, LATINV);                        // starting edge of the pulse
    if(x == 1) uSec(500);                                           // prevent too narrow a pulse if there is just one count
    PulsePin[i] = pin;                                              // save the details
    PulseCnt[i] = x;
    PulseActive = true;
}


void fun_pulsin(void) { //allowas timeouts up to 10 seconds
    int pin, polarity;
    unsigned int t1, t2;

	getargs(&ep, 7, ",");
	if((argc &1) != 1 || argc < 3) error("Invalid syntax");
	char code;
	if(!(code=codecheck(argv[0])))argv[0]+=2;
	pin = getinteger(argv[0]);
	if(!code)pin=codemap(pin);
    if(IsInvalidPin(pin)) error("Invalid pin");
	if(ExtCurrentConfig[pin] != EXT_DIG_IN) error("Pin | is not an input",pin);
    polarity = getinteger(argv[2]);

    t1 = t2 = 100000;                                               // default timeout is 100mS
    if(argc >= 5) t1 = t2 = getint(argv[4], 5, 10000000);
    if(argc == 7) t2 = getint(argv[6], 5, 10000000);
    iret = -1;                                                      // in anticipation of a timeout
    writeusclock(0);
    if(polarity) {
        while(PinRead(pin)) if(readusclock() > t1) return;
        while(!PinRead(pin)) if(readusclock() > t1) return;
        writeusclock(0);
        while(PinRead(pin)) if(readusclock() > t2) return;
    } else {
        while(!PinRead(pin)) if(readusclock() > t1) return;
        while(PinRead(pin)) if(readusclock() > t1) return;
        writeusclock(0);
        while(!PinRead(pin)) if(readusclock() > t2) return;
    }
    t1 = readusclock();
    iret = t1;
    targ = T_INT;
}
/****************************************************************************************************************************
IR routines
*****************************************************************************************************************************/

void cmd_ir(void) {
    unsigned char *p;
    int i, pin, dev, cmd;
    if(checkstring(cmdline, "CLOSE")) {
        if(IrState == IR_CLOSED) error("Not Open");
        if(CallBackEnabled==1) gpio_set_irq_enabled_with_callback(PinDef[IRpin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &gpio_callback);
        else gpio_set_irq_enabled(PinDef[IRpin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
        IrInterrupt = NULL;
        CallBackEnabled &= (~1);
        ExtCfg(IRpin, EXT_NOT_CONFIG, 0);
    } else if((p = checkstring(cmdline, "SEND"))) {
        getargs(&p, 5, ",");
        char code;
        if(!(code=codecheck(argv[0])))argv[0]+=2;
        pin = getinteger(argv[0]);
        if(!code)pin=codemap(pin);
        if(IsInvalidPin(pin)) error("Invalid pin");
        dev = getint(argv[2], 0, 0b11111);
        cmd = getint(argv[4], 0, 0b1111111);
        if(ExtCurrentConfig[pin] >= EXT_COM_RESERVED)  error("Pin % is in use",pin);
        ExtCfg(pin, EXT_DIG_OUT, 0);
        cmd = (dev << 7) | cmd;
        IRSendSignal(pin, 186);
        for(i = 0; i < 12; i++) {
            uSec(600);
            if(cmd & 1)
                IRSendSignal(pin, 92);
            else
                IRSendSignal(pin, 46);
            cmd >>= 1;
        }
    } else {
        getargs(&cmdline, 5, ",");
        if(IRpin==99)error("Pin not configured for IR");
        if(IrState != IR_CLOSED) error("Already open");
        if(argc%2 == 0 || argc == 0) error("Invalid syntax");
        IrVarType = 0;
        IrDev = findvar(argv[0], V_FIND);
        if(vartbl[VarIndex].type & T_CONST) error("Cannot change a constant");
        if(vartbl[VarIndex].type & T_STR)  error("Invalid variable");
        if(vartbl[VarIndex].type & T_NBR) IrVarType |= 0b01;
        IrCmd = findvar(argv[2], V_FIND);
        if(vartbl[VarIndex].type & T_CONST) error("Cannot change a constant");
        if(vartbl[VarIndex].type & T_STR)  error("Invalid variable");
        if(vartbl[VarIndex].type & T_NBR) IrVarType |= 0b10;
        InterruptUsed = true;
        IrInterrupt = GetIntAddress(argv[4]);							// get the interrupt location
        IrInit();
    }
}


void IrInit(void) {
    writeusclock(0);
    if(ExtCurrentConfig[IRpin] >= EXT_COM_RESERVED)  error("Pin % is in use",IRpin);
    ExtCfg(IRpin, EXT_IR, 0);
    ExtCfg(IRpin, EXT_COM_RESERVED, 0);
    if(!CallBackEnabled){
        gpio_set_irq_enabled_with_callback(PinDef[IRpin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
        CallBackEnabled=1;
    } else {
        gpio_set_irq_enabled(PinDef[IRpin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
        CallBackEnabled|=1;
    }
    IrReset();
}


void IrReset(void) {
	IrState = IR_WAIT_START;
    IrCount = 0;
    writeIRclock(0);
}


// this modulates (at about 38KHz) the IR beam for transmit
// half_cycles is the number of half cycles to send.  ie, 186 is about 2.4mSec
void IRSendSignal(int pin, int half_cycles) {
    while(half_cycles--) {
        PinSetBit(pin, LATINV);
        uSec(13);
    }
}

void PWMoff(int slice){
    if(slice==0 && PWM0Apin!=99 && ExtCurrentConfig[PWM0Apin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM0Apin,EXT_NOT_CONFIG,0);
    }
    if(slice==0 && PWM0Bpin!=99 && ExtCurrentConfig[PWM0Bpin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM0Bpin,EXT_NOT_CONFIG,0);
    }
    if(slice==1 && PWM1Apin!=99 && ExtCurrentConfig[PWM1Apin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM1Apin,EXT_NOT_CONFIG,0);
    }
    if(slice==1 && PWM1Bpin!=99 && ExtCurrentConfig[PWM1Bpin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM1Bpin,EXT_NOT_CONFIG,0);
    }
    if(slice==2 && PWM2Apin!=99 && ExtCurrentConfig[PWM2Apin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM2Apin,EXT_NOT_CONFIG,0);
    }
    if(slice==2 && PWM2Bpin!=99 && ExtCurrentConfig[PWM2Bpin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM2Bpin,EXT_NOT_CONFIG,0);
    }
    if(slice==3 && PWM3Apin!=99 && ExtCurrentConfig[PWM3Apin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM3Apin,EXT_NOT_CONFIG,0);
    }
    if(slice==3 && PWM3Bpin!=99 && ExtCurrentConfig[PWM3Bpin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM3Bpin,EXT_NOT_CONFIG,0);
    }
    if(slice==4 && PWM4Apin!=99 && ExtCurrentConfig[PWM4Apin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM4Apin,EXT_NOT_CONFIG,0);
    }
    if(slice==4 && PWM4Bpin!=99 && ExtCurrentConfig[PWM4Bpin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM4Bpin,EXT_NOT_CONFIG,0);
    }
    if(slice==5 && PWM5Apin!=99 && ExtCurrentConfig[PWM5Apin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM5Apin,EXT_NOT_CONFIG,0);
    }
    if(slice==5 && PWM5Bpin!=99 && ExtCurrentConfig[PWM5Bpin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM5Bpin,EXT_NOT_CONFIG,0);
    }
    if(slice==6 && PWM6Apin!=99 && ExtCurrentConfig[PWM6Apin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM6Apin,EXT_NOT_CONFIG,0);
    }
    if(slice==6 && PWM6Bpin!=99 && ExtCurrentConfig[PWM6Bpin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM6Bpin,EXT_NOT_CONFIG,0);
    }
    if(slice==7 && PWM7Apin!=99 && ExtCurrentConfig[PWM7Apin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM7Apin,EXT_NOT_CONFIG,0);
    }
    if(slice==7 && PWM7Bpin!=99 && ExtCurrentConfig[PWM7Bpin] < EXT_BOOT_RESERVED){
        ExtCfg(PWM7Bpin,EXT_NOT_CONFIG,0);
    }
    pwm_set_enabled(slice, false);
}
#ifndef PICOMITEVGA
void cmd_backlight(void){
    if(!(Option.DISPLAY_TYPE<=I2C_PANEL || Option.DISPLAY_TYPE>=BufferedPanel ) && Option.DISPLAY_BL){
        MMFLOAT frequency=1000.0;
        getargs(&cmdline,1,",");
        MMFLOAT duty=getint(argv[0],0,100);
        int wrap=(Option.CPU_Speed*1000)/frequency;
        int high=(int)((MMFLOAT)Option.CPU_Speed/frequency*duty*10.0);
        int div=1;
        while(wrap>65535){
            wrap>>=1;
            if(duty>=0.0)high>>=1;
            div<<=1;
        }
        wrap--;
        if(div!=1)pwm_set_clkdiv(BacklightSlice,(float)div);
        pwm_set_wrap(BacklightSlice, wrap);
        pwm_set_chan_level(BacklightSlice, BacklightChannel, high);
    } else if(Option.DISPLAY_TYPE<=I2C_PANEL){
        getargs(&cmdline,1,",");
        int level=getint(argv[0],0,100);
        level*=255;
        level/=100;
        I2C_Send_Command(0x81);//SETCONTRAST
        I2C_Send_Command((uint8_t)level);
    } else if(Option.DISPLAY_TYPE>=SSDPANEL){
        SetBacklightSSD1963(getint(cmdline, 0, 100));
    } else if(Option.DISPLAY_TYPE==SSD1306SPI){
        getargs(&cmdline,1,",");
        int level=getint(argv[0],0,100);
        level*=255;
        level/=100;
        spi_write_command(0x81);//SETCONTRAST
        spi_write_command((uint8_t)level);
    } else error("Backlight not set up");
}
#endif

void cmd_pwm(void){
    char *tp;
    int div=1, high1, high2;
    MMFLOAT duty1=-1.0, duty2=-1.0;
    getargs(&cmdline,7,",");
    int CPU_Speed=frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    int slice=getint(argv[0],0,7);
    if(slice==BacklightSlice)error("Channel in use for backlight");
    if(tp=checkstring(argv[2],"OFF")){
        PWMoff(slice);
        if(slice==0)slice0=0;
        if(slice==1)slice1=0;
        if(slice==2)slice2=0;
        if(slice==3)slice3=0;
        if(slice==4)slice4=0;
        if(slice==5)slice5=0;
        if(slice==6)slice6=0;
        if(slice==7)slice7=0;
        return;
    }
    MMFLOAT frequency=getnumber(argv[2]);
    if(frequency>(MMFLOAT)(CPU_Speed>>2)*1000.0)error("Invalid frequency");
    if(*argv[4]){
        duty1=getnumber(argv[4]);
        if(duty1>100.0 || duty1<0.0)error("Syntax");
    }
    if(argc==7){
        duty2=getnumber(argv[6]);
        if(duty2>100.0 || duty2<0.0)error("Syntax");
    }
    int wrap=(CPU_Speed*1000)/frequency;
    if(duty1>=0.0)high1=(int)((MMFLOAT)CPU_Speed/frequency*duty1*10.0);
    if(duty2>=0.0)high2=(int)((MMFLOAT)CPU_Speed/frequency*duty2*10.0);
    while(wrap>65535){
        wrap>>=1;
        if(duty1>=0.0)high1>>=1;
        if(duty2>=0.0)high2>>=1;
        div<<=1;
    }
    if(div>256)error("Invalid frequency");
    wrap--;
    if(high1)high1--;
    if(high2)high2--;
    pwm_set_clkdiv(slice,(float)div);
    pwm_set_wrap(slice, wrap);
    if(slice==0 && PWM0Apin==99 && duty1>=0.0)error("Pin not set for PWM");
    if(slice==0 && PWM0Bpin==99 && duty2>=0.0)error("Pin not set for PWM");
    if(slice==1 && PWM1Apin==99 && duty1>=0.0)error("Pin not set for PWM");
    if(slice==1 && PWM1Bpin==99 && duty2>=0.0)error("Pin not set for PWM");
    if(slice==2 && PWM2Apin==99 && duty1>=0.0)error("Pin not set for PWM");
    if(slice==2 && PWM2Bpin==99 && duty2>=0.0)error("Pin not set for PWM");
    if(slice==3 && PWM3Apin==99 && duty1>=0.0)error("Pin not set for PWM");
    if(slice==3 && PWM3Bpin==99 && duty2>=0.0)error("Pin not set for PWM");
    if(slice==4 && PWM4Apin==99 && duty1>=0.0)error("Pin not set for PWM");
    if(slice==4 && PWM4Bpin==99 && duty2>=0.0)error("Pin not set for PWM");
    if(slice==5 && PWM5Apin==99 && duty1>=0.0)error("Pin not set for PWM");
    if(slice==5 && PWM5Bpin==99 && duty2>=0.0)error("Pin not set for PWM");
    if(slice==6 && PWM6Apin==99 && duty1>=0.0)error("Pin not set for PWM");
    if(slice==6 && PWM6Bpin==99 && duty2>=0.0)error("Pin not set for PWM");
    if(slice==7 && PWM7Apin==99 && duty1>=0.0)error("Pin not set for PWM");
    if(slice==7 && PWM7Bpin==99 && duty2>=0.0)error("Pin not set for PWM");
    if(slice==0 && PWM0Apin!=99 && duty1>=0.0){
        ExtCfg(PWM0Apin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_A, high1);
    }
    if(slice==0 && PWM0Bpin!=99 && duty2>=0.0){
        ExtCfg(PWM0Bpin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_B, high2);
    }
    if(slice==1 && PWM1Apin!=99 && duty1>=0.0){
        ExtCfg(PWM1Apin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_A, high1);
    }
    if(slice==1 && PWM1Bpin!=99 && duty2>=0.0){
        ExtCfg(PWM1Bpin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_B, high2);
    }
    if(slice==2 && PWM2Apin!=99 && duty1>=0.0){
        ExtCfg(PWM2Apin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_A, high1);
    }
    if(slice==2 && PWM2Bpin!=99 && duty2>=0.0){
        ExtCfg(PWM2Bpin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_B, high2);
    }
    if(slice==3 && PWM3Apin!=99 && duty1>=0.0){
        ExtCfg(PWM3Apin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_A, high1);
    }
    if(slice==3 && PWM3Bpin!=99 && duty2>=0.0){
        ExtCfg(PWM3Bpin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_B, high2);
    }
        if(slice==4 && PWM4Apin!=99 && duty1>=0.0){
        ExtCfg(PWM4Apin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_A, high1);
    }
    if(slice==4 && PWM4Bpin!=99 && duty2>=0.0){
        ExtCfg(PWM4Bpin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_B, high2);
    }
        if(slice==5 && PWM5Apin!=99 && duty1>=0.0){
        ExtCfg(PWM5Apin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_A, high1);
    }
    if(slice==5 && PWM5Bpin!=99 && duty2>=0.0){
        ExtCfg(PWM5Bpin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_B, high2);
    }
        if(slice==6 && PWM6Apin!=99 && duty1>=0.0){
        ExtCfg(PWM6Apin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_A, high1);
    }
    if(slice==6 && PWM6Bpin!=99 && duty2>=0.0){
        ExtCfg(PWM6Bpin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_B, high2);
    }
        if(slice==7 && PWM7Apin!=99 && duty1>=0.0){
        ExtCfg(PWM7Apin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_A, high1);
    }
    if(slice==7 && PWM7Bpin!=99 && duty2>=0.0){
        ExtCfg(PWM7Bpin,EXT_COM_RESERVED,0);
        pwm_set_chan_level(slice, PWM_CHAN_B, high2);
    }
        if(slice==0 && slice0==0){
        pwm_set_enabled(slice, true);
        slice0=1;
    }
        if(slice==1 && slice1==0){
        pwm_set_enabled(slice, true);
        slice1=1;
    }
        if(slice==2 && slice2==0){
        pwm_set_enabled(slice, true);
        slice2=1;
    }
        if(slice==3 && slice3==0){
        pwm_set_enabled(slice, true);
        slice3=1;
    }
        if(slice==4 && slice4==0){
        pwm_set_enabled(slice, true);
        slice4=1;
    }
        if(slice==5 && slice5==0){
        pwm_set_enabled(slice, true);
        slice5=1;
    }
        if(slice==6 && slice6==0){
        pwm_set_enabled(slice, true);
        slice6=1;
    }
        if(slice==7 && slice7==0){
        pwm_set_enabled(slice, true);
        slice7=1;
    }
}


/****************************************************************************************************************************
 The KEYPAD command
*****************************************************************************************************************************/

static char keypad_pins[8];
MMFLOAT *KeypadVar;
unsigned char *KeypadInterrupt = NULL;
void KeypadClose(void);

void cmd_keypad(void) {
    int i, j, code;

    if(checkstring(cmdline, "CLOSE"))
        KeypadClose();
    else {
        getargs(&cmdline, 19, ",");
        if(argc%2 == 0 || argc < 17) error("Invalid syntax");
        if(KeypadInterrupt != NULL) error("Already open");
        KeypadVar = findvar(argv[0], V_FIND);
        if(vartbl[VarIndex].type & T_CONST) error("Cannot change a constant");
        if(!(vartbl[VarIndex].type & T_NBR)) error("Floating point variable required");
        InterruptUsed = true;
        KeypadInterrupt = GetIntAddress(argv[2]);					// get the interrupt location
        for(i = 0; i < 8; i++) {
            if(i == 7 && argc < 19) {
                keypad_pins[i] = 0;
                break;
            }
        	code=0;
        	if(!(code=codecheck(argv[(i + 2) * 2])))argv[(i + 2) * 2]+=2;
        	j = getinteger(argv[(i + 2) * 2]);
        	if(!code)j=codemap(j);
            if(ExtCurrentConfig[j] >= EXT_COM_RESERVED)  error("Pin | is in use",j);
//            if(i < 4) {
            ExtCfg(j, EXT_DIG_IN, ODCSET);
            ExtCfg(j, EXT_COM_RESERVED, 0);
            keypad_pins[i] = j;
        }
    }
}


void KeypadClose(void) {
    int i;
    if(KeypadInterrupt == NULL) return;
    for(i = 0; i < 8; i++) {
        if(keypad_pins[i]) {
            ExtCfg(keypad_pins[i], EXT_NOT_CONFIG, 0);				// all set to unconfigured
        }
    }
    KeypadInterrupt = NULL;
}


int KeypadCheck(void) {
    static unsigned char count = 0, keydown = false;
    int i, j;
    const char PadLookup[16] = { 1, 2, 3, 20, 4, 5, 6, 21, 7, 8, 9, 22, 10, 0, 11, 23 };

    if(count++ % 64) return false;                                  // only check every 64 loops through the interrupt processor

    for(j = 4; j < 8; j++) {                                        // j controls the pull down pins
        if(keypad_pins[j]) {                                        // we might just have 3 pull down pins
            PinSetBit(keypad_pins[j], ODCCLR);                      // pull it low
            for(i = 0; i < 4; i++) {                                // i is the row sense inputs
                if(PinRead(keypad_pins[i]) == 0) {                  // if it is low we have found a keypress
                    if(keydown) goto exitcheck;                     // we have already reported this, so just exit
                    uSec(40 * 1000);                                // wait 40mS and check again
                    if(PinRead(keypad_pins[i]) != 0) goto exitcheck;// must be contact bounce if it is now high
                    *KeypadVar = PadLookup[(i << 2) | (j - 4)];     // lookup the key value and set the variable
                    PinSetBit(keypad_pins[j], ODCSET);
                    keydown = true;                                 // record that we know that the key is down
                    return true;                                    // and tell the interrupt processor that we are good to go
                }
            }
            PinSetBit(keypad_pins[j], ODCSET);                      // wasn't this pin, clear the pulldown
        }
    }
    keydown = false;                                                // no key down, record the fact
    return false;

exitcheck:
    PinSetBit(keypad_pins[j], ODCSET);
    return false;
}




/****************************************************************************************************************************
 The LCD command
*****************************************************************************************************************************/

void LCD_Nibble(int Data, int Flag, int Wait_uSec);
void LCD_Byte(int Data, int Flag, int Wait_uSec);
void LcdPinSet(int pin, int val);
static char lcd_pins[6];

void cmd_lcd(unsigned char *lcd)
 {
    unsigned char *p;
    int i, j, code;

    if((p = checkstring(lcd, "INIT"))) {
        getargs(&p, 11, ",");
        if(argc != 11) error("Invalid syntax");
        if(*lcd_pins) error("Already open");
        for(i = 0; i < 6; i++) {
        	code=0;
        	if(!(code=codecheck(argv[i * 2])))argv[i * 2]+=2;
            lcd_pins[i] = getinteger(argv[i * 2]);
        	if(!code)lcd_pins[i]=codemap(lcd_pins[i]);
            if(ExtCurrentConfig[(int)lcd_pins[i]] >= EXT_COM_RESERVED)  error("Pin | is in use",lcd_pins[i]);
            ExtCfg(lcd_pins[i], EXT_DIG_OUT, 0);
            ExtCfg(lcd_pins[i], EXT_COM_RESERVED, 0);
        }
        LCD_Nibble(0b0011, 0, 5000);                                // reset
        LCD_Nibble(0b0011, 0, 5000);                                // reset
        LCD_Nibble(0b0011, 0, 5000);                                // reset
        LCD_Nibble(0b0010, 0, 2000);                                // 4 bit mode
        LCD_Byte(0b00101100, 0, 600);                               // 4 bits, 2 lines
        LCD_Byte(0b00001100, 0, 600);                               // display on, no cursor
        LCD_Byte(0b00000110, 0, 600);                               // increment on write
        LCD_Byte(0b00000001, 0, 3000);                              // clear the display
        return;
    }

    if(!*lcd_pins) error("Not open");
    if(checkstring(lcd, "CLOSE")) {
        for(i = 0; i < 6; i++) {
			ExtCfg(lcd_pins[i], EXT_NOT_CONFIG, 0);					// all set to unconfigured
			ExtSet(lcd_pins[i], 0);									// all outputs (when set) default to low
            *lcd_pins = 0;
        }
    } else if((p = checkstring(lcd, "CLEAR"))) {                // clear the display
        LCD_Byte(0b00000001, 0, 3000);
    } else if((p = checkstring(cmdline, "CMD")) || (p = checkstring(cmdline, "DATA"))) { // send a command or data
        getargs(&p, MAX_ARG_COUNT * 2, ",");
        for(i = 0; i < argc; i += 2) {
            j = getint(argv[i], 0, 255);
            LCD_Byte(j, toupper(*cmdline) == 'D', 0);
        }
    } else {
        const char linestart[4] = {0, 64, 20, 84};
        int center, pos;

        getargs(&lcd, 5, ",");
        if(argc != 5) error("Invalid syntax");
        i = getint(argv[0], 1, 4);
        pos = 1;
        if(checkstring(argv[2], "C8"))
            center = 8;
        else if(checkstring(argv[2], "C16"))
            center = 16;
        else if(checkstring(argv[2], "C20"))
            center = 20;
        else if(checkstring(argv[2], "C40"))
            center = 40;
        else {
            center = 0;
            pos = getint(argv[2], 1, 256);
        }
        p = getstring(argv[4]);                                     // returns an MMBasic string
        i = 128 + linestart[i - 1] + (pos - 1);
        LCD_Byte(i, 0, 600);
        for(j = 0; j < (center - *p) / 2; j++) {
            LCD_Byte(' ', 1, 0);
        }
        for(i = 1; i <= *p; i++) {
            LCD_Byte(p[i], 1, 0);
            j++;
        }
        for(; j < center; j++) {
            LCD_Byte(' ', 1, 0);
        }
    }
}

void LCD_Nibble(int Data, int Flag, int Wait_uSec) {
    int i;
    LcdPinSet(lcd_pins[4], Flag);
    for(i = 0; i < 4; i++)
        LcdPinSet(lcd_pins[i], (Data >> i) & 1);
    LcdPinSet(lcd_pins[5], 1); uSec(250); LcdPinSet(lcd_pins[5], 0);
    if(Wait_uSec)
        uSec(Wait_uSec);
    else
        uSec(250);
}


void LCD_Byte(int Data, int Flag, int Wait_uSec) {
    LCD_Nibble(Data/16, Flag, 0);
    LCD_Nibble(Data, Flag, Wait_uSec);
}


void LcdPinSet(int pin, int val) {
    PinSetBit(pin, val ? LATSET : LATCLR);
}

void DHT22(unsigned char *p) {
     int pin;
    long long int r;
    int i, timeout, dht22=0;;
    MMFLOAT *temp, *humid;

    getargs(&p, 7, ",");
    if(!(argc == 5 || argc == 7)) error("Incorrect number of arguments");

    // get the two variables
	temp = findvar(argv[2], V_FIND);
	if(!(vartbl[VarIndex].type & T_NBR)) error("Invalid variable");
	humid = findvar(argv[4], V_FIND);
	if(!(vartbl[VarIndex].type & T_NBR)) error("Invalid variable");

    // get the pin number and set it up
    // get the pin number and set it up
	char code;
	if(!(code=codecheck(argv[0])))argv[0]+=2;
	pin = getinteger(argv[0]);
	if(!code)pin=codemap(pin);
    if(IsInvalidPin(pin)) error("Invalid pin");
    if(ExtCurrentConfig[pin] != EXT_NOT_CONFIG)  error("Pin % is in use");
    ExtCfg(pin, EXT_DIG_OUT, 0);
    
    if(argc==7){
    	dht22=getint(argv[6],0,1);
    }
    // pulse the pin low for 1mS
    uSec(1000+dht22*18000);
    writeusclock(0);
    timeout = 400;
    PinSetBit(pin, CNPUSET);
    PinSetBit(pin, TRISSET);
    uSec(5);
    // wait for the DHT22 to pull the pin low and return it high then take it low again
    while(PinRead(pin)) if(readusclock() > timeout) goto error_exit;
    while(!PinRead(pin)) if(readusclock() > timeout) goto error_exit;
    while(PinRead(pin)) if(readusclock() > timeout) goto error_exit;
//    PInt(readusclock());PRet();

    // now we wait for the pin to go high and measure how long it stays high (> 50uS is a one bit, < 50uS is a zero bit)
    for(r = i = 0; i < 40; i++) {
    	timeout=400;
        while(!PinRead(pin)) if(readusclock() > timeout) goto error_exit;
        timeout=400;writeusclock(0);
        while(PinRead(pin)) if(readusclock() > timeout) goto error_exit;
        r <<= 1;
        r |= (readusclock() > 50);
    }

    // we have all 40 bits
    // first validate against the checksum
    if( ( ( ((r >> 8) & 0xff) + ((r >> 16) & 0xff) + ((r >> 24) & 0xff) + ((r >> 32) & 0xff) ) & 0xff) != (r & 0xff)) goto error_exit;                                           // returning temperature
    if(dht22==0){
		*temp = (MMFLOAT)((r >> 8) &0x7fff) / 10.0;                       // get the temperature
		if((r >> 8) &0x8000) *temp = -*temp;                            // the top bit is the sign
		*humid = (MMFLOAT)(r >> 24) / 10.0;                               // get the humidity
    } else {
		*temp = (MMFLOAT)((signed char)((r>>16) & 0xFF));                       // get the temperature
		*humid = (MMFLOAT)((signed char)(r >> 32));                               // get the humidity
    }
    goto normal_exit;

error_exit:
    *temp = *humid = 1000.0;                                        // an obviously incorrect reading

normal_exit:
    ExtCfg(pin, EXT_NOT_CONFIG, 0);
    PinSetBit(pin, LATCLR);
}

void __not_in_flash_func(WS2812)(unsigned char *q){
       void *ptr1 = NULL;
        int64_t *dest=NULL;
        uint32_t pin, red , green, blue, colour;
        int T0H=0,T0L=0,T1H=0,T1L=0;
        char *p;
        int i, j, bit, nbr=0;
        int ticks_per_millisecond=ticks_per_second/1000; 
    	getargs(&q, 7, ",");
        if(argc != 7)error("Argument count");
    	p=argv[0];
    	if(toupper(*p)=='O'){
    		T0H=16777215 + setuptime-((7*ticks_per_millisecond)/20000) ;
    		T1H=16777215 + setuptime-((14*ticks_per_millisecond)/20000) ;
    		T0L=16777215 + setuptime-((13*ticks_per_millisecond)/20000) ;
    		T1L=16777215 + setuptime-((9*ticks_per_millisecond)/20000) ;
    	} else if(toupper(*p)=='B'){
    		T0H=16777215 + setuptime-((8*ticks_per_millisecond)/20000) ;
    		T1H=16777215 + setuptime-((16*ticks_per_millisecond)/20000) ;
    		T0L=16777215 + setuptime-((15*ticks_per_millisecond)/20000) ;
    		T1L=16777215 + setuptime-((6*ticks_per_millisecond)/20000) ;
    	} else if(toupper(*p)=='S'){
    		T0H=16777215 + setuptime-((8*ticks_per_millisecond)/20000) ;
    		T0L=16777215 + setuptime-((11*ticks_per_millisecond)/20000) ;
    		T1H=16777215 + setuptime-((17*ticks_per_millisecond)/20000) ;
    		T1L=16777215 + setuptime-((6*ticks_per_millisecond)/20000) ;
    	} else error("Syntax");
        nbr=getint(argv[4],1,256);
        if(nbr>1){
            ptr1 = findvar(argv[6], V_FIND | V_EMPTY_OK);
            if(vartbl[VarIndex].type & T_INT) {
                if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
                if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
                    error("Argument 1 must be integer array");
                } else {
                    if((vartbl[VarIndex].dims[0] - OptionBase + 1)<nbr)error("Array size");
                }
                dest = (long long int *)ptr1;
            } else error("Argument 1 must be integer array");
        } else {
            colour=getint(argv[6],0,0xFFFFFF);
            dest = (long long int *)&colour;
        }
        unsigned char code;
        if(!(code=codecheck(argv[2])))argv[2]+=2;
        pin = getinteger(argv[2]);
        if(!code)pin=codemap(pin);
        if(IsInvalidPin(pin)) error("Invalid pin");
        int gppin=PinDef[pin].GPno;
        if(!(ExtCurrentConfig[pin] == EXT_DIG_OUT || ExtCurrentConfig[pin] == EXT_NOT_CONFIG)) error("Pin is not off or an output");
        if(ExtCurrentConfig[pin] == EXT_NOT_CONFIG)ExtCfg(pin, EXT_DIG_OUT, 0);
		p=GetTempMemory((nbr+1)*3);
		uSec(60);
    	for(i=0;i<nbr;i++){
    		green=(dest[i]>>8) & 0xFF;
    		red=(dest[i]>>16) & 0xFF;
    		blue=dest[i] & 0xFF;
			p[0]=0;p[1]=0;p[2]=0;
    		for(j=0;j<8;j++){
    			bit=1<<j;
    			if( green &  (1<<(7-j)) )p[0] |= bit;
    			if(red   & (1<<(7-j)))p[1] |= bit;
    			if(blue  & (1<<(7-j)))p[2] |= bit;
    		}
    		p+=3;
    	}
    	p-=(nbr*3);
        disable_interrupts();
    	for(i=0;i<nbr*3;i++){
    		for(j=0;j<8;j++){
    			if(*p & 1){
                    gpio_put(gppin,true);
                    shortpause(T1H);
                    gpio_put(gppin,false);
                    shortpause(T1L);
    			} else {
                    gpio_put(gppin,true);
                    shortpause(T0H);
                    gpio_put(gppin,false);
                    shortpause(T0L);
    			}
    			*p>>=1;
    		}
    		p++;
    	}
        enable_interrupts();
}

unsigned short FloatToUint32(MMFLOAT x) {
    if(x<0 || x > 4294967295)
        error("Number range");
    return (x >= 0 ? (unsigned int)(x + 0.5) : (unsigned int)(x - 0.5)) ;
}

void __not_in_flash_func(cmd_bitbang)(void){
	unsigned char *tp;
	tp = checkstring(cmdline, "WS2812");
	if(tp) {
		WS2812(tp);
		return;
	}
	tp = checkstring(cmdline, "LCD");
	if(tp) {
		cmd_lcd(tp);
		return;
	}
	tp = checkstring(cmdline, "HUMID");
	if(tp) {
		DHT22(tp);
		return;
	}
	tp = checkstring(cmdline, "BITSTREAM");
	if(tp) {
		void *ptr1 = NULL;
		int i,num;
		uint32_t pin;
        int ticks_per_millisecond=ticks_per_second/1000;
		MMFLOAT *a1float=NULL;
		int64_t *a1int=NULL;
		unsigned int *data;
		getargs(&tp, 5,",");
		if(!(argc == 5)) error("Argument count");
		num=getint(argv[2],1,10000);
        unsigned char code;
        if(!(code=codecheck(argv[0])))argv[0]+=2;
        pin = getinteger(argv[0]);
        if(!code)pin=codemap(pin);
        if(IsInvalidPin(pin)) error("Invalid pin");
        int gppin=(1<<PinDef[pin].GPno);
        if(!(ExtCurrentConfig[pin] == EXT_DIG_OUT || ExtCurrentConfig[pin] == EXT_NOT_CONFIG)) error("Pin is not off or an output");
        if(ExtCurrentConfig[pin] == EXT_NOT_CONFIG)ExtCfg(pin, EXT_DIG_OUT, 0);
        ptr1 = findvar(argv[4], V_FIND | V_EMPTY_OK | V_NOFIND_ERR);
        if(vartbl[VarIndex].type & T_NBR) {
            if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
            if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
                error("Argument 2 must be an array");
            }
            if((vartbl[VarIndex].dims[0] - OptionBase) < num-1)error("Array too small");
            a1float = (MMFLOAT *)ptr1;
        } else if(vartbl[VarIndex].type & T_INT) {
            if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
            if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
                error("Argument 2 must be an array");
            }
            if((vartbl[VarIndex].dims[0] - OptionBase) < num-1)error("Array too small");
            a1int = (int64_t *)ptr1;
        } else error("Argument 2 must be an array");
        data=GetTempMemory(num * sizeof(unsigned int));
        if(a1float!=NULL){
            for(i=0; i< num;i++)data[i]= FloatToUint32(*a1float++);
        } else {
            for(i=0; i< num;i++){
                if(*a1int <0 || *a1int>67108)error("Number range");
                data[i]= *a1int++ ;
            }
        }
        for(i=0; i< num;i++){
            data[i]=16777215 + setuptime-((data[i]*ticks_per_millisecond)/1000) ;
        }
//        data[0]+=((ticks_per_millisecond/2000)+(250000-Option.CPU_Speed)/1000);
        disable_interrupts();
    	for(i=0;i<num;i++){
            gpio_xor_mask(gppin);
            shortpause(data[i])
    	}
        enable_interrupts();
		return;
	}
}
void cmd_adc(void){
	unsigned char *tp;
	tp = checkstring(cmdline, "OPEN");
	if(tp) {
        getargs(&tp,5,",");
        if(ADCopen)error("Already open");
        if(!(argc==3 || argc==5))error("Syntax");
        int nbr=getint(argv[2],1,4); //number of ADC channels
        frequency=(float)getnumber(argv[0])*nbr;
        if(frequency<48000000.0/65536.0 || frequency> 48000000.0/96.0)error("Invalid frequency");
        if(!(ExtCurrentConfig[31] == EXT_ANA_IN || ExtCurrentConfig[31] == EXT_NOT_CONFIG)) error("Pin GP26 is not off or an ADC input");
        if(ExtCurrentConfig[31] == EXT_NOT_CONFIG)ExtCfg(31, EXT_ANA_IN, 0);
        ExtCfg(31, EXT_COM_RESERVED, 0);
        if(nbr>=2){
            if(!(ExtCurrentConfig[32] == EXT_ANA_IN || ExtCurrentConfig[32] == EXT_NOT_CONFIG)) error("Pin GP27 is not off or an ADC input");
            if(ExtCurrentConfig[32] == EXT_NOT_CONFIG)ExtCfg(32, EXT_ANA_IN, 0);
            ExtCfg(32, EXT_COM_RESERVED, 0);
        }
        if(nbr>=3){
            if(!(ExtCurrentConfig[34] == EXT_ANA_IN || ExtCurrentConfig[34] == EXT_NOT_CONFIG)) error("Pin GP28 is not off or an ADC input");
            if(ExtCurrentConfig[34] == EXT_NOT_CONFIG)ExtCfg(34, EXT_ANA_IN, 0);
            ExtCfg(34, EXT_COM_RESERVED, 0);
        }
        if(nbr>=4){
            if(!(ExtCurrentConfig[44] == EXT_ANA_IN || ExtCurrentConfig[44] == EXT_NOT_CONFIG)) error("Pin GP29 is not off or an ADC input");
            if(ExtCurrentConfig[44] == EXT_NOT_CONFIG)ExtCfg(44, EXT_ANA_IN, 0);
            ExtCfg(44, EXT_COM_RESERVED, 0);
        }
        if(argc==5){
        	InterruptUsed = true;
        	ADCInterrupt = GetIntAddress(argv[4]);							// get the interrupt location
    	} else ADCInterrupt = NULL;
        ADCopen=nbr;
		return;
	}
	tp = checkstring(cmdline, "FREQUENCY");
	if(tp) {
        getargs(&tp,1,",");
        if(!ADCopen)error("Not open");
        float localfrequency=(float)getnumber(argv[0])*ADCopen;
        if(localfrequency<48000000.0/65536.0 || localfrequency> 48000000.0/96.0)error("Invalid frequency");
        frequency=localfrequency;
		return;
	}
	tp = checkstring(cmdline, "START");
	if(tp) {
        void *ptr1 = NULL;
        void *ptr2 = NULL;
        void *ptr3 = NULL;
        void *ptr4 = NULL;
        getargs(&tp, 7, ",");
		if(!ADCopen)error("ADC not open");
        if(!(argc >= 1))error("Argument count");
        a1float=NULL; a2float=NULL; a3float=NULL; a4float=NULL;
        ADCmax=0;
        ADCpos=0;
        ptr1 = findvar(argv[0], V_FIND | V_EMPTY_OK | V_NOFIND_ERR);
        if(vartbl[VarIndex].type & T_NBR) {
            if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
            if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
                error("Argument 1 must be float array");
            }
            a1float = (MMFLOAT *)ptr1;
        } else error("Argument 1 must be float array");
        ADCmax=(vartbl[VarIndex].dims[0] - OptionBase);
        if(argc>=3){
           if(ADCopen<2)error("Second channel not open");
           ptr2 = findvar(argv[2], V_FIND | V_EMPTY_OK | V_NOFIND_ERR);
            if(vartbl[VarIndex].type & T_NBR) {
                if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
                if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
                    error("Argument 2 must be float array");
                }
                a2float = (MMFLOAT *)ptr2;
            } else error("Argument 2 must be float array");
            if((vartbl[VarIndex].dims[0] - OptionBase) !=ADCmax)error("Arrays should be the same size");
        }
        if(argc>=5){
           if(ADCopen<3)error("Third channel not open");
           ptr3 = findvar(argv[4], V_FIND | V_EMPTY_OK | V_NOFIND_ERR);
            if(vartbl[VarIndex].type & T_NBR) {
                if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
                if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
                    error("Argument 3 must be float array");
                }
                a3float = (MMFLOAT *)ptr3;
            } else error("Argument 3 must be float array");
            if((vartbl[VarIndex].dims[0] - OptionBase) !=ADCmax)error("Arrays should be the same size");
        }
        if(argc>=7){
           if(ADCopen<4)error("Fourth channel not open");
           ptr4 = findvar(argv[6], V_FIND | V_EMPTY_OK | V_NOFIND_ERR);
            if(vartbl[VarIndex].type & T_NBR) {
                if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
                if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
                    error("Argument 4 must be float array");
                }
                a4float = (MMFLOAT *)ptr4;
            } else error("Argument 4 must be float array");
            if((vartbl[VarIndex].dims[0] - OptionBase) !=ADCmax)error("Arrays should be the same size");
        }
        ADCmax++;
        ADCbuffer=GetMemory(ADCmax*ADCopen*2);
        adc_init();
        adc_set_round_robin(ADCopen==1 ? 1 : ADCopen==2 ? 3 : ADCopen==3 ? 7 : 15);
        adc_fifo_setup(
            true,    // Write each completed conversion to the sample FIFO
            true,    // Enable DMA data request (DREQ)
            1,       // DREQ (and IRQ) asserted when at least 1 sample present
            false,   // We won't see the ERR bit because of 8 bit reads; disable.
            false     // Shift each sample to 8 bits when pushing to FIFO
        );
        float div=(500000.0/frequency*96.000);
        if(div==96.0)div=0;
        adc_set_clkdiv(div);
        // Set up the DMA to start transferring data as soon as it appears in FIFO
        dma_chan = 4;
        dma_channel_claim (dma_chan);
        dma_channel_config cfg = dma_channel_get_default_config(dma_chan);

        // Reading from constant address, writing to incrementing byte addresses
        channel_config_set_transfer_data_size(&cfg, DMA_SIZE_16);
        channel_config_set_read_increment(&cfg, false);
        channel_config_set_write_increment(&cfg, true);
        channel_config_set_irq_quiet(&cfg, true);

        // Pace transfers based on availability of ADC samples
        channel_config_set_dreq(&cfg, DREQ_ADC);

        dma_channel_configure(dma_chan, &cfg,
            (uint8_t *)ADCbuffer,    // dst
            &adc_hw->fifo,  // src
            ADCmax*ADCopen,  // transfer count
            true            // start immediately
        );
        adc_run(true);

        // Once DMA finishes, stop any new conversions from starting, and clean up
        // the FIFO in case the ADC was still mid-conversion.
        if(!ADCInterrupt){
            while(dma_channel_is_busy(dma_chan))tight_loop_contents();
            __compiler_memory_barrier();
            adc_run(false);
            adc_fifo_drain();
            int k=0;
            for(int i=0;i<ADCmax;i++){
                for(int j=0;j<ADCopen;j++){
                    if(j==0)*a1float++ = (MMFLOAT)ADCbuffer[k++]/4095.0*VCC;
                    if(j==1)*a2float++ = (MMFLOAT)ADCbuffer[k++]/4095.0*VCC;
                    if(j==2)*a3float++ = (MMFLOAT)ADCbuffer[k++]/4095.0*VCC;
                    if(j==3)*a4float++ = (MMFLOAT)ADCbuffer[k++]/4095.0*VCC;
                }
            }
            FreeMemory((void *)ADCbuffer);
            dma_channel_unclaim(dma_chan);
        } else dmarunning=1;
		return;
	}
	tp = checkstring(cmdline, "CLOSE");
	if(tp) {
        if(!ADCopen)error("Not open");
        adc_set_round_robin(0);
        adc_set_clkdiv(0);
        ExtCfg(31, EXT_NOT_CONFIG, 0);
        if(ADCopen>=2)ExtCfg(32, EXT_NOT_CONFIG, 0);
        if(ADCopen>=3)ExtCfg(34, EXT_NOT_CONFIG, 0);
        if(ADCopen>=4)ExtCfg(44, EXT_NOT_CONFIG, 0);
        ADCopen=0;
		return;
	}
    error("Syntax");
}
void ClearExternalIO(void) {
    int i;
  	CloseAudio(1);

    if(CallBackEnabled==1) gpio_set_irq_enabled_with_callback(PinDef[IRpin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &gpio_callback);
    else if(CallBackEnabled & 1){
        gpio_set_irq_enabled(PinDef[IRpin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
        CallBackEnabled &= (~1);
    }
    if(CallBackEnabled==2) gpio_set_irq_enabled_with_callback(PinDef[Option.INT1pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &gpio_callback);
    else if(CallBackEnabled & 2){
        gpio_set_irq_enabled(PinDef[Option.INT1pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
        CallBackEnabled &= (~2);
    }
    if(CallBackEnabled==2) gpio_set_irq_enabled_with_callback(PinDef[Option.INT2pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &gpio_callback);
    else if(CallBackEnabled & 4){
        gpio_set_irq_enabled(PinDef[Option.INT2pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
        CallBackEnabled &= (~4);
    }
    if(CallBackEnabled==8) gpio_set_irq_enabled_with_callback(PinDef[Option.INT3pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &gpio_callback);
    else  if(CallBackEnabled & 8){
        gpio_set_irq_enabled(PinDef[Option.INT3pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
        CallBackEnabled &= (~8);
    }
    if(CallBackEnabled==16) gpio_set_irq_enabled_with_callback(PinDef[Option.INT4pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &gpio_callback);
    else  if(CallBackEnabled & 16){
        gpio_set_irq_enabled(PinDef[Option.INT4pin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
        CallBackEnabled &= (~16);
    }
    CallBackEnabled &= (~32);
    for(i=0;i<MAXBLITBUF;i++){
    	blitbuffptr[i] = NULL;
    }
    KeypadClose();
    if(*lcd_pins){
        for(i = 0; i < 6; i++) {
          ExtCfg(lcd_pins[i], EXT_NOT_CONFIG, 0);                   // all set to unconfigured
            *lcd_pins = 0;
        }
    }
    memset(lcd_pins,0,sizeof(lcd_pins));
    SerialClose(1); 
    SerialClose(2);                                 // same for serial ports
    if(!I2C0locked)i2c_disable();   
    if(!I2C1locked)i2c2_disable();   
    SPIClose();
    SPI2Close();
    if(IRpin!=99){
        gpio_set_irq_enabled_with_callback(PinDef[IRpin].GPno, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &gpio_callback);
        IrInterrupt = NULL;
        ExtCfg(IRpin, EXT_NOT_CONFIG, 0);
    }
    ResetDisplay();
    IrState = IR_CLOSED;
    IrInterrupt = NULL;
    IrGotMsg = false;
	for(i = 1; i < NBRPINS ; i++) {
		if(CheckPin(i, CP_NOABORT | CP_IGNORE_INUSE | CP_IGNORE_RESERVED)) {    // don't reset invalid or boot reserved pins
          ExtCfg(i, EXT_NOT_CONFIG, 0);                                       // all set to unconfigured
		}
	}
	for(i = 0; i < NBRINTERRUPTS; i++) {
      inttbl[i].pin = 0;                                            // disable all interrupts
  	}

    if(CheckPin(41, CP_NOABORT | CP_IGNORE_INUSE | CP_IGNORE_RESERVED))ExtCfg(41,EXT_DIG_OUT,Option.PWM);
    if(CheckPin(42, CP_NOABORT | CP_IGNORE_INUSE | CP_IGNORE_RESERVED))ExtCfg(42,EXT_DIG_IN,0);
    if(CheckPin(43, CP_NOABORT | CP_IGNORE_INUSE | CP_IGNORE_RESERVED)){
        gpio_init(PinDef[HEARTBEATpin].GPno);
        gpio_set_dir(PinDef[HEARTBEATpin].GPno, GPIO_OUT);
        ExtCurrentConfig[PinDef[HEARTBEATpin].pin]=EXT_HEARTBEAT;
    }
    FreeMemorySafe((void **)&ds18b20Timers);
    if(CheckPin(44, CP_NOABORT | CP_IGNORE_INUSE | CP_IGNORE_RESERVED))ExtCfg(44,EXT_ANA_IN,0);
	InterruptReturn = NULL;
	InterruptUsed = false;
//  OnKeyGOSUB = NULL;
    KeypadInterrupt = NULL;

    for(i = 0; i < NBRSETTICKS; i++) TickInt[i] = NULL;
    for(i = 0; i < NBRSETTICKS; i++) TickActive[i] = 1;

	for(i = 0; i < NBR_PULSE_SLOTS; i++) PulseCnt[i] = 0;             // disable any pending pulse commands
    PulseActive = false;
    slice0=0;slice1=0;slice2=0;slice3=0;slice4=0;slice5=0;slice6=0;slice7=0;
    for(i=0; i<=7;i++)if(!(i==Option.AUDIO_SLICE || i==BacklightSlice))PWMoff(i);
    IRpin=99;
    PWM0Apin=99;
    PWM1Apin=99;
    PWM2Apin=99;
    PWM3Apin=99;
    PWM4Apin=99;
    PWM5Apin=99;
    PWM6Apin=99;
    PWM7Apin=99;
    PWM0Bpin=99;
    PWM1Bpin=99;
    PWM2Bpin=99;
    PWM3Bpin=99;
    PWM4Bpin=99;
    PWM5Bpin=99;
    PWM6Bpin=99;
    PWM7Bpin=99;
    UART1RXpin=99;
    UART1TXpin=99;
    UART0TXpin=99;
    UART0RXpin=99;
    SPI0TXpin=99;
    SPI0RXpin=99;
    SPI0SCKpin=99;
    SPI1TXpin=99;
    SPI1RXpin=99;
    SPI1SCKpin=99;
    if(!I2C0locked){
        I2C0SDApin=99;
        I2C0SCLpin=99;
    }
    if(!I2C1locked){
        I2C1SDApin=99;
        I2C1SCLpin=99;	
    }
    if(ADCopen) ExtCfg(31, EXT_NOT_CONFIG, 0);
    if(ADCopen>=2)ExtCfg(32, EXT_NOT_CONFIG, 0);
    if(ADCopen>=3)ExtCfg(34, EXT_NOT_CONFIG, 0);
//    if(ADCopen>=4)ExtCfg(44, EXT_NOT_CONFIG, 0);
    ADCopen=0;
    adc_set_round_robin(0);
    adc_set_clkdiv(0);
    if(dma_channel_is_claimed(dma_chan))dma_channel_unclaim(dma_chan);
    dmarunning=0;
    ADCInterrupt=NULL;
    KeyInterrupt=NULL;
    keyselect=0;
    CMM1=0;
}



void __not_in_flash_func(TM_EXTI_Handler_1)(void) {
	if(ExtCurrentConfig[Option.INT1pin] == EXT_PER_IN) {
        if(--INT1Timer <= 0) {
            INT1Value = INT1Count;
            INT1Timer = INT1InitTimer;
            INT1Count = 0;
        }
	}
    else {
        if(CFuncInt1)
            CallCFuncInt1();                                        // Hardware interrupt 2 for a CFunction (see CFunction.c)
        else
            INT1Count++;
    }
}



// perform the counting functions for INT2
void __not_in_flash_func(TM_EXTI_Handler_2)(void) {
    if(ExtCurrentConfig[Option.INT2pin] == EXT_PER_IN) {
        if(--INT2Timer <= 0) {
            INT2Value = INT2Count;
            INT2Timer = INT2InitTimer;
            INT2Count = 0;
        }
    }
    else {
        if(CFuncInt2)
            CallCFuncInt2();                                        // Hardware interrupt 2 for a CFunction (see CFunction.c)
        else
            INT2Count++;
    }
}




// perform the counting functions for INT3
void __not_in_flash_func(TM_EXTI_Handler_3)(void) {
	if(ExtCurrentConfig[Option.INT3pin] == EXT_PER_IN) {
        if(--INT3Timer <= 0) {
            INT3Value = INT3Count;
            INT3Timer = INT3InitTimer;
            INT3Count = 0;
        }
	}
	else
		INT3Count++;

}




// perform the counting functions for INT4
void __not_in_flash_func(TM_EXTI_Handler_4)(void) {
	if(ExtCurrentConfig[Option.INT4pin] == EXT_PER_IN) {
        if(--INT4Timer <= 0) {
            INT4Value = INT4Count;
            INT4Timer = INT4InitTimer;
            INT4Count = 0;
        }
	}
	else
		INT4Count++;

}
void __not_in_flash_func(IRHandler)(void) {
    int ElapsedMicroSec;
    static unsigned int LastIrBits;
        ElapsedMicroSec = readIRclock();
        switch(IrState) {
            case IR_WAIT_START:
                writeIRclock(0);                                           // reset the timer
                IrState = IR_WAIT_START_END;                        // wait for the end of the start bit
                break;
            case IR_WAIT_START_END:
                if(ElapsedMicroSec > 2000 && ElapsedMicroSec < 2800)
                    IrState = SONY_WAIT_BIT_START;                  // probably a Sony remote, now wait for the first data bit
                else if(ElapsedMicroSec > 8000 && ElapsedMicroSec < 10000)
                    IrState = NEC_WAIT_FIRST_BIT_START;             // probably an NEC remote, now wait for the first data bit
                else {
                    IrReset();                                      // the start bit was not valid
                    break;
                }
                IrCount = 0;                                        // count the bits in the message
                IrBits = 0;                                         // reset the bit accumulator
                writeIRclock(0);                                           // reset the timer
                break;
            case SONY_WAIT_BIT_START:
                if(ElapsedMicroSec < 300 || ElapsedMicroSec > 900) { IrReset(); break; }
                writeIRclock(0);                                           // reset the timer
                IrState = SONY_WAIT_BIT_END;                         // wait for the end of this data bit
                break;
            case SONY_WAIT_BIT_END:
                if(ElapsedMicroSec < 300 || ElapsedMicroSec > 1500 || IrCount > 20) { IrReset(); break; }
                IrBits |= (ElapsedMicroSec > 900) << IrCount;       // get the data bit
                IrCount++;                                          // and increment our count
                writeIRclock(0);                                           // reset the timer
                IrState = SONY_WAIT_BIT_START;                       // go back and wait for the next data bit
                break;
            case NEC_WAIT_FIRST_BIT_START:
            	if(ElapsedMicroSec > 2000 && ElapsedMicroSec < 2500) {
                    IrBits = LastIrBits;                            // key is held down so just repeat the last code
                    IrCount = 32;                                   // and signal that we are finished
                    IrState = NEC_WAIT_BIT_END;
                    break;
                }
                else if(ElapsedMicroSec > 4000 && ElapsedMicroSec < 5000)
                    IrState = NEC_WAIT_BIT_END;                     // wait for the end of this data bit
                else {
                    IrReset();                                      // the start bit was not valid
                    break;
                }
                writeIRclock(0);                                           // reset the timer
                break;
            case NEC_WAIT_BIT_START:
                if(ElapsedMicroSec < 400 || ElapsedMicroSec > 1800) { IrReset(); break; }
                IrBits |= (ElapsedMicroSec > 840) << (31 - IrCount);// get the data bit
                LastIrBits = IrBits;
                IrCount++;                                          // and increment our count
                writeIRclock(0);                                           // reset the timer
                IrState = NEC_WAIT_BIT_END;                         // wait for the end of this data bit
                break;
            case NEC_WAIT_BIT_END:
                if(ElapsedMicroSec < 400 || ElapsedMicroSec > 700) { IrReset(); break; }
                if(IrCount == 32) break;
                writeIRclock(0);                                           // reset the timer
                IrState = NEC_WAIT_BIT_START;                       // go back and wait for the next data bit
                break;
        }
    }
void __not_in_flash_func(gpio_callback)(uint gpio, uint32_t events) {
    if(gpio==PinDef[IRpin].GPno && (CallBackEnabled & 1))IRHandler();
    else if(gpio==PinDef[Option.INT1pin].GPno && (CallBackEnabled & 2))TM_EXTI_Handler_1();
    else if(gpio==PinDef[Option.INT2pin].GPno && (CallBackEnabled & 4))TM_EXTI_Handler_2();
    else if(gpio==PinDef[Option.INT3pin].GPno && (CallBackEnabled & 8))TM_EXTI_Handler_3();
    else if(gpio==PinDef[Option.INT4pin].GPno && (CallBackEnabled & 16))TM_EXTI_Handler_4();
    else if(Option.KeyboardConfig |= NO_KEYBOARD && gpio==PinDef[KEYBOARD_CLOCK].GPno && (CallBackEnabled & 32)) CNInterrupt();
    else error("Internal error");
}
