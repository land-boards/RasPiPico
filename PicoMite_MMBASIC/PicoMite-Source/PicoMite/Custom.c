/***********************************************************************************************************************
PicoMite MMBasic

custom.c

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
#include <stdio.h>

#include "MMBasic_Includes.h"
#include "Hardware_Includes.h"
#define STATIC static

/*************************************************************************************************************************
**************************************************************************************************************************
IMPORTANT:
This module is empty and should be used for your special functions and commands.  In the standard distribution this file  
will never be changed, so your code should be safe here.  You should avoid placing commands and functions in other files as
they may be changed and you would then need to re insert your changes in a new release of the source.

**************************************************************************************************************************
**************************************************************************************************************************/
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

/********************************************************************************************************************************************
 custom commands and functions
 each function is responsible for decoding a command
 all function names are in the form cmd_xxxx() (for a basic command) or fun_xxxx() (for a basic function) so, if you want to search for the
 function responsible for the NAME command look for cmd_name

 There are 4 items of information that are setup before the command is run.
 All these are globals.

 int cmdtoken	This is the token number of the command (some commands can handle multiple
				statement types and this helps them differentiate)

 unsigned char *cmdline	This is the command line terminated with a zero unsigned char and trimmed of leading
				spaces.  It may exist anywhere in memory (or even ROM).

 unsigned char *nextstmt	This is a pointer to the next statement to be executed.  The only thing a
				command can do with it is save it or change it to some other location.

 unsigned char *CurrentLinePtr  This is read only and is set to NULL if the command is in immediate mode.

 The only actions a command can do to change the program flow is to change nextstmt or
 execute longjmp(mark, 1) if it wants to abort the program.

 ********************************************************************************************************************************************/
/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/claim.h"
#define PIO_NUM(pio) ((pio) == pio0 ? 0 : 1)
#define CLKMIN ((Option.CPU_Speed*125)>>13)
#define CLKMAX (Option.CPU_Speed *1000)

#ifndef PICOMITEVGA
#include "pio.h"
#include "pio_instructions.h"
check_hw_layout(pio_hw_t, sm[0].clkdiv, PIO_SM0_CLKDIV_OFFSET);
check_hw_layout(pio_hw_t, sm[1].clkdiv, PIO_SM1_CLKDIV_OFFSET);
check_hw_layout(pio_hw_t, instr_mem[0], PIO_INSTR_MEM0_OFFSET);
check_hw_layout(pio_hw_t, inte0, PIO_IRQ0_INTE_OFFSET);
check_hw_layout(pio_hw_t, txf[1], PIO_TXF1_OFFSET);
check_hw_layout(pio_hw_t, rxf[3], PIO_RXF3_OFFSET);
check_hw_layout(pio_hw_t, ints1, PIO_IRQ1_INTS_OFFSET);

static_assert(NUM_PIO_STATE_MACHINES * NUM_PIOS <= 8, "");
static uint8_t claimed;

void pio_sm_claim(PIO pio, uint sm) {
    check_sm_param(sm);
    uint which = pio_get_index(pio);
    if (which) {
        hw_claim_or_assert(&claimed, NUM_PIO_STATE_MACHINES + sm, "PIO 1 SM (%d - 4) already claimed");
    } else {
        hw_claim_or_assert(&claimed, sm, "PIO 0 SM %d already claimed");
    }
}

void pio_claim_sm_mask(PIO pio, uint sm_mask) {
    for(uint i = 0; sm_mask; i++, sm_mask >>= 1u) {
        if (sm_mask & 1u) pio_sm_claim(pio, i);
    }
}

void pio_sm_unclaim(PIO pio, uint sm) {
    check_sm_param(sm);
    uint which = pio_get_index(pio);
    hw_claim_clear(&claimed, which * NUM_PIO_STATE_MACHINES + sm);
}

int pio_claim_unused_sm(PIO pio, bool required) {
    // PIO index is 0 or 1.
    uint which = pio_get_index(pio);
    uint base = which * NUM_PIO_STATE_MACHINES;
    int index = hw_claim_unused_from_range((uint8_t*)&claimed, required, base,
                                      base + NUM_PIO_STATE_MACHINES - 1, "No PIO state machines are available");
    return index >= (int)base ? index - (int)base : -1;
}

