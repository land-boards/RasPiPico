/***********************************************************************************************************************
PicoMite MMBasic

I2C.c

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


#include "MMBasic_Includes.h"
#include "Hardware_Includes.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"

#define PinRead(a)  gpio_get(PinDef[a].GPno)

// Declare functions
void i2cEnable(unsigned char *p);
void i2cDisable(unsigned char *p);
void i2cSend(unsigned char *p);
void i2cSendSlave(unsigned char *p, int channel);
void i2cReceive(unsigned char *p);
void i2c_disable(void);
void i2c_enable(int bps);
void i2c_masterCommand(int timer);
void i2cCheck(unsigned char *p);
void i2c2Enable(unsigned char *p);
void i2c2Disable(unsigned char *p);
void i2c2Send(unsigned char *p);
void i2c2Receive(unsigned char *p);
void i2c2_disable(void);
void i2c2_enable(int bps);
void i2c2_masterCommand(int timer);
void i2c2Check(unsigned char *p);
static MMFLOAT *I2C_Rcvbuf_Float;										// pointer to the master receive buffer for a MMFLOAT
static long long int *I2C_Rcvbuf_Int;								// pointer to the master receive buffer for an integer
static char *I2C_Rcvbuf_String;										// pointer to the master receive buffer for a string
static unsigned int I2C_Addr;										// I2C device address
static volatile unsigned int I2C_Sendlen;							// length of the master send buffer
static volatile unsigned int I2C_Rcvlen;							// length of the master receive buffer
static unsigned char I2C_Send_Buffer[256];                                   // I2C send buffer
unsigned int I2C_enabled=0;									// I2C enable marker
unsigned int I2C_Timeout;									// master timeout value
volatile unsigned int I2C_Status;										// status flags
static int mmI2Cvalue;
	// value of MM.I2C
static MMFLOAT *I2C2_Rcvbuf_Float;										// pointer to the master receive buffer for a MMFLOAT
static long long int *I2C2_Rcvbuf_Int;								// pointer to the master receive buffer for an integer
static char *I2C2_Rcvbuf_String;										// pointer to the master receive buffer for a string
static unsigned int I2C2_Addr;										// I2C device address
static volatile unsigned int I2C2_Sendlen;							// length of the master send buffer
static volatile unsigned int I2C2_Rcvlen;							// length of the master receive buffer
static unsigned char I2C_Send_Buffer[256];                                   // I2C send buffer
unsigned int I2C2_enabled=0;									// I2C enable marker
unsigned int I2C2_Timeout;									// master timeout value
volatile unsigned int I2C2_Status;										// status flags
static char I2C_Rcv_Buffer[256];                                // I2C receive buffer
static unsigned int I2C_Slave_Addr;                                 // slave address
char *I2C_Slave_Send_IntLine;                                   // pointer to the slave send interrupt line number
char *I2C_Slave_Receive_IntLine;                                // pointer to the slave receive interrupt line number
static char I2C2_Rcv_Buffer[256];                                // I2C receive buffer
char *I2C2_Slave_Send_IntLine;                                   // pointer to the slave send interrupt line number
char *I2C2_Slave_Receive_IntLine;                                // pointer to the slave receive interrupt line number
static unsigned int I2C2_Slave_Addr;                                 // slave address
int noRTC=0;
extern void SaveToBuffer(void);
extern void CompareToBuffer(void);
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
extern void DrawRectangleMEM(int x1, int y1, int x2, int y2, int c);
extern void DrawBitmapMEM(int x1, int y1, int width, int height, int scale, int fc, int bc, unsigned char *bitmap);
void i2cSlave(unsigned char *p);
void i2c2Slave(unsigned char *p);
void i2cReceiveSlave(unsigned char *p, int channel);
	/*******************************************************************************************
							  I2C related commands in MMBasic
                              ===============================
These are the functions responsible for executing the I2C related commands in MMBasic
They are supported by utility functions that are grouped at the end of this file

********************************************************************************************/
void I2C_Send_Command(char command){
	int i2cret;
	int i2caddr=SSD1306_I2C_Addr;
	I2C_Send_Buffer[0]=0;
	I2C_Send_Buffer[1]=command;
	I2C_Sendlen=2;
	I2C_Timeout=1000;
	if(I2C1locked)i2cret=i2c_write_timeout_us(i2c1, (uint8_t)i2caddr, (uint8_t *)I2C_Send_Buffer, I2C_Sendlen,false, I2C_Timeout*1000);
	else i2cret=i2c_write_timeout_us(i2c0, (uint8_t)i2caddr, (uint8_t *)I2C_Send_Buffer, I2C_Sendlen,false, I2C_Timeout*1000);
	mmI2Cvalue=0;
	if(i2cret==PICO_ERROR_GENERIC)mmI2Cvalue=1;
	if(i2cret==PICO_ERROR_TIMEOUT)mmI2Cvalue=2;
//	mmI2Cvalue=HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)i2caddr, I2C_Send_Buffer, I2C_Sendlen, I2C_Timeout);
}
void I2C_Send_Data(unsigned char* data, int n){
	int i2cret;
	int i, i2caddr=SSD1306_I2C_Addr;
	I2C_Sendlen=n+1;
	I2C_Send_Buffer[0]=0x40;
	I2C_Timeout=1000;
	for(i=1;i<=n;i++){
		I2C_Send_Buffer[i]=data[i-1];
	}
	if(I2C1locked)i2cret=i2c_write_timeout_us(i2c1, (uint8_t)i2caddr, (uint8_t *)I2C_Send_Buffer, I2C_Sendlen,false, I2C_Timeout*1000);
	else i2cret=i2c_write_timeout_us(i2c0, (uint8_t)i2caddr, (uint8_t *)I2C_Send_Buffer, I2C_Sendlen,false, I2C_Timeout*1000);
	mmI2Cvalue=0;
	if(i2cret==PICO_ERROR_GENERIC)mmI2Cvalue=1;
	if(i2cret==PICO_ERROR_TIMEOUT)mmI2Cvalue=2;
}
#ifndef PICOMITEVGA
void ConfigDisplayI2C(unsigned char *p) {
	int DISPLAY_TYPE;
    getargs(&p, 5, ",");
    if(!(argc == 3 || argc == 5)) error("Argument count");
    if(checkstring(argv[0], "SSD1306I2C")) {
        DISPLAY_TYPE = SSD1306I2C;
    } else if(checkstring(argv[0], "SSD1306I2C32")) {
        DISPLAY_TYPE = SSD1306I2C32;
    } else
        error("Invalid display type");

    if(checkstring(argv[2], "L") || checkstring(argv[2], "LANDSCAPE"))
        Option.DISPLAY_ORIENTATION = LANDSCAPE;
    else if(checkstring(argv[2], "P") || checkstring(argv[2], "PORTRAIT"))
        Option.DISPLAY_ORIENTATION = PORTRAIT;
    else if(checkstring(argv[2], "RL") || checkstring(argv[2], "RLANDSCAPE"))
        Option.DISPLAY_ORIENTATION = RLANDSCAPE;
    else if(checkstring(argv[2], "RP") || checkstring(argv[2], "RPORTRAIT"))
        Option.DISPLAY_ORIENTATION = RPORTRAIT;
    else error("Orientation");
    Option.I2Coffset=0;
	if(argc==5)Option.I2Coffset=getint(argv[4],0,10);
	if(!(I2C0locked || I2C1locked))error("SYSTEM I2C not configured");
	Option.Refresh = 1;
	Option.DISPLAY_TYPE=DISPLAY_TYPE;
}

