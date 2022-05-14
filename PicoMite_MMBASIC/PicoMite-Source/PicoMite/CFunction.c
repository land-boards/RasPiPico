/*
 * CFunctions.c
 *
 *  Created on: 3 Jul 2020
 *      Author: peter
 */
#include "MMBasic_Includes.h"
#include "Hardware_Includes.h"
//Vector to CFunction static RAM

//Vector to CFunction routine called every mSec
unsigned int CFuncmSec = (unsigned int)NULL;
extern volatile uint64_t uSecTimer;
extern volatile uint64_t FastTimer;
//extern TIM_HandleTypeDef htim2;
extern uint32_t ticks_per_microsecond;
void CallExecuteProgram(char *p);
void CallCFuncmSec(void);
extern const void * const CallTable[];
extern void routinechecksExternal(void);
//Vector to CFunction routine called every command (ie, from the BASIC interrupt checker)
unsigned int CFuncInt1 = (unsigned int)NULL;
//Vector to CFunction routine called by the interrupt 2 handler
unsigned int CFuncInt2 = (unsigned int)NULL;
static uint64_t timer(void){ return time_us_64();}
static int64_t PinReadFunc(int a){return gpio_get(PinDef[a].GPno);}
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


// used by CallCFunction() below to find a CFunction or CSub in program flash or the library
unsigned int *FindCFunction(unsigned int *p, unsigned char *CmdPtr) {
    while(*p != 0xffffffff) {
        if(*p++ == (unsigned int)(CmdPtr-ProgMemory)) return p;
        p += (*p + 4) / sizeof(unsigned int);
    }
    return p;
}

long long int CallCFunction(unsigned char *CmdPtr, unsigned char *ArgList, unsigned char *DefP, unsigned char *CallersLinePtr) {
    void *arg[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    int typ[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    long long int ret, i64[10];
    MMFLOAT ff[10];
    unsigned char *pp;
    int i,type;
    uint32_t ii;
    unsigned int *p=(void * const )CallTable;
    MMFLOAT ftmp;
//    if((uint32_t)p > 0x10000000)error("Internal error");
    // find the C code in flash
    if(*ArgList == '(') ArgList++;                                  // and step over it
    p = FindCFunction((unsigned int *)CFunctionFlash, CmdPtr);      // search through the program flash looking for a match to the function being called
    if(*p == 0xffffffff) error("Internal fault 5(sorry)");

    // next, get the argument types (if specified)
    {  // first copy the type list to a buffer and trim the following closing bracket (if there)
        char buf[MAXSTRLEN];
        unsigned char *p = buf;
        if(*DefP == '(') DefP++;
        while(*DefP && *DefP != ')' && *DefP != '\'') *p++ = *DefP++;
        *p = 0;
        p = buf;
        skipspace(p);
        CheckIfTypeSpecified(p, &i, true);
        if(i != DefaultType) {
            // if there is a type list get each entry
            getargs(&p, 19, ",");
            for(i = 0; i < argc; i+=2) {                            // get each definition
                CheckIfTypeSpecified(argv[i], &typ[i/2], false);
                typ[i/2] &= ~T_IMPLIED;
            }
        }
    }

    // we have found the CFunction or CSub and the types on its command line
    CurrentLinePtr = CallersLinePtr;                                // report errors at the caller
    if(*ArgList != ')') {
        getargs(&ArgList, 19, ",");                                 // expand the command line of the caller
        for(i = 0; i < argc; i += 2) {
            // if this is a straight variable we want to pass a pointer to its value in RAM
            if(isnamestart((uint8_t)*argv[i]) && (*skipvar(argv[i], false) == 0 || *skipvar(argv[i], false) == ')') && !(FindSubFun(argv[i], 1) >= 0 && strchr(argv[i], '(') != NULL)) {
                arg[i/2] = findvar(argv[i], V_FIND | V_EMPTY_OK /* | V_NOFIND_ERR */ );   // if the argument
                if(typ[i/2] != 0 && !(TypeMask(vartbl[VarIndex].type) & typ[i/2])) error("Incompatible type");
            } else {
                // else it must be an expression of some sort
                // get the value based on the type specified in the definition
                switch(typ[i/2]) {
                    case T_INT: i64[i/2] = getinteger(argv[i]);
                                arg[i/2] = &i64[i/2];
                                break;
                    case T_NBR: ftmp = getnumber(argv[i]);
                                ff[i/2] = ftmp;
                                arg[i/2] = &ff[i/2];
                                break;
                    case T_STR: arg[i/2] = GetTempMemory(STRINGSIZE);
                                Mstrcpy(arg[i/2], getstring(argv[i]));
                                break;
                    default:    // the type has not been specified (old style CFunction)
                                type = T_NOTYPE;
                                evaluate(argv[i], &ftmp, &i64[i/2], &pp, &type, false);
                                ff[i/2] = ftmp;
                                if(type & T_NBR) {
                                    arg[i/2] = &ff[i/2];
                                } else if(type & T_INT)
                                    arg[i/2] = &i64[i/2];
                                else {
                                    arg[i/2] = GetTempMemory(STRINGSIZE);
                                    Mstrcpy(arg[i/2], pp);
                                }
                                break;
                }
            }
        }
    }
    p++;      // step over the size word

    // run the function in flash
    ii = *p++;
    p = (unsigned int *)((unsigned int) p | 0x1);
    ret = ((long long int (*)(void *, void *, void *, void *, void *, void *, void *, void *, void *, void *)) (p + ii)) (arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);              // run the CFunction

    return ret;
}
// If the CFuncmSec vector is set then call the CFunction
void CallCFuncmSec(void){
    typedef void func(void);
    func* f=(func*)(void *)CFuncmSec;
    f();
}

// save the interpreter state if re entering it
void CallExecuteProgram(char *p) {
    char *nextstmtSaved = nextstmt;
    LocalIndex++;
    ExecuteProgram(p);
    nextstmt = nextstmtSaved;
    LocalIndex--;
    TempMemoryIsChanged = true;                                     // signal that temporary memory should be checked
}

// If the CFuncmInt1 vector is set then call the CFunction
void CallCFuncInt1(void){
    typedef void func(void);
    func* f=(func*)(void *)CFuncInt1;
    f();
}

// If the CFuncmInt2 vector is set then call the CFunction
void CallCFuncInt2(void){
    typedef void func(void);
    func* f=(func*)(void *)CFuncInt2;
    f();
}


