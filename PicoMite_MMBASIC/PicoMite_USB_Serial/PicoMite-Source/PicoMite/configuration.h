/***********************************************************************************************************************
PicoMite MMBasic

configuration.h

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

************************************************************************************************************************/#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#ifdef __cplusplus
extern "C" {
#endif
#define FLASH_TARGET_OFFSET (1024 * 1024) 
#ifdef PICOMITEVGA
#define MagicKey 0x67AA22F4
#else
#define MagicKey 0x86AA33E7
#endif

#define MMFLOAT double
#define ALL_MEMORY_SIZE (180*1024) 
#ifdef PICOMITEVGA
#define HEAP_MEMORY_SIZE (108*1024) 
#else
#define HEAP_MEMORY_SIZE (124*1024) 
#endif
#define MAX_PROG_SIZE HEAP_MEMORY_SIZE
#define SAVEDVARS_FLASH_SIZE 16384
#define FLASH_ERASE_SIZE 4096
#define MAXFLASHSLOTS (((1024*1024)-FLASH_ERASE_SIZE-SAVEDVARS_FLASH_SIZE)/MAX_PROG_SIZE)-1
#define MAXVARS             512                     // 8 + MAXVARLEN + MAXDIM * 2  (ie, 56 bytes) - these do not incl array members
#define MAXVARHASH				MAXVARS/2

// more static memory allocations (less important)
#define MAXFORLOOPS         20                      // each entry uses 17 bytes
#define MAXDOLOOPS          20                      // each entry uses 12 bytes
#define MAXGOSUB            50                     // each entry uses 4 bytes
#define MAX_MULTILINE_IF    20                      // each entry uses 8 bytes
#define MAXTEMPSTRINGS      64                      // each entry takes up 4 bytes
#define MAXSUBFUN           224               // each entry takes up 4 bytes
#define MAXSUBHASH          MAXSUBFUN
// operating characteristics
#define MAXVARLEN           32                      // maximum length of a variable name
#define MAXSTRLEN           255                     // maximum length of a string
#define STRINGSIZE          256                     // must be 1 more than MAXSTRLEN.  2 of these buffers are staticaly created
#define MAXOPENFILES        10                      // maximum number of open files
#define MAXDIM              5                       // maximum nbr of dimensions to an array
#define CONSOLE_RX_BUF_SIZE 256
#define CONSOLE_TX_BUF_SIZE 256
#define MAXOPENFILES  10
#define MAXCOMPORTS 2
#define MAXERRMSG           64                      // max error msg size (MM.ErrMsg$ is truncated to this)
#define MAXSOUNDS           4
#define MAXKEYLEN           64
// define the maximum number of arguments to PRINT, INPUT, WRITE, ON, DIM, ERASE, DATA and READ
// each entry uses zero bytes.  The number is limited by the length of a command line
#define MAX_ARG_COUNT       50
#define STR_AUTO_PRECISION  999 
#define STR_SIG_DIGITS 9                            // number of significant digits to use when converting MMFLOAT to a string
#define NBRSETTICKS         4                       // the number of SETTICK interrupts available
#define NBRPINS             44
#define MAXPROMPTLEN        49                      // max length of a prompt incl the terminating null
#define BREAK_KEY           3                       // the default value (CTRL-C) for the break key.  Reset at the command prompt.
#define FNV_prime           16777619
#define FNV_offset_basis    2166136261
#define use_hash
#define DISKCHECKRATE       500                    //check for removal of SDcard every 500mSec
#define EDIT_BUFFER_SIZE    Option.HEAP_SIZE-1024-3*HRes// this is the maximum RAM that we can get
#define SCREENWIDTH     80
#define SCREENHEIGHT    24                          // this is the default and it can be changed using the OPTION command
#define CONSOLE_BAUDRATE        115200               // only applies to the serial console
#define MAXCFUNCTION	20
#define SAVEDVARS_FLASH_SIZE 16384
#define FLASH_ERASE_SIZE 4096
#define MAX_CPU     252000
#define MAXBLITBUF 32
#define MAXRESTORE          16
#define CONFIG_TITLE		0
#define CONFIG_LOWER		1
#define CONFIG_UPPER		2
#define UNUSED       (1 << 0)
#define ANALOG_IN    (1 << 1)
#define DIGITAL_IN   (1 << 2)
#define DIGITAL_OUT   (1 << 3)
#define UART1TX     (1 << 4)
#define UART1RX     (1 << 5)
#define UART0TX     (1 << 6)
#define UART0RX     (1 << 7)
#define I2C0SDA     (1 << 8)
#define I2C0SCL     (1 << 9)
#define I2C1SDA     (1 << 10)
#define I2C1SCL     (1 << 11)
#define SPI0RX     (1 << 12)
#define SPI0TX     (1 << 13)
#define SPI0SCK     (1 << 14)
#define SPI1RX     (1 << 15)
#define SPI1TX     (1 << 16)
#define SPI1SCK     (1 << 17)
#define PWM0A     (1 << 18)
#define PWM0B     (1 << 19)
#define PWM1A     (1 << 20)
#define PWM1B     (1 << 21)
#define PWM2A     (1 << 22)
#define PWM2B     (1 << 23)
#define PWM3A     (1 << 24)
#define PWM3B     (1 << 25)
#define PWM4A     (1 << 26)
#define PWM4B     (1 << 27)
#define PWM5A     (1 << 28)
#define PWM5B     (1 << 29)
#define PWM6A     (1 << 30)
#define PWM6B     (1 << 31)
#define PWM7A     4294967296
#define PWM7B     8589934592
//#define DO_NOT_RESET (1 << 5)
//#define HEARTBEAT    (1 << 6)
#define HEARTBEATpin  43
#define PROGSTART (FLASH_TARGET_OFFSET + FLASH_ERASE_SIZE + SAVEDVARS_FLASH_SIZE + ((MAXFLASHSLOTS) * MAX_PROG_SIZE))
#ifdef __cplusplus
}
#endif
#endif /* __CONFIGURATION_H */