void InitDisplayI2C(int InitOnly){
	if(Option.DISPLAY_TYPE == 0 || Option.DISPLAY_TYPE > I2C_PANEL) return;
//	I2Con();
//	i2c_enable(display_details[Option.DISPLAY_TYPE].speed);
    DrawRectangle = DrawRectangleMEM;
    DrawBitmap = DrawBitmapMEM;
    DisplayHRes = display_details[Option.DISPLAY_TYPE].horizontal;
    DisplayVRes = display_details[Option.DISPLAY_TYPE].vertical;
    I2C_Send_Command(0xAE);//DISPLAYOFF
    I2C_Send_Command(0xD5);//DISPLAYCLOCKDIV
    I2C_Send_Command(0x80);//the suggested ratio &H80
    I2C_Send_Command(0xA8);//MULTIPLEX
    if(Option.DISPLAY_TYPE==SSD1306I2C)I2C_Send_Command(0x3F);
    else if(Option.DISPLAY_TYPE==SSD1306I2C32)I2C_Send_Command(0x1F);
    I2C_Send_Command(0xD3);//DISPLAYOFFSET
    I2C_Send_Command(0x0);//no offset
    I2C_Send_Command(0x40);//STARTLINE
    I2C_Send_Command(0x8D);//CHARGEPUMP
    I2C_Send_Command(0x14);
    I2C_Send_Command(0x20);//MEMORYMODE
    I2C_Send_Command(0x00);//&H0 act like ks0108
    I2C_Send_Command(0xA1);//SEGREMAP OR 1
    I2C_Send_Command(0xC8);//COMSCANDEC
    I2C_Send_Command(0xDA);//COMPINS
    if(Option.DISPLAY_TYPE==SSD1306I2C)I2C_Send_Command(0x12);
    else if(Option.DISPLAY_TYPE==SSD1306I2C32)I2C_Send_Command(0x02);
    I2C_Send_Command(0x81);//SETCONTRAST
    I2C_Send_Command(0xCF);
    I2C_Send_Command(0xd9);//SETPRECHARGE
    I2C_Send_Command(0xF1);
    I2C_Send_Command(0xDB);//VCOMDETECT
    I2C_Send_Command(0x40);
    I2C_Send_Command(0xA4);//DISPLAYALLON_RESUME
    I2C_Send_Command(0xA6);//NORMALDISPLAY
    I2C_Send_Command(0xAF);//DISPLAYON
    if(Option.DISPLAY_ORIENTATION & 1) {
        VRes=DisplayVRes;
        HRes=DisplayHRes;
    } else {
        VRes=DisplayHRes;
        HRes=DisplayVRes;
    }
    if(!InitOnly) {
    	ResetDisplay();
    	ClearScreen(0);
		Display_Refresh();
    }
}
#endif

void cmd_i2c(void) {
    unsigned char *p;//, *pp;
	if(I2C0SDApin==99 || I2C0SCLpin==99)error("Pin not set for I2C");

    if((p = checkstring(cmdline, "OPEN")) != NULL)
        i2cEnable(p);
    else if((p = checkstring(cmdline, "CLOSE")) != NULL)
        i2cDisable(p);
    else if((p = checkstring(cmdline, "WRITE")) != NULL)
        i2cSend(p);
    else if((p = checkstring(cmdline, "READ")) != NULL)
        i2cReceive(p);
    else if((p = checkstring(cmdline, "CHECK")) != NULL)
        i2cCheck(p);
    else if((p = checkstring(cmdline, "SLAVE OPEN")) != NULL)
        i2cSlave(p);
    else if((p = checkstring(cmdline, "SLAVE READ")) != NULL)
        i2cReceiveSlave(p,0);
    else if((p = checkstring(cmdline, "SLAVE WRITE")) != NULL)
        i2cSendSlave(p,0);
    else if((p = checkstring(cmdline, "SLAVE CLOSE")) != NULL)
        i2cDisable(p);
   else
        error("Unknown command");
}
void cmd_i2c2(void) {
    unsigned char *p;//, *pp;
	if(I2C1SDApin==99 || I2C1SCLpin==99)error("Pin not set for I2C2");

    if((p = checkstring(cmdline, "OPEN")) != NULL)
        i2c2Enable(p);
    else if((p = checkstring(cmdline, "CLOSE")) != NULL)
        i2c2Disable(p);
    else if((p = checkstring(cmdline, "WRITE")) != NULL)
        i2c2Send(p);
    else if((p = checkstring(cmdline, "READ")) != NULL)
        i2c2Receive(p);
    else if((p = checkstring(cmdline, "CHECK")) != NULL)
        i2c2Check(p);
    else if((p = checkstring(cmdline, "SLAVE OPEN")) != NULL)
        i2c2Slave(p);
    else if((p = checkstring(cmdline, "SLAVE READ")) != NULL)
        i2cReceiveSlave(p,1);
    else if((p = checkstring(cmdline, "SLAVE WRITE")) != NULL)
        i2cSendSlave(p,1);
    else if((p = checkstring(cmdline, "SLAVE CLOSE")) != NULL)
        i2c2Disable(p);
    else
        error("Unknown command");
}

void __not_in_flash_func(i2c0_irq_handler)(void) {
    // Get interrupt status
    uint32_t status = i2c0->hw->intr_stat;
    //is a write request? Or a read request ? event
    if (status & I2C_IC_INTR_STAT_R_RX_FULL_BITS) {
		i2c0->hw->intr_mask = I2C_IC_INTR_MASK_M_RD_REQ_BITS;
		I2C_Status |= I2C_Status_Slave_Receive_Rdy;
    } else if (status & I2C_IC_INTR_STAT_R_RD_REQ_BITS) {
        i2c0->hw->clr_rd_req;      
		I2C_Status |= I2C_Status_Slave_Send_Rdy;
    }
}
void __not_in_flash_func(i2c1_irq_handler)(void) {
    // Get interrupt status
    uint32_t status = i2c1->hw->intr_stat;
    //is a write request? Or a read request ? event
    if (status & I2C_IC_INTR_STAT_R_RX_FULL_BITS) {
		i2c1->hw->intr_mask = I2C_IC_INTR_MASK_M_RD_REQ_BITS;
		I2C2_Status |= I2C_Status_Slave_Receive_Rdy;
    } else if (status & I2C_IC_INTR_STAT_R_RD_REQ_BITS) {
        i2c1->hw->clr_rd_req;      
		I2C2_Status |= I2C_Status_Slave_Send_Rdy;
    }
}
void i2cSlave(unsigned char *p){
    int options, addr, mask;
    getargs(&p, 5, ",");
    if(argc != 5) error("Argument count");
    if(I2C_Status & I2C_Status_Slave) error("Slave already open");
    addr = getinteger(argv[0]);
    ExtCfg(I2C0SDApin, EXT_COM_RESERVED, 0);
    ExtCfg(I2C0SCLpin, EXT_COM_RESERVED, 0);
    gpio_pull_up(PinDef[I2C0SDApin].GPno);
    gpio_pull_up(PinDef[I2C0SCLpin].GPno);
	i2c_init(i2c0, 400000);
    I2C_Slave_Addr = addr;
    I2C_Slave_Send_IntLine = GetIntAddress(argv[2]);                // get the interrupt routine's location
    I2C_Slave_Receive_IntLine = GetIntAddress(argv[4]);             // get the interrupt routine's location
    InterruptUsed = true;
	i2c_set_slave_mode(i2c0,true, I2C_Slave_Addr);
	// Enable the I2C interrupts we want to process
	i2c0->hw->intr_mask = I2C_IC_INTR_STAT_R_RX_FULL_BITS | I2C_IC_INTR_MASK_M_RD_REQ_BITS;

	// Set up the interrupt handler to service I2C interrupts
	irq_set_exclusive_handler(I2C0_IRQ, i2c0_irq_handler);
	irq_set_enabled(I2C0_IRQ, true);
	I2C_Status=I2C_Status_Slave;

}
void i2c2Slave(unsigned char *p){
    int options, addr, mask;
    getargs(&p, 5, ",");
    if(argc != 5) error("Argument count");
    if(I2C2_Status & I2C_Status_Slave) error("Slave already open");
    addr = getinteger(argv[0]);
    ExtCfg(I2C1SDApin, EXT_COM_RESERVED, 0);
    ExtCfg(I2C1SCLpin, EXT_COM_RESERVED, 0);
    gpio_pull_up(PinDef[I2C1SDApin].GPno);
    gpio_pull_up(PinDef[I2C1SCLpin].GPno);
	i2c_init(i2c1, 400000);
    I2C2_Slave_Addr = addr;
    I2C2_Slave_Send_IntLine = GetIntAddress(argv[2]);                // get the interrupt routine's location
    I2C2_Slave_Receive_IntLine = GetIntAddress(argv[4]);             // get the interrupt routine's location
    InterruptUsed = true;
	i2c_set_slave_mode(i2c1,true, I2C2_Slave_Addr);
	// Enable the I2C interrupts we want to process
	i2c1->hw->intr_mask = I2C_IC_INTR_STAT_R_RX_FULL_BITS | I2C_IC_INTR_MASK_M_RD_REQ_BITS;

	// Set up the interrupt handler to service I2C interrupts
	irq_set_exclusive_handler(I2C1_IRQ, i2c1_irq_handler);
	irq_set_enabled(I2C1_IRQ, true);
	I2C2_Status=I2C_Status_Slave;

}
int DoRtcI2C(int addr) {
    if(I2C0locked){
    	I2C_Addr = addr;                                                // address of the device
		i2c_masterCommand(1);
	}
	else {
    	I2C2_Addr = addr;                                                // address of the device
		i2c2_masterCommand(1);
	}
    return !mmI2Cvalue;
}