bool pio_sm_is_claimed(PIO pio, uint sm) {
    check_sm_param(sm);
    uint which = pio_get_index(pio);
    return hw_is_claimed(&claimed, which * NUM_PIO_STATE_MACHINES + sm);
}

static_assert(PIO_INSTRUCTION_COUNT <= 32, "");
static uint32_t _used_instruction_space[2];

static int _pio_find_offset_for_program(PIO pio, const pio_program_t *program) {
    assert(program->length <= PIO_INSTRUCTION_COUNT);
    uint32_t used_mask = _used_instruction_space[pio_get_index(pio)];
    uint32_t program_mask = (1u << program->length) - 1;
    if (program->origin >= 0) {
        if (program->origin > 32 - program->length) return -1;
        return used_mask & (program_mask << program->origin) ? -1 : program->origin;
    } else {
        // work down from the top always
        for (int i = 32 - program->length; i >= 0; i--) {
            if (!(used_mask & (program_mask << (uint) i))) {
                return i;
            }
        }
        return -1;
    }
}

bool pio_can_add_program(PIO pio, const pio_program_t *program) {
    uint32_t save = hw_claim_lock();
    bool rc =  -1 != _pio_find_offset_for_program(pio, program);
    hw_claim_unlock(save);
    return rc;
}

static bool _pio_can_add_program_at_offset(PIO pio, const pio_program_t *program, uint offset) {
    valid_params_if(PIO, offset < PIO_INSTRUCTION_COUNT);
    valid_params_if(PIO, offset + program->length <= PIO_INSTRUCTION_COUNT);
    if (program->origin >= 0 && (uint)program->origin != offset) return false;
    uint32_t used_mask = _used_instruction_space[pio_get_index(pio)];
    uint32_t program_mask = (1u << program->length) - 1;
    return !(used_mask & (program_mask << offset));
}

bool pio_can_add_program_at_offset(PIO pio, const pio_program_t *program, uint offset) {
    uint32_t save = hw_claim_lock();
    bool rc = _pio_can_add_program_at_offset(pio, program, offset);
    hw_claim_unlock(save);
    return rc;
}

static void _pio_add_program_at_offset(PIO pio, const pio_program_t *program, uint offset) {
    if (!_pio_can_add_program_at_offset(pio, program, offset)) {
        error("No program space");
    }
    for (uint i = 0; i < program->length; ++i) {
        uint16_t instr = program->instructions[i];
        pio->instr_mem[offset + i] = pio_instr_bits_jmp != _pio_major_instr_bits(instr) ? instr : instr + offset;
    }
    uint32_t program_mask = (1u << program->length) - 1;
    _used_instruction_space[pio_get_index(pio)] |= program_mask << offset;
}

// these assert if unable
uint pio_add_program(PIO pio, const pio_program_t *program) {
    uint32_t save = hw_claim_lock();
    int offset = _pio_find_offset_for_program(pio, program);
    if (offset < 0) {
        error("No program space");
    }
    _pio_add_program_at_offset(pio, program, (uint)offset);
    hw_claim_unlock(save);
    return (uint)offset;
}

void pio_add_program_at_offset(PIO pio, const pio_program_t *program, uint offset) {
    uint32_t save = hw_claim_lock();
    _pio_add_program_at_offset(pio, program, offset);
    hw_claim_unlock(save);
}

void pio_remove_program(PIO pio, const pio_program_t *program, uint loaded_offset) {
    uint32_t program_mask = (1u << program->length) - 1;
    program_mask <<= loaded_offset;
    uint32_t save = hw_claim_lock();
    assert(program_mask == (_used_instruction_space[pio_get_index(pio)] & program_mask));
    _used_instruction_space[pio_get_index(pio)] &= ~program_mask;
    hw_claim_unlock(save);
}

void pio_clear_instruction_memory(PIO pio) {
    uint32_t save = hw_claim_lock();
    _used_instruction_space[pio_get_index(pio)] = 0;
    for(uint i=0;i<PIO_INSTRUCTION_COUNT;i++) {
        pio->instr_mem[i] = pio_encode_jmp(i);
    }
    hw_claim_unlock(save);
}

