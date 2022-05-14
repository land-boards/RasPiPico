/***********************************************************************************************************************
PicoMite MMBasic

I2C.h

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

//#define INCLUDE_I2C_SLAVE                                           // uncomment this to include i2c slave functions

/**********************************************************************************
 the C language function associated with commands, functions or operators should be
 declared here
**********************************************************************************/
#if !defined(INCLUDE_COMMAND_TABLE) && !defined(INCLUDE_TOKEN_TABLE)


void cmd_i2c(void);
void cmd_i2c2(void);
void cmd_rtc(void);
void fun_mmi2c(void);

#endif




/**********************************************************************************
 All command tokens tokens (eg, PRINT, FOR, etc) should be inserted in this table
**********************************************************************************/
#ifdef INCLUDE_COMMAND_TABLE

	{ "I2C",	T_CMD,		0, cmd_i2c              },
	{ "I2C2",	T_CMD,		0, cmd_i2c2              },
  	{ "RTC",    T_CMD,      0, cmd_rtc              },
#endif


/**********************************************************************************
 All other tokens (keywords, functions, operators) should be inserted in this table
**********************************************************************************/
#ifdef INCLUDE_TOKEN_TABLE

	{ "MM.I2C",	T_FNA | T_INT,	0, fun_mmi2c		},
#endif



#if !defined(INCLUDE_COMMAND_TABLE) && !defined(INCLUDE_TOKEN_TABLE)
#ifndef I2C_HEADER
#define I2C_HEADER

// states of the master state machine
#define I2C_State_Idle     0					    // Bus Idle
#define I2C_State_Start    1					    // Sending Start or Repeated Start
#define I2C_State_10Bit    2					    // Sending a 10 bit address
#define I2C_State_10BitRcv 3					    // 10 bit address receive
#define I2C_State_RcvAddr  4					    // Receive address
#define I2C_State_Send     5					    // Sending Data
#define I2C_State_Receive  6					    // Receiving data
#define I2C_State_Ack      7					    // Sending Acknowledgement
#define I2C_State_Stop     8					    // Sending Stop

// defines for I2C_Status
#define I2C_Status_Enabled			0x00000001
#define I2C_Status_MasterCmd			0x00000002
#define I2C_Status_NoAck			0x00000010
#define I2C_Status_Timeout			0x00000020
#define I2C_Status_InProgress			0x00000040
#define I2C_Status_Completed			0x00000080
#define I2C_Status_Interrupt			0x00000100
#define I2C_Status_BusHold			0x00000200
#define I2C_Status_10BitAddr			0x00000400
#define I2C_Status_BusOwned			0x00000800
#define I2C_Status_Send				0x00001000
#define I2C_Status_Receive		    	0x00002000
#define I2C_Status_Disable			0x00004000
#define I2C_Status_Master			0x00008000
#define I2C_Status_Slave			0x00010000
#define I2C_Status_Slave_Send			0x00020000
#define I2C_Status_Slave_Receive		0x00040000
#define I2C_Status_Slave_Send_Rdy		0x00080000
#define I2C_Status_Slave_Receive_Rdy            0x00100000
#define I2C_Status_Slave_Receive_Full           0x00200000
#define SSD1306_I2C_Addr 0x3c

// Global variables provided by I2C.c
extern unsigned int I2C_Timer;                                      // master timeout counter
extern char *I2C_IntLine;                                           // pointer to the master interrupt line number
extern void i2c_disable(void);
extern void i2c2_disable(void);
extern void RtcGetTime(int noerror);
extern void ConfigDisplayI2C(unsigned char *p);
extern void InitDisplayI2C(int InitOnly);
extern unsigned int I2C2_enabled;									// I2C enable marker
extern unsigned int I2C_enabled;									// I2C enable marker
extern unsigned int I2C_Timeout;									// master timeout value
extern unsigned int I2C2_Timeout;									// master timeout value
extern volatile unsigned int I2C_Status;                            // status flags
extern volatile unsigned int I2C2_Status;                            // status flags
extern int noRTC;
extern char *I2C_Slave_Send_IntLine;                                // pointer to the slave send interrupt line number
extern char *I2C_Slave_Receive_IntLine;                             // pointer to the slave receive interrupt line number
extern char *I2C2_Slave_Send_IntLine;                                // pointer to the slave send interrupt line number
extern char *I2C2_Slave_Receive_IntLine;                             // pointer to the slave receive interrupt line number
extern void I2C_Send_Command(char command);
#endif
#endif