void RtcGetTime(int noerror) {
    char *buff=GetTempMemory(STRINGSIZE);                                                   // Received data is stored here
    int DS1307;
	if(I2C0locked){
		I2C_Sendlen = 1;                                                // send one byte
		I2C_Rcvlen = 0;
		I2C_Status = 0;
		I2C_Send_Buffer[0] = 0;                                           // the first register to read
		if(!(DS1307 = DoRtcI2C(0x68))) {
			I2C_Send_Buffer[0] = 2;                                       // the first register is different for the PCF8563
			if(!DoRtcI2C(0x51)) goto error_exit;
		}
		I2C_Rcvbuf_String = buff;                                       // we want a string of bytes
		I2C_Rcvbuf_Float = NULL;
		I2C_Rcvbuf_Int = NULL;
		I2C_Rcvlen = 7;                                                 // get 7 bytes
		I2C_Sendlen = 0;
		if(!DoRtcI2C(DS1307 ? 0x68 : 0x51)) goto error_exit;
	} else {
		I2C2_Sendlen = 1;                                                // send one byte
		I2C2_Rcvlen = 0;
		I2C2_Status = 0;
		I2C_Send_Buffer[0] = 0;                                           // the first register to read
		if(!(DS1307 = DoRtcI2C(0x68))) {
			I2C_Send_Buffer[0] = 2;                                       // the first register is different for the PCF8563
			if(!DoRtcI2C(0x51)) goto error_exit;
		}
		I2C2_Rcvbuf_String = buff;                                       // we want a string of bytes
		I2C2_Rcvbuf_Float = NULL;
		I2C2_Rcvbuf_Int = NULL;
		I2C2_Rcvlen = 7;                                                 // get 7 bytes
		I2C2_Sendlen = 0;
		if(!DoRtcI2C(DS1307 ? 0x68 : 0x51)) goto error_exit;
	}
    mT4IntEnable(0);
    second = ((buff[0] & 0x7f) >> 4) * 10 + (buff[0] & 0x0f);
    minute = ((buff[1] & 0x7f) >> 4) * 10 + (buff[1] & 0x0f);
    hour = ((buff[2] & 0x3f) >> 4) * 10 + (buff[2] & 0x0f);
    day = ((buff[DS1307 ? 4:3] & 0x3f) >> 4) * 10 + (buff[DS1307 ? 4:3] & 0x0f);
    month = ((buff[5] & 0x1f) >> 4) * 10 + (buff[5] & 0x0f);
    year = (buff[6] >> 4) * 10 + (buff[6] & 0x0f) + 2000;
    mT4IntEnable(1);
    return;

error_exit:
	if(noerror){
		noRTC=1;
		return;
	}
    if(CurrentLinePtr) error("RTC not responding");
    if(Option.RTC){
		MMPrintString("RTC not responding");
    	MMPrintString("\r\n");
	}
}
// universal function to send/receive data to/from the RTC
// addr is the I2C address WITHOUT the read/write bit
char CvtToBCD(unsigned char *p, int min, int max) {
    long long int t;
    t = getint(p, min, max) % 100;
    return ((t / 10) << 4) | (t % 10);
}


char CvtCharsToBCD(unsigned char *p, int min, int max) {
    int t;
    t = (p[0] - '0') * 10 + (p[1] - '0');
//    dp("|%c|  |%c|  %d   %d   %d", p[0], p[1], t, min, max);
    if(!isdigit(p[0]) || !isdigit(p[1]) || t < min || t > max) error("Date/time format");
    return ((t / 10) << 4) | (t % 10);
}