// Set the value of all PIO pins. This is done by forcibly executing
// instructions on a "victim" state machine, sm. Ideally you should choose one
// which is not currently running a program. This is intended for one-time
// setup of initial pin states.
void pio_sm_set_pins(PIO pio, uint sm, uint32_t pins) {
    check_pio_param(pio);
    check_sm_param(sm);
    uint32_t pinctrl_saved = pio->sm[sm].pinctrl;
    uint remaining = 32;
    uint base = 0;
    while (remaining) {
        uint decrement = remaining > 5 ? 5 : remaining;
        pio->sm[sm].pinctrl =
                (decrement << PIO_SM0_PINCTRL_SET_COUNT_LSB) |
                (base << PIO_SM0_PINCTRL_SET_BASE_LSB);
        pio_sm_exec(pio, sm, pio_encode_set(pio_pins, pins & 0x1fu));
        remaining -= decrement;
        base += decrement;
        pins >>= 5;
    }
    pio->sm[sm].pinctrl = pinctrl_saved;
}

void pio_sm_set_pins_with_mask(PIO pio, uint sm, uint32_t pinvals, uint32_t pin_mask) {
    check_pio_param(pio);
    check_sm_param(sm);
    uint32_t pinctrl_saved = pio->sm[sm].pinctrl;
    while (pin_mask) {
        uint base = (uint)__builtin_ctz(pin_mask);
        pio->sm[sm].pinctrl =
                (1u << PIO_SM0_PINCTRL_SET_COUNT_LSB) |
                (base << PIO_SM0_PINCTRL_SET_BASE_LSB);
        pio_sm_exec(pio, sm, pio_encode_set(pio_pins, (pinvals >> base) & 0x1u));
        pin_mask &= pin_mask - 1;
    }
    pio->sm[sm].pinctrl = pinctrl_saved;
}

void pio_sm_set_pindirs_with_mask(PIO pio, uint sm, uint32_t pindirs, uint32_t pin_mask) {
    check_pio_param(pio);
    check_sm_param(sm);
    uint32_t pinctrl_saved = pio->sm[sm].pinctrl;
    while (pin_mask) {
        uint base = (uint)__builtin_ctz(pin_mask);
        pio->sm[sm].pinctrl =
                (1u << PIO_SM0_PINCTRL_SET_COUNT_LSB) |
                (base << PIO_SM0_PINCTRL_SET_BASE_LSB);
        pio_sm_exec(pio, sm, pio_encode_set(pio_pindirs, (pindirs >> base) & 0x1u));
        pin_mask &= pin_mask - 1;
    }
    pio->sm[sm].pinctrl = pinctrl_saved;
}

void pio_sm_set_consecutive_pindirs(PIO pio, uint sm, uint pin, uint count, bool is_out) {
    check_pio_param(pio);
    check_sm_param(sm);
    valid_params_if(PIO, pin < 32u);
    uint32_t pinctrl_saved = pio->sm[sm].pinctrl;
    uint pindir_val = is_out ? 0x1f : 0;
    while (count > 5) {
        pio->sm[sm].pinctrl = (5u << PIO_SM0_PINCTRL_SET_COUNT_LSB) | (pin << PIO_SM0_PINCTRL_SET_BASE_LSB);
        pio_sm_exec(pio, sm, pio_encode_set(pio_pindirs, pindir_val));
        count -= 5;
        pin = (pin + 5) & 0x1f;
    }
    pio->sm[sm].pinctrl = (count << PIO_SM0_PINCTRL_SET_COUNT_LSB) | (pin << PIO_SM0_PINCTRL_SET_BASE_LSB);
    pio_sm_exec(pio, sm, pio_encode_set(pio_pindirs, pindir_val));
    pio->sm[sm].pinctrl = pinctrl_saved;
}

void pio_sm_init(PIO pio, uint sm, uint initial_pc, const pio_sm_config *config) {
    valid_params_if(PIO, initial_pc < PIO_INSTRUCTION_COUNT);
    // Halt the machine, set some sensible defaults
    pio_sm_set_enabled(pio, sm, false);

    if (config) {
        pio_sm_set_config(pio, sm, config);
    } else {
        pio_sm_config c = pio_get_default_sm_config();
        pio_sm_set_config(pio, sm, &c);
    }

    pio_sm_clear_fifos(pio, sm);

    // Clear FIFO debug flags
    const uint32_t fdebug_sm_mask =
            (1u << PIO_FDEBUG_TXOVER_LSB) |
            (1u << PIO_FDEBUG_RXUNDER_LSB) |
            (1u << PIO_FDEBUG_TXSTALL_LSB) |
            (1u << PIO_FDEBUG_RXSTALL_LSB);
    pio->fdebug = fdebug_sm_mask << sm;

    // Finally, clear some internal SM state
    pio_sm_restart(pio, sm);
    pio_sm_clkdiv_restart(pio, sm);
    pio_sm_exec(pio, sm, pio_encode_jmp(initial_pc));
}

