/***********************************************************************************************************************
PicoMite MMBasic

MM_Misc.h

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

void cmd_autosave(void);
void cmd_option(void);
void cmd_pause(void);
void cmd_timer(void);
void cmd_date(void);
void cmd_time(void);
void cmd_ireturn(void);
void cmd_poke(void);
void cmd_settick(void);
void cmd_watchdog(void);
void cmd_cpu(void);
void cmd_cfunction(void);
void cmd_longString(void);
void cmd_sort(void);
void cmd_csubinterrupt(void);
void fun_timer(void);
void fun_date(void);
void fun_time(void);
void fun_device(void);
void fun_keydown(void);
void fun_peek(void);
void fun_restart(void);
void fun_day(void);
void fun_info(void);
void fun_LLen(void);
void fun_LGetByte(void);
void fun_LGetStr(void);
void fun_LCompare(void);
void fun_LInstr(void);
void fun_epoch(void);
void fun_datetime(void);
void fun_json(void);
void cmd_update(void);
void fun_format(void);
#endif




/**********************************************************************************
 All command tokens tokens (eg, PRINT, FOR, etc) should be inserted in this table
**********************************************************************************/
#ifdef INCLUDE_COMMAND_TABLE

	{ (unsigned char *)"AutoSave",		T_CMD,				0, cmd_autosave	},
	{ (unsigned char *)"Option",			T_CMD,				0, cmd_option	},
	{ (unsigned char *)"Pause",			T_CMD,				0, cmd_pause	},
	{ (unsigned char *)"Timer",			T_CMD | T_FUN,      0, cmd_timer	},
	{ (unsigned char *)"Date$",			T_CMD | T_FUN,      0, cmd_date		},
	{ (unsigned char *)"Time$",			T_CMD | T_FUN,      0, cmd_time		},
	{ (unsigned char *)"IReturn",		T_CMD,				0, cmd_ireturn 	},
	{ (unsigned char *)"Poke",			T_CMD,				0, cmd_poke		},
	{ (unsigned char *)"SetTick",		T_CMD,				0, cmd_settick 	},
	{ (unsigned char *)"WatchDog",		T_CMD,				0, cmd_watchdog	},
	{ (unsigned char *)"CPU",			T_CMD,				0, cmd_cpu 	},
	{ (unsigned char *)"Sort",			T_CMD,				0, cmd_sort 	},
    { (unsigned char *)"DefineFont",     T_CMD,				0, cmd_cfunction},
    { (unsigned char *)"End DefineFont", T_CMD,				0, cmd_null 	},
	{ (unsigned char *)"LongString",	T_CMD,				0, cmd_longString	},
	{ (unsigned char *)"Interrupt", 	T_CMD,              	0, cmd_csubinterrupt},
	{ (unsigned char *)"CSub",           T_CMD,              0, cmd_cfunction},
	{ (unsigned char *)"End CSub",       T_CMD,              0, cmd_null     },
	{ (unsigned char *)"Update Firmware",       T_CMD,              0, cmd_update     },

#endif


/**********************************************************************************
 All other tokens (keywords, functions, operators) should be inserted in this table
**********************************************************************************/
#ifdef INCLUDE_TOKEN_TABLE

	{ (unsigned char *)"Timer",	T_FNA | T_NBR ,		0, fun_timer	},
	{ (unsigned char *)"LInStr(",		T_FUN | T_INT,		0, fun_LInstr		},
	{ (unsigned char *)"LCompare(",		T_FUN | T_INT,		0, fun_LCompare		},
	{ (unsigned char *)"LLen(",		T_FUN | T_INT,		0, fun_LLen		},
	{ (unsigned char *)"LGetStr$(",		T_FUN | T_STR,		0, fun_LGetStr		},
	{ (unsigned char *)"LGetByte(",		T_FUN | T_INT,		0, fun_LGetByte		},
	{ (unsigned char *)"Date$",	T_FNA | T_STR,		0, fun_date	},
	{ (unsigned char *)"Day$(",	T_FUN | T_STR,		0, fun_day	},
	{ (unsigned char *)"Peek(",		T_FUN  | T_INT | T_STR | T_NBR,			0, fun_peek		},
	{ (unsigned char *)"Time$",	T_FNA | T_STR,		0, fun_time	},
	{ (unsigned char *)"MM.Device$",	T_FNA | T_STR,		0, fun_device   },
	{ (unsigned char *)"MM.Watchdog",T_FNA | T_INT,		0, fun_restart	},
	{ (unsigned char *)"Epoch(",		T_FUN  | T_INT,			0, fun_epoch		},
	{ (unsigned char *)"DateTime$(",		T_FUN | T_STR,		0, fun_datetime		},
	{ (unsigned char *)"MM.Info(",		T_FUN | T_INT  | T_NBR| T_STR,		0, fun_info		},
	{ (unsigned char *)"Format$(",	T_FUN  | T_STR,			0, fun_format	},

#endif


#if !defined(INCLUDE_COMMAND_TABLE) && !defined(INCLUDE_TOKEN_TABLE)
    // General definitions used by other modules

    #ifndef MISC_HEADER
    #define MISC_HEADER

   	extern void OtherOptions(void);
	extern int InterruptUsed;
	extern int OptionErrorCheck;

    extern unsigned char *InterruptReturn;
    extern int check_interrupt(void);
    extern unsigned char *GetIntAddress(unsigned char *p);
    extern void CrunchData(unsigned char **p, int c);

    // struct for the interrupt configuration
    #define T_LOHI   1
    #define T_HILO   2
    #define T_BOTH   3
    struct s_inttbl {
            int pin;                                   // the pin on which the interrupt is set
            int last;					// the last value of the pin (ie, hi or low)
            char *intp;					// pointer to the interrupt routine
            int lohi;                                  // trigger condition (T_LOHI, T_HILO, etc).
    };
    #define NBRINTERRUPTS	    10			// number of interrupts that can be set
    extern struct s_inttbl inttbl[NBRINTERRUPTS];

    extern int TickPeriod[NBRSETTICKS];
    extern volatile int TickTimer[NBRSETTICKS];
    extern unsigned char *TickInt[NBRSETTICKS];
	extern volatile unsigned char TickActive[NBRSETTICKS];
	extern unsigned int CurrentCpuSpeed;
	extern unsigned int PeripheralBusSpeed;
	extern unsigned char *OnKeyGOSUB;
	extern unsigned char EchoOption;
	extern unsigned int GetPeekAddr(unsigned char *p);
	extern unsigned int GetPokeAddr(unsigned char *p);
	extern void disable_sd(void);
	extern void disable_systemspi(void);
	extern void disable_systemi2c(void);
	extern void disable_audio(void);
#endif
#endif