void cmd_rtc(void) {
    char buff[7];                                                   // Received data is stored here
    int DS1307;
    unsigned char *p;
    void *ptr = NULL;
	if(!(I2C0locked || I2C1locked))error("SYSTEM I2C not configured");
    if(checkstring(cmdline, "GETTIME")) {
        RtcGetTime(0);
        return;
    }
    if((p = checkstring(cmdline, "SETTIME")) != NULL) {
		int Fulldate=0;
        getargs(&p, 11, ",");
		if(I2C0locked){
			if(argc == 1) {
				// single argument - assume the data is in DATETIME2 format used by GUI FORMATBOX
				p = getCstring(argv[0]);
				if(!(p[2] == '/' || p[2] == '-' )|| !(p[11] == ':' || p[13] == ':')) error("Date/time format");
				if(p[13] == ':')Fulldate=2;
				if(p[14 + Fulldate] == ':') 
					I2C_Send_Buffer[1] = CvtCharsToBCD(p + 15 + Fulldate, 0, 59);  // seconds
				else
					I2C_Send_Buffer[1] = 0;                             // seconds defaults to zero
				I2C_Send_Buffer[2] = CvtCharsToBCD(p + 12 + Fulldate, 0, 59);      // minutes
				I2C_Send_Buffer[3] = CvtCharsToBCD(p + 9 + Fulldate, 0, 23);       // hour
				I2C_Send_Buffer[5] = CvtCharsToBCD(p, 1, 31);           // day
				I2C_Send_Buffer[6] = CvtCharsToBCD(p + 3, 1, 12);       // month
				I2C_Send_Buffer[7] = CvtCharsToBCD(p + 6 + Fulldate, 0, 99);       // year
			} else  {
				// multiple arguments - data should be in the original yy, mm, dd, etc format
				if(argc != 11) error("Argument count");
				I2C_Send_Buffer[1] = CvtToBCD(argv[10], 0, 59);         // seconds
				I2C_Send_Buffer[2] = CvtToBCD(argv[8], 0, 59);          // minutes
				I2C_Send_Buffer[3] = CvtToBCD(argv[6], 0, 23);          // hour
				I2C_Send_Buffer[5] = CvtToBCD(argv[4], 1, 31);          // day
				I2C_Send_Buffer[6] = CvtToBCD(argv[2], 1, 12);          // month
				I2C_Send_Buffer[7] = CvtToBCD(argv[0], 0, 2099);        // year
			}
			I2C_Send_Buffer[0] = 0;                                     // turn off the square wave
			I2C_Send_Buffer[4] = 1;
			I2C_Rcvlen = 0;
			I2C_Sendlen = 9;                                            // send 7 bytes
			if(!DoRtcI2C(0x68)) {
				I2C_Send_Buffer[9] = I2C_Send_Buffer[7];                // year
				I2C_Send_Buffer[8] = I2C_Send_Buffer[6];                // month
				I2C_Send_Buffer[7] = 1;
				I2C_Send_Buffer[6] = I2C_Send_Buffer[5];                // day
				I2C_Send_Buffer[5] = I2C_Send_Buffer[3];                // hour
				I2C_Send_Buffer[4] = I2C_Send_Buffer[2];                // minutes
				I2C_Send_Buffer[3] = I2C_Send_Buffer[1];                // seconds
				I2C_Send_Buffer[0] = I2C_Send_Buffer[1] = I2C_Send_Buffer[2] = 0;  // set the register pointer to the first register then zero the first two registers
				I2C_Sendlen = 10;                                       // send 10 bytes
				if(!DoRtcI2C(0x51)) error("RTC not responding");
			}
		} else {
			if(argc == 1) {
				// single argument - assume the data is in DATETIME2 format used by GUI FORMATBOX
				p = getCstring(argv[0]);
				if(!(p[2] == '/' || p[2] == '-' )|| !(p[11] == ':' || p[13] == ':')) error("Date/time format");
				if(p[13] == ':')Fulldate=2;
				if(p[14 + Fulldate] == ':') 
					I2C_Send_Buffer[1] = CvtCharsToBCD(p + 15 + Fulldate, 0, 59);  // seconds
				else
					I2C_Send_Buffer[1] = 0;                             // seconds defaults to zero
				I2C_Send_Buffer[2] = CvtCharsToBCD(p + 12 + Fulldate, 0, 59);      // minutes
				I2C_Send_Buffer[3] = CvtCharsToBCD(p + 9 + Fulldate, 0, 23);       // hour
				I2C_Send_Buffer[5] = CvtCharsToBCD(p, 1, 31);           // day
				I2C_Send_Buffer[6] = CvtCharsToBCD(p + 3, 1, 12);       // month
				I2C_Send_Buffer[7] = CvtCharsToBCD(p + 6 + Fulldate, 0, 99);       // year
			} else  {
				// multiple arguments - data should be in the original yy, mm, dd, etc format
				if(argc != 11) error("Argument count");
				I2C_Send_Buffer[1] = CvtToBCD(argv[10], 0, 59);         // seconds
				I2C_Send_Buffer[2] = CvtToBCD(argv[8], 0, 59);          // minutes
				I2C_Send_Buffer[3] = CvtToBCD(argv[6], 0, 23);          // hour
				I2C_Send_Buffer[5] = CvtToBCD(argv[4], 1, 31);          // day
				I2C_Send_Buffer[6] = CvtToBCD(argv[2], 1, 12);          // month
				I2C_Send_Buffer[7] = CvtToBCD(argv[0], 0, 2099);        // year
			}
			I2C_Send_Buffer[0] = 0;                                     // turn off the square wave
			I2C_Send_Buffer[4] = 1;
			I2C2_Rcvlen = 0;
			I2C2_Sendlen = 9;                                            // send 7 bytes
			if(!DoRtcI2C(0x68)) {
				I2C_Send_Buffer[9] = I2C_Send_Buffer[7];                // year
				I2C_Send_Buffer[8] = I2C_Send_Buffer[6];                // month
				I2C_Send_Buffer[7] = 1;
				I2C_Send_Buffer[6] = I2C_Send_Buffer[5];                // day
				I2C_Send_Buffer[5] = I2C_Send_Buffer[3];                // hour
				I2C_Send_Buffer[4] = I2C_Send_Buffer[2];                // minutes
				I2C_Send_Buffer[3] = I2C_Send_Buffer[1];                // seconds
				I2C_Send_Buffer[0] = I2C_Send_Buffer[1] = I2C_Send_Buffer[2] = 0;  // set the register pointer to the first register then zero the first two registers
				I2C2_Sendlen = 10;                                       // send 10 bytes
				if(!DoRtcI2C(0x51)) error("RTC not responding");
			}
		}
        RtcGetTime(0);
    } else if((p = checkstring(cmdline, "GETREG")) != NULL) {
        getargs(&p, 3, ",");
        if(argc != 3) error("Argument count");
		if(I2C0locked){
			I2C_Sendlen = 1;                                            // send one byte
			I2C_Rcvlen = 0;
			*I2C_Send_Buffer = getint(argv[0], 0, 255);                 // the register to read
		} else {
			I2C2_Sendlen = 1;                                            // send one byte
			I2C2_Rcvlen = 0;
			*I2C_Send_Buffer = getint(argv[0], 0, 255);                 // the register to read
		}
        ptr = findvar(argv[2], V_FIND);
        if(vartbl[VarIndex].type & T_CONST) error("Cannot change a constant");
        if(vartbl[VarIndex].type & T_STR)  error("Invalid variable");

        if(!(DS1307 = DoRtcI2C(0x68))) {
            if(!DoRtcI2C(0x51)) error("RTC not responding");
        }
		if(I2C0locked){
			I2C_Rcvbuf_String = buff;                                   // we want a string of bytes
			I2C_Rcvbuf_Float = NULL;
			I2C_Rcvbuf_Int = NULL;
			I2C_Rcvlen = 1;                                             // get 1 byte
			I2C_Sendlen = 0;
		} else {
			I2C2_Rcvbuf_String = buff;                                   // we want a string of bytes
			I2C2_Rcvbuf_Float = NULL;
			I2C2_Rcvbuf_Int = NULL;
			I2C2_Rcvlen = 1;                                             // get 1 byte
			I2C2_Sendlen = 0;
		}
        if(!DoRtcI2C(DS1307 ? 0x68 : 0x51)) error("RTC not responding1");
        if(vartbl[VarIndex].type & T_NBR)
            *(MMFLOAT *)ptr = buff[0];
        else
            *(long long int *)ptr = buff[0];
    } else if((p = checkstring(cmdline, "SETREG")) != NULL) {
        getargs(&p, 3, ",");
        if(argc != 3) error("Argument count");
		if(I2C0locked){
			I2C_Rcvlen = 0;
			I2C_Send_Buffer[0] = getint(argv[0], 0, 255);               // set the register pointer
			I2C_Send_Buffer[1] = getint(argv[2], 0, 255);               // and the data to be written
			I2C_Sendlen = 2;                                            // send 2 bytes
		} else {
			I2C2_Rcvlen = 0;
			I2C_Send_Buffer[0] = getint(argv[0], 0, 255);               // set the register pointer
			I2C_Send_Buffer[1] = getint(argv[2], 0, 255);               // and the data to be written
			I2C2_Sendlen = 2;                                            // send 2 bytes
		}
        if(!DoRtcI2C(0x68)) {
            if(!DoRtcI2C(0x51)) error("RTC not responding");
        }
    } else
        error("Unknown command");

}

// enable the I2C1 module - master mode
void i2cEnable(unsigned char *p) {
	int speed, timeout;
	getargs(&p, 3, ",");
	if(argc != 3) error("Invalid syntax");
	speed = getinteger(argv[0]);
	if(!(speed ==100 || speed == 400)) error("Valid speeds 100, 400");
	timeout = getinteger(argv[2]);
	if(timeout < 0 || (timeout > 0 && timeout < 100)) error("Number out of bounds" );
	if(I2C_enabled || I2C_Status & I2C_Status_Slave) error("I2C already OPEN");
	I2C_Timeout = timeout;
	i2c_enable(speed);

}
// enable the I2C1 module - master mode
void i2c2Enable(unsigned char *p) {
	int speed, timeout;
	getargs(&p, 3, ",");
	if(argc != 3) error("Invalid syntax");
	speed = getinteger(argv[0]);
	if(!(speed ==100 || speed == 400)) error("Valid speeds 100, 400");
	timeout = getinteger(argv[2]);
	if(timeout < 0 || (timeout > 0 && timeout < 100)) error("Number out of bounds" );
	if(I2C2_enabled || I2C2_Status & I2C_Status_Slave) error("I2C already OPEN");
	I2C2_Timeout = timeout;
	i2c2_enable(speed);

}


// disable the I2C1 module - master mode
void i2cDisable(unsigned char *p) {
	if(!I2C0locked)	i2c_disable();
	else error("Allocated to System I2C");
}

// disable the I2C1 module - master mode
void i2c2Disable(unsigned char *p) {
	if(!I2C1locked)	i2c2_disable();
	else error("Allocated to System I2C");
}

// send data to an I2C slave - master mode
void i2cSend(unsigned char *p) {
	int addr, i2c_options, sendlen, i;
	void *ptr = NULL;
	unsigned char *cptr = NULL;

	getargs(&p, 99, ",");
	if(!(argc & 0x01) || (argc < 7)) error("Invalid syntax");
	if(!I2C_enabled)error("I2C not open");
	addr = getinteger(argv[0]);
	i2c_options = getinteger(argv[2]);
	if(i2c_options < 0 || i2c_options > 3) error("Number out of bounds");
	I2C_Status = 0;
	if(i2c_options & 0x01) I2C_Status = I2C_Status_BusHold;
	I2C_Addr = addr;
	sendlen = getinteger(argv[4]);
	if(sendlen < 1 || sendlen > 255) error("Number out of bounds");

	if(sendlen == 1 || argc > 7) {		// numeric expressions for data
		if(sendlen != ((argc - 5) >> 1)) error("Incorrect argument count");
		for (i = 0; i < sendlen; i++) {
			I2C_Send_Buffer[i] = getinteger(argv[i + i + 6]);
		}
	} else {		// an array of MMFLOAT, integer or a string
		ptr = findvar(argv[6], V_NOFIND_NULL | V_EMPTY_OK);
		if(ptr == NULL) error("Invalid variable");
		if((vartbl[VarIndex].type & T_STR) && vartbl[VarIndex].dims[0] == 0) {		// string
			cptr = (unsigned char *)ptr;
			cptr++;																	// skip the length byte in a MMBasic string
			for (i = 0; i < sendlen; i++) {
				I2C_Send_Buffer[i] = (int)(*(cptr + i));
			}
		} else if((vartbl[VarIndex].type & T_NBR) && vartbl[VarIndex].dims[0] > 0 && vartbl[VarIndex].dims[1] == 0) {		// numeric array
			if( (((MMFLOAT *)ptr - vartbl[VarIndex].val.fa) + sendlen) > (vartbl[VarIndex].dims[0] + 1 - OptionBase) ) {
				error("Insufficient data");
			} else {
				for (i = 0; i < sendlen; i++) {
					I2C_Send_Buffer[i] = (int)(*((MMFLOAT *)ptr + i));
				}
			}
		} else if((vartbl[VarIndex].type & T_INT) && vartbl[VarIndex].dims[0] > 0 && vartbl[VarIndex].dims[1] == 0) {		// integer array
			if( (((long long int *)ptr - vartbl[VarIndex].val.ia) + sendlen) > (vartbl[VarIndex].dims[0] + 1 - OptionBase) ) {
				error("Insufficient data");
			} else {
				for (i = 0; i < sendlen; i++) {
					I2C_Send_Buffer[i] = (int)(*((long long int *)ptr + i));
				}
			}
		} else error("Invalid variable");
	}
	I2C_Sendlen = sendlen;
	I2C_Rcvlen = 0;

	i2c_masterCommand(1);
}
// send data to an I2C slave - master mode
void i2cSendSlave(unsigned char *p, int channel) {
	int addr, i2c_options, sendlen, i;
	void *ptr = NULL;
	unsigned char *cptr = NULL;
	getargs(&p, 99, ",");
	if(!(argc >=3)) error("Invalid syntax");
	if(!((I2C_Status & I2C_Status_Slave && channel==0) || (I2C2_Status & I2C_Status_Slave && channel==1)))error("I2C slave not open");
	unsigned char *bbuff;
	if(channel==0){
		bbuff=I2C_Send_Buffer;
	} else {
		bbuff=I2C_Send_Buffer;
	}
	sendlen = getinteger(argv[0]);
	if(sendlen < 1 || sendlen > 255) error("Number out of bounds");

	if(sendlen == 1 || argc > 3) {		// numeric expressions for data
		if(sendlen != ((argc - 1) >> 1)) error("Incorrect argument count");
		for (i = 0; i < sendlen; i++) {
			bbuff[i] = getinteger(argv[i + i + 2]);
		}
	} else {		// an array of MMFLOAT, integer or a string
		ptr = findvar(argv[2], V_NOFIND_NULL | V_EMPTY_OK);
		if(ptr == NULL) error("Invalid variable");
		if((vartbl[VarIndex].type & T_STR) && vartbl[VarIndex].dims[0] == 0) {		// string
			cptr = (unsigned char *)ptr;
			cptr++;																	// skip the length byte in a MMBasic string
			for (i = 0; i < sendlen; i++) {
				bbuff[i] = (int)(*(cptr + i));
			}
		} else if((vartbl[VarIndex].type & T_NBR) && vartbl[VarIndex].dims[0] > 0 && vartbl[VarIndex].dims[1] == 0) {		// numeric array
			if( (((MMFLOAT *)ptr - vartbl[VarIndex].val.fa) + sendlen) > (vartbl[VarIndex].dims[0] + 1 - OptionBase) ) {
				error("Insufficient data");
			} else {
				for (i = 0; i < sendlen; i++) {
					bbuff[i] = (int)(*((MMFLOAT *)ptr + i));
				}
			}
		} else if((vartbl[VarIndex].type & T_INT) && vartbl[VarIndex].dims[0] > 0 && vartbl[VarIndex].dims[1] == 0) {		// integer array
			if( (((long long int *)ptr - vartbl[VarIndex].val.ia) + sendlen) > (vartbl[VarIndex].dims[0] + 1 - OptionBase) ) {
				error("Insufficient data");
			} else {
				for (i = 0; i < sendlen; i++) {
					bbuff[i] = (int)(*((long long int *)ptr + i));
				}
			}
		} else error("Invalid variable");
	}
	if(channel==0)i2c_write_raw_blocking(i2c0, bbuff, sendlen);
	else i2c_write_raw_blocking(i2c1, bbuff, sendlen);
}
// send data to an I2C slave - master mode
void i2c2Send(unsigned char *p) {
	int addr, i2c2_options, sendlen, i;
	void *ptr = NULL;
	unsigned char *cptr = NULL;

	getargs(&p, 99, ",");
	if(!(argc & 0x01) || (argc < 7)) error("Invalid syntax");
	if(!I2C2_enabled)error("I2C not open");
	addr = getinteger(argv[0]);
	i2c2_options = getinteger(argv[2]);
	if(i2c2_options < 0 || i2c2_options > 3) error("Number out of bounds");
	I2C2_Status = 0;
	if(i2c2_options & 0x01) I2C2_Status = I2C_Status_BusHold;
	I2C2_Addr = addr;
	sendlen = getinteger(argv[4]);
	if(sendlen < 1 || sendlen > 255) error("Number out of bounds");

	if(sendlen == 1 || argc > 7) {		// numeric expressions for data
		if(sendlen != ((argc - 5) >> 1)) error("Incorrect argument count");
		for (i = 0; i < sendlen; i++) {
			I2C_Send_Buffer[i] = getinteger(argv[i + i + 6]);
		}
	} else {		// an array of MMFLOAT, integer or a string
		ptr = findvar(argv[6], V_NOFIND_NULL | V_EMPTY_OK);
		if(ptr == NULL) error("Invalid variable");
		if((vartbl[VarIndex].type & T_STR) && vartbl[VarIndex].dims[0] == 0) {		// string
			cptr = (unsigned char *)ptr;
			cptr++;																	// skip the length byte in a MMBasic string
			for (i = 0; i < sendlen; i++) {
				I2C_Send_Buffer[i] = (int)(*(cptr + i));
			}
		} else if((vartbl[VarIndex].type & T_NBR) && vartbl[VarIndex].dims[0] > 0 && vartbl[VarIndex].dims[1] == 0) {		// numeric array
			if( (((MMFLOAT *)ptr - vartbl[VarIndex].val.fa) + sendlen) > (vartbl[VarIndex].dims[0] + 1 - OptionBase) ) {
				error("Insufficient data");
			} else {
				for (i = 0; i < sendlen; i++) {
					I2C_Send_Buffer[i] = (int)(*((MMFLOAT *)ptr + i));
				}
			}
		} else if((vartbl[VarIndex].type & T_INT) && vartbl[VarIndex].dims[0] > 0 && vartbl[VarIndex].dims[1] == 0) {		// integer array
			if( (((long long int *)ptr - vartbl[VarIndex].val.ia) + sendlen) > (vartbl[VarIndex].dims[0] + 1 - OptionBase) ) {
				error("Insufficient data");
			} else {
				for (i = 0; i < sendlen; i++) {
					I2C_Send_Buffer[i] = (int)(*((long long int *)ptr + i));
				}
			}
		} else error("Invalid variable");
	}
	I2C2_Sendlen = sendlen;
	I2C2_Rcvlen = 0;

	i2c2_masterCommand(1);
}

void i2cCheck(unsigned char *p) {
	int addr;
	uint8_t rxdata;
	getargs(&p, 1, ",");
	if(!I2C_enabled)error("I2C not open");
	addr = getinteger(argv[0]);
    if(addr<0 || addr>0x7F)error("Invalid I2C address");
	int ret=i2c_read_blocking(i2c0, addr, &rxdata, 1, false);
	mmI2Cvalue = ret < 0 ? 1 : 0;
}
void i2c2Check(unsigned char *p) {
	int addr;
	uint8_t rxdata;
	getargs(&p, 1, ",");
	if(!I2C2_enabled)error("I2C not open");
	addr = getinteger(argv[0]);
    if(addr<0 || addr>0x7F)error("Invalid I2C address");
	int ret=i2c_read_blocking(i2c1, addr, &rxdata, 1, false);
	mmI2Cvalue = ret < 0 ? 1 : 0;
}
// receive data from an I2C slave - master mode
void i2cReceive(unsigned char *p) {
	int addr, i2c_options, rcvlen;
	void *ptr = NULL;
	getargs(&p, 7, ",");
	if(argc != 7) error("Invalid syntax");
	if(!I2C_enabled)error("I2C not open");
	addr = getinteger(argv[0]);
	i2c_options = getint(argv[2],0,1);
	I2C_Status = 0;
    I2C_Rcvbuf_Float = NULL;
    I2C_Rcvbuf_Int = NULL;
    I2C_Rcvbuf_String = NULL;
	if(i2c_options & 0x01) I2C_Status = I2C_Status_BusHold;
	I2C_Addr = addr;
	rcvlen = getinteger(argv[4]);
	if(rcvlen < 1 || rcvlen > 255) error("Number out of bounds");

	ptr = findvar(argv[6], V_FIND | V_EMPTY_OK);
    if(vartbl[VarIndex].type & T_CONST) error("Cannot change a constant");
	if(ptr == NULL) error("Invalid variable");
	if(vartbl[VarIndex].type & T_NBR) {
        if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
        if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
            if(rcvlen != 1) error("Invalid variable");
        } else {		// An array
            if( (((MMFLOAT *)ptr - vartbl[VarIndex].val.fa) + rcvlen) > (vartbl[VarIndex].dims[0] + 1 - OptionBase) )
                error("Insufficient space in array");
        }
        I2C_Rcvbuf_Float = (MMFLOAT*)ptr;
    } else if(vartbl[VarIndex].type & T_INT) {
        if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
        if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
            if(rcvlen != 1) error("Invalid variable");
        } else {		// An array
            if( (((long long int *)ptr - vartbl[VarIndex].val.ia) + rcvlen) > (vartbl[VarIndex].dims[0] + 1 - OptionBase) )
                error("Insufficient space in array");
        }
        I2C_Rcvbuf_Int = (long long int *)ptr;
    } else if(vartbl[VarIndex].type & T_STR) {
        if(vartbl[VarIndex].dims[0] != 0) error("Invalid variable");
        *(char *)ptr = rcvlen;
        I2C_Rcvbuf_String = (char *)ptr + 1;
    } else error("Invalid variable");
	I2C_Rcvlen = rcvlen;

	I2C_Sendlen = 0;
//	PInt((uint32_t)I2C_Rcvbuf_String);
	i2c_masterCommand(1);
//	PIntComma(rcvlen);
//	PInt((uint32_t)I2C_Rcvbuf_String);PRet();
//	if(vartbl[VarIndex].type & T_STR)*(char *)ptr = rcvlen;
}
void i2cReceiveSlave(unsigned char *p, int channel) {
	int addr, i2c_options, rcvlen;
	void *ptr = NULL;
    MMFLOAT *rcvdlenFloat=NULL;
	long long int *rcvdlenInt=NULL;
	int count=1;
    I2C_Rcvbuf_Float = NULL;
    I2C_Rcvbuf_Int = NULL;
    I2C_Rcvbuf_String = NULL;
    I2C2_Rcvbuf_Float = NULL;
    I2C2_Rcvbuf_Int = NULL;
    I2C2_Rcvbuf_String = NULL;
	getargs(&p, 5, ",");
	if(argc != 5) error("Invalid syntax");
	if(!((I2C_Status & I2C_Status_Slave && channel==0) || (I2C2_Status & I2C_Status_Slave && channel==1)))error("I2C slave not open");
	rcvlen = getinteger(argv[0]);
	if(rcvlen < 1 || rcvlen > 255) error("Number out of bounds");
	ptr = findvar(argv[2], V_FIND | V_EMPTY_OK);
    if(vartbl[VarIndex].type & T_CONST) error("Cannot change a constant");
	if(ptr == NULL) error("Invalid variable");
	if(vartbl[VarIndex].type & T_NBR) {
        if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
        if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
            if(rcvlen != 1) error("Invalid variable");
        } else {		// An array
            if( (((MMFLOAT *)ptr - vartbl[VarIndex].val.fa) + rcvlen) > (vartbl[VarIndex].dims[0] + 1 - OptionBase) )
                error("Insufficient space in array");
        }
        I2C_Rcvbuf_Float = (MMFLOAT*)ptr;
    } else if(vartbl[VarIndex].type & T_INT) {
        if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
        if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
            if(rcvlen != 1) error("Invalid variable");
        } else {		// An array
            if( (((long long int *)ptr - vartbl[VarIndex].val.ia) + rcvlen) > (vartbl[VarIndex].dims[0] + 1 - OptionBase) )
                error("Insufficient space in array");
        }
        I2C_Rcvbuf_Int = (long long int *)ptr;
    } else if(vartbl[VarIndex].type & T_STR) {
        if(vartbl[VarIndex].dims[0] != 0) error("Invalid variable");
        *(char *)ptr = rcvlen;
        I2C_Rcvbuf_String = (char *)ptr + 1;
    } else error("Invalid variable");
    ptr = findvar(argv[4], V_FIND);
    if(vartbl[VarIndex].type & T_CONST) error("Cannot change a constant");
    if(vartbl[VarIndex].type & T_NBR)  rcvdlenFloat = (MMFLOAT *)ptr;
    else if(vartbl[VarIndex].type & T_INT) rcvdlenInt = (long long int *)ptr;
	else error("Invalid variable");

	unsigned char *bbuff;
	if(channel==0){
		bbuff=I2C_Send_Buffer;
		i2c_read_raw_blocking(i2c0, bbuff, 1);
		if(rcvlen>1){
			I2CTimer=0;
			while(count <rcvlen && I2CTimer<rcvlen/10+2){
				if(i2c0->hw->status & 8)i2c_read_raw_blocking(i2c0, &bbuff[count++], 1);
			}
		}
	} else {
		bbuff=I2C_Send_Buffer;
		i2c_read_raw_blocking(i2c1, bbuff, 1);
		if(rcvlen>1){
			I2CTimer=0;
			while(count <rcvlen && I2CTimer<rcvlen/10+2){
				if(i2c1->hw->status & 8)i2c_read_raw_blocking(i2c1, &bbuff[count++], 1);
			}
		}
	}
	for(int i=0;i<rcvlen;i++){
		if(I2C_Rcvbuf_String!=NULL){
			*I2C_Rcvbuf_String=bbuff[i];
			I2C_Rcvbuf_String++;
		}
		if(I2C_Rcvbuf_Float!=NULL){
			*I2C_Rcvbuf_Float=bbuff[i];
			I2C_Rcvbuf_Float++;
		}
		if(I2C_Rcvbuf_Int!=NULL){
			*I2C_Rcvbuf_Int=bbuff[i];
			I2C_Rcvbuf_Int++;
		}
	}
	if(!(rcvdlenFloat == NULL))
          *rcvdlenFloat = (MMFLOAT)count;
      else
          *rcvdlenInt = (long long int)count;

	if(channel==0)i2c0->hw->intr_mask = I2C_IC_INTR_STAT_R_RX_FULL_BITS | I2C_IC_INTR_MASK_M_RD_REQ_BITS;
	else i2c1->hw->intr_mask = I2C_IC_INTR_STAT_R_RX_FULL_BITS | I2C_IC_INTR_MASK_M_RD_REQ_BITS;
}
// receive data from an I2C slave - master mode
void i2c2Receive(unsigned char *p) {
	int addr, i2c2_options, rcvlen;
	void *ptr = NULL;
	getargs(&p, 7, ",");
	if(argc != 7) error("Invalid syntax");
	if(!I2C2_enabled)error("I2C not open");
	addr = getinteger(argv[0]);
	i2c2_options = getint(argv[2],0,1);
	I2C2_Status = 0;
	if(i2c2_options & 0x01) I2C2_Status = I2C_Status_BusHold;
	I2C2_Addr = addr;
    I2C2_Rcvbuf_Float = NULL;
    I2C2_Rcvbuf_Int = NULL;
    I2C2_Rcvbuf_String = NULL;
	rcvlen = getinteger(argv[4]);
	if(rcvlen < 1 || rcvlen > 255) error("Number out of bounds");

	ptr = findvar(argv[6], V_FIND | V_EMPTY_OK);
    if(vartbl[VarIndex].type & T_CONST) error("Cannot change a constant");
	if(ptr == NULL) error("Invalid variable");
	if(vartbl[VarIndex].type & T_NBR) {
        if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
        if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
            if(rcvlen != 1) error("Invalid variable");
        } else {		// An array
            if( (((MMFLOAT *)ptr - vartbl[VarIndex].val.fa) + rcvlen) > (vartbl[VarIndex].dims[0] + 1 - OptionBase) )
                error("Insufficient space in array");
        }
        I2C2_Rcvbuf_Float = (MMFLOAT*)ptr;
    } else if(vartbl[VarIndex].type & T_INT) {
        if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
        if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
            if(rcvlen != 1) error("Invalid variable");
        } else {		// An array
            if( (((long long int *)ptr - vartbl[VarIndex].val.ia) + rcvlen) > (vartbl[VarIndex].dims[0] + 1 - OptionBase) )
                error("Insufficient space in array");
        }
        I2C2_Rcvbuf_Int = (long long int *)ptr;
    } else if(vartbl[VarIndex].type & T_STR) {
        if(vartbl[VarIndex].dims[0] != 0) error("Invalid variable");
        *(char *)ptr = rcvlen;
        I2C2_Rcvbuf_String = (char *)ptr + 1;
    } else error("Invalid variable");
	I2C2_Rcvlen = rcvlen;

	I2C2_Sendlen = 0;

	i2c2_masterCommand(1);
}

/**************************************************************************************************
Enable the I2C1 module - master mode
***************************************************************************************************/
void i2c_enable(int bps) {
//    HAL_I2C_DeInit(&hi2c1);
//    ExtCfg(I2C0SDApin, EXT_NOT_CONFIG, 0);
//    ExtCfg(I2C0SCLpin, EXT_NOT_CONFIG, 0);
/*    PinSetBit(I2C0SDApin, TRISSET);
    if(PinRead(I2C0SDApin) == 0) {
    	int i;
        // it appears as if something is holding SLA low
        // try pulsing the clock to get rid of it
        PinSetBit(I2C0SCLpin, TRISCLR);
        PinSetBit(I2C0SCLpin, LATCLR);
        for(i = 0; i < 20; i++) {
            uSec(100);
            PinSetBit(I2C0SCLpin, LATINV);
            if(PinRead(I2C0SDApin) == 0) break;
        }
        PinSetBit(I2C0SCLpin, TRISSET);
    }*/
    ExtCfg(I2C0SDApin, EXT_COM_RESERVED, 0);
    ExtCfg(I2C0SCLpin, EXT_COM_RESERVED, 0);
	i2c_init(i2c0, bps * 1000);
    gpio_pull_up(PinDef[I2C0SDApin].GPno);
    gpio_pull_up(PinDef[I2C0SCLpin].GPno);
	I2C_enabled=1;
}
void i2c2_enable(int bps) {
//    HAL_I2C_DeInit(&hi2c2);
/*    ExtCfg(I2C1SDApin, EXT_NOT_CONFIG, 0);
    ExtCfg(I2C1SCLpin, EXT_NOT_CONFIG, 0);
    PinSetBit(I2C1SDApin, TRISSET);
    if(PinRead(I2C1SDApin) == 0) {
        int i;
        // it appears as if something is holding SLA low
        // try pulsing the clock to get rid of it
        PinSetBit(I2C1SCLpin, TRISCLR);
        PinSetBit(I2C1SCLpin, LATCLR);
        for(i = 0; i < 20; i++) {
           uSec(100);
           PinSetBit(I2C1SCLpin, LATINV);
           if(PinRead(I2C1SDApin) == 0) break;
        }
        PinSetBit(I2C1SCLpin, TRISSET);
    }*/
    ExtCfg(I2C1SDApin, EXT_COM_RESERVED, 0);
    ExtCfg(I2C1SCLpin, EXT_COM_RESERVED, 0);
	i2c_init(i2c1, bps * 1000);
    gpio_pull_up(PinDef[I2C1SDApin].GPno);
    gpio_pull_up(PinDef[I2C1SCLpin].GPno);
	I2C2_enabled=1;
}

/**************************************************************************************************
Disable the I2C1 module - master mode
***************************************************************************************************/
void i2c_disable() {
	if(I2C_Status & I2C_Status_Slave) {
		irq_set_enabled(I2C0_IRQ, false);
		irq_remove_handler(I2C0_IRQ, i2c0_irq_handler);
		i2c_set_slave_mode(i2c0,false, I2C_Slave_Addr);
		i2c0->hw->intr_mask = 0;
	}
    I2C_Status = I2C_Status_Disable;
	I2C_Rcvbuf_String = NULL;                                       // pointer to the master receive buffer
    I2C_Rcvbuf_Float = NULL;
    I2C_Rcvbuf_Int = NULL;
	I2C_Sendlen = 0;												// length of the master send buffer
	I2C_Rcvlen = 0;													// length of the master receive buffer
	I2C_Addr = 0;													// I2C device address
	I2C_Timeout = 0;												// master timeout value
	i2c_deinit(i2c0);
	I2C_enabled = 0;
    if(I2C0SDApin!=99)ExtCfg(I2C0SDApin, EXT_NOT_CONFIG, 0);
    if(I2C0SCLpin!=99)ExtCfg(I2C0SCLpin, EXT_NOT_CONFIG, 0);
}
void i2c2_disable() {
	if(I2C2_Status & I2C_Status_Slave) {
		irq_set_enabled(I2C1_IRQ, false);
		irq_remove_handler(I2C1_IRQ, i2c1_irq_handler);
		i2c_set_slave_mode(i2c1,false, I2C_Slave_Addr);
		i2c1->hw->intr_mask = 0;
	}
    I2C2_Status = I2C_Status_Disable;
	I2C2_Rcvbuf_String = NULL;                                       // pointer to the master receive buffer
    I2C2_Rcvbuf_Float = NULL;
    I2C2_Rcvbuf_Int = NULL;
	I2C2_Sendlen = 0;												// length of the master send buffer
	I2C2_Rcvlen = 0;													// length of the master receive buffer
	I2C2_Addr = 0;													// I2C device address
	I2C2_Timeout = 0;												// master timeout value
	i2c_deinit(i2c1);
	I2C2_enabled = 0;
    if(I2C1SDApin!=99)ExtCfg(I2C1SDApin, EXT_NOT_CONFIG, 0);
    if(I2C1SCLpin!=99)ExtCfg(I2C1SCLpin, EXT_NOT_CONFIG, 0);
}
/**************************************************************************************************
Send and/or Receive data - master mode
***************************************************************************************************/
void i2c_masterCommand(int timer) {
//	unsigned char start_type,
	unsigned char i,i2caddr=I2C_Addr;
	if(I2C_Sendlen){
		int i2cret=i2c_write_timeout_us(i2c0, (uint8_t)i2caddr, (uint8_t *)I2C_Send_Buffer, I2C_Sendlen,(I2C_Status == I2C_Status_BusHold ? true:false), I2C_Timeout*1000);
		mmI2Cvalue=0;
		if(i2cret==PICO_ERROR_GENERIC)mmI2Cvalue=1;
		if(i2cret==PICO_ERROR_TIMEOUT)mmI2Cvalue=2;
	}
	if(I2C_Rcvlen){
		unsigned char *I2C_Rcv_Buffer=GetTempMemory(STRINGSIZE);
		int i2cret=i2c_read_timeout_us(i2c0, (uint8_t)i2caddr, (uint8_t *)I2C_Rcv_Buffer, I2C_Rcvlen, (I2C_Status == I2C_Status_BusHold ? true:false), I2C_Timeout*1000);
		mmI2Cvalue=0;
		if(i2cret==PICO_ERROR_GENERIC)mmI2Cvalue=1;
		if(i2cret==PICO_ERROR_TIMEOUT)mmI2Cvalue=2;
		for(i=0;i<I2C_Rcvlen;i++){
			if(I2C_Rcvbuf_String!=NULL){
				*I2C_Rcvbuf_String=I2C_Rcv_Buffer[i];
				I2C_Rcvbuf_String++;
			}
			if(I2C_Rcvbuf_Float!=NULL){
				*I2C_Rcvbuf_Float=I2C_Rcv_Buffer[i];
				I2C_Rcvbuf_Float++;
			}
			if(I2C_Rcvbuf_Int!=NULL){
				*I2C_Rcvbuf_Int=I2C_Rcv_Buffer[i];
				I2C_Rcvbuf_Int++;
			}
		}
	}
}

void i2c2_masterCommand(int timer) {
//	unsigned char start_type,
	unsigned char i,i2c2addr=I2C2_Addr;
	if(I2C2_Sendlen){
		int i2cret=i2c_write_timeout_us(i2c1, (uint8_t)i2c2addr, (uint8_t *)I2C_Send_Buffer, I2C2_Sendlen,(I2C2_Status == I2C_Status_BusHold ? true:false), I2C2_Timeout*1000);
		mmI2Cvalue=0;
		if(i2cret==PICO_ERROR_GENERIC)mmI2Cvalue=1;
		if(i2cret==PICO_ERROR_TIMEOUT)mmI2Cvalue=2;
	}
	if(I2C2_Rcvlen){
		unsigned char *I2C2_Rcv_Buffer=GetTempMemory(STRINGSIZE);
		int i2cret=i2c_read_timeout_us(i2c1, (uint8_t)i2c2addr, (uint8_t *)I2C2_Rcv_Buffer, I2C2_Rcvlen, (I2C2_Status == I2C_Status_BusHold ? true:false), I2C2_Timeout*1000);
		mmI2Cvalue=0;
		if(i2cret==PICO_ERROR_GENERIC)mmI2Cvalue=1;
		if(i2cret==PICO_ERROR_TIMEOUT)mmI2Cvalue=2;
		for(i=0;i<I2C2_Rcvlen;i++){
			if(I2C2_Rcvbuf_String!=NULL){
				*I2C2_Rcvbuf_String=I2C2_Rcv_Buffer[i];
				I2C2_Rcvbuf_String++;
			}
			if(I2C2_Rcvbuf_Float!=NULL){
				*I2C2_Rcvbuf_Float=I2C2_Rcv_Buffer[i];
				I2C2_Rcvbuf_Float++;
			}
			if(I2C2_Rcvbuf_Int!=NULL){
				*I2C2_Rcvbuf_Int=I2C2_Rcv_Buffer[i];
				I2C2_Rcvbuf_Int++;
			}
		}
	}
}

void fun_mmi2c(void) {
	iret = mmI2Cvalue;
    targ = T_INT;
}