void pio_sm_drain_tx_fifo(PIO pio, uint sm) {
    uint instr = (pio->sm[sm].shiftctrl & PIO_SM0_SHIFTCTRL_AUTOPULL_BITS) ? pio_encode_out(pio_null, 32) :
                 pio_encode_pull(false, false);
    while (!pio_sm_is_tx_fifo_empty(pio, sm)) {
        pio_sm_exec(pio, sm, instr);
    }
}
#else
#include "hardware/pio.h"
#include "hardware/pio_instructions.h"
static inline uint32_t pio_sm_calc_wrap(uint wrap_target, uint wrap) {
    uint32_t calc=0;
    valid_params_if(PIO, wrap < PIO_INSTRUCTION_COUNT);
    valid_params_if(PIO, wrap_target < PIO_INSTRUCTION_COUNT);
    return  (calc & ~(PIO_SM0_EXECCTRL_WRAP_TOP_BITS | PIO_SM0_EXECCTRL_WRAP_BOTTOM_BITS)) |
            (wrap_target << PIO_SM0_EXECCTRL_WRAP_BOTTOM_LSB) |
            (wrap << PIO_SM0_EXECCTRL_WRAP_TOP_LSB);
}
#endif

void cmd_pio(void){
    unsigned char *tp;
    tp = checkstring(cmdline, "EXECUTE");
    if(tp){
        int i;
        getargs(&tp, (MAX_ARG_COUNT * 2) - 1, (unsigned char *)",");
        if((argc & 0x01) == 0) error("Syntax");
        if(argc<5)error("Syntax");
#ifdef PICOMITEVGA
        PIO pio = (getint(argv[0],1,1) ? pio1: pio0);
#else
        PIO pio = (getint(argv[0],0,1) ? pio1: pio0);
#endif
        int sm=getint(argv[2],0,3);
        for(i = 4; i < argc; i += 2) {
            pio_sm_exec(pio, sm, getint(argv[i],0,65535));
        }
        return;
    }
    tp = checkstring(cmdline, "WRITE");
    if(tp){
        int i=6;
        getargs(&tp, (MAX_ARG_COUNT * 2) - 1, (unsigned char *)",");
        if((argc & 0x01) == 0) error("Syntax");
        if(argc<5)error("Syntax");
#ifdef PICOMITEVGA
        PIO pio = (getint(argv[0],1,1) ? pio1: pio0);
#else
        PIO pio = (getint(argv[0],0,1) ? pio1: pio0);
#endif
        int sm=getint(argv[2],0,3);
        int count=getint(argv[4],0,MAX_ARG_COUNT-3);
        while(count--) {
            pio_sm_put_blocking(pio, sm, getint(argv[i],0,0xFFFFFFFF));
            i+=2;
        }
        return;
    }
    tp = checkstring(cmdline, "READ");
    if(tp){
        unsigned char *p;
        unsigned int nbr, *d;
        long long int *dd;
        int i;
        getargs(&tp, (MAX_ARG_COUNT * 2) - 1, (unsigned char *)",");
        if((argc & 0x01) == 0) error("Syntax");
        if(argc<5)error("Syntax");
#ifdef PICOMITEVGA
        PIO pio = (getint(argv[0],1,1) ? pio1: pio0);
#else
        PIO pio = (getint(argv[0],0,1) ? pio1: pio0);
#endif
        int sm=getint(argv[2],0,3);
        dd = GetReceiveDataBuffer(argv[4], &nbr);
        while(nbr--) {
       	    *dd = pio_sm_get(pio, sm);
            if(pio->fdebug & (1<<(sm + 16)))*dd=-1;
       	    *dd++;
        }
        return;
    }
    tp = checkstring(cmdline, "PROGRAM LINE");
    if(tp){
        getargs(&tp,5,",");
        if(argc!=5)error("Syntax");
#ifdef PICOMITEVGA
        PIO pio = (getint(argv[0],1,1) ? pio1: pio0);
#else
        PIO pio = (getint(argv[0],0,1) ? pio1: pio0);
#endif
        int slot=getint(argv[2],0,31);
        int instruction=getint(argv[4],0,0xFFFF);
        pio->instr_mem[slot]=instruction;
        return;
    }
    
    tp = checkstring(cmdline, "CLEAR");
    if(tp){
        getargs(&tp,1,",");
#ifdef PICOMITEVGA
        PIO pio = (getint(argv[0],1,1) ? pio1: pio0);
#else
        PIO pio = (getint(argv[0],0,1) ? pio1: pio0);
#endif
        for(int sm=0;sm<4;sm++){
            hw_clear_bits(&pio->ctrl, 1 << (PIO_CTRL_SM_ENABLE_LSB + sm));
            pio->sm[sm].pinctrl=(5<<26);
            pio->sm[sm].execctrl=(0x1f<<12);
            pio->sm[sm].shiftctrl=(3<<18);
        }
        pio_clear_instruction_memory(pio);
        return;
    }

    tp = checkstring(cmdline, "PROGRAM");
    if(tp){
        struct pio_program program;
        getargs(&tp,3,",");
        if(argc!=3)error("Syntax");
        void *prt1;
        program.length=32;
        program.origin=0;
#ifdef PICOMITEVGA
        PIO pio = (getint(argv[0],1,1) ? pio1: pio0);
#else
        PIO pio = (getint(argv[0],0,1) ? pio1: pio0);
#endif
	    void *ptr1 = findvar(argv[2], V_FIND | V_EMPTY_OK | V_NOFIND_ERR);
        if(vartbl[VarIndex].type & T_INT) {
            if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
            if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
                error("Argument 2 must be integer array");
            }
            if((vartbl[VarIndex].dims[0] - OptionBase)!=7)error("Array size");
            program.instructions = (const uint16_t *)ptr1;
        } else error("Argument 2 must be integer array");
        for(int sm=0;sm<4;sm++)hw_clear_bits(&pio->ctrl, 1 << (PIO_CTRL_SM_ENABLE_LSB + sm));
        pio_clear_instruction_memory(pio);
        int offset=pio_add_program(pio, &program);
        return;
    }
    tp = checkstring(cmdline, "START");
    if(tp){
        getargs(&tp,3,",");
        if(argc!=3)error("Syntax");
#ifdef PICOMITEVGA
        PIO pio = (getint(argv[0],1,1) ? pio1: pio0);
#else
        PIO pio = (getint(argv[0],0,1) ? pio1: pio0);
#endif
        int sm=getint(argv[2],0,3);
        pio_sm_set_enabled(pio, sm, true);
        pio_sm_restart(pio, sm);
        return;
    }
    tp = checkstring(cmdline, "STOP");
    if(tp){
        getargs(&tp,3,",");
        if(argc!=3)error("Syntax");
#ifdef PICOMITEVGA
        PIO pio = (getint(argv[0],1,1) ? pio1: pio0);
#else
        PIO pio = (getint(argv[0],0,1) ? pio1: pio0);
#endif
        int sm=getint(argv[2],0,3);
        pio_sm_set_enabled(pio, sm, false);
        return;
    }
    tp = checkstring(cmdline, "INIT MACHINE");
    if(tp){
        int start=0;
        getargs(&tp,13,",");
        if(argc<5)error("Syntax");
        pio_sm_config mypio=pio_get_default_sm_config();
#ifdef PICOMITEVGA
        PIO pio = (getint(argv[0],1,1) ? pio1: pio0);
#else
        PIO pio = (getint(argv[0],0,1) ? pio1: pio0);
#endif
        int sm=getint(argv[2],0,3);
        float clock=getnumber(argv[4]);
        if(clock<CLKMIN || clock> CLKMAX)error("Clock must be in range % to %",CLKMIN,CLKMAX);
        clock=(float)Option.CPU_Speed*1000.0/clock;
        if(argc>5 && *argv[6])mypio.pinctrl = getint(argv[6],0,0xFFFFFFFF);
        if(argc>7 && *argv[8])mypio.execctrl= getint(argv[8],0,0xFFFFFFFF);
        if(argc>9 && *argv[10])mypio.shiftctrl= getint(argv[10],0,0xFFFFFFFF);
        if(argc>11) start=getint(argv[12],0,31);
        mypio.clkdiv = (uint32_t) (clock * (1 << 16));
        pio_sm_set_config(pio, sm, &mypio);
        pio_sm_init(pio, sm, start, &mypio);
        return;
    }
    error("Syntax");
}
void fun_pio(void){
    unsigned char *tp;
    tp = checkstring(ep, "PINCTRL");
    if(tp){
        getargs(&tp,13,",");
        if(argc<3)error("Syntax");
        iret=(getint(argv[0],0,5)<<29); // no of side set pins
        if(argc>1 && *argv[2])iret|=(getint(argv[2],0,5)<<26); // no of set pins
        if(argc>3 && *argv[4])iret|=(getint(argv[4],0,29)<<20); // no of OUT pins
        if(argc>5 && *argv[6]){
            if(!toupper(*argv[6])=='G')error("Syntax");
            argv[6]++;
            if(!toupper(*argv[6])=='P')error("Syntax");
            argv[6]++;
            iret|=(getint(argv[6],0,29)<<15); // IN base
        }
        if(argc>7 && *argv[8]){
            if(!toupper(*argv[8])=='G')error("Syntax");
            argv[8]++;
            if(!toupper(*argv[8])=='P')error("Syntax");
            argv[8]++;
            iret|=(getint(argv[8],0,29)<<10); // SIDE SET base
        }
        if(argc>9 && *argv[10]){
            if(!toupper(*argv[10])=='G')error("Syntax");
            argv[10]++;
            if(!toupper(*argv[10])=='P')error("Syntax");
            argv[10]++;
            iret|=(getint(argv[10],0,29)<<5); // SET base
        }
        if(argc==13){
            if(!toupper(*argv[12])=='G')error("Syntax");
            argv[12]++;
            if(!toupper(*argv[12])=='P')error("Syntax");
            argv[12]++;
            iret|=getint(argv[12],0,29); //OUT base
        }
        targ=T_INT;
        return;
    }
    tp = checkstring(ep, "EXECCTRL");
    if(tp){
        getargs(&tp,5,",");
        if(argc!=5)error("Syntax");
        iret=(getint(argv[0],0,29)<<24); // jmp pin
        iret |= pio_sm_calc_wrap(getint(argv[2],0,31), getint(argv[4],0,31));
        targ=T_INT;
        return;
    }
   tp = checkstring(ep, "SHIFTCTRL");
    if(tp){
        getargs(&tp,7,",");
        if(argc<1)error("Syntax");
        iret=(getint(argv[0],0,31)<<20); // push threshold
        iret|=(getint(argv[2],0,31)<<25); // pull threshold
        if(argc>3 && *argv[4])iret|=(getint(argv[4],0,1)<<16); // autopush
        if(argc>5 && *argv[6])iret|=(getint(argv[6],0,1)<<15); // autopull
        targ=T_INT;
        return;
    }
     tp = checkstring(ep, "FSTAT");
    if(tp){
        getargs(&tp,1,",");
#ifdef PICOMITEVGA
        PIO pio = (getint(argv[0],1,1) ? pio1: pio0);
#else
        PIO pio = (getint(argv[0],0,1) ? pio1: pio0);
#endif
        iret=pio->fstat; // jmp pin
        targ=T_INT;
        return;
    }
    tp = checkstring(ep, "FDEBUG");
    if(tp){
        getargs(&tp,1,",");
#ifdef PICOMITEVGA
        PIO pio = (getint(argv[0],1,1) ? pio1: pio0);
#else
        PIO pio = (getint(argv[0],0,1) ? pio1: pio0);
#endif
        iret=pio->fdebug; // jmp pin
        targ=T_INT;
        return;
    }
    tp = checkstring(ep, "FLEVEL");
    if(tp){
        getargs(&tp,1,",");
#ifdef PICOMITEVGA
        PIO pio = (getint(argv[0],1,1) ? pio1: pio0);
#else
        PIO pio = (getint(argv[0],0,1) ? pio1: pio0);
#endif
        iret=pio->flevel; // jmp pin
        targ=T_INT;
        return;
    }
    error("Syntax");


}