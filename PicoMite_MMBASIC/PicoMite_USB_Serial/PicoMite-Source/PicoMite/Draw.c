/***********************************************************************************************************************
PicoMite MMBasic

Draw.c

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
#define LONG long
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define RoundUptoInt(a)     (((a) + (32 - 1)) & (~(32 - 1)))// round up to the nearest whole integer
void DrawFilledCircle(int x, int y, int radius, int r, int fill, int ints_per_line, uint32_t *br, MMFLOAT aspect, MMFLOAT aspect2);
void hline(int x0, int x1, int y, int f, int ints_per_line, uint32_t *br);
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
typedef struct _BMPDECODER
{
        LONG lWidth;
        LONG lHeight;
        LONG lImageOffset;
        WORD wPaletteEntries;
        BYTE bBitsPerPixel;
        BYTE bHeaderType;
        BYTE blBmMarkerFlag : 1;
        BYTE blCompressionType : 3;
        BYTE bNumOfPlanes : 3;
        BYTE b16bit565flag : 1;
        BYTE aPalette[256][3]; /* Each palette entry has RGB */
} BMPDECODER;
/***************************************************************************/
// define the fonts

    #include "font1.h"
    #include "Misc_12x20_LE.h"
    #include "Hom_16x24_LE.h"
    #include "Fnt_10x16.h"
    #include "Inconsola.h"
    #include "ArialNumFontPlus.h"
    #include "Font_8x6.h"
    #include "arial_bold.h"
#ifdef PICOMITEVGA
    #include "Include.h"
#endif
    #include "smallfont.h"

    unsigned char *FontTable[FONT_TABLE_SIZE] = {   (unsigned char *)font1,
                                                    (unsigned char *)Misc_12x20_LE,
#ifdef PICOMITEVGA
                                                    (unsigned char *)arial_bold,
#else
                                                    (unsigned char *)Hom_16x24_LE,
#endif
                                                    (unsigned char *)Fnt_10x16,
                                                    (unsigned char *)Inconsola,
                                                    (unsigned char *)ArialNumFontPlus,
													(unsigned char *)F_6x8_LE,
													(unsigned char *)TinyFont,
                                                    NULL,
                                                    NULL,
                                                    NULL,
                                                    NULL,
                                                    NULL,
                                                    NULL,
                                                    NULL,
                                                    NULL,
                                                };

/***************************************************************************/
// the default function for DrawRectangle() and DrawBitmap()

int gui_font;
int gui_fcolour;
int gui_bcolour;
int low_y=480, high_y=0, low_x=800, high_x=0;
int PrintPixelMode=0;

int CurrentX=0, CurrentY=0;                                             // the current default position for the next char to be written
int DisplayHRes, DisplayVRes;                                       // the physical characteristics of the display
char * blitbuffptr[MAXBLITBUF];                                  //Buffer pointers for the BLIT command
int CMM1=0;
// the MMBasic programming characteristics of the display
// note that HRes == 0 is an indication that a display is not configured
int HRes = 0, VRes = 0;
int lastx,lasty;
const int colourmap[16]={BLACK,BLUE,GREEN,CYAN,RED,MAGENTA,YELLOW,WHITE,MYRTLE,COBALT,MIDGREEN,CERULEAN,RUST,FUCHSIA,BROWN,LILAC};
// pointers to the drawing primitives
#ifdef PICOMITEVGA
int gui_font_width, gui_font_height, last_bcolour, last_fcolour;
volatile int CursorTimer=0;               // used to time the flashing cursor
void (*DrawPixel)(int x1, int y1, int c) = (void (*)(int , int , int ))DisplayNotSet;
extern volatile int QVgaScanLine;
struct D3D* struct3d[MAX3D + 1] = { NULL };
s_camera camera[MAXCAM + 1];
#endif
void (*DrawRectangle)(int x1, int y1, int x2, int y2, int c) = (void (*)(int , int , int , int , int ))DisplayNotSet;
void (*DrawBitmap)(int x1, int y1, int width, int height, int scale, int fc, int bc, unsigned char *bitmap) = (void (*)(int , int , int , int , int , int , int , unsigned char *))DisplayNotSet;
void (*ScrollLCD) (int lines) = (void (*)(int ))DisplayNotSet;
void (*DrawBuffer)(int x1, int y1, int x2, int y2, unsigned char *c) = (void (*)(int , int , int , int , unsigned char * ))DisplayNotSet;
void (*ReadBuffer)(int x1, int y1, int x2, int y2, unsigned char *c) = (void (*)(int , int , int , int , unsigned char * ))DisplayNotSet;
void (*DrawBufferFast)(int x1, int y1, int x2, int y2, unsigned char *c) = (void (*)(int , int , int , int , unsigned char * ))DisplayNotSet;
void (*ReadBufferFast)(int x1, int y1, int x2, int y2, unsigned char *c) = (void (*)(int , int , int , int , unsigned char * ))DisplayNotSet;
void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int c, int fill);
// these are the GUI commands that are common to the MX170 and MX470 versions
// in the case of the MX170 this function is called directly by MMBasic when the GUI command is used
// in the case of the MX470 it is called by MX470GUI in GUI.c
const int colours[16]={0x00,0xFF,0x4000,0x40ff,0x8000,0x80ff,0xff00,0xffff,0xff0000,0xff00FF,0xff4000,0xff40ff,0xff8000,0xff80ff,0xffff00,0xffffff};

void cmd_guiMX170(void) {
    unsigned char *p;

  if(Option.DISPLAY_TYPE == 0) error("Display not configured");
    // display a bitmap stored in an integer or string
    if((p = checkstring(cmdline, "BITMAP"))) {
        int x, y, fc, bc, h, w, scale, t, bytes;
        unsigned char *s;
        MMFLOAT f;
        long long int i64;

        getargs(&p, 15, ",");
        if(!(argc & 1) || argc < 5) error("Argument count");

        // set the defaults
        h = 8; w = 8; scale = 1; bytes = 8; fc = gui_fcolour; bc = gui_bcolour;

        x = getinteger(argv[0]);
        y = getinteger(argv[2]);

        // get the type of argument 3 (the bitmap) and its value (integer or string)
        t = T_NOTYPE;
        evaluate(argv[4], &f, &i64, &s, &t, true);
        if(t & T_NBR)
            error("Invalid argument");
        else if(t & T_INT)
            s = (char *)&i64;
        else if(t & T_STR)
            bytes = *s++;

        if(argc > 5 && *argv[6]) w = getint(argv[6], 1, HRes);
        if(argc > 7 && *argv[8]) h = getint(argv[8], 1, VRes);
        if(argc > 9 && *argv[10]) scale = getint(argv[10], 1, 15);
        if(argc > 11 && *argv[12]) fc = getint(argv[12], 0, WHITE);
        if(argc == 15) bc = getint(argv[14], -1, WHITE);
        if(h * w > bytes * 8) error("Not enough data");
        DrawBitmap(x, y, w, h, scale, fc, bc, (unsigned char *)s);
        if(Option.Refresh)Display_Refresh();
        return;
    }
#ifndef PICOMITEVGA
    if((p = checkstring(cmdline, "BEEP"))) {
        if(Option.TOUCH_Click == 0) error("Click option not set");
        ClickTimer = getint(p, 0, INT_MAX) + 1;
      return;
  	}
    if((p = checkstring(cmdline, "RESET"))) {
        if((checkstring(p, "LCDPANEL"))) {
            InitDisplaySPI(true);
            InitDisplayI2C(true);
            if(Option.TOUCH_CS) {
                GetTouchValue(CMD_PENIRQ_ON);                                      // send the controller the command to turn on PenIRQ
                GetTouchAxis(CMD_MEASURE_X);
            }
            return;
        }
    }

    if((p = checkstring(cmdline, "CALIBRATE"))) {
        int tlx, tly, trx, try, blx, bly, brx, bry, midy;
        char *s;
        if(Option.TOUCH_CS == 0) error("Touch not configured");

        if(*p && *p != '\'') {                                      // if the calibration is provided on the command line
            getargs(&p, 9, ",");
            if(argc != 9) error("Argument count");
            Option.TOUCH_SWAPXY = getinteger(argv[0]);
            Option.TOUCH_XZERO = getinteger(argv[2]);
            Option.TOUCH_YZERO = getinteger(argv[4]);
            Option.TOUCH_XSCALE = getinteger(argv[6]) / 10000.0;
            Option.TOUCH_YSCALE = getinteger(argv[8]) / 10000.0;
            if(!CurrentLinePtr) SaveOptions();
            return;
        } else {
            if(CurrentLinePtr) error("Invalid in a program");
        }
        calibrate=1;
        GetCalibration(TARGET_OFFSET, TARGET_OFFSET, &tlx, &tly);
        GetCalibration(HRes - TARGET_OFFSET, TARGET_OFFSET, &trx, &try);
        if(abs(trx - tlx) < CAL_ERROR_MARGIN && abs(tly - try) < CAL_ERROR_MARGIN) {
            calibrate=0;
            error("Touch hardware failure");
        }

        GetCalibration(TARGET_OFFSET, VRes - TARGET_OFFSET, &blx, &bly);
        GetCalibration(HRes - TARGET_OFFSET, VRes - TARGET_OFFSET, &brx, &bry);
        calibrate=0;
        midy = max(max(tly, try), max(bly, bry)) / 2;
        Option.TOUCH_SWAPXY = ((tly < midy && try > midy) || (tly > midy && try < midy));

        if(Option.TOUCH_SWAPXY) {
            swap(tlx, tly);
            swap(trx, try);
            swap(blx, bly);
            swap(brx, bry);
        }

        Option.TOUCH_XSCALE = (MMFLOAT)(HRes - TARGET_OFFSET * 2) / (MMFLOAT)(trx - tlx);
        Option.TOUCH_YSCALE = (MMFLOAT)(VRes - TARGET_OFFSET * 2) / (MMFLOAT)(bly - tly);
        Option.TOUCH_XZERO = ((MMFLOAT)tlx - ((MMFLOAT)TARGET_OFFSET / Option.TOUCH_XSCALE));
        Option.TOUCH_YZERO = ((MMFLOAT)tly - ((MMFLOAT)TARGET_OFFSET / Option.TOUCH_YSCALE));
        SaveOptions();
        brx = (HRes - TARGET_OFFSET) - ((brx - Option.TOUCH_XZERO) * Option.TOUCH_XSCALE);
        bry = (VRes - TARGET_OFFSET) - ((bry - Option.TOUCH_YZERO)*Option.TOUCH_YSCALE);
        if(abs(brx) > CAL_ERROR_MARGIN || abs(bry) > CAL_ERROR_MARGIN) {
            s = "Warning: Inaccurate calibration\r\n";
        } else
            s = "Done. No errors\r\n";
        CurrentX = CurrentY = 0;
        MMPrintString(s);
        strcpy(inpbuf, "Deviation X = "); IntToStr(inpbuf + strlen(inpbuf), brx, 10);
        strcat(inpbuf, ", Y = "); IntToStr(inpbuf + strlen(inpbuf), bry, 10); strcat(inpbuf, " (pixels)\r\n");
        MMPrintString(inpbuf);
        if(!Option.DISPLAY_CONSOLE) {
            GUIPrintString(0, 0, 0x11, JUSTIFY_LEFT, JUSTIFY_TOP, ORIENT_NORMAL, WHITE, BLACK, s);
            GUIPrintString(0, 36, 0x11, JUSTIFY_LEFT, JUSTIFY_TOP, ORIENT_NORMAL, WHITE, BLACK, inpbuf);
        }
        return;
    }
#endif
    if((p = checkstring(cmdline, "TEST"))) {
        if((checkstring(p, "LCDPANEL"))) {
            int t;
            t = ((HRes > VRes) ? HRes : VRes) / 7;
            while(getConsole() < '\r') {
                DrawCircle(rand() % HRes, rand() % VRes, (rand() % t) + t/5, 1, 1, rgb((rand() % 8)*256/8, (rand() % 8)*256/8, (rand() % 8)*256/8), 1);
            }
            ClearScreen(gui_bcolour);
            return;
        }
#ifndef PICOMITEVGA
        if((checkstring(p, "TOUCH"))) {
            int x, y;
            ClearScreen(gui_bcolour);
            while(getConsole() < '\r') {
                x = GetTouch(GET_X_AXIS);
                y = GetTouch(GET_Y_AXIS);
                if(x != TOUCH_ERROR && y != TOUCH_ERROR) DrawBox(x - 1, y - 1, x + 1, y + 1, 0, WHITE, WHITE);
            }
            ClearScreen(gui_bcolour);
            return;
        }
#endif
    }
    error("Unknown command");
}



void  getargaddress (unsigned char *p, long long int **ip, MMFLOAT **fp, int *n){
    unsigned char *ptr=NULL;
    *fp=NULL;
    *ip=NULL;
    char pp[STRINGSIZE]={0};
    strcpy(pp,(char *)p);
    if(!isnamestart(pp[0])){ //found a literal
        *n=1;
        return;
    }
    ptr = findvar(pp, V_FIND | V_EMPTY_OK | V_NOFIND_NULL);
    if(ptr && vartbl[VarIndex].type & T_NBR) {
        if(vartbl[VarIndex].dims[0] <= 0){ //simple variable
            *n=1;
            return;
        } else { // array or array element
            if(*n == 0)*n=vartbl[VarIndex].dims[0] + 1 - OptionBase;
            else *n = (vartbl[VarIndex].dims[0] + 1 - OptionBase)< *n ? (vartbl[VarIndex].dims[0] + 1 - OptionBase) : *n;
            skipspace(p);
            do {
                p++;
            } while(isnamechar(*p));
            if(*p == '!') p++;
            if(*p == '(') {
                p++;
                skipspace(p);
                if(*p != ')') { //array element
                    *n=1;
                    return;
                }
            }
        }
        if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
        *fp = (MMFLOAT*)ptr;
    } else if(ptr && vartbl[VarIndex].type & T_INT) {
        if(vartbl[VarIndex].dims[0] <= 0){
            *n=1;
            return;
        } else {
            if(*n == 0)*n=vartbl[VarIndex].dims[0] + 1 - OptionBase;
            else *n = (vartbl[VarIndex].dims[0] + 1 - OptionBase)< *n ? (vartbl[VarIndex].dims[0] + 1 - OptionBase) : *n;
            skipspace(p);
            do {
                p++;
            } while(isnamechar(*p));
            if(*p == '%') p++;
            if(*p == '(') {
                p++;
                skipspace(p);
                if(*p != ')') { //array element
                    *n=1;
                    return;
                }
            }
        }
        if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
        *ip = (long long int *)ptr;
    } else {
    	*n=1; //may be a function call
    }
}

/****************************************************************************************************

 General purpose drawing routines

****************************************************************************************************/
int rgb(int r, int g, int b) {
    return RGB(r, g, b);
}
void getcoord(char *p, int *x, int *y) {
	unsigned char *tp, *ttp;
	char b[STRINGSIZE];
	char savechar;
	tp = getclosebracket(p);
	savechar=*tp;
	*tp = 0;														// remove the closing brackets
	strcpy(b, p);													// copy the coordinates to the temp buffer
	*tp = savechar;														// put back the closing bracket
	ttp = b+1;
	// kludge (todo: fix this)
	{
		getargs(&ttp, 3, ",");										// this is a macro and must be the first executable stmt in a block
		if(argc != 3) error("Invalid Syntax");
		*x = getinteger(argv[0]);
		*y = getinteger(argv[2]);
	}
}

int getColour(char *c, int minus){
	int colour;
	if(CMM1){
		colour = getint(c,(minus ? -1: 0),15);
		if(colour>=0)colour=colourmap[colour];
	} else colour=getint(c,(minus ? -1: 0),0xFFFFFFF);
	return colour;

}
#ifndef PICOMITEVGA
void DrawPixel(int x, int y, int c) {
    DrawRectangle(x, y, x, y, c);
}
#endif
void ClearScreen(int c) {
    DrawRectangle(0, 0, HRes - 1, VRes - 1, c);
}
void DrawBuffered(int xti, int yti, int c, int complete){
	static unsigned char pos=0;
	static unsigned char movex, movey, movec;
	static short xtilast[8];
	static short ytilast[8];
	static int clast[8];
	xtilast[pos]=xti;
	ytilast[pos]=yti;
	clast[pos]=c;
	if(complete==1){
		if(pos==1){
			DrawPixel(xtilast[0],ytilast[0],clast[0]);
		} else {
			DrawLine(xtilast[0],ytilast[0],xtilast[pos-1],ytilast[pos-1],1,clast[0]);
		}
		pos=0;
	} else {
		if(pos==0){
			movex = movey = movec = 1;
			pos+=1;
		} else {
			if(xti==xtilast[0] && abs(yti-ytilast[pos-1])==1)movex=0;else movex=1;
			if(yti==ytilast[0] && abs(xti-xtilast[pos-1])==1)movey=0;else movey=1;
			if(c==clast[0])movec=0;else movec=1;
			if(movec==0 && (movex==0 || movey==0) && pos<6) pos+=1;
			else {
				if(pos==1){
					DrawPixel(xtilast[0],ytilast[0],clast[0]);
				} else {
					DrawLine(xtilast[0],ytilast[0],xtilast[pos-1],ytilast[pos-1],1,clast[0]);
				}
				movex = movey = movec = 1;
				xtilast[0]=xti;
				ytilast[0]=yti;
				clast[0]=c;
				pos=1;
			}
		}
	}
}


/**************************************************************************************************
Draw a line on a the video output
  x1, y1 - the start coordinate
  x2, y2 - the end coordinate
    w - the width of the line (ignored for diagional lines)
  c - the colour to use
***************************************************************************************************/
#define abs( a)     (((a)> 0) ? (a) : -(a))

void DrawLine(int x1, int y1, int x2, int y2, int w, int c) {


    if(y1 == y2) {
        DrawRectangle(x1, y1, x2, y2 + w - 1, c);                   // horiz line
    if(Option.Refresh)Display_Refresh();
        return;
    }
    if(x1 == x2) {
        DrawRectangle(x1, y1, x2 + w - 1, y2, c);                   // vert line
    if(Option.Refresh)Display_Refresh();
        return;
    }
    int  dx, dy, sx, sy, err, e2;
    dx = abs(x2 - x1); sx = x1 < x2 ? 1 : -1;
    dy = -abs(y2 - y1); sy = y1 < y2 ? 1 : -1;
    err = dx + dy;
    while(1) {
        DrawBuffered(x1, y1, c,0);
        e2 = 2 * err;
        if (e2 >= dy) {
            if (x1 == x2) break;
            err += dy; x1 += sx;
        }
        if (e2 <= dx) {
            if (y1 == y2) break;
            err += dx; y1 += sy;
        }
    }
    DrawBuffered(0, 0, 0, 1);
    if(Option.Refresh)Display_Refresh();
}


/**********************************************************************************************
Draw a box
     x1, y1 - the start coordinate
     x2, y2 - the end coordinate
     w      - the width of the sides of the box (can be zero)
     c      - the colour to use for sides of the box
     fill   - the colour to fill the box (-1 for no fill)
***********************************************************************************************/
void DrawBox(int x1, int y1, int x2, int y2, int w, int c, int fill) {
    int t;

    // make sure the coordinates are in the right sequence
    if(x2 <= x1) { t = x1; x1 = x2; x2 = t; }
    if(y2 <= y1) { t = y1; y1 = y2; y2 = t; }
    if(w > x2 - x1) w = x2 - x1;
    if(w > y2 - y1) w = y2 - y1;

    if(w > 0) {
        w--;
        DrawRectangle(x1, y1, x2, y1 + w, c);                       // Draw the top horiz line
        DrawRectangle(x1, y2 - w, x2, y2, c);                       // Draw the bottom horiz line
        DrawRectangle(x1, y1, x1 + w, y2, c);                       // Draw the left vert line
        DrawRectangle(x2 - w, y1, x2, y2, c);                       // Draw the right vert line
        w++;
    }

    if(fill >= 0)
        DrawRectangle(x1 + w, y1 + w, x2 - w, y2 - w, fill);
}



/**********************************************************************************************
Draw a box with rounded corners
     x1, y1 - the start coordinate
     x2, y2 - the end coordinate
     radius - the radius (in pixels) of the arc forming the corners
     c      - the colour to use for sides
     fill   - the colour to fill the box (-1 for no fill)
***********************************************************************************************/
void DrawRBox(int x1, int y1, int x2, int y2, int radius, int c, int fill) {
    int f, ddF_x, ddF_y, xx, yy;

    f = 1 - radius;
    ddF_x = 1;
    ddF_y = -2 * radius;
    xx = 0;
    yy = radius;

    while(xx < yy) {
        if(f >= 0) {
            yy-=1;
            ddF_y += 2;
            f += ddF_y;
        }
        xx+=1;
        ddF_x += 2;
        f += ddF_x  ;
        DrawPixel(x2 + xx - radius, y2 + yy - radius, c);           // Bottom Right Corner
        DrawPixel(x2 + yy - radius, y2 + xx - radius, c);           // ^^^
        DrawPixel(x1 - xx + radius, y2 + yy - radius, c);           // Bottom Left Corner
        DrawPixel(x1 - yy + radius, y2 + xx - radius, c);           // ^^^

        DrawPixel(x2 + xx - radius, y1 - yy + radius, c);           // Top Right Corner
        DrawPixel(x2 + yy - radius, y1 - xx + radius, c);           // ^^^
        DrawPixel(x1 - xx + radius, y1 - yy + radius, c);           // Top Left Corner
        DrawPixel(x1 - yy + radius, y1 - xx + radius, c);           // ^^^
        if(fill >= 0) {
            DrawLine(x2 + xx - radius - 1, y2 + yy - radius, x1 - xx + radius + 1, y2 + yy - radius, 1, fill);
            DrawLine(x2 + yy - radius - 1, y2 + xx - radius, x1 - yy + radius + 1, y2 + xx - radius, 1, fill);
            DrawLine(x2 + xx - radius - 1, y1 - yy + radius, x1 - xx + radius + 1, y1 - yy + radius, 1, fill);
            DrawLine(x2 + yy - radius - 1, y1 - xx + radius, x1 - yy + radius + 1, y1 - xx + radius, 1, fill);
        }
    }
    if(fill >= 0) DrawRectangle(x1 + 1, y1 + radius, x2 - 1, y2 - radius, fill);
    DrawRectangle(x1 + radius - 1, y1, x2 - radius + 1, y1, c);     // top side
    DrawRectangle(x1 + radius - 1, y2,  x2 - radius + 1, y2, c);    // botom side
    DrawRectangle(x1, y1 + radius, x1, y2 - radius, c);             // left side
    DrawRectangle(x2, y1 + radius, x2, y2 - radius, c);             // right side
        if(Option.Refresh)Display_Refresh();

}




/***********************************************************************************************
Draw a circle on the video output
  x, y - the center of the circle
  radius - the radius of the circle
    w - width of the line drawing the circle
  c - the colour to use for the circle
  fill - the colour to use for the fill or -1 if no fill
  aspect - the ration of the x and y axis (a MMFLOAT).  1.0 gives a prefect circle
***********************************************************************************************/
/***********************************************************************************************
Draw a circle on the video output
	x, y - the center of the circle
	radius - the radius of the circle
    w - width of the line drawing the circle
	c - the colour to use for the circle
	fill - the colour to use for the fill or -1 if no fill
	aspect - the ration of the x and y axis (a MMFLOAT).  1.0 gives a prefect circle
***********************************************************************************************/
void DrawCircle(int x, int y, int radius, int w, int c, int fill, MMFLOAT aspect) {
   int a, b, P;
   int A, B;
   int asp;
   MMFLOAT aspect2;
   if(w>1){
	   if(fill>=0){ // thick border with filled centre
		   DrawCircle(x,y,radius,0,c,c,aspect);
		    aspect2=((aspect*(MMFLOAT)radius)-(MMFLOAT)w)/((MMFLOAT)(radius-w));
		   DrawCircle(x,y,radius-w,0,fill,fill,aspect2);
	   } else { //thick border with empty centre
		   	int r1=radius-w,r2=radius, xs=-1,xi=0, i,j,k,m, ll=radius;
		    if(aspect>1.0)ll=(int)((MMFLOAT)radius*aspect);
		    int ints_per_line=RoundUptoInt((ll*2)+1)/32;
		    uint32_t *br=(uint32_t *)GetTempMemory(((ints_per_line+1)*((r2*2)+1))*4);
		    DrawFilledCircle(x, y, r2, r2, 1, ints_per_line, br, aspect, aspect);
		    aspect2=((aspect*(MMFLOAT)r2)-(MMFLOAT)w)/((MMFLOAT)r1);
		    DrawFilledCircle(x, y, r1, r2, 0, ints_per_line, br, aspect, aspect2);
		    x=(int)((MMFLOAT)x+(MMFLOAT)r2*(1.0-aspect));
		 	for(j=0;j<r2*2+1;j++){
		 		for(i=0;i<ints_per_line;i++){
		 			k=br[i+j*ints_per_line];
		 			for(m=0;m<32;m++){
		 				if(xs==-1 && (k & 0x80000000)){
		 					xs=m;
		 					xi=i;
		 				}
		 				if(xs!=-1 && !(k & 0x80000000)){
							DrawRectangle(x-r2+xs+xi*32, y-r2+j, x-r2+m+i*32, y-r2+j, c);
		 					xs=-1;
		 				}
		 				k<<=1;
		 			}
		 		}
				if(xs!=-1){
					DrawRectangle(x-r2+xs+xi*32, y-r2+j, x-r2+m+i*32, y-r2+j, c);
					xs=-1;
				}
			}
	   }

   } else { //single thickness outline
	   int w1=w,r1=radius;
	   if(fill>=0){
		   while(w >= 0 && radius > 0) {
		       a = 0;
		       b = radius;
		       P = 1 - radius;
		       asp = aspect * (MMFLOAT)(1 << 10);

		       do {
		         A = (a * asp) >> 10;
		         B = (b * asp) >> 10;
		         if(fill >= 0 && w >= 0) {
		             DrawRectangle(x-A, y+b, x+A, y+b, fill);
		             DrawRectangle(x-A, y-b, x+A, y-b, fill);
		             DrawRectangle(x-B, y+a, x+B, y+a, fill);
		             DrawRectangle(x-B, y-a, x+B, y-a, fill);
		         }
		          if(P < 0)
		             P+= 3 + 2*a++;
		          else
		             P+= 5 + 2*(a++ - b--);

		        } while(a <= b);
		        w--;
		        radius--;
		   }
	   }
	   if(c!=fill){
		   w=w1; radius=r1;
		   while(w >= 0 && radius > 0) {
		       a = 0;
		       b = radius;
		       P = 1 - radius;
		       asp = aspect * (MMFLOAT)(1 << 10);
		       do {
		         A = (a * asp) >> 10;
		         B = (b * asp) >> 10;
		         if(w) {
		             DrawPixel(A+x, b+y, c);
		             DrawPixel(B+x, a+y, c);
		             DrawPixel(x-A, b+y, c);
		             DrawPixel(x-B, a+y, c);
		             DrawPixel(B+x, y-a, c);
		             DrawPixel(A+x, y-b, c);
		             DrawPixel(x-A, y-b, c);
		             DrawPixel(x-B, y-a, c);
		         }
		          if(P < 0)
		             P+= 3 + 2*a++;
		          else
		             P+= 5 + 2*(a++ - b--);

		        } while(a <= b);
		        w--;
		        radius--;
		   }
	   }
   }

    if(Option.Refresh)Display_Refresh();
}



void ClearTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int ints_per_line, uint32_t *br) {
        if (x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1) == 0)return;

        long a, b, y, last;
        long  dx01,  dy01,  dx02,  dy02, dx12,  dy12,  sa, sb;

        if (y0 > y1) {
            swap(y0, y1);
            swap(x0, x1);
        }
        if (y1 > y2) {
            swap(y2, y1);
            swap(x2, x1);
        }
        if (y0 > y1) {
            swap(y0, y1);
            swap(x0, x1);
        }

            dx01 = x1 - x0;  dy01 = y1 - y0;  dx02 = x2 - x0;
            dy02 = y2 - y0; dx12 = x2 - x1;  dy12 = y2 - y1;
            sa = 0; sb = 0;
            if(y1 == y2) {
                last = y1;                                          //Include y1 scanline
            } else {
                last = y1 - 1;                                      // Skip it
            }
            for (y = y0; y <= last; y++){
                a = x0 + sa / dy01;
                b = x0 + sb / dy02;
                sa = sa + dx01;
                sb = sb + dx02;
                a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
                b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
                if(a > b)swap(a, b);
                hline(a, b,  y, 0, ints_per_line, br);
            }
            sa = dx12 * (y - y1);
            sb = dx02 * ( y- y0);
            while (y <= y2){
                a = x1 + sa / dy12;
                b = x0 + sb / dy02;
                sa = sa + dx12;
                sb = sb + dx02;
                a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
                b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
                if(a > b) swap(a, b);
                hline(a, b,  y, 0, ints_per_line, br);
                y = y + 1;
            }
}
#define ABS(X) ((X)>0 ? (X) : (-(X)))

void CalcLine(int x1, int y1, int x2, int y2, short *xmin, short *xmax) {

    if(y1 == y2) {
    	if(y1<0)y1=0;
    	if(y1>=480)y1=479;
    	if(y2<0)y2=0;
    	if(y2>=480)y2=479;
		if(x1<xmin[y1])xmin[y1]=x1;
		if(x2<xmin[y1])xmin[y1]=x2;
		if(x1>xmax[y1])xmax[y1]=x1;
		if(x2>xmax[y1])xmax[y1]=x2;
        return;
    }
    if(x1 == x2) {
		if(y2<y1)swap(y2,y1);
    	if(y1<0)y1=0;
    	if(y1>=480)y1=479;
    	if(y2<0)y2=0;
    	if(y2>=480)y2=479;
		for(int y=y1;y<=y2;y++) {
			if(x1<xmin[y])xmin[y]=x1;
			if(x1>xmax[y])xmax[y]=x1;
		}
        return;
    }
	// uses a variant of Bresenham's line algorithm:
	//   https://en.wikipedia.org/wiki/Talk:Bresenham%27s_line_algorithm
	if (y1 > y2) {
		swap(y1, y2);
		swap(x1, x2);
	}
	if(y1<0)y1=0;
	if(y1>=480)y1=479;
	if(y2<0)y2=0;
	if(y2>=480)y2=479;
	int absX = ABS(x1-x2);          // absolute value of coordinate distances
	int absY = ABS(y1-y2);
	int offX = x2<x1 ? 1 : -1;      // line-drawing direction offsets
	int offY = y2<y1 ? 1 : -1;
	int x = x2;                     // incremental location
	int y = y2;
	int err;
	if(x<xmin[y])xmin[y]=x;
	if(x>xmax[y])xmax[y]=x;
	if (absX > absY) {

		// line is more horizontal; increment along x-axis
		err = absX / 2;
		while (x != x1) {
			err = err - absY;
			if (err < 0) {
				y   += offY;
				err += absX;
			}
			x += offX;
    		if(x<xmin[y])xmin[y]=x;
    		if(x>xmax[y])xmax[y]=x;
		}
	} else {

		// line is more vertical; increment along y-axis
		err = absY / 2;
		while (y != y1) {
			err = err - absX;
			if (err < 0) {
				x   += offX;
				err += absY;
			}
			y += offY;
    		if(x<xmin[y])xmin[y]=x;
    		if(x>xmax[y])xmax[y]=x;
		}
	}
}

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int c, int f) {
	if(x0 * (y1 - y2) +  x1 * (y2 - y0) +  x2 * (y0 - y1)==0){ // points are co-linear i.e zero area
		if (y0 > y1) {
			swap(y0, y1);
			swap(x0, x1);
		}
		if (y1 > y2) {
			swap(y2, y1);
			swap(x2, x1);
		}
		if (y0 > y1) {
			swap(y0, y1);
			swap(x0, x1);
		}
		DrawLine(x0,y0,x2,y2,1,c);
	} else {
		if(f == -1){
			// draw only the outline
			DrawLine(x0, y0, x1, y1, 1, c);
			DrawLine(x1, y1, x2, y2, 1, c);
			DrawLine(x2, y2, x0, y0, 1, c);
		} else {
			if (y0 > y1) {
				swap(y0, y1);
				swap(x0, x1);
			}
			if (y1 > y2) {
				swap(y2, y1);
				swap(x2, x1);
			}
			if (y0 > y1) {
				swap(y0, y1);
				swap(x0, x1);
			}
			short *xmin=(short *)GetMemory(480*sizeof(short));
			short *xmax=(short *)GetMemory(480*sizeof(short));

			int y;
			for(y=y0; y<=y2; y++){
				if(y>=0 && y<480){
					xmin[y]=32767;
					xmax[y]=-1;
				}
			}
			CalcLine(x0, y0, x1, y1, xmin, xmax);
			CalcLine(x1, y1, x2, y2, xmin, xmax);
			CalcLine(x2, y2, x0, y0, xmin, xmax);
			for(y=y0;y<=y2;y++){
				if(y>=0 && y<VRes)DrawRectangle(xmin[y], y, xmax[y], y, c);
			}
            if(c!=f){
				DrawLine(x0, y0, x1, y1, 1, c);
				DrawLine(x1, y1, x2, y2, 1, c);
				DrawLine(x2, y2, x0, y0, 1, c);
            }
            FreeMemory((unsigned char *)xmin);
            FreeMemory((unsigned char *)xmax);
		}
	}

}


/******************************************************************************************
 Print a char on the LCD display
 Any characters not in the font will print as a space.
 The char is printed at the current location defined by CurrentX and CurrentY
*****************************************************************************************/
void GUIPrintChar(int fnt, int fc, int bc, char c, int orientation) {
    unsigned char *p, *fp, *np = NULL, *AllocatedMemory = NULL;
    int BitNumber, BitPos, x, y, newx, newy, modx, mody, scale = fnt & 0b1111;
    int height, width;
    if(PrintPixelMode==1)bc=-1;
    if(PrintPixelMode==2){
    	int s=bc;
    	bc=fc;
    	fc=s;
    }
    if(PrintPixelMode==5){
    	fc=bc;
    	bc=-1;
    }

    // to get the +, - and = chars for font 6 we fudge them by scaling up font 1
    if((fnt & 0xf0) == 0x50 && (c == '-' || c == '+' || c == '=')) {
        fp = (unsigned char *)FontTable[0];
        scale = scale * 4;
    } else
        fp = (unsigned char *)FontTable[fnt >> 4];

    height = fp[1];
    width = fp[0];
    modx = mody = 0;
    if(orientation > ORIENT_VERT){
        AllocatedMemory = np = GetMemory(width * height);
        if (orientation == ORIENT_INVERTED) {
            modx -= width * scale -1;
            mody -= height * scale -1;
        }
        else if (orientation == ORIENT_CCW90DEG) {
            mody -= width * scale;
        }
        else if (orientation == ORIENT_CW90DEG){
            modx -= height * scale -1;
        }
    }

    if(c >= fp[2] && c < fp[2] + fp[3]) {
        p = fp + 4 + (int)(((c - fp[2]) * height * width) / 8);

        if(orientation > ORIENT_VERT) {                             // non-standard orientation
            if (orientation == ORIENT_INVERTED) {
                for(y = 0; y < height; y++) {
                    newy = height - y - 1;
                    for(x=0; x < width; x++) {
                        newx = width - x - 1;
                        if((p[((y * width) + x)/8] >> (((height * width) - ((y * width) + x) - 1) %8)) & 1) {
                            BitNumber=((newy * width) + newx);
                            BitPos = 128 >> (BitNumber % 8);
                            np[BitNumber / 8] |= BitPos;
                        }
                    }
                }
            } else if (orientation == ORIENT_CCW90DEG) {
                for(y = 0; y < height; y++) {
                    newx = y;
                    for(x = 0; x < width; x++) {
                        newy = width - x - 1;
                        if((p[((y * width) + x)/8] >> (((height * width) - ((y * width) + x) - 1) %8)) & 1) {
                            BitNumber=((newy * height) + newx);
                            BitPos = 128 >> (BitNumber % 8);
                            np[BitNumber / 8] |= BitPos;
                        }
                    }
                }
            } else if (orientation == ORIENT_CW90DEG) {
                for(y = 0; y < height; y++) {
                    newx = height - y - 1;
                    for(x=0; x < width; x++) {
                        newy = x;
                        if((p[((y * width) + x)/8] >> (((height * width) - ((y * width) + x) - 1) %8)) & 1) {
                            BitNumber=((newy * height) + newx);
                            BitPos = 128 >> (BitNumber % 8);
                            np[BitNumber / 8] |= BitPos;
                        }
                    }
                }
            }
        }  else np = p;

        if(orientation < ORIENT_CCW90DEG) DrawBitmap(CurrentX + modx, CurrentY + mody, width, height, scale, fc, bc, np);
        else DrawBitmap(CurrentX + modx, CurrentY + mody, height, width, scale, fc, bc, np);
    } else {
        if(orientation < ORIENT_CCW90DEG) DrawRectangle(CurrentX + modx, CurrentY + mody, CurrentX + modx + (width * scale), CurrentY + mody + (height * scale), bc);
        else DrawRectangle(CurrentX + modx, CurrentY + mody, CurrentX + modx + (height * scale), CurrentY + mody + (width * scale), bc);
    }

    // to get the . and degree symbols for font 6 we draw a small circle
    if((fnt & 0xf0) == 0x50) {
        if(orientation > ORIENT_VERT) {
            if(orientation == ORIENT_INVERTED) {
                if(c == '.') DrawCircle(CurrentX + modx + (width * scale)/2, CurrentY + mody + 7 * scale, 4 * scale, 0, fc, fc, 1.0);
                if(c == 0x60) DrawCircle(CurrentX + modx + (width * scale)/2, CurrentY + mody + (height * scale)- 9 * scale, 6 * scale, 2 * scale, fc, -1, 1.0);
            } else if(orientation == ORIENT_CCW90DEG) {
                if(c == '.') DrawCircle(CurrentX + modx + (height * scale) - 7 * scale, CurrentY + mody + (width * scale)/2, 4 * scale, 0, fc, fc, 1.0);
                if(c == 0x60) DrawCircle(CurrentX + modx + 9 * scale, CurrentY + mody + (width * scale)/2, 6 * scale, 2 * scale, fc, -1, 1.0);
            } else if(orientation == ORIENT_CW90DEG) {
                if(c == '.') DrawCircle(CurrentX + modx + 7 * scale, CurrentY + mody + (width * scale)/2, 4 * scale, 0, fc, fc, 1.0);
                if(c == 0x60) DrawCircle(CurrentX + modx + (height * scale)- 9 * scale, CurrentY + mody + (width * scale)/2, 6 * scale, 2 * scale, fc, -1, 1.0);
            }

        } else {
            if(c == '.') DrawCircle(CurrentX + modx + (width * scale)/2, CurrentY + mody + (height * scale) - 7 * scale, 4 * scale, 0, fc, fc, 1.0);
            if(c == 0x60) DrawCircle(CurrentX + modx + (width * scale)/2, CurrentY + mody + 9 * scale, 6 * scale, 2 * scale, fc, -1, 1.0);
        }
    }

    if(orientation == ORIENT_NORMAL) CurrentX += width * scale;
    else if (orientation == ORIENT_VERT) CurrentY += height * scale;
    else if (orientation == ORIENT_INVERTED) CurrentX -= width * scale;
    else if (orientation == ORIENT_CCW90DEG) CurrentY -= width * scale;
    else if (orientation == ORIENT_CW90DEG ) CurrentY += width * scale;
    if(orientation > ORIENT_VERT) FreeMemory(AllocatedMemory);
}


/******************************************************************************************
 Print a string on the LCD display
 The string must be a C string (not an MMBasic string)
 Any characters not in the font will print as a space.
*****************************************************************************************/
void GUIPrintString(int x, int y, int fnt, int jh, int jv, int jo, int fc, int bc, char *str) {
    CurrentX = x;  CurrentY = y;
    if(jo == ORIENT_NORMAL) {
        if(jh == JUSTIFY_CENTER) CurrentX -= (strlen(str) * GetFontWidth(fnt)) / 2;
        if(jh == JUSTIFY_RIGHT)  CurrentX -= (strlen(str) * GetFontWidth(fnt));
        if(jv == JUSTIFY_MIDDLE) CurrentY -= GetFontHeight(fnt) / 2;
        if(jv == JUSTIFY_BOTTOM) CurrentY -= GetFontHeight(fnt);
    }
    else if(jo == ORIENT_VERT) {
        if(jh == JUSTIFY_CENTER) CurrentX -= GetFontWidth(fnt)/ 2;
        if(jh == JUSTIFY_RIGHT)  CurrentX -= GetFontWidth(fnt);
        if(jv == JUSTIFY_MIDDLE) CurrentY -= (strlen(str) * GetFontHeight(fnt)) / 2;
        if(jv == JUSTIFY_BOTTOM) CurrentY -= (strlen(str) * GetFontHeight(fnt));
    }
    else if(jo == ORIENT_INVERTED) {
        if(jh == JUSTIFY_CENTER) CurrentX += (strlen(str) * GetFontWidth(fnt)) / 2;
        if(jh == JUSTIFY_RIGHT)  CurrentX += (strlen(str) * GetFontWidth(fnt));
        if(jv == JUSTIFY_MIDDLE) CurrentY += GetFontHeight(fnt) / 2;
        if(jv == JUSTIFY_BOTTOM) CurrentY += GetFontHeight(fnt);
    }
    else if(jo == ORIENT_CCW90DEG) {
        if(jh == JUSTIFY_CENTER) CurrentX -=  GetFontHeight(fnt) / 2;
        if(jh == JUSTIFY_RIGHT)  CurrentX -=  GetFontHeight(fnt);
        if(jv == JUSTIFY_MIDDLE) CurrentY += (strlen(str) * GetFontWidth(fnt)) / 2;
        if(jv == JUSTIFY_BOTTOM) CurrentY += (strlen(str) * GetFontWidth(fnt));
    }
    else if(jo == ORIENT_CW90DEG) {
        if(jh == JUSTIFY_CENTER) CurrentX += GetFontHeight(fnt) / 2;
        if(jh == JUSTIFY_RIGHT)  CurrentX += GetFontHeight(fnt);
        if(jv == JUSTIFY_MIDDLE) CurrentY -= (strlen(str) * GetFontWidth(fnt)) / 2;
        if(jv == JUSTIFY_BOTTOM) CurrentY -= (strlen(str) * GetFontWidth(fnt));
    }
    while(*str) {
        if(*str == 0xff) {
//            fc = rgb(0, 0, 255);                                // this is specially for GUI FORMATBOX
            str++;
            GUIPrintChar(fnt, bc, fc, *str++, jo);
        } else
            GUIPrintChar(fnt, fc, bc, *str++, jo);
    }
}

/****************************************************************************************************

 MMBasic commands and functions

****************************************************************************************************/


// get and decode the justify$ string used in TEXT and GUI CAPTION
// the values are returned via pointers
int GetJustification(char *p, int *jh, int *jv, int *jo) {
    switch(toupper(*p++)) {
        case 'L':   *jh = JUSTIFY_LEFT; break;
        case 'C':   *jh = JUSTIFY_CENTER; break;
        case 'R':   *jh = JUSTIFY_RIGHT; break;
        case  0 :   return true;
        default:    p--;
    }
    skipspace(p);
    switch(toupper(*p++)) {
        case 'T':   *jv = JUSTIFY_TOP; break;
        case 'M':   *jv = JUSTIFY_MIDDLE; break;
        case 'B':   *jv = JUSTIFY_BOTTOM; break;
        case  0 :   return true;
        default:    p--;
    }
    skipspace(p);
    switch(toupper(*p++)) {
        case 'N':   *jo = ORIENT_NORMAL; break;                     // normal
        case 'V':   *jo = ORIENT_VERT; break;                       // vertical text (top to bottom)
        case 'I':   *jo = ORIENT_INVERTED; break;                   // inverted
        case 'U':   *jo = ORIENT_CCW90DEG; break;                   // rotated CCW 90 degrees
        case 'D':   *jo = ORIENT_CW90DEG; break;                    // rotated CW 90 degrees
        case  0 :   return true;
        default:    return false;
    }
    return *p == 0;
}



void cmd_text(void) {
    int x, y, font, scale, fc, bc;
    char *s;
    int jh = 0, jv = 0, jo = 0;

    getargs(&cmdline, 17, ",");                                     // this is a macro and must be the first executable stmt
    if(Option.DISPLAY_TYPE == 0) error("Display not configured");
    if(!(argc & 1) || argc < 5) error("Argument count");
    x = getinteger(argv[0]);
    y = getinteger(argv[2]);
    s = getCstring(argv[4]);

    if(argc > 5 && *argv[6])
        if(!GetJustification(argv[6], &jh, &jv, &jo))
            if(!GetJustification(getCstring(argv[6]), &jh, &jv, &jo))
                error("Justification");;

    font = (gui_font >> 4) + 1; scale = (gui_font & 0b1111); fc = gui_fcolour; bc = gui_bcolour;        // the defaults
    if(argc > 7 && *argv[8]) {
        if(*argv[8] == '#') argv[8]++;
        font = getint(argv[8], 1, FONT_TABLE_SIZE);
    }
    if(FontTable[font - 1] == NULL) error("Invalid font #%", font);
    if(argc > 9 && *argv[10]) scale = getint(argv[10], 1, 15);
    if(argc > 11 && *argv[12]) fc = getint(argv[12], 0, WHITE);
    if(argc ==15) bc = getint(argv[14], -1, WHITE);
    if(DISPLAY_TYPE== MONOVGA && ((fc && bc) || (fc==0 && bc==0)))error("Foreground and Background colours are the same");
    GUIPrintString(x, y, ((font - 1) << 4) | scale, jh, jv, jo, fc, bc, s);
    if(Option.Refresh)Display_Refresh();
}



void cmd_pixel(void) {
    if(Option.DISPLAY_TYPE == 0) error("Display not configured");
	if(CMM1){
		int x, y, value;
		getcoord(cmdline, &x, &y);
		cmdline = getclosebracket(cmdline) + 1;
		while(*cmdline && tokenfunction(*cmdline) != op_equal) cmdline++;
		if(!*cmdline) error("Invalid syntax");
		++cmdline;
		if(!*cmdline) error("Invalid syntax");
		value = getColour(cmdline,0);
		DrawPixel(x, y, value);
		lastx = x; lasty = y;
	} else {
        int x1, y1, c=0, n=0 ,i, nc=0;
        long long int *x1ptr, *y1ptr, *cptr;
        MMFLOAT *x1fptr, *y1fptr, *cfptr;
        getargs(&cmdline, 5,",");
        if(!(argc == 3 || argc == 5)) error("Argument count");
        getargaddress(argv[0], &x1ptr, &x1fptr, &n);
        if(n != 1) getargaddress(argv[2], &y1ptr, &y1fptr, &n);
        if(n==1){ //just a single point
            c = gui_fcolour;                                    // setup the defaults
            x1 = getinteger(argv[0]);
            y1 = getinteger(argv[2]);
            if(argc == 5)
                c = getint(argv[4], 0, WHITE);
            else
                c = gui_fcolour;
            DrawPixel(x1, y1, c);
        } else {
            c = gui_fcolour;                                        // setup the defaults
            if(argc == 5){
                getargaddress(argv[4], &cptr, &cfptr, &nc); 
                if(nc == 1) c = getint(argv[4], 0, WHITE);
                else if(nc>1) {
                    if(nc < n) n=nc; //adjust the dimensionality
                    for(i=0;i<nc;i++){
                        c = (cfptr == NULL ? cptr[i] : (int)cfptr[i]);
                        if(c < 0 || c > WHITE) error("% is invalid (valid is % to %)", (int)c, 0, WHITE);
                    }
                }
            }
            for(i=0;i<n;i++){
                x1 = (x1fptr == NULL ? x1ptr[i] : (int)x1fptr[i]);
                y1 = (y1fptr == NULL ? y1ptr[i] : (int)y1fptr[i]);
                if(nc > 1) c = (cfptr == NULL ? cptr[i] : (int)cfptr[i]);
                DrawPixel(x1, y1, c);
            }
        }
    }
    if(Option.Refresh)Display_Refresh();
}


void cmd_circle(void) {
    if(Option.DISPLAY_TYPE == 0) error("Display not configured");
	if(CMM1){
		int x, y, radius, colour, fill;
		float aspect;
		getargs(&cmdline, 9, ",");
		if(argc%2 == 0 || argc < 3) error("Invalid syntax");
		if(*argv[0] != '(') error("Expected opening bracket");
		if(toupper(*argv[argc - 1]) == 'F') {
	    	argc -= 2;
	    	fill = true;
	    } else fill = false;
		getcoord(argv[0] , &x, &y);
		radius = getinteger(argv[2]);
		if(radius == 0) return;                                         //nothing to draw
		if(radius < 1) error("Invalid argument");
		if(argc > 3 && *argv[4])colour = getColour(argv[4],0);
		else colour = gui_fcolour;

		if(argc > 5 && *argv[6])
		    aspect = getnumber(argv[6]);
		else
		    aspect = 1;

		DrawCircle(x, y, radius, (fill ? 0:1), colour, (fill ? colour : -1), aspect);
		lastx = x; lasty = y;
	} else {
        int x, y, r, w=0, c=0, f=0, n=0 ,i, nc=0, nw=0, nf=0, na=0;
        MMFLOAT a;
        long long int *xptr, *yptr, *rptr, *fptr, *wptr, *cptr, *aptr;
        MMFLOAT *xfptr, *yfptr, *rfptr, *ffptr, *wfptr, *cfptr, *afptr;
        getargs(&cmdline, 13,",");
        if(!(argc & 1) || argc < 5) error("Argument count");
        getargaddress(argv[0], &xptr, &xfptr, &n);
        if(n != 1) {
            getargaddress(argv[2], &yptr, &yfptr, &n);
            getargaddress(argv[4], &rptr, &rfptr, &n);
        }
        if(n==1){
            w = 1; c = gui_fcolour; f = -1; a = 1;                          // setup the defaults
            x = getinteger(argv[0]);
            y = getinteger(argv[2]);
            r = getinteger(argv[4]);
            if(argc > 5 && *argv[6]) w = getint(argv[6], 0, 100);
            if(argc > 7 && *argv[8]) a = getnumber(argv[8]);
            if(argc > 9 && *argv[10]) c = getint(argv[10], 0, WHITE);
            if(argc > 11) f = getint(argv[12], -1, WHITE);
            int save_refresh=Option.Refresh;
            Option.Refresh=0;
            DrawCircle(x, y, r, w, c, f, a);
            Option.Refresh=save_refresh;
        } else {
            w = 1; c = gui_fcolour; f = -1; a = 1;                          // setup the defaults
            if(argc > 5 && *argv[6]) {
                getargaddress(argv[6], &wptr, &wfptr, &nw); 
                if(nw == 1) w = getint(argv[6], 0, 100);
                else if(nw>1) {
                    if(nw > 1 && nw < n) n=nw; //adjust the dimensionality
                    for(i=0;i<nw;i++){
                        w = (wfptr == NULL ? wptr[i] : (int)wfptr[i]);
                        if(w < 0 || w > 100) error("% is invalid (valid is % to %)", (int)w, 0, 100);
                    }
                }
            }
            if(argc > 7 && *argv[8]){
                getargaddress(argv[8], &aptr, &afptr, &na); 
                if(na == 1) a = getnumber(argv[8]);
                if(na > 1 && na < n) n=na; //adjust the dimensionality
            }
            if(argc > 9 && *argv[10]){
                getargaddress(argv[10], &cptr, &cfptr, &nc); 
                if(nc == 1) c = getint(argv[10], 0, WHITE);
                else if(nc>1) {
                    if(nc > 1 && nc < n) n=nc; //adjust the dimensionality
                    for(i=0;i<nc;i++){
                        c = (cfptr == NULL ? cptr[i] : (int)cfptr[i]);
                        if(c < 0 || c > WHITE) error("% is invalid (valid is % to %)", (int)c, 0, WHITE);
                    }
                }
            }
            if(argc > 11){
                getargaddress(argv[12], &fptr, &ffptr, &nf); 
                if(nf == 1) f = getint(argv[12], -1, WHITE);
                else if(nf>1) {
                    if(nf > 1 && nf < n) n=nf; //adjust the dimensionality
                    for(i=0;i<nf;i++){
                        f = (ffptr == NULL ? fptr[i] : (int)ffptr[i]);
                        if(f < 0 || f > WHITE) error("% is invalid (valid is % to %)", (int)f, 0, WHITE);
                    }
                }
            }
            int save_refresh=Option.Refresh;
            Option.Refresh=0;
            for(i=0;i<n;i++){
                x = (xfptr==NULL ? xptr[i] : (int)xfptr[i]);
                y = (yfptr==NULL ? yptr[i] : (int)yfptr[i]);
                r = (rfptr==NULL ? rptr[i] : (int)rfptr[i])-1;
                if(nw > 1) w = (wfptr==NULL ? wptr[i] : (int)wfptr[i]);
                if(nc > 1) c = (cfptr==NULL ? cptr[i] : (int)cfptr[i]);
                if(nf > 1) f = (ffptr==NULL ? fptr[i] : (int)ffptr[i]);
                if(na > 1) a = (afptr==NULL ? (MMFLOAT)aptr[i] : afptr[i]);
                DrawCircle(x, y, r, w, c, f, a);
            }
            Option.Refresh=save_refresh;
        }
    }
    if(Option.Refresh)Display_Refresh();
}


void cmd_line(void) {
    if(Option.DISPLAY_TYPE == 0) error("Display not configured");
	if(CMM1){
		int x1, y1, x2, y2, colour, box, fill;
		char *p;
		getargs(&cmdline, 5, ",");

		// check if it is actually a LINE INPUT command
		if(argc < 1) error("Invalid syntax");
		x1 = lastx; y1 = lasty; colour = gui_fcolour; box = false; fill = false;	// set the defaults for optional components
		p = argv[0];
		if(tokenfunction(*p) != op_subtract) {
			// the start point is specified - get the coordinates and step over to where the minus token should be
			if(*p != '(') error("Expected opening bracket");
			getcoord(p , &x1, &y1);
			p = getclosebracket(p) + 1;
			skipspace(p);
		}
		if(tokenfunction(*p) != op_subtract) error("Invalid syntax");
		p++;
		skipspace(p);
		if(*p != '(') error("Expected opening bracket");
		getcoord(p , &x2, &y2);
		if(argc > 1 && *argv[2]){
			colour = getColour(argv[2],0);
		}
		if(argc == 5) {
			box = (strchr(argv[4], 'b') != NULL || strchr(argv[4], 'B') != NULL);
			fill = (strchr(argv[4], 'f') != NULL || strchr(argv[4], 'F') != NULL);
		}
		if(box)
			DrawBox(x1, y1, x2, y2, 1, colour, (fill ? colour : -1));						// draw a box
		else
			DrawLine(x1, y1, x2, y2, 1, colour);								// or just a line

		lastx = x2; lasty = y2;											// save in case the user wants the last value
	} else {
        int x1, y1, x2, y2, w=0, c=0, n=0 ,i, nc=0, nw=0;
        long long int *x1ptr, *y1ptr, *x2ptr, *y2ptr, *wptr, *cptr;
        MMFLOAT *x1fptr, *y1fptr, *x2fptr, *y2fptr, *wfptr, *cfptr;
        getargs(&cmdline, 11,",");
        if(!(argc & 1) || argc < 7) error("Argument count");
        getargaddress(argv[0], &x1ptr, &x1fptr, &n);
        if(n != 1) {
            getargaddress(argv[2], &y1ptr, &y1fptr, &n);
            getargaddress(argv[4], &x2ptr, &x2fptr, &n);
            getargaddress(argv[6], &y2ptr, &y2fptr, &n);
        }
        if(n==1){
            c = gui_fcolour;  w = 1;                                        // setup the defaults
            x1 = getinteger(argv[0]);
            y1 = getinteger(argv[2]);
            x2 = getinteger(argv[4]);
            y2 = getinteger(argv[6]);
            if(argc > 7 && *argv[8]){
                w = getint(argv[8], 1, 100);
            }
            if(argc == 11) c = getint(argv[10], 0, WHITE);
            DrawLine(x1, y1, x2, y2, w, c);        
        } else {
            c = gui_fcolour;  w = 1;                                        // setup the defaults
            if(argc > 7 && *argv[8]){
                getargaddress(argv[8], &wptr, &wfptr, &nw); 
                if(nw == 1) w = getint(argv[8], 0, 100);
                else if(nw>1) {
                    if(nw > 1 && nw < n) n=nw; //adjust the dimensionality
                    for(i=0;i<nw;i++){
                        w = (wfptr == NULL ? wptr[i] : (int)wfptr[i]);
                        if(w < 0 || w > 100) error("% is invalid (valid is % to %)", (int)w, 0, 100);
                    }
                }
            }
            if(argc == 11){
                getargaddress(argv[10], &cptr, &cfptr, &nc); 
                if(nc == 1) c = getint(argv[10], 0, WHITE);
                else if(nc>1) {
                    if(nc > 1 && nc < n) n=nc; //adjust the dimensionality
                    for(i=0;i<nc;i++){
                        c = (cfptr == NULL ? cptr[i] : (int)cfptr[i]);
                        if(c < 0 || c > WHITE) error("% is invalid (valid is % to %)", (int)c, 0, WHITE);
                    }
                }
            }
            for(i=0;i<n;i++){
                x1 = (x1fptr==NULL ? x1ptr[i] : (int)x1fptr[i]);
                y1 = (y1fptr==NULL ? y1ptr[i] : (int)y1fptr[i]);
                x2 = (x2fptr==NULL ? x2ptr[i] : (int)x2fptr[i]);
                y2 = (y2fptr==NULL ? y2ptr[i] : (int)y2fptr[i]);
                if(nw > 1) w = (wfptr==NULL ? wptr[i] : (int)wfptr[i]);
                if(nc > 1) c = (cfptr==NULL ? cptr[i] : (int)cfptr[i]);
                DrawLine(x1, y1, x2, y2, w, c);
            }
        }
    }
    if(Option.Refresh)Display_Refresh();
}


void cmd_box(void) {
    int x1, y1, wi, h, w=0, c=0, f=0,  n=0 ,i, nc=0, nw=0, nf=0,hmod,wmod;
    long long int *x1ptr, *y1ptr, *wiptr, *hptr, *wptr, *cptr, *fptr;
    MMFLOAT *x1fptr, *y1fptr, *wifptr, *hfptr, *wfptr, *cfptr, *ffptr;
    getargs(&cmdline, 13,",");
    if(Option.DISPLAY_TYPE == 0) error("Display not configured");
    if(!(argc & 1) || argc < 7) error("Argument count");
    getargaddress(argv[0], &x1ptr, &x1fptr, &n);
    if(n != 1) {
        getargaddress(argv[2], &y1ptr, &y1fptr, &n);
        getargaddress(argv[4], &wiptr, &wifptr, &n);
        getargaddress(argv[6], &hptr, &hfptr, &n);
    }
    if(n == 1){
        c = gui_fcolour; w = 1; f = -1;                                 // setup the defaults
        x1 = getinteger(argv[0]);
        y1 = getinteger(argv[2]);
        wi = getinteger(argv[4]) ;
        h = getinteger(argv[6]) ;
        wmod=(wi > 0 ? -1 : 1);
        hmod=(h > 0 ? -1 : 1);
        if(argc > 7 && *argv[8]) w = getint(argv[8], 0, 100);
        if(argc > 9 && *argv[10]) c = getint(argv[10], 0, WHITE);
        if(argc == 13) f = getint(argv[12], -1, WHITE);
        if(wi != 0 && h != 0) DrawBox(x1, y1, x1 + wi + wmod, y1 + h + hmod, w, c, f);
    } else {
        c = gui_fcolour;  w = 1;                                        // setup the defaults
        if(argc > 7 && *argv[8]){
            getargaddress(argv[8], &wptr, &wfptr, &nw); 
            if(nw == 1) w = getint(argv[8], 0, 100);
            else if(nw>1) {
                if(nw > 1 && nw < n) n=nw; //adjust the dimensionality
                for(i=0;i<nw;i++){
                    w = (wfptr == NULL ? wptr[i] : (int)wfptr[i]);
                    if(w < 0 || w > 100) error("% is invalid (valid is % to %)", (int)w, 0, 100);
                }
            }
        }
        if(argc > 9 && *argv[10]) {
            getargaddress(argv[10], &cptr, &cfptr, &nc); 
            if(nc == 1) c = getint(argv[10], 0, WHITE);
            else if(nc>1) {
                if(nc > 1 && nc < n) n=nc; //adjust the dimensionality
                for(i=0;i<nc;i++){
                    c = (cfptr == NULL ? cptr[i] : (int)cfptr[i]);
                    if(c < 0 || c > WHITE) error("% is invalid (valid is % to %)", (int)c, 0, WHITE);
                }
            }
        }
        if(argc == 13){
            getargaddress(argv[12], &fptr, &ffptr, &nf); 
            if(nf == 1) f = getint(argv[12], 0, WHITE);
            else if(nf>1) {
                if(nf > 1 && nf < n) n=nf; //adjust the dimensionality
                for(i=0;i<nf;i++){
                    f = (ffptr == NULL ? fptr[i] : (int)ffptr[i]);
                    if(f < -1 || f > WHITE) error("% is invalid (valid is % to %)", (int)f, -1, WHITE);
                }
            }
        }
        for(i=0;i<n;i++){
            x1 = (x1fptr==NULL ? x1ptr[i] : (int)x1fptr[i]);
            y1 = (y1fptr==NULL ? y1ptr[i] : (int)y1fptr[i]);
            wi = (wifptr==NULL ? wiptr[i] : (int)wifptr[i]);
            h =  (hfptr==NULL ? hptr[i] : (int)hfptr[i]);
            wmod=(wi > 0 ? -1 : 1);
            hmod=(h > 0 ? -1 : 1);
            if(nw > 1) w = (wfptr==NULL ? wptr[i] : (int)wfptr[i]);
            if(nc > 1) c = (cfptr==NULL ? cptr[i] : (int)cfptr[i]);
            if(nf > 1) f = (ffptr==NULL ? fptr[i] : (int)ffptr[i]);
            if(wi != 0 && h != 0) DrawBox(x1, y1, x1 + wi + wmod, y1 + h + hmod, w, c, f);

        }
    }
    if(Option.Refresh)Display_Refresh();
}


void bezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, int c){
    float tmp,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7,tmp8,t=0.0,xt=x0,yt=y0;
    int i, xti,yti,xtlast=-1, ytlast=-1;
    for (i=0; i<500; i++)
    {
    	tmp = 1.0 - t;
    	tmp3 = t * t;
    	tmp4 = tmp * tmp;
    	tmp1 = tmp3 * t;
    	tmp2 = tmp4 * tmp;
    	tmp5 = 3.0 * t;
    	tmp6 = 3.0 *  tmp3;
    	tmp7 = tmp5 * tmp4;
    	tmp8 = tmp6 * tmp;
    	xti=(int)xt;
    	yti=(int)yt;
    	xt =    ((((tmp2 * x0) + (tmp7 * x1)) + (tmp8 * x2)) + (tmp1 * x3));
    	yt =    ((((tmp2 * y0) + (tmp7 * y1)) +	(tmp8 * y2)) +(tmp1 * y3));
    	if((xti!=xtlast) || (yti!=ytlast)) {
    		DrawBuffered(xti, yti, c, 0);
    		xtlast=xti;
    		ytlast=yti;
    	}
    	t+=0.002;
    }
	DrawBuffered(0, 0, 0, 1);
}


void pointcalc(int angle, int x, int y, int r2, int *x0, int * y0){
	float c1,s1;
	int quad;
	angle %=360;
	switch(angle){
	case 0:
		*x0=x;
		*y0=y-r2;
		break;
	case 45:
		*x0=x+r2+1;
		*y0=y-r2;
		break;
	case 90:
		*x0=x+r2+1;
		*y0=y;
		break;
	case 135:
		*x0=x+r2+1;
		*y0=y+r2;
		break;
	case 180:
		*x0=x;
		*y0=y+r2;
		break;
	case 225:
		*x0=x-r2;
		*y0=y+r2;
		break;
	case 270:
		*x0=x-r2;
		*y0=y;
		break;
	case 315:
		*x0=x-r2;
		*y0=y-r2;
		break;
	default:
		c1=cosf(Rad(angle));
		s1=sinf(Rad(angle));
		quad = (angle / 45) % 8;
		switch(quad){
		case 0:
			*y0=y-r2;
			*x0=x+s1*r2/c1;
			break;
		case 1:
 		  *x0=x+r2+1;
 		  *y0=y-c1*r2/s1;
 		  break;
		case 2:
 		  *x0=x+r2+1;
 		  *y0=y-c1*r2/s1;
 		  break;
		case 3:
 		  *y0=y+r2;
 		  *x0=x-s1*r2/c1;
 		  break;
		case 4:
 		  *y0=y+r2;
 		  *x0=x-s1*r2/c1;
 		  break;
		case 5:
 		  *x0=x-r2;
 		  *y0=y+c1*r2/s1;
 		  break;
		case 6:
			*x0=x-r2;
			*y0=y+c1*r2/s1;
			break;
		case 7:
			*y0=y-r2;
			*x0=x+s1*r2/c1;
			break;
		}
	}
}
void cmd_arc(void){
	// Parameters are:
	// X coordinate of centre of arc
	// Y coordinate of centre of arc
	// inner radius of arc
	// outer radius of arc - omit it 1 pixel wide
	// start radial of arc in degrees
	// end radial of arc in degrees
	// Colour of arc
	int x, y, r1, r2, c ,i ,j, k, xs=-1, xi=0, m;
	int rad1, rad2, rad3, rstart, quadr;
	int x0, y0, x1, y1, x2, y2, xr, yr;
	getargs(&cmdline, 13,",");
    if(!(argc == 11 || argc == 13)) error("Argument count");
    if(Option.DISPLAY_TYPE == 0) error("Display not configured");
    x = getinteger(argv[0]);
    y = getinteger(argv[2]);
    r1 = getinteger(argv[4]);
    if(*argv[6])r2 = getinteger(argv[6]);
    else {
    	r2=r1;
    	r1--;
    }
    if(r2 < r1)error("Inner radius < outer");
    rad1 = getnumber(argv[8]);
    rad2 = getnumber(argv[10]);
    while(rad1<0.0)rad1+=360.0;
    while(rad2<0.0)rad2+=360.0;
	if(rad1==rad2)error("Radials");
     if(argc == 13)
        c = getint(argv[12], 0, WHITE);
    else
        c = gui_fcolour;
    while(rad2<rad1)rad2+=360;
    rad3=rad1+360;
    rstart=rad2;
    int quad1 = (rad1 / 45) % 8;
    x2=x;y2=y;
    int ints_per_line=RoundUptoInt((r2*2)+1)/32;
    uint32_t *br=(uint32_t *)GetTempMemory(((ints_per_line+1)*((r2*2)+1))*4);
    DrawFilledCircle(x, y, r2, r2, 1, ints_per_line, br, 1.0, 1.0);
    DrawFilledCircle(x, y, r1, r2, 0, ints_per_line, br, 1.0, 1.0);
    while(rstart<rad3){
		pointcalc(rstart, x, y, r2, &x0, &y0);
   		quadr = (rstart / 45) % 8;
    	if(quadr==quad1 && rad3-rstart<45){
    		pointcalc(rad3, x, y, r2, &x1, &y1);
    		ClearTriangle(x0-x+r2, y0-y+r2, x1-x+r2, y1-y+r2, x2-x+r2, y2-y+r2, ints_per_line, br);
    		rstart=rad3;
    	} else {
    			rstart +=45;
    			rstart -= (rstart % 45);
    			pointcalc(rstart, x, y, r2, &xr, &yr);
    			ClearTriangle(x0-x+r2, y0-y+r2, xr-x+r2, yr-y+r2, x2-x+r2, y2-y+r2, ints_per_line, br);
    	}
    }
    int save_refresh=Option.Refresh;
    Option.Refresh=0;
 	for(j=0;j<r2*2+1;j++){
 		for(i=0;i<ints_per_line;i++){
 			k=br[i+j*ints_per_line];
 			for(m=0;m<32;m++){
 				if(xs==-1 && (k & 0x80000000)){
 					xs=m;
 					xi=i;
 				}
 				if(xs!=-1 && !(k & 0x80000000)){
					DrawRectangle(x-r2+xs+xi*32, y-r2+j, x-r2+m+i*32, y-r2+j, c);
 					xs=-1;
 				}
 				k<<=1;
 			}
 		}
		if(xs!=-1){
			DrawRectangle(x-r2+xs+xi*32, y-r2+j, x-r2+m+i*32, y-r2+j, c);
			xs=-1;
		}
	}
	Option.Refresh=save_refresh;
    if(Option.Refresh)Display_Refresh();
}
typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} rgb_t;
#define TFLOAT float
typedef struct {
    TFLOAT  xpos;       // current position and heading
    TFLOAT  ypos;       // (uses floating-point numbers for
    TFLOAT  heading;    //  increased accuracy)

    rgb_t  pen_color;   // current pen color
    rgb_t  fill_color;  // current fill color
    bool   pendown;     // currently drawing?
    bool   filled;      // currently filling?
} fill_t;
fill_t main_fill;
fill_t backup_fill;
int    main_fill_poly_vertex_count = 0;       // polygon vertex count
TFLOAT *main_fill_polyX=NULL; // polygon vertex x-coords
TFLOAT *main_fill_polyY=NULL; // polygon vertex y-coords

void fill_set_pen_color(int red, int green, int blue)
{
    main_fill.pen_color.red = red;
    main_fill.pen_color.green = green;
    main_fill.pen_color.blue = blue;
}

void fill_set_fill_color(int red, int green, int blue)
{
    main_fill.fill_color.red = red;
    main_fill.fill_color.green = green;
    main_fill.fill_color.blue = blue;
}
static void fill_begin_fill()
{
    main_fill.filled = true;
    main_fill_poly_vertex_count = 0;
}

static void fill_end_fill(int count, int ystart, int yend)
{
    // based on public-domain fill algorithm in C by Darel Rex Finley, 2007
    //   from http://alienryderflex.com/polygon_fill/

    TFLOAT *nodeX=GetMemory(count * sizeof(TFLOAT));     // x-coords of polygon intercepts
    int nodes;                              // size of nodeX
    int y, i, j;                         // current pixel and loop indices
    TFLOAT temp;                            // temporary variable for sorting
    int f=(main_fill.fill_color.red<<16) | (main_fill.fill_color.green<<8) | main_fill.fill_color.blue;
    int c= (main_fill.pen_color.red<<16) | (main_fill.pen_color.green<<8) | main_fill.pen_color.blue;
    int xstart, xend;
    //  loop through the rows of the image

    for (y = ystart; y < yend; y++) {

        //  build a list of polygon intercepts on the current line
        nodes = 0;
        j = main_fill_poly_vertex_count-1;
        for (i = 0; i < main_fill_poly_vertex_count; i++) {
            if ((main_fill_polyY[i] <  (TFLOAT)y &&
                 main_fill_polyY[j] >= (TFLOAT)y) ||
                (main_fill_polyY[j] <  (TFLOAT)y &&
                 main_fill_polyY[i] >= (TFLOAT)y)) {

                // intercept found; record it
                nodeX[nodes++] = (main_fill_polyX[i] +
                        ((TFLOAT)y - main_fill_polyY[i]) /
                        (main_fill_polyY[j] - main_fill_polyY[i]) *
                        (main_fill_polyX[j] - main_fill_polyX[i]));
            }
            j = i;
        }

        //  sort the nodes via simple insertion sort
        for (i = 1; i < nodes; i++) {
            temp = nodeX[i];
            for (j = i; j > 0 && temp < nodeX[j-1]; j--) {
                nodeX[j] = nodeX[j-1];
            }
            nodeX[j] = temp;
        }

        //  fill the pixels between node pairs
        for (i = 0; i < nodes; i += 2) {
        	xstart=(int)floorf(nodeX[i])+1;
        	xend=(int)ceilf(nodeX[i+1])-1;
        	DrawLine(xstart,y,xend,y,1, f);
        }
    }

    main_fill.filled = false;

    // redraw polygon (filling is imperfect and can occasionally occlude sides)
    for (i = 0; i < main_fill_poly_vertex_count; i++) {
        int x0 = (int)roundf(main_fill_polyX[i]);
        int y0 = (int)roundf(main_fill_polyY[i]);
        int x1 = (int)roundf(main_fill_polyX[(i+1) %
            main_fill_poly_vertex_count]);
        int y1 = (int)roundf(main_fill_polyY[(i+1) %
            main_fill_poly_vertex_count]);
        DrawLine(x0, y0, x1, y1, 1, c);
    }
    FreeMemory((void *)nodeX);
}


void cmd_polygon(void){
	int xcount=0;
	long long int *xptr=NULL, *yptr=NULL,xptr2=0, yptr2=0, *polycount=NULL, *cptr=NULL, *fptr=NULL;
	MMFLOAT *polycountf=NULL, *cfptr=NULL, *ffptr=NULL, *xfptr=NULL, *yfptr=NULL, xfptr2=0, yfptr2=0;
	int i, f=0, c, xtot=0, ymax=0, ymin=1000000;
    int n=0, nx=0, ny=0, nc=0, nf=0;
    getargs(&cmdline, 9,",");
    if(Option.DISPLAY_TYPE == 0) error("Display not configured");
    getargaddress(argv[0], &polycount, &polycountf, &n);
    if(n==1){
    	xcount = xtot = getinteger(argv[0]);
    	if(xcount<3 || xcount>9999)error("Invalid number of vertices");
        getargaddress(argv[2], &xptr, &xfptr, &nx);
        if(nx<xtot)error("X Dimensions %", nx);
        getargaddress(argv[4], &yptr, &yfptr, &ny);
        if(ny<xtot)error("Y Dimensions %", ny);
        if(xptr)xptr2=*xptr;
        else xfptr2=*xfptr;
        if(yptr)yptr2=*yptr;
        else yfptr2=*yfptr;
        c = gui_fcolour;                                    // setup the defaults
        if(argc > 5 && *argv[6]) c = getint(argv[6], 0, WHITE);
        if(argc > 7 && *argv[8]){
        	main_fill_polyX=(TFLOAT  *)GetTempMemory(xtot * sizeof(TFLOAT));
        	main_fill_polyY=(TFLOAT  *)GetTempMemory(xtot * sizeof(TFLOAT));
        	f = getint(argv[8], 0, WHITE);
    		fill_set_fill_color((f>>16) & 0xFF, (f>>8) & 0xFF , f & 0xFF);
        	fill_set_pen_color((c>>16) & 0xFF, (c>>8) & 0xFF , c & 0xFF);
        	fill_begin_fill();
        }
       for(i=0;i<xcount-1;i++){
          	if(argc > 7){
                  main_fill_polyX[main_fill_poly_vertex_count] = (xfptr==NULL ? (TFLOAT )*xptr++ : (TFLOAT )*xfptr++) ;
                  main_fill_polyY[main_fill_poly_vertex_count] = (yfptr==NULL ? (TFLOAT )*yptr++ : (TFLOAT )*yfptr++) ;
                  if(main_fill_polyY[main_fill_poly_vertex_count]>ymax)ymax=main_fill_polyY[main_fill_poly_vertex_count];
                  if(main_fill_polyY[main_fill_poly_vertex_count]<ymin)ymin=main_fill_polyY[main_fill_poly_vertex_count];
                  main_fill_poly_vertex_count++;
          	} else {
          		int x1=(xfptr==NULL ? *xptr++ : (int)*xfptr++);
          		int x2=(xfptr==NULL ? *xptr : (int)*xfptr);
          		int y1=(yfptr==NULL ? *yptr++ : (int)*yfptr++);
          		int y2=(yfptr==NULL ? *yptr : (int)*yfptr);
           		DrawLine(x1,y1,x2,y2, 1, c);
           	}
        }
        if(argc > 7){
            main_fill_polyX[main_fill_poly_vertex_count] = (xfptr==NULL ? (TFLOAT )*xptr++ : (TFLOAT )*xfptr++) ;
            main_fill_polyY[main_fill_poly_vertex_count] = (yfptr==NULL ? (TFLOAT )*yptr++ : (TFLOAT )*yfptr++) ;
            if(main_fill_polyY[main_fill_poly_vertex_count]>ymax)ymax=main_fill_polyY[main_fill_poly_vertex_count];
            if(main_fill_polyY[main_fill_poly_vertex_count]<ymin)ymin=main_fill_polyY[main_fill_poly_vertex_count];
            if(main_fill_polyY[main_fill_poly_vertex_count]!=main_fill_polyY[0] || main_fill_polyX[main_fill_poly_vertex_count] != main_fill_polyX[0]){
                	main_fill_poly_vertex_count++;
                	main_fill_polyX[main_fill_poly_vertex_count]=main_fill_polyX[0];
                	main_fill_polyY[main_fill_poly_vertex_count]=main_fill_polyY[0];
            }
            main_fill_poly_vertex_count++;
        	if(main_fill_poly_vertex_count>5){
        		fill_end_fill(xcount,ymin,ymax);
        	} else if(main_fill_poly_vertex_count==5){
				DrawTriangle(main_fill_polyX[0],main_fill_polyY[0],main_fill_polyX[1],main_fill_polyY[1],main_fill_polyX[2],main_fill_polyY[2],f,f);
				DrawTriangle(main_fill_polyX[0],main_fill_polyY[0],main_fill_polyX[2],main_fill_polyY[2],main_fill_polyX[3],main_fill_polyY[3],f,f);
				if(f!=c){
					DrawLine(main_fill_polyX[0],main_fill_polyY[0],main_fill_polyX[1],main_fill_polyY[1],1,c);
					DrawLine(main_fill_polyX[1],main_fill_polyY[1],main_fill_polyX[2],main_fill_polyY[2],1,c);
					DrawLine(main_fill_polyX[2],main_fill_polyY[2],main_fill_polyX[3],main_fill_polyY[3],1,c);
					DrawLine(main_fill_polyX[3],main_fill_polyY[3],main_fill_polyX[4],main_fill_polyY[4],1,c);
				}
			} else {
				DrawTriangle(main_fill_polyX[0],main_fill_polyY[0],main_fill_polyX[1],main_fill_polyY[1],main_fill_polyX[2],main_fill_polyY[2],c,f);
        	}
       } else {
    		int x1=(xfptr==NULL ? *xptr : (int)*xfptr);
    		int x2=(xfptr==NULL ? xptr2 : (int)xfptr2);
    		int y1=(yfptr==NULL ? *yptr : (int)*yfptr);
    		int y2=(yfptr==NULL ? yptr2 : (int)yfptr2);
    		DrawLine(x1,y1,x2,y2, 1, c);
        }
    } else {
    	int *cc=GetTempMemory(n*sizeof(int)); //array for foreground colours
    	int *ff=GetTempMemory(n*sizeof(int)); //array for background colours
    	int xstart ,j, xmax=0;
    	for(i=0;i<n;i++){
    		if((polycountf == NULL ? polycount[i] : (int)polycountf[i])>xmax)xmax=(polycountf == NULL ? polycount[i] : (int)polycountf[i]);
    		if(!(polycountf == NULL ? polycount[i] : (int)polycountf[i]))break;
    		xtot+=(polycountf == NULL ? polycount[i] : (int)polycountf[i]);
    		if((polycountf == NULL ? polycount[i] : (int)polycountf[i])<3 || (polycountf == NULL ? polycount[i] : (int)polycountf[i])>9999)error("Invalid number of vertices, polygon %",i);
    	}
    	n=i;
        getargaddress(argv[2], &xptr, &xfptr, &nx);
        if(nx<xtot)error("X Dimensions %", nx);
        getargaddress(argv[4], &yptr, &yfptr, &ny);
        if(ny<xtot)error("Y Dimensions %", ny);
    	main_fill_polyX=(TFLOAT  *)GetTempMemory(xmax * sizeof(TFLOAT));
    	main_fill_polyY=(TFLOAT  *)GetTempMemory(xmax * sizeof(TFLOAT));
		if(argc > 5 && *argv[6]){ //foreground colour specified
			getargaddress(argv[6], &cptr, &cfptr, &nc);
			if(nc == 1) for(i=0;i<n;i++)cc[i] = getint(argv[6], 0, WHITE);
			else {
				if(nc < n) error("Foreground colour Dimensions");
				for(i=0;i<n;i++){
					cc[i] = (cfptr == NULL ? cptr[i] : (int)cfptr[i]);
					if(cc[i] < 0 || cc[i] > 0xFFFFFF) error("% is invalid (valid is % to %)", (int)cc[i], 0, 0xFFFFFF);
				}
			}
		} else for(i=0;i<n;i++)cc[i] = gui_fcolour;
		if(argc > 7){ //background colour specified
			getargaddress(argv[8], &fptr, &ffptr, &nf);
			if(nf == 1) for(i=0;i<n;i++) ff[i] = getint(argv[8], 0, WHITE);
			else {
				if(nf < n) error("Background colour Dimensions");
				for(i=0;i<n;i++){
					ff[i] = (ffptr == NULL ? fptr[i] : (int)ffptr[i]);
					if(ff[i] < 0 || ff[i] > 0xFFFFFF) error("% is invalid (valid is % to %)", (int)ff[i], 0, 0xFFFFFF);
				}
			}
		}
    	xstart=0;
    	for(i=0;i<n;i++){
            if(xptr)xptr2=*xptr;
            else xfptr2=*xfptr;
            if(yptr)yptr2=*yptr;
            else yfptr2=*yfptr;
    		ymax=0;
    		ymin=1000000;
    		main_fill_poly_vertex_count=0;
        	xcount = (int)(polycountf == NULL ? polycount[i] : (int)polycountf[i]);
            if(argc > 7 && *argv[8]){
            	fill_set_pen_color((cc[i]>>16) & 0xFF, (cc[i]>>8) & 0xFF , cc[i] & 0xFF);
        		fill_set_fill_color((ff[i]>>16) & 0xFF, (ff[i]>>8) & 0xFF , ff[i] & 0xFF);
            	fill_begin_fill();
            }
           for(j=xstart;j<xstart+xcount-1;j++){
            	if(argc > 7){
                    main_fill_polyX[main_fill_poly_vertex_count] = (xfptr==NULL ? (TFLOAT )*xptr++ : (TFLOAT )*xfptr++) ;
                    main_fill_polyY[main_fill_poly_vertex_count] = (yfptr==NULL ? (TFLOAT )*yptr++ : (TFLOAT )*yfptr++) ;
                    if(main_fill_polyY[main_fill_poly_vertex_count]>ymax)ymax=main_fill_polyY[main_fill_poly_vertex_count];
                    if(main_fill_polyY[main_fill_poly_vertex_count]<ymin)ymin=main_fill_polyY[main_fill_poly_vertex_count];
                    main_fill_poly_vertex_count++;
            	} else {
            		int x1=(xfptr==NULL ? *xptr++ : (int)*xfptr++);
            		int x2=(xfptr==NULL ? *xptr : (int)*xfptr);
            		int y1=(yfptr==NULL ? *yptr++ : (int)*yfptr++);
            		int y2=(yfptr==NULL ? *yptr : (int)*yfptr);
            		DrawLine(x1,y1,x2,y2, 1, cc[i]);
            	}
            }
            if(argc > 7){
                main_fill_polyX[main_fill_poly_vertex_count] = (xfptr==NULL ? (TFLOAT )*xptr++ : (TFLOAT )*xfptr++) ;
                main_fill_polyY[main_fill_poly_vertex_count] = (yfptr==NULL ? (TFLOAT )*yptr++ : (TFLOAT )*yfptr++) ;
                if(main_fill_polyY[main_fill_poly_vertex_count]>ymax)ymax=main_fill_polyY[main_fill_poly_vertex_count];
                if(main_fill_polyY[main_fill_poly_vertex_count]<ymin)ymin=main_fill_polyY[main_fill_poly_vertex_count];
                if(main_fill_polyY[main_fill_poly_vertex_count]!=main_fill_polyY[0] || main_fill_polyX[main_fill_poly_vertex_count] != main_fill_polyX[0]){
                    	main_fill_poly_vertex_count++;
                    	main_fill_polyX[main_fill_poly_vertex_count]=main_fill_polyX[0];
                    	main_fill_polyY[main_fill_poly_vertex_count]=main_fill_polyY[0];
                }
                main_fill_poly_vertex_count++;
            	if(main_fill_poly_vertex_count>5){
            		fill_end_fill(xcount,ymin,ymax);
            	} else if(main_fill_poly_vertex_count==5){
    				DrawTriangle(main_fill_polyX[0],main_fill_polyY[0],main_fill_polyX[1],main_fill_polyY[1],main_fill_polyX[2],main_fill_polyY[2],ff[i],ff[i]);
    				DrawTriangle(main_fill_polyX[0],main_fill_polyY[0],main_fill_polyX[2],main_fill_polyY[2],main_fill_polyX[3],main_fill_polyY[3],ff[i],ff[i]);
    				if(ff[i]!=cc[i]){
    					DrawLine(main_fill_polyX[0],main_fill_polyY[0],main_fill_polyX[1],main_fill_polyY[1],1,cc[i]);
    					DrawLine(main_fill_polyX[1],main_fill_polyY[1],main_fill_polyX[2],main_fill_polyY[2],1,cc[i]);
    					DrawLine(main_fill_polyX[2],main_fill_polyY[2],main_fill_polyX[3],main_fill_polyY[3],1,cc[i]);
    					DrawLine(main_fill_polyX[3],main_fill_polyY[3],main_fill_polyX[4],main_fill_polyY[4],1,cc[i]);
    				}
    			} else {
    				DrawTriangle(main_fill_polyX[0],main_fill_polyY[0],main_fill_polyX[1],main_fill_polyY[1],main_fill_polyX[2],main_fill_polyY[2],cc[i],ff[i]);
            	}
            } else {
        		int x1=(xfptr==NULL ? *xptr : (int)*xfptr);
        		int x2=(xfptr==NULL ? xptr2 : (int)xfptr2);
        		int y1=(yfptr==NULL ? *yptr : (int)*yfptr);
        		int y2=(yfptr==NULL ? yptr2 : (int)yfptr2);
        		DrawLine(x1,y1,x2,y2, 1, cc[i]);
            	if(xfptr!=NULL)xfptr++;
            	else xptr++;
            	if(yfptr!=NULL)yfptr++;
            	else yptr++;
            }

            xstart+=xcount;
    	}
    }
}


void cmd_rbox(void) {
    int x1, y1, wi, h, w=0, c=0, f=0,  r=0, n=0 ,i, nc=0, nw=0, nf=0,hmod,wmod;
    long long int *x1ptr, *y1ptr, *wiptr, *hptr, *wptr, *cptr, *fptr;
    MMFLOAT *x1fptr, *y1fptr, *wifptr, *hfptr, *wfptr, *cfptr, *ffptr;
    getargs(&cmdline, 13,",");
    if(Option.DISPLAY_TYPE == 0) error("Display not configured");
    if(!(argc & 1) || argc < 7) error("Argument count");
    getargaddress(argv[0], &x1ptr, &x1fptr, &n);
    if(n != 1) {
        getargaddress(argv[2], &y1ptr, &y1fptr, &n);
        getargaddress(argv[4], &wiptr, &wifptr, &n);
        getargaddress(argv[6], &hptr, &hfptr, &n);
    }
    if(n == 1){
        c = gui_fcolour; w = 1; f = -1; r = 10;                         // setup the defaults
        x1 = getinteger(argv[0]);
        y1 = getinteger(argv[2]);
        w = getinteger(argv[4]);
        h = getinteger(argv[6]);
        wmod=(w > 0 ? -1 : 1);
        hmod=(h > 0 ? -1 : 1);
        if(argc > 7 && *argv[8]) r = getint(argv[8], 0, 100);
        if(argc > 9 && *argv[10]) c = getint(argv[10], 0, WHITE);
        if(argc == 13) f = getint(argv[12], -1, WHITE);
        if(w != 0 && h != 0) DrawRBox(x1, y1, x1 + w + wmod, y1 + h + hmod, r, c, f);
    } else {
        c = gui_fcolour;  w = 1;                                        // setup the defaults
        if(argc > 7 && *argv[8]){
            getargaddress(argv[8], &wptr, &wfptr, &nw); 
            if(nw == 1) w = getint(argv[8], 0, 100);
            else if(nw>1) {
                if(nw > 1 && nw < n) n=nw; //adjust the dimensionality
                for(i=0;i<nw;i++){
                    w = (wfptr == NULL ? wptr[i] : (int)wfptr[i]);
                    if(w < 0 || w > 100) error("% is invalid (valid is % to %)", (int)w, 0, 100);
                }
            }
        }
        if(argc > 9 && *argv[10]) {
            getargaddress(argv[10], &cptr, &cfptr, &nc); 
            if(nc == 1) c = getint(argv[10], 0, WHITE);
            else if(nc>1) {
                if(nc > 1 && nc < n) n=nc; //adjust the dimensionality
                for(i=0;i<nc;i++){
                    c = (cfptr == NULL ? cptr[i] : (int)cfptr[i]);
                    if(c < 0 || c > WHITE) error("% is invalid (valid is % to %)", (int)c, 0, WHITE);
                }
            }
        }
        if(argc == 13){
            getargaddress(argv[12], &fptr, &ffptr, &nf); 
            if(nf == 1) f = getint(argv[12], 0, WHITE);
            else if(nf>1) {
                if(nf > 1 && nf < n) n=nf; //adjust the dimensionality
                for(i=0;i<nf;i++){
                    f = (ffptr == NULL ? fptr[i] : (int)ffptr[i]);
                    if(f < -1 || f > WHITE) error("% is invalid (valid is % to %)", (int)f, -1, WHITE);
                }
            }
        }
        for(i=0;i<n;i++){
            x1 = (x1fptr==NULL ? x1ptr[i] : (int)x1fptr[i]);
            y1 = (y1fptr==NULL ? y1ptr[i] : (int)y1fptr[i]);
            wi = (wifptr==NULL ? wiptr[i] : (int)wifptr[i]);
            h =  (hfptr==NULL ? hptr[i] : (int)hfptr[i]);
            wmod=(wi > 0 ? -1 : 1);
            hmod=(h > 0 ? -1 : 1);
            if(nw > 1) w = (wfptr==NULL ? wptr[i] : (int)wfptr[i]);
            if(nc > 1) c = (cfptr==NULL ? cptr[i] : (int)cfptr[i]);
            if(nf > 1) f = (ffptr==NULL ? fptr[i] : (int)ffptr[i]);
            if(wi != 0 && h != 0) DrawRBox(x1, y1, x1 + wi + wmod, y1 + h + hmod, w, c, f);
        }
    }
    if(Option.Refresh)Display_Refresh();
}
// this function positions the cursor within a PRINT command
void fun_at(void) {
    char buf[27];
	getargs(&ep, 5, ",");
	if(commandfunction(cmdtoken) != cmd_print) error("Invalid function");
//	if((argc == 3 || argc == 5)) error("Incorrect number of arguments");
//	AutoLineWrap = false;
	CurrentX = getinteger(argv[0]);
	if(argc>=3  && *argv[2])CurrentY = getinteger(argv[2]);
	if(argc == 5) {
	    PrintPixelMode = getinteger(argv[4]);
    	if(PrintPixelMode < 0 || PrintPixelMode > 7) {
        	PrintPixelMode = 0;
        	error("Number out of bounds");
        }
    } else
	    PrintPixelMode = 0;

    // BJR: VT100 set cursor location: <esc>[y;xf
    //      where x and y are ASCII string integers.
    //      Assumes overall font size of 6x12 pixels (480/80 x 432/36), including gaps between characters and lines

    sprintf(buf, "\033[%d;%df", (int)CurrentY/(FontTable[gui_font >> 4][1] * (gui_font & 0b1111))+1, (int)CurrentX/(FontTable[gui_font >> 4][0] * (gui_font & 0b1111)));
    SSPrintString(buf);								                // send it to the USB
	if(PrintPixelMode==2 || PrintPixelMode==5)SSPrintString("\033[7m");
    targ=T_STR;
    sret = "\0";                                                    // normally pointing sret to a string in flash is illegal
}


// these three functions were written by Peter Mather (matherp on the Back Shed forum)
// read the contents of a PIXEL out of screen memory
void fun_pixel(void) {
    if((void *)ReadBuffer == (void *)DisplayNotSet) error("Invalid on this display");
    int p;
    int x, y;
    getargs(&ep, 3, ",");
    if(argc != 3) error("Argument count");
    x = getinteger(argv[0]);
    y = getinteger(argv[2]);
    ReadBuffer(x, y, x, y, (char *)&p);
    iret = p & 0xFFFFFF;
    targ = T_INT;
}

void cmd_triangle(void) {                                           // thanks to Peter Mather (matherp on the Back Shed forum)
    int x1, y1, x2, y2, x3, y3, c=0, f=0,  n=0,i, nc=0, nf=0;
    long long int *x3ptr, *y3ptr, *x1ptr, *y1ptr, *x2ptr, *y2ptr, *fptr, *cptr;
    MMFLOAT *x3fptr, *y3fptr, *x1fptr, *y1fptr, *x2fptr, *y2fptr, *ffptr, *cfptr;
    getargs(&cmdline, 15,",");
    if(Option.DISPLAY_TYPE == 0) error("Display not configured");
    if(!(argc & 1) || argc < 11) error("Argument count");
    getargaddress(argv[0], &x1ptr, &x1fptr, &n);
    if(n != 1) {
    	int cn=n;
        getargaddress(argv[2], &y1ptr, &y1fptr, &n);
        if(n<cn)cn=n;
        getargaddress(argv[4], &x2ptr, &x2fptr, &n);
        if(n<cn)cn=n;
        getargaddress(argv[6], &y2ptr, &y2fptr, &n);
        if(n<cn)cn=n;
        getargaddress(argv[8], &x3ptr, &x3fptr, &n);
        if(n<cn)cn=n;
        getargaddress(argv[10], &y3ptr, &y3fptr, &n);
        if(n<cn)cn=n;
        n=cn;
    }
    if(n == 1){
        c = gui_fcolour; f = -1;
        x1 = getinteger(argv[0]);
        y1 = getinteger(argv[2]);
        x2 = getinteger(argv[4]);
        y2 = getinteger(argv[6]);
        x3 = getinteger(argv[8]);
        y3 = getinteger(argv[10]);
        if(argc >= 13 && *argv[12]) c = getint(argv[12], BLACK, WHITE);
        if(argc == 15) f = getint(argv[14], -1, WHITE);
        DrawTriangle(x1, y1, x2, y2, x3, y3, c, f);
    } else {
        c = gui_fcolour; f = -1;
        if(argc >= 13 && *argv[12]) {
            getargaddress(argv[12], &cptr, &cfptr, &nc); 
            if(nc == 1) c = getint(argv[10], 0, WHITE);
            else if(nc>1) {
                if(nc > 1 && nc < n) n=nc; //adjust the dimensionality
                for(i=0;i<nc;i++){
                    c = (cfptr == NULL ? cptr[i] : (int)cfptr[i]);
                    if(c < 0 || c > WHITE) error("% is invalid (valid is % to %)", (int)c, 0, WHITE);
                }
            }
        }
        if(argc == 15){
            getargaddress(argv[14], &fptr, &ffptr, &nf); 
            if(nf == 1) f = getint(argv[14], -1, WHITE);
            else if(nf>1) {
                if(nf > 1 && nf < n) n=nf; //adjust the dimensionality
                for(i=0;i<nf;i++){
                    f = (ffptr == NULL ? fptr[i] : (int)ffptr[i]);
                    if(f < -1 || f > WHITE) error("% is invalid (valid is % to %)", (int)f, -1, WHITE);
                }
            }
        }
        for(i=0;i<n;i++){
            x1 = (x1fptr==NULL ? x1ptr[i] : (int)x1fptr[i]);
            y1 = (y1fptr==NULL ? y1ptr[i] : (int)y1fptr[i]);
            x2 = (x2fptr==NULL ? x2ptr[i] : (int)x2fptr[i]);
            y2 = (y2fptr==NULL ? y2ptr[i] : (int)y2fptr[i]);
            x3 = (x3fptr==NULL ? x3ptr[i] : (int)x3fptr[i]);
            y3 = (y3fptr==NULL ? y3ptr[i] : (int)y3fptr[i]);
            if(x1==x2 && x1==x3 && y1==y2 && y1==y3 && x1==-1 && y1==-1)return;
            if(nc > 1) c = (cfptr==NULL ? cptr[i] : (int)cfptr[i]);
            if(nf > 1) f = (ffptr==NULL ? fptr[i] : (int)ffptr[i]);
            DrawTriangle(x1, y1, x2, y2, x3, y3, c, f);
        }
    }
    if(Option.Refresh)Display_Refresh();
}

void cmd_cls(void) {
    if(Option.DISPLAY_TYPE == 0) error("Display not configured");
#ifndef PICOMITEVGA
    HideAllControls();
#endif
    skipspace(cmdline);
    if(!(*cmdline == 0 || *cmdline == '\''))
        ClearScreen(getint(cmdline, 0, WHITE));
    else
        ClearScreen(gui_bcolour);
    CurrentX = CurrentY = 0;
    if(Option.Refresh)Display_Refresh();
}



void fun_rgb(void) {
    getargs(&ep, 5, ",");
    if(argc == 5)
        iret = rgb(getint(argv[0], 0, 255), getint(argv[2], 0, 255), getint(argv[4], 0, 255));
    else if(argc == 1) {
        if(checkstring(argv[0], "WHITE"))        iret = WHITE;
        else if(checkstring(argv[0], "YELLOW"))  iret = YELLOW;
        else if(checkstring(argv[0], "LILAC"))   iret = LILAC;
        else if(checkstring(argv[0], "BROWN"))   iret = BROWN;
        else if(checkstring(argv[0], "FUCHSIA")) iret = FUCHSIA;
        else if(checkstring(argv[0], "RUST"))    iret = RUST;
        else if(checkstring(argv[0], "MAGENTA")) iret = MAGENTA;
        else if(checkstring(argv[0], "RED"))     iret = RED;
        else if(checkstring(argv[0], "CYAN"))    iret = CYAN;
        else if(checkstring(argv[0], "GREEN"))   iret = GREEN;
        else if(checkstring(argv[0], "CERULEAN"))iret = CERULEAN;
        else if(checkstring(argv[0], "MIDGREEN"))iret = MIDGREEN;
        else if(checkstring(argv[0], "COBALT"))  iret = COBALT;
        else if(checkstring(argv[0], "MYRTLE"))  iret = MYRTLE;
        else if(checkstring(argv[0], "BLUE"))    iret = BLUE;
        else if(checkstring(argv[0], "BLACK"))   iret = BLACK;
        else if(checkstring(argv[0], "GRAY"))    iret = GRAY;
        else if(checkstring(argv[0], "GREY"))    iret = GRAY;
        else if(checkstring(argv[0], "LIGHTGRAY"))    iret = LITEGRAY;
        else if(checkstring(argv[0], "LIGHTGREY"))    iret = LITEGRAY;
        else if(checkstring(argv[0], "ORANGE"))    iret = ORANGE;
        else if(checkstring(argv[0], "PINK"))    iret = PINK;
        else if(checkstring(argv[0], "GOLD"))    iret = GOLD;
        else if(checkstring(argv[0], "SALMON"))    iret = SALMON;
        else if(checkstring(argv[0], "BEIGE"))    iret = BEIGE;
        else error("Invalid colour: $", argv[0]);
    } else
        error("Syntax");
    targ = T_INT;
}



void fun_mmhres(void) {
    iret = HRes;
    targ = T_INT;
}



void fun_mmvres(void) {
    iret = VRes;
    targ = T_INT;
}
extern BYTE BDEC_bReadHeader(BMPDECODER *pBmpDec, int fnbr);
extern BYTE BMP_bDecode_memory(int x, int y, int xlen, int ylen, int fnbr, char *p);
#ifdef PICOMITEVGA
void cmd_blit(void) {
    int x1, y1, x2, y2, w, h, bnbr;
    unsigned char *buff = NULL;
    unsigned char *p;
    if(Option.DISPLAY_TYPE == 0) error("Display not configured");
    if((p = checkstring(cmdline, "LOAD"))) {
        int fnbr;
        int xOrigin, yOrigin, xlen, ylen;
        BMPDECODER BmpDec;
        // get the command line arguments
        getargs(&p, 11, ",");                                            // this MUST be the first executable line in the function
        if(*argv[0] == '#') argv[0]++;                              // check if the first arg is prefixed with a #
        bnbr = getint(argv[0], 1, MAXBLITBUF) - 1;                  // get the buffer number
        if(argc == 0) error("Argument count");
        if(!InitSDCard()) return;
        p = getCstring(argv[2]);                                        // get the file name
        xOrigin = yOrigin = 0;
        if(argc >= 5 && *argv[4]) xOrigin = getinteger(argv[4]);                    // get the x origin (optional) argument
        if(argc >= 7 && *argv[6]) yOrigin = getinteger(argv[6]);                    // get the y origin (optional) argument
        if(xOrigin<0 || yOrigin<0)error("Coordinates");
        xlen = ylen =-1;
        if(argc >= 9 && *argv[8]) xlen = getinteger(argv[8]);                    // get the x length (optional) argument
        if(argc == 11 ) ylen = getinteger(argv[10]);                    // get the y length (optional) argument
        // open the file
        if(strchr(p, '.') == NULL) strcat(p, ".BMP");
        fnbr = FindFreeFileNbr();
        if(!BasicFileOpen(p, fnbr, FA_READ)) return;
        BDEC_bReadHeader(&BmpDec, fnbr);
        FileClose(fnbr);
        if(xlen==-1)xlen=BmpDec.lWidth;
        if(ylen==-1)ylen=BmpDec.lHeight;
        if(xlen+xOrigin>BmpDec.lWidth || ylen+yOrigin>BmpDec.lHeight)error("Coordinates");
        char *q=GetTempMemory(xlen * ylen * 3);
        blitbuffptr[bnbr] = GetMemory(xlen * ylen +4 );
        memset(q,0xFF,xlen * ylen * 3);
        fnbr = FindFreeFileNbr();
        if(!BasicFileOpen(p, fnbr, FA_READ)) return;
        BMP_bDecode_memory(xOrigin, yOrigin, xlen, ylen, fnbr, q);
        short *bw=(short *)&blitbuffptr[bnbr][0];
        short *bh=(short *)&blitbuffptr[bnbr][2];
        *bw=xlen;
        *bh=ylen;
        char *t=&blitbuffptr[bnbr][4];
        int i=xlen*ylen;
        while(i--){
            if(HRes==320)*t = ((q[2] & 0x80)>> 4) | ((q[1] & 0xC0)>>5) | ((q[0] & 0x80)>>7);
            else *t=(char)(((uint16_t)q[2]+(uint16_t)q[1]+(uint16_t)q[0])<0x180? 0 : 1);
            t++;
            q+=3;
        }
        FileClose(fnbr);
        return;
    }
    if((p = checkstring(cmdline, "READ"))) {
        getargs(&p, 9, ",");
        if((void *)ReadBuffer == (void *)DisplayNotSet) error("Invalid on this display");
        if(argc !=9) error("Syntax");
        if(*argv[0] == '#') argv[0]++;                              // check if the first arg is prefixed with a #
        bnbr = getint(argv[0], 1, MAXBLITBUF) - 1;                  // get the buffer number
        x1 = getinteger(argv[2]);
        y1 = getinteger(argv[4]);
        w = getinteger(argv[6]);
        h = getinteger(argv[8]);
        if(w < 1 || h < 1) return;
        if(x1 < 0) { x2 -= x1; w += x1; x1 = 0; }
        if(y1 < 0) { y2 -= y1; h += y1; y1 = 0; }
        if(x1 + w > HRes) w = HRes - x1;
        if(y1 + h > VRes) h = VRes - y1;
        if(w < 1 || h < 1 || x1 < 0 || x1 + w > HRes || y1 < 0 || y1 + h > VRes ) return;
        if(blitbuffptr[bnbr] == NULL){
            blitbuffptr[bnbr] = GetMemory(w * h + 4);
            ReadBufferFast(x1, y1, x1 + w - 1, y1 + h - 1, &blitbuffptr[bnbr][4]);
            short *bw=(short *)&blitbuffptr[bnbr][0];
            short *bh=(short *)&blitbuffptr[bnbr][2];
            *bw=w;
            *bh=h;
        } else error("Buffer in use");
    } else if((p = checkstring(cmdline, "WRITE"))) {
        getargs(&p, 9, ",");
        if(!(argc == 9 || argc==5)) error("Syntax");
        if(*argv[0] == '#') argv[0]++;                              // check if the first arg is prefixed with a #
        bnbr = getint(argv[0], 1, MAXBLITBUF) - 1;                  // get the buffer number
        x1 = getinteger(argv[2]);
        y1 = getinteger(argv[4]);
        short *bw=(short *)&blitbuffptr[bnbr][0];
        short *bh=(short *)&blitbuffptr[bnbr][2];
        w=*bw;
        h=*bh;
        if(argc >= 7 && *argv[6])w = getinteger(argv[6]);
        if(argc >= 9 && *argv[8])h = getinteger(argv[8]);
        if(w < 1 || h < 1) return;
        if(x1 < 0) { x2 -= x1; w += x1; x1 = 0; }
        if(y1 < 0) { y2 -= y1; h += y1; y1 = 0; }
        if(x1 + w > HRes) w = HRes - x1;
        if(y1 + h > VRes) h = VRes - y1;
        if(w < 1 || h < 1 || x1 < 0 || x1 + w > HRes || y1 < 0 || y1 + h > VRes ) return;
        if(blitbuffptr[bnbr] != NULL){
            DrawBufferFast(x1, y1, x1 + w - 1, y1 + h - 1, &blitbuffptr[bnbr][4]);
        } else error("Buffer not in use");
    } else if((p = checkstring(cmdline, "CLOSE"))) {
        getargs(&p, 1, ",");
        if(*argv[0] == '#') argv[0]++;                              // check if the first arg is prefixed with a #
        bnbr = getint(argv[0], 1, MAXBLITBUF) - 1;                  // get the buffer number
        if(blitbuffptr[bnbr] != NULL){
            FreeMemory(blitbuffptr[bnbr]);
            blitbuffptr[bnbr] = NULL;
        } else error("Buffer not in use");
        // get the number
     } else {
        int memory, max_x;
        getargs(&cmdline, 11, ",");
        if((void *)ReadBuffer == (void *)DisplayNotSet) error("Invalid on this display");
        if(argc != 11) error("Syntax");
        x1 = getinteger(argv[0]);
        y1 = getinteger(argv[2]);
        x2 = getinteger(argv[4]);
        y2 = getinteger(argv[6]);
        w = getinteger(argv[8]);
        h = getinteger(argv[10]);
        if(w < 1 || h < 1) return;
        if(x1 < 0) { x2 -= x1; w += x1; x1 = 0; }
        if(x2 < 0) { x1 -= x2; w += x2; x2 = 0; }
        if(y1 < 0) { y2 -= y1; h += y1; y1 = 0; }
        if(y2 < 0) { y1 -= y2; h += y2; y2 = 0; }
        if(x1 + w > HRes) w = HRes - x1;
        if(x2 + w > HRes) w = HRes - x2;
        if(y1 + h > VRes) h = VRes - y1;
        if(y2 + h > VRes) h = VRes - y2;
        if(w < 1 || h < 1 || x1 < 0 || x1 + w > HRes || x2 < 0 || x2 + w > HRes || y1 < 0 || y1 + h > VRes || y2 < 0 || y2 + h > VRes) return;
        if(x1 >= x2) {
            max_x = 1;
            buff = GetMemory(max_x * h);
            while(w > max_x){
                ReadBufferFast(x1, y1, x1 + max_x - 1, y1 + h - 1, buff);
                DrawBufferFast(x2, y2, x2 + max_x - 1, y2 + h - 1, buff);
                x1 += max_x;
                x2 += max_x;
                w -= max_x;
            }
            ReadBufferFast(x1, y1, x1 + w - 1, y1 + h - 1, buff);
            DrawBufferFast(x2, y2, x2 + w - 1, y2 + h - 1, buff);
            FreeMemory(buff);
            return;
        }
        if(x1 < x2) {
            int start_x1, start_x2;
            max_x = 1;
            buff = GetMemory(max_x * h);
            start_x1 = x1 + w - max_x;
            start_x2 = x2 + w - max_x;
            while(w > max_x){
                ReadBufferFast(start_x1, y1, start_x1 + max_x - 1, y1 + h - 1, buff);
                DrawBufferFast(start_x2, y2, start_x2 + max_x - 1, y2 + h - 1, buff);
                w -= max_x;
                start_x1 -= max_x;
                start_x2 -= max_x;
            }
            ReadBufferFast(x1, y1, x1 + w - 1, y1 + h - 1, buff);
            DrawBufferFast(x2, y2, x2 + w - 1, y2 + h - 1, buff);
            FreeMemory(buff);
            return;
        }
    }
}
#else
void cmd_blit(void) {
    int x1, y1, x2, y2, w, h, bnbr;
    unsigned char *buff = NULL;
    unsigned char *p;
    if(Option.DISPLAY_TYPE == 0) error("Display not configured");
    if((p = checkstring(cmdline, "LOAD"))) {
        int fnbr;
        int xOrigin, yOrigin, xlen, ylen;
        BMPDECODER BmpDec;
        // get the command line arguments
        getargs(&p, 11, ",");                                            // this MUST be the first executable line in the function
        if(*argv[0] == '#') argv[0]++;                              // check if the first arg is prefixed with a #
        bnbr = getint(argv[0], 1, MAXBLITBUF) - 1;                  // get the buffer number
        if(argc == 0) error("Argument count");
        if(!InitSDCard()) return;
        p = getCstring(argv[2]);                                        // get the file name
        xOrigin = yOrigin = 0;
        if(argc >= 5 && *argv[4]) xOrigin = getinteger(argv[4]);                    // get the x origin (optional) argument
        if(argc >= 7 && *argv[6]) yOrigin = getinteger(argv[6]);                    // get the y origin (optional) argument
        if(xOrigin<0 || yOrigin<0)error("Coordinates");
        xlen = ylen =-1;
        if(argc >= 9 && *argv[8]) xlen = getinteger(argv[8]);                    // get the x length (optional) argument
        if(argc == 11 ) ylen = getinteger(argv[10]);                    // get the y length (optional) argument
        // open the file
        if(strchr(p, '.') == NULL) strcat(p, ".BMP");
        fnbr = FindFreeFileNbr();
        if(!BasicFileOpen(p, fnbr, FA_READ)) return;
        BDEC_bReadHeader(&BmpDec, fnbr);
        FileClose(fnbr);
        if(xlen==-1)xlen=BmpDec.lWidth;
        if(ylen==-1)ylen=BmpDec.lHeight;
        if(xlen+xOrigin>BmpDec.lWidth || ylen+yOrigin>BmpDec.lHeight)error("Coordinates");
        blitbuffptr[bnbr] = GetMemory(xlen * ylen * 3 +4 );
        memset(blitbuffptr[bnbr],0xFF,xlen * ylen * 3 +4 );
        fnbr = FindFreeFileNbr();
        if(!BasicFileOpen(p, fnbr, FA_READ)) return;
        BMP_bDecode_memory(xOrigin, yOrigin, xlen, ylen, fnbr, &blitbuffptr[bnbr][4]);
        short *bw=(short *)&blitbuffptr[bnbr][0];
        short *bh=(short *)&blitbuffptr[bnbr][2];
        *bw=xlen;
        *bh=ylen;
        FileClose(fnbr);
        return;
    }
    if((p = checkstring(cmdline, "READ"))) {
        getargs(&p, 9, ",");
        if((void *)ReadBuffer == (void *)DisplayNotSet) error("Invalid on this display");
        if(argc !=9) error("Syntax");
        if(*argv[0] == '#') argv[0]++;                              // check if the first arg is prefixed with a #
        bnbr = getint(argv[0], 1, MAXBLITBUF) - 1;                  // get the buffer number
        x1 = getinteger(argv[2]);
        y1 = getinteger(argv[4]);
        w = getinteger(argv[6]);
        h = getinteger(argv[8]);
        if(w < 1 || h < 1) return;
        if(x1 < 0) { x2 -= x1; w += x1; x1 = 0; }
        if(y1 < 0) { y2 -= y1; h += y1; y1 = 0; }
        if(x1 + w > HRes) w = HRes - x1;
        if(y1 + h > VRes) h = VRes - y1;
        if(w < 1 || h < 1 || x1 < 0 || x1 + w > HRes || y1 < 0 || y1 + h > VRes ) return;
        if(blitbuffptr[bnbr] == NULL){
            blitbuffptr[bnbr] = GetMemory(w * h * 3 + 4);
            ReadBuffer(x1, y1, x1 + w - 1, y1 + h - 1, &blitbuffptr[bnbr][4]);
            short *bw=(short *)&blitbuffptr[bnbr][0];
            short *bh=(short *)&blitbuffptr[bnbr][2];
            *bw=w;
            *bh=h;
        } else error("Buffer in use");
    } else if((p = checkstring(cmdline, "WRITE"))) {
        getargs(&p, 9, ",");
        if(!(argc == 9 || argc==5)) error("Syntax");
        if(*argv[0] == '#') argv[0]++;                              // check if the first arg is prefixed with a #
        bnbr = getint(argv[0], 1, MAXBLITBUF) - 1;                  // get the buffer number
        x1 = getinteger(argv[2]);
        y1 = getinteger(argv[4]);
        short *bw=(short *)&blitbuffptr[bnbr][0];
        short *bh=(short *)&blitbuffptr[bnbr][2];
        w=*bw;
        h=*bh;
        if(argc >= 7 && *argv[6])w = getinteger(argv[6]);
        if(argc >= 9 && *argv[8])h = getinteger(argv[8]);
        if(w < 1 || h < 1) return;
        if(x1 < 0) { x2 -= x1; w += x1; x1 = 0; }
        if(y1 < 0) { y2 -= y1; h += y1; y1 = 0; }
        if(x1 + w > HRes) w = HRes - x1;
        if(y1 + h > VRes) h = VRes - y1;
        if(w < 1 || h < 1 || x1 < 0 || x1 + w > HRes || y1 < 0 || y1 + h > VRes ) return;
        if(blitbuffptr[bnbr] != NULL){
            DrawBuffer(x1, y1, x1 + w - 1, y1 + h - 1, &blitbuffptr[bnbr][4]);
        } else error("Buffer not in use");
    } else if((p = checkstring(cmdline, "CLOSE"))) {
        getargs(&p, 1, ",");
        if(*argv[0] == '#') argv[0]++;                              // check if the first arg is prefixed with a #
        bnbr = getint(argv[0], 1, MAXBLITBUF) - 1;                  // get the buffer number
        if(blitbuffptr[bnbr] != NULL){
            FreeMemory(blitbuffptr[bnbr]);
            blitbuffptr[bnbr] = NULL;
        } else error("Buffer not in use");
        // get the number
     } else {
        int memory, max_x;
        getargs(&cmdline, 11, ",");
        if((void *)ReadBuffer == (void *)DisplayNotSet) error("Invalid on this display");
        if(argc != 11) error("Syntax");
        x1 = getinteger(argv[0]);
        y1 = getinteger(argv[2]);
        x2 = getinteger(argv[4]);
        y2 = getinteger(argv[6]);
        w = getinteger(argv[8]);
        h = getinteger(argv[10]);
        if(w < 1 || h < 1) return;
        if(x1 < 0) { x2 -= x1; w += x1; x1 = 0; }
        if(x2 < 0) { x1 -= x2; w += x2; x2 = 0; }
        if(y1 < 0) { y2 -= y1; h += y1; y1 = 0; }
        if(y2 < 0) { y1 -= y2; h += y2; y2 = 0; }
        if(x1 + w > HRes) w = HRes - x1;
        if(x2 + w > HRes) w = HRes - x2;
        if(y1 + h > VRes) h = VRes - y1;
        if(y2 + h > VRes) h = VRes - y2;
        if(w < 1 || h < 1 || x1 < 0 || x1 + w > HRes || x2 < 0 || x2 + w > HRes || y1 < 0 || y1 + h > VRes || y2 < 0 || y2 + h > VRes) return;
        if(x1 >= x2) {
            max_x = 1;
            buff = GetMemory(max_x * h * 3);
            while(w > max_x){
                ReadBuffer(x1, y1, x1 + max_x - 1, y1 + h - 1, buff);
                DrawBuffer(x2, y2, x2 + max_x - 1, y2 + h - 1, buff);
                x1 += max_x;
                x2 += max_x;
                w -= max_x;
            }
            ReadBuffer(x1, y1, x1 + w - 1, y1 + h - 1, buff);
            DrawBuffer(x2, y2, x2 + w - 1, y2 + h - 1, buff);
            FreeMemory(buff);
            return;
        }
        if(x1 < x2) {
            int start_x1, start_x2;
            max_x = 1;
            buff = GetMemory(max_x * h * 3);
            start_x1 = x1 + w - max_x;
            start_x2 = x2 + w - max_x;
            while(w > max_x){
                ReadBuffer(start_x1, y1, start_x1 + max_x - 1, y1 + h - 1, buff);
                DrawBuffer(start_x2, y2, start_x2 + max_x - 1, y2 + h - 1, buff);
                w -= max_x;
                start_x1 -= max_x;
                start_x2 -= max_x;
            }
            ReadBuffer(x1, y1, x1 + w - 1, y1 + h - 1, buff);
            DrawBuffer(x2, y2, x2 + w - 1, y2 + h - 1, buff);
            FreeMemory(buff);
            return;
        }
    }
}

#endif

void cmd_font(void) {
    getargs(&cmdline, 3, ",");
    if(argc < 1) error("Argument count");
    if(*argv[0] == '#') ++argv[0];
    if(argc == 3)
        SetFont(((getint(argv[0], 1, FONT_TABLE_SIZE) - 1) << 4) | getint(argv[2], 1, 15));
    else
        SetFont(((getint(argv[0], 1, FONT_TABLE_SIZE) - 1) << 4) | 1);
    if(Option.DISPLAY_CONSOLE && !CurrentLinePtr) {                 // if we are at the command prompt on the LCD
        PromptFont = gui_font;
        if(CurrentY + gui_font_height >= VRes) {
            ScrollLCD(CurrentY + gui_font_height - VRes);           // scroll up if the font change split the line over the bottom
            CurrentY -= (CurrentY + gui_font_height - VRes);
        }
    }
}



void cmd_colour(void) {
    getargs(&cmdline, 3, ",");
    if(argc < 1) error("Argument count");
    gui_fcolour = getColour(argv[0], 0);
    if(argc == 3)  gui_bcolour = getColour(argv[2], 0);
    last_fcolour = gui_fcolour;
    last_bcolour = gui_bcolour;
    if(!CurrentLinePtr) {
        PromptFC = gui_fcolour;
        PromptBC = gui_bcolour;
    }
}
#ifdef PICOMITEVGA
void cmd_tile(void){
    uint32_t bcolour=0xFFFFFFFF,fcolour=0xFFFFFFFF;
    int xlen=1,ylen=1;
    getargs(&cmdline, 11, ",");
    if(!(DISPLAY_TYPE==MONOVGA))return;
    if(argc<5)error("Syntax");
    int x=getint(argv[0],0,39);
    int y=getint(argv[2],0,29);
    int tilebcolour, tilefcolour ;
    if(*argv[4]){
        tilefcolour = getColour(argv[4], 0);
        fcolour = ((tilefcolour & 0x800000)>> 20) | ((tilefcolour & 0xC000)>>13) | ((tilefcolour & 0x80)>>7);
        fcolour= (fcolour<<12) | (fcolour<<8) | (fcolour<<4) | fcolour;
    }
    if(argc>=7 && *argv[6]){
        tilebcolour = getColour(argv[6], 0);
        bcolour = ((tilebcolour & 0x800000)>> 20) | ((tilebcolour & 0xC000)>>13) | ((tilebcolour & 0x80)>>7);
        bcolour= (bcolour<<12) | (bcolour<<8) | (bcolour<<4) | bcolour;
    }
    if(argc>=9 && *argv[8]){
        xlen=getint(argv[8],1,40-x);
    }
    if(argc>=11 && *argv[10]){
        ylen=getint(argv[10],1,30-y);
    }
    for(int xp=x;xp<x+xlen;xp++){
        for(int yp=y;yp<y+ylen;yp++){
            if(fcolour!=0xFFFFFFFF) tilefcols[yp*40+xp]=(uint16_t)fcolour;
            if(bcolour!=0xFFFFFFFF) tilebcols[yp*40+xp]=(uint16_t)bcolour;
        }
    }
}
void cmd_mode(void){
    int mode =getint(cmdline,1,2);
    if(mode==2){
        DISPLAY_TYPE=COLOURVGA; 
    } else {
        DISPLAY_TYPE=MONOVGA;
    }
    memset(WriteBuf, 0, 38400);
//    uSec(10000);
    ResetDisplay();
    CurrentX = CurrentY =0;
    if(mode==2){
        ClearScreen(Option.DefaultBC);
        SetFont((6<<4) | 1) ;
    } else SetFont(1) ;
}
#endif
void fun_mmcharwidth(void) {
  if(Option.DISPLAY_TYPE == 0) error("Display not configured");
    iret = FontTable[gui_font >> 4][0] * (gui_font & 0b1111);
    targ = T_INT;
}


void fun_mmcharheight(void) {
  if(Option.DISPLAY_TYPE == 0) error("Display not configured");
    iret = FontTable[gui_font >> 4][1] * (gui_font & 0b1111);
    targ = T_INT;
}










/****************************************************************************************************
 ****************************************************************************************************

 Basic drawing primitives for a user defined LCD display driver (ie, OPTION LCDPANEL USER)
 all drawing is done using either DrawRectangleUser() or DrawBitmapUser()

 ****************************************************************************************************
****************************************************************************************************/
void cmd_refresh(void){
    if(Option.DISPLAY_TYPE == 0) error("Display not configured");
    low_y=0; high_y=DisplayVRes-1; low_x=0; high_x=DisplayHRes-1;
	Display_Refresh();
}


#ifdef PICOMITEVGA

void Display_Refresh(void){
}

void DrawPixelMono(int x, int y, int c){
    if(x<0 || y<0 || x>=HRes || y>=VRes)return;
	uint8_t *p=(uint8_t *)(((uint32_t) WriteBuf)+(y*(HRes>>3))+(x>>3));
	uint8_t bit = 1<<(x % 8);
	if(c)*p |=bit;
	else *p &= ~bit;
}
void DrawRectangleMono(int x1, int y1, int x2, int y2, int c){
    int x,y,x1p, x2p, t;
    unsigned char mask;
    unsigned char *p;
    if(x1 < 0) x1 = 0;
    if(x1 >= HRes) x1 = HRes - 1;
    if(x2 < 0) x2 = 0;
    if(x2 >= HRes) x2 = HRes - 1;
    if(y1 < 0) y1 = 0;
    if(y1 >= VRes) y1 = VRes - 1;
    if(y2 < 0) y2 = 0;
    if(y2 >= VRes) y2 = VRes - 1;
    if(x2 <= x1) { t = x1; x1 = x2; x2 = t; }
    if(y2 <= y1) { t = y1; y1 = y2; y2 = t; }
    if(x2 <= x1) { t = x1; x1 = x2; x2 = t; }
    if(y2 <= y1) { t = y1; y1 = y2; y2 = t; }
    if(x1==x2){
        for(y=y1;y<=y2;y++){
            p=&WriteBuf[(y*(HRes>>3))+(x1>>3)];
            mask=1<<(x1 % 8); //get the bit position for this bit
            if(c){
                *p|=mask;
            } else {
                *p&=(~mask);
            }
        }
    } else {
        for(y=y1;y<=y2;y++){
            x1p=x1;
            x2p=x2;
            if((x1 % 8) !=0){
                p=&WriteBuf[(y*(HRes>>3))+(x1>>3)];
                for(x=x1;x<=x2 && (x % 8)!=0;x++){
                    mask=1<<(x % 8); //get the bit position for this bit
                    if(c){
                        *p|=mask;
                    } else {
                        *p&=(~mask);
                    }
                    x1p++;
                }
            }
            if(x1p-1!=x2 && (x2 % 8)!=7){
                p=&WriteBuf[(y*(HRes>>3))+(x2p>>3)];
                for(x=(x2 & 0xFFF8);x<=x2 ;x++){
                    mask=1<<(x % 8); //get the bit position for this bit
                    if(c){
                        *p|=mask;
                    } else {
                        *p&=(~mask);
                    }
                    x2p--;
                }
            }
            p=&WriteBuf[(y*(HRes>>3))+(x1p>>3)];
            for(x=x1p;x<x2p;x+=8){
                if(c){
                    *p++=0xFF;
                } else {
                    *p++=0;
                }
            }
        }
    }
}

void DrawBitmapMono(int x1, int y1, int width, int height, int scale, int fc, int bc, unsigned char *bitmap){
    int i, j, k, m, x, y, loc;
    unsigned char mask;
    if(x1>=HRes || y1>=VRes || x1+width*scale<0 || y1+height*scale<0)return;
    for(i = 0; i < height; i++) {                                   // step thru the font scan line by line
        for(j = 0; j < scale; j++) {                                // repeat lines to scale the font
            for(k = 0; k < width; k++) {                            // step through each bit in a scan line
                for(m = 0; m < scale; m++) {                        // repeat pixels to scale in the x axis
                    x=x1 + k * scale + m ;
                    y=y1 + i * scale + j ;
                    mask=1<<(x % 8); //get the bit position for this bit
                    if(x >= 0 && x < HRes && y >= 0 && y < VRes) {  // if the coordinates are valid
                        loc=(y*(HRes>>3))+(x>>3);
                        if((bitmap[((i * width) + k)/8] >> (((height * width) - ((i * width) + k) - 1) %8)) & 1) {
                            if(fc){
                            	WriteBuf[loc]|=mask;
                             } else {
                            	 WriteBuf[loc]&= ~mask;
                             }
                       } else {
                            if(bc>0){
                            	WriteBuf[loc]|=mask;
                            } else if(bc==0) {
                            	WriteBuf[loc]&= ~mask;
                            }
                        }
                   }
                }
            }
        }
    }

}

void ScrollLCDMono(int lines){
    if(lines==0)return;
     if(lines >= 0) {
        for(int i=0;i<VRes-lines;i++) {
            int d=i*(HRes>>3),s=(i+lines)*(HRes>>3); 
            for(int c=0;c<(HRes>>3);c++)WriteBuf[d+c]=WriteBuf[s+c];
        }
        DrawRectangle(0, VRes-lines, HRes - 1, VRes - 1, 0); // erase the lines to be scrolled off
    } else {
    	lines=-lines;
        for(int i=VRes-1;i>=lines;i--) {
            int d=i*(HRes>>3),s=(i-lines)*(HRes>>3); 
            for(int c=0;c<(HRes>>3);c++)WriteBuf[d+c]=WriteBuf[s+c];
        }
        DrawRectangle(0, 0, HRes - 1, lines - 1, 0); // erase the lines introduced at the top
    }
}
void DrawBufferMono(int x1, int y1, int x2, int y2, unsigned char *p){
	int x,y, t, loc;
    unsigned char mask;
    union colourmap
    {
    char rgbbytes[4];
    unsigned int rgb;
    } c;
    // make sure the coordinates are kept within the display area
    if(x2 <= x1) { t = x1; x1 = x2; x2 = t; }
    if(y2 <= y1) { t = y1; y1 = y2; y2 = t; }
    if(x1 < 0) x1 = 0;
    if(x1 >= HRes) x1 = HRes - 1;
    if(x2 < 0) x2 = 0;
    if(x2 >= HRes) x2 = HRes - 1;
    if(y1 < 0) y1 = 0;
    if(y1 >= VRes) y1 = VRes - 1;
    if(y2 < 0) y2 = 0;
    if(y2 >= VRes) y2 = VRes - 1;
	for(y=y1;y<=y2;y++){
    	for(x=x1;x<=x2;x++){
	        c.rgbbytes[0]=*p++; 
	        c.rgbbytes[1]=*p++;
	        c.rgbbytes[2]=*p++;
            c.rgbbytes[3]=0;
            loc=(y*(HRes>>3))+(x>>3);
            mask=1<<(x % 8); //get the bit position for this bit
            if(c.rgb){
            	WriteBuf[loc]|=mask;
            } else {
            	WriteBuf[loc]&=(~mask);
            }
        }
    }
}
void DrawBufferMonoFast(int x1, int y1, int x2, int y2, unsigned char *p){
	int x,y, t, loc;
    unsigned char mask;
    // make sure the coordinates are kept within the display area
    if(x2 <= x1) { t = x1; x1 = x2; x2 = t; }
    if(y2 <= y1) { t = y1; y1 = y2; y2 = t; }
    if(x1 < 0) x1 = 0;
    if(x1 >= HRes) x1 = HRes - 1;
    if(x2 < 0) x2 = 0;
    if(x2 >= HRes) x2 = HRes - 1;
    if(y1 < 0) y1 = 0;
    if(y1 >= VRes) y1 = VRes - 1;
    if(y2 < 0) y2 = 0;
    if(y2 >= VRes) y2 = VRes - 1;
	for(y=y1;y<=y2;y++){
    	for(x=x1;x<=x2;x++){
            loc=(y*(HRes>>3))+(x>>3);
            mask=1<<(x % 8); //get the bit position for this bit
            if(*p++){
            	WriteBuf[loc]|=mask;
            } else {
            	WriteBuf[loc]&=(~mask);
            }
        }
    }
}

void ReadBufferMono(int x1, int y1, int x2, int y2, unsigned char *c){
    int x,y,t,loc;
    uint8_t *pp;
    unsigned char mask;
    if(x2 <= x1) { t = x1; x1 = x2; x2 = t; }
    if(y2 <= y1) { t = y1; y1 = y2; y2 = t; }
    int xx1=x1, yy1=y1, xx2=x2, yy2=y2;
    if(x1 < 0) xx1 = 0;
    if(x1 >= HRes) xx1 = HRes - 1;
    if(x2 < 0) xx2 = 0;
    if(x2 >= HRes) xx2 = HRes - 1;
    if(y1 < 0) yy1 = 0;
    if(y1 >= VRes) yy1 = VRes - 1;
    if(y2 < 0) yy2 = 0;
    if(y2 >= VRes) yy2 = VRes - 1;
	for(y=y1;y<=y2;y++){
    	for(x=x1;x<=x2;x++){
            loc=(y*(HRes>>3))+(x>>3);
            mask=1<<(x % 8); //get the bit position for this bit
            if(WriteBuf[loc]&mask){
                *c++=0xFF;
                *c++=0xFF;
                *c++=0xFF;
            } else {
                *c++=0x00;
                *c++=0x00;
                *c++=0x00;
            }
        }
    }
}
void ReadBufferMonoFast(int x1, int y1, int x2, int y2, unsigned char *c){
    int x,y,t,loc;
    uint8_t *pp;
    unsigned char mask;
    if(x2 <= x1) { t = x1; x1 = x2; x2 = t; }
    if(y2 <= y1) { t = y1; y1 = y2; y2 = t; }
    int xx1=x1, yy1=y1, xx2=x2, yy2=y2;
    if(x1 < 0) xx1 = 0;
    if(x1 >= HRes) xx1 = HRes - 1;
    if(x2 < 0) xx2 = 0;
    if(x2 >= HRes) xx2 = HRes - 1;
    if(y1 < 0) yy1 = 0;
    if(y1 >= VRes) yy1 = VRes - 1;
    if(y2 < 0) yy2 = 0;
    if(y2 >= VRes) yy2 = VRes - 1;
	for(y=y1;y<=y2;y++){
    	for(x=x1;x<=x2;x++){
            loc=(y*(HRes>>3))+(x>>3);
            mask=1<<(x % 8); //get the bit position for this bit
            if(WriteBuf[loc]&mask){
                *c++=0xFF;
            } else {
                *c++=0x00;
            }
        }
    }
}
void DrawPixelColour(int x, int y, int c){
    if(x<0 || y<0 || x>=HRes || y>=VRes)return;
    unsigned char colour = ((c & 0x800000)>> 20) | ((c & 0xC000)>>13) | ((c & 0x80)>>7);
	uint8_t *p=(uint8_t *)(((uint32_t) WriteBuf)+(y*(HRes>>1))+(x>>1));
    if(x & 1){
        *p &=0x0F;
        *p |=(colour<<4);
    } else {
        *p &=0xF0;
        *p |= colour;
    }
}
void DrawRectangleColour(int x1, int y1, int x2, int y2, int c){
    int x,y,x1p,x2p,t, loc;
    unsigned char mask;
    unsigned char colour = ((c & 0x800000)>> 20) | ((c & 0xC000)>>13) | ((c & 0x80)>>7);
    unsigned char bcolour=(colour<<4) | colour;
    if(x1 < 0) x1 = 0;
    if(x1 >= HRes) x1 = HRes - 1;
    if(x2 < 0) x2 = 0;
    if(x2 >= HRes) x2 = HRes - 1;
    if(y1 < 0) y1 = 0;
    if(y1 >= VRes) y1 = VRes - 1;
    if(y2 < 0) y2 = 0;
    if(y2 >= VRes) y2 = VRes - 1;
    if(x2 <= x1) { t = x1; x1 = x2; x2 = t; }
    if(y2 <= y1) { t = y1; y1 = y2; y2 = t; }
    if(x2 <= x1) { t = x1; x1 = x2; x2 = t; }
    if(y2 <= y1) { t = y1; y1 = y2; y2 = t; }
    for(y=y1;y<=y2;y++){
        x1p=x1;
        x2p=x2;
        uint8_t *p=(uint8_t *)(((uint32_t) WriteBuf)+(y*(HRes>>1))+(x1>>1));
        if((x1 % 2) == 1){
            *p &=0x0F;
            *p |=(colour<<4);
            p++;
            x1p++;
        }
        if((x2 % 2) == 0){
            uint8_t *q=(uint8_t *)(((uint32_t) WriteBuf)+(y*(HRes>>1))+(x2>>1));
            *q &=0xF0;
            *q |= colour;
            x2p--;
        }
        for(x=x1p;x<x2p;x+=2){
            *p++=bcolour;
        }
    }
}
void DrawBitmapColour(int x1, int y1, int width, int height, int scale, int fc, int bc, unsigned char *bitmap){
    int i, j, k, m, x, y, loc;
    unsigned char mask;
    if(x1>=HRes || y1>=VRes || x1+width*scale<0 || y1+height*scale<0)return;
    unsigned char fcolour = ((fc & 0x800000)>> 20) | ((fc & 0xC000)>>13) | ((fc & 0x80)>>7);
    unsigned char bcolour = ((bc & 0x800000)>> 20) | ((bc & 0xC000)>>13) | ((bc & 0x80)>>7);

    for(i = 0; i < height; i++) {                                   // step thru the font scan line by line
        for(j = 0; j < scale; j++) {                                // repeat lines to scale the font
            for(k = 0; k < width; k++) {                            // step through each bit in a scan line
                for(m = 0; m < scale; m++) {                        // repeat pixels to scale in the x axis
                    x=x1 + k * scale + m ;
                    y=y1 + i * scale + j ;
                    if(x >= 0 && x < HRes && y >= 0 && y < VRes) {  // if the coordinates are valid
	                    uint8_t *p=(uint8_t *)(((uint32_t) WriteBuf)+(y*(HRes>>1))+(x>>1));
                        if((bitmap[((i * width) + k)/8] >> (((height * width) - ((i * width) + k) - 1) %8)) & 1) {
                            if(x & 1){
                                *p &=0x0F;
                                *p |=(fcolour<<4);
                            } else {
                                *p &=0xF0;
                                *p |= fcolour;
                            }
                        } else {
                            if(bc>=0){
                                if(x & 1){
                                    *p &=0x0F;
                                    *p |=(bcolour<<4);
                                } else {
                                    *p &=0xF0;
                                    *p |= bcolour;
                                }
                            }
                        }
                   }
                }
            }
        }
    }

}

void ScrollLCDColour(int lines){
    if(lines==0)return;
     if(lines >= 0) {
        for(int i=0;i<VRes-lines;i++) {
            int d=i*(HRes>>1),s=(i+lines)*(HRes>>1); 
            for(int c=0;c<(HRes>>1);c++)WriteBuf[d+c]=WriteBuf[s+c];
        }
        DrawRectangle(0, VRes-lines, HRes - 1, VRes - 1, PromptBC); // erase the lines to be scrolled off
    } else {
    	lines=-lines;
        for(int i=VRes-1;i>=lines;i--) {
            int d=i*(HRes>>1),s=(i-lines)*(HRes>>1); 
            for(int c=0;c<(HRes>>1);c++)WriteBuf[d+c]=WriteBuf[s+c];
        }
        DrawRectangle(0, 0, HRes - 1, lines - 1, PromptBC); // erase the lines introduced at the top
    }
}
void DrawBufferColour(int x1, int y1, int x2, int y2, unsigned char *p){
	int x,y, t;
    union colourmap
    {
    char rgbbytes[4];
    unsigned int rgb;
    } c;
    unsigned char fcolour;
    uint8_t *pp;
    // make sure the coordinates are kept within the display area
    if(x2 <= x1) { t = x1; x1 = x2; x2 = t; }
    if(y2 <= y1) { t = y1; y1 = y2; y2 = t; }
    if(x1 < 0) x1 = 0;
    if(x1 >= HRes) x1 = HRes - 1;
    if(x2 < 0) x2 = 0;
    if(x2 >= HRes) x2 = HRes - 1;
    if(y1 < 0) y1 = 0;
    if(y1 >= VRes) y1 = VRes - 1;
    if(y2 < 0) y2 = 0;
    if(y2 >= VRes) y2 = VRes - 1;
	for(y=y1;y<=y2;y++){
    	for(x=x1;x<=x2;x++){
	        c.rgbbytes[0]=*p++; //this order swaps the bytes to match the .BMP file
	        c.rgbbytes[1]=*p++;
	        c.rgbbytes[2]=*p++;
            fcolour = ((c.rgb & 0x800000)>> 20) | ((c.rgb & 0xC000)>>13) | ((c.rgb & 0x80)>>7);
            pp=(uint8_t *)(((uint32_t) WriteBuf)+(y*(HRes>>1))+(x>>1));
            if(x & 1){
                *pp &=0x0F;
                *pp |=(fcolour<<4);
            } else {
                *pp &=0xF0;
                *pp |= fcolour;
            }
        }
    }
}
void DrawBufferColourFast(int x1, int y1, int x2, int y2, unsigned char *p){
	int x,y, t;
    union colourmap
    {
    char rgbbytes[4];
    unsigned int rgb;
    } c;
    unsigned char fcolour;
    uint8_t *pp;
    // make sure the coordinates are kept within the display area
    if(x2 <= x1) { t = x1; x1 = x2; x2 = t; }
    if(y2 <= y1) { t = y1; y1 = y2; y2 = t; }
    if(x1 < 0) x1 = 0;
    if(x1 >= HRes) x1 = HRes - 1;
    if(x2 < 0) x2 = 0;
    if(x2 >= HRes) x2 = HRes - 1;
    if(y1 < 0) y1 = 0;
    if(y1 >= VRes) y1 = VRes - 1;
    if(y2 < 0) y2 = 0;
    if(y2 >= VRes) y2 = VRes - 1;
	for(y=y1;y<=y2;y++){
    	for(x=x1;x<=x2;x++){
            pp=(uint8_t *)(((uint32_t) WriteBuf)+(y*(HRes>>1))+(x>>1));
            if(x & 1){
                *pp &=0x0F;
                *pp |=((*p++)<<4);
            } else {
                *pp &=0xF0;
                *pp |= *p++;
            }
        }
    }
}
void ReadBufferColour(int x1, int y1, int x2, int y2, unsigned char *c){
    int x,y,t;
    uint8_t *pp;
    if(x2 <= x1) { t = x1; x1 = x2; x2 = t; }
    if(y2 <= y1) { t = y1; y1 = y2; y2 = t; }
    int xx1=x1, yy1=y1, xx2=x2, yy2=y2;
    if(x1 < 0) xx1 = 0;
    if(x1 >= HRes) xx1 = HRes - 1;
    if(x2 < 0) xx2 = 0;
    if(x2 >= HRes) xx2 = HRes - 1;
    if(y1 < 0) yy1 = 0;
    if(y1 >= VRes) yy1 = VRes - 1;
    if(y2 < 0) yy2 = 0;
    if(y2 >= VRes) yy2 = VRes - 1;
	for(y=y1;y<=y2;y++){
    	for(x=x1;x<=x2;x++){
	        pp=(uint8_t *)(((uint32_t) WriteBuf)+(y*(HRes>>1))+(x>>1));
            if(x & 1){
                t=colours[(*pp)>>4];
            } else {
                t=colours[(*pp)&0x0F];
            }
            *c++=(t&0xFF);
            *c++=(t>>8)&0xFF;
            *c++=t>>16;
        }
    }
}
void ReadBufferColourFast(int x1, int y1, int x2, int y2, unsigned char *c){
    int x,y,t,last=0;
    uint8_t *pp;
    if(x2 <= x1) { t = x1; x1 = x2; x2 = t; }
    if(y2 <= y1) { t = y1; y1 = y2; y2 = t; }
    int xx1=x1, yy1=y1, xx2=x2, yy2=y2;
    if(x1 < 0) xx1 = 0;
    if(x1 >= HRes) xx1 = HRes - 1;
    if(x2 < 0) xx2 = 0;
    if(x2 >= HRes) xx2 = HRes - 1;
    if(y1 < 0) yy1 = 0;
    if(y1 >= VRes) yy1 = VRes - 1;
    if(y2 < 0) yy2 = 0;
    if(y2 >= VRes) yy2 = VRes - 1;
	for(y=y1;y<=y2;y++){
    	for(x=x1;x<=x2;x++){
	        pp=(uint8_t *)(((uint32_t) WriteBuf)+(y*(HRes>>1))+(x>>1));
            if(!(x & 1)){
                *c++=((*pp)&0x0F);
            } else {
                *c++=((*pp)>>4);
            }
        }
    }
    if(last)*c++=t;
}
void fun_getscanline(void){
    iret=QVgaScanLine;
    targ=T_INT;
}
#else
// Draw a filled rectangle
// this is the basic drawing primitive used by most drawing routines
//    x1, y1, x2, y2 - the coordinates
//    c - the colour
void DrawRectangleUser(int x1, int y1, int x2, int y2, int c){
    char callstr[256];
    char *nextstmtSaved = nextstmt;
    if(FindSubFun("MM.USER_RECTANGLE", 0) >= 0) {
        strcpy(callstr, "MM.USER_RECTANGLE");
        strcat(callstr, " "); IntToStr(callstr + strlen(callstr), x1, 10);
        strcat(callstr, ","); IntToStr(callstr + strlen(callstr), y1, 10);
        strcat(callstr, ","); IntToStr(callstr + strlen(callstr), x2, 10);
        strcat(callstr, ","); IntToStr(callstr + strlen(callstr), y2, 10);
        strcat(callstr, ","); IntToStr(callstr + strlen(callstr), c, 10);
        callstr[strlen(callstr)+1] = 0;                             // two NULL chars required to terminate the call
        LocalIndex++;
        ExecuteProgram(callstr);
        nextstmt = nextstmtSaved;
        LocalIndex--;
        TempMemoryIsChanged = true;                                 // signal that temporary memory should be checked
    } else
        error("MM.USER_RECTANGLE not defined");
}


//Print the bitmap of a char on the video output
//    x, y - the top left of the char
//    width, height - size of the char's bitmap
//    scale - how much to scale the bitmap
//      fc, bc - foreground and background colour
//    bitmap - pointer to the bitmap
void DrawBitmapUser(int x1, int y1, int width, int height, int scale, int fc, int bc, unsigned char *bitmap){
    char callstr[256];
    char *nextstmtSaved = nextstmt;
    if(FindSubFun("MM.USER_BITMAP", 0) >= 0) {
        strcpy(callstr, "MM.USER_BITMAP");
        strcat(callstr, " "); IntToStr(callstr + strlen(callstr), x1, 10);
        strcat(callstr, ","); IntToStr(callstr + strlen(callstr), y1, 10);
        strcat(callstr, ","); IntToStr(callstr + strlen(callstr), width, 10);
        strcat(callstr, ","); IntToStr(callstr + strlen(callstr), height, 10);
        strcat(callstr, ","); IntToStr(callstr + strlen(callstr), scale, 10);
        strcat(callstr, ","); IntToStr(callstr + strlen(callstr), fc, 10);
        strcat(callstr, ","); IntToStr(callstr + strlen(callstr), bc, 10);
        strcat(callstr, ",&H"); IntToStr(callstr + strlen(callstr), (unsigned int)bitmap, 16);
        callstr[strlen(callstr)+1] = 0;                             // two NULL chars required to terminate the call
        LocalIndex++;
        ExecuteProgram(callstr);
        LocalIndex--;
        TempMemoryIsChanged = true;                                 // signal that temporary memory should be checked
        nextstmt = nextstmtSaved;
    } else
        error("MM.USER_BITMAP not defined");
}


void ScrollLCDSPI(int lines){
    if(lines==0)return;
     unsigned char *buff=GetMemory(3*HRes);
     if(lines >= 0) {
        for(int i=0;i<VRes-lines;i++) {
        	ReadBuffer(0, i+lines, HRes -1, i+lines, buff);
			DrawBuffer(0, i, HRes - 1, i, buff);        	
        }
        DrawRectangle(0, VRes-lines, HRes - 1, VRes - 1, gui_bcolour); // erase the lines to be scrolled off
    } else {
    	lines=-lines;
        for(int i=VRes-1;i>=lines;i--) {
        	ReadBuffer(0, i-lines, HRes -1, i-lines, buff);
			DrawBuffer(0, i, HRes - 1, i, buff);        	
        }
        DrawRectangle(0, 0, HRes - 1, lines - 1, gui_bcolour); // erase the lines introduced at the top
    }
    FreeMemory(buff);
}
#endif

/****************************************************************************************************

 General purpose routines

****************************************************************************************************/





int GetFontWidth(int fnt) {
    return FontTable[fnt >> 4][0] * (fnt & 0b1111);
}


int GetFontHeight(int fnt) {
    return FontTable[fnt >> 4][1] * (fnt & 0b1111);
}


void SetFont(int fnt) {
    if(FontTable[fnt >> 4] == NULL) error("Invalid font number #%", (fnt >> 4)+1);
    gui_font_width = FontTable[fnt >> 4][0] * (fnt & 0b1111);
    gui_font_height = FontTable[fnt >> 4][1] * (fnt & 0b1111);
   if(Option.DISPLAY_CONSOLE) {
        Option.Height = VRes/gui_font_height;
        Option.Width = HRes/gui_font_width;
    }
    gui_font = fnt;
}


void ResetDisplay(void) {
        SetFont(Option.DefaultFont);
        gui_fcolour = Option.DefaultFC;
        gui_bcolour = Option.DefaultBC;
    PromptFont = Option.DefaultFont;
    PromptFC = Option.DefaultFC;
    PromptBC = Option.DefaultBC;
#ifdef PICOMITEVGA
        HRes=DISPLAY_TYPE == COLOURVGA ? 320 : 640;
        VRes=DISPLAY_TYPE == COLOURVGA ? 240 : 480;
        if(DISPLAY_TYPE == COLOURVGA){
            DrawRectangle=DrawRectangleColour;
            DrawBitmap= DrawBitmapColour;
            ScrollLCD=ScrollLCDColour;
            DrawBuffer=DrawBufferColour;
            ReadBuffer=ReadBufferColour;
            DrawBufferFast=DrawBufferColourFast;
            ReadBufferFast=ReadBufferColourFast;
            DrawPixel=DrawPixelColour;
        } else {
            DrawRectangle=DrawRectangleMono;
            DrawBitmap= DrawBitmapMono;
            ScrollLCD=ScrollLCDMono;
            DrawBuffer=DrawBufferMono;
            ReadBuffer=ReadBufferMono;
            DrawBufferFast=DrawBufferMonoFast;
            ReadBufferFast=ReadBufferMonoFast;
            DrawPixel=DrawPixelMono;
            gui_fcolour = WHITE;
            gui_bcolour = BLACK;
            PromptFC = gui_fcolour;
            PromptBC = gui_bcolour;
        }
        for(int x=0;x<40;x++){
            for(int y=0;y<30;y++){
                tilefcols[y*40+x]=Option.VGAFC;
                tilebcols[y*40+x]=Option.VGABC;
            }
        }
#else
    ResetGUI();
#endif
}
void hline(int x0, int x1, int y, int f, int ints_per_line, uint32_t *br) { //draw a horizontal line
    uint32_t w1, xx1, w0, xx0, x, xn, i;
    const uint32_t a[]={0xFFFFFFFF,0x7FFFFFFF,0x3FFFFFFF,0x1FFFFFFF,0xFFFFFFF,0x7FFFFFF,0x3FFFFFF,0x1FFFFFF,
                        0xFFFFFF,0x7FFFFF,0x3FFFFF,0x1FFFFF,0xFFFFF,0x7FFFF,0x3FFFF,0x1FFFF,
                        0xFFFF,0x7FFF,0x3FFF,0x1FFF,0xFFF,0x7FF,0x3FF,0x1FF,
                        0xFF,0x7F,0x3F,0x1F,0x0F,0x07,0x03,0x01};
    const uint32_t b[]={0x80000000,0xC0000000,0xe0000000,0xf0000000,0xf8000000,0xfc000000,0xfe000000,0xff000000,
                        0xff800000,0xffC00000,0xffe00000,0xfff00000,0xfff80000,0xfffc0000,0xfffe0000,0xffff0000,
                        0xffff8000,0xffffC000,0xffffe000,0xfffff000,0xfffff800,0xfffffc00,0xfffffe00,0xffffff00,
                        0xffffff80,0xffffffC0,0xffffffe0,0xfffffff0,0xfffffff8,0xfffffffc,0xfffffffe,0xffffffff};
    w0 = y * (ints_per_line);
    xx0 = 0;
    w1 = y * (ints_per_line) + x1/32;
    xx1 = (x1 & 0x1F);
    w0 = y * (ints_per_line) + x0/32;
    xx0 = (x0 & 0x1F);
    w1 = y * (ints_per_line) + x1/32;
    xx1 = (x1 & 0x1F);
    if(w1==w0){ //special case both inside same word
        x=(a[xx0] & b[xx1]);
        xn=~x;
        if(f)br[w0] |= x; else  br[w0] &= xn;                   // turn on the pixel
    } else {
        if(w1-w0>1){ //first deal with full words
            for(i=w0+1;i<w1;i++){
            // draw the pixel
            	br[i]=0;
                if(f)br[i] = 0xFFFFFFFF;          // turn on the pixels
            }
        }
        x=~a[xx0];
        br[w0] &= x;
        x=~x;
        if(f)br[w0] |= x;                         // turn on the pixel
        x=~b[xx1];
        br[w1] &= x;
        x=~x;
        if(f)br[w1] |= x;                         // turn on the pixel
    }
}

void DrawFilledCircle(int x, int y, int radius, int r, int fill, int ints_per_line, uint32_t *br, MMFLOAT aspect, MMFLOAT aspect2) {
   int a, b, P;
   int A, B, asp;
   	   x=(int)((MMFLOAT)r*aspect)+radius;
   	   y=r+radius;
       a = 0;
       b = radius;
       P = 1 - radius;
       asp = aspect2 * (MMFLOAT)(1 << 10);
       do {
	         A = (a * asp) >> 10;
	         B = (b * asp) >> 10;
	         hline(x-A-radius, x+A-radius,  y+b-radius, fill, ints_per_line, br);
	         hline(x-A-radius, x+A-radius,  y-b-radius, fill, ints_per_line, br);
	         hline(x-B-radius, x+B-radius,  y+a-radius, fill, ints_per_line, br);
	         hline(x-B-radius, x+B-radius,  y-a-radius, fill, ints_per_line, br);
	         if(P < 0)
	            P+= 3 + 2*a++;
	          else
	            P+= 5 + 2*(a++ - b--);

        } while(a <= b);
}
/******************************************************************************************
 Print a char on the LCD display (SSD1963 and in landscape only).  It handles control chars
 such as newline and will wrap at the end of the line and scroll the display if necessary.

 The char is printed at the current location defined by CurrentX and CurrentY
 *****************************************************************************************/
void DisplayPutC(char c) {

    if(!Option.DISPLAY_CONSOLE) return;
    // if it is printable and it is going to take us off the right hand end of the screen do a CRLF
    if(c >= FontTable[gui_font >> 4][2] && c < FontTable[gui_font >> 4][2] + FontTable[gui_font >> 4][3]) {
        if(CurrentX + gui_font_width > HRes) {
            DisplayPutC('\r');
            DisplayPutC('\n');
        }
    }

    // handle the standard control chars
    switch(c) {
        case '\b':  CurrentX -= gui_font_width;
            if (CurrentX < 0) CurrentX = 0;
            return;
        case '\r':  CurrentX = 0;
                    return;
        case '\n':  CurrentY += gui_font_height;
                    if(CurrentY + gui_font_height >= VRes) {
                        ScrollLCD(CurrentY + gui_font_height - VRes);
                        CurrentY -= (CurrentY + gui_font_height - VRes);
                    }
                    return;
        case '\t':  do {
                        DisplayPutC(' ');
                    } while((CurrentX/gui_font_width) % 4 /******Option.Tab*/);
                    return;
    }
    GUIPrintChar(gui_font, gui_fcolour, gui_bcolour, c, ORIENT_NORMAL);            // print it
}
void ShowCursor(int show) {
  static int visible = false;
    int newstate;
    if(!Option.DISPLAY_CONSOLE) return;
  newstate = ((CursorTimer <= CURSOR_ON) && show);                  // what should be the state of the cursor?
  if(visible == newstate) return;                                   // we can skip the rest if the cursor is already in the correct state
  visible = newstate;                                               // draw the cursor BELOW the font
    DrawLine(CurrentX, CurrentY + gui_font_height-1, CurrentX + gui_font_width, CurrentY + gui_font_height-1, (gui_font_height<=8 ? 1 : 2), visible ? gui_fcolour : gui_bcolour);
}
#ifdef PICOMITEVGA

#define ABS(X) ((X)>0 ? (X) : (-(X)))

void DrawPolygon(int n, short *xcoord, short *ycoord, int face){
	int i, facecount=struct3d[n]->facecount[face];
    int c=struct3d[n]->line[face];
    int f=struct3d[n]->fill[face];
	// first deal with outline only
	if(struct3d[n]->fill[face]==0xFFFFFFFF){
	    for(i=0;i<facecount;i++){
       		if(i<facecount-1){
       			DrawLine(xcoord[i],ycoord[i],xcoord[i+1],ycoord[i+1],1,c);
       		} else {
       			DrawLine(xcoord[i],ycoord[i],xcoord[0],ycoord[0],1,c);
       		}
	    }
	} else {
		if(facecount==3){
			DrawTriangle(xcoord[0],ycoord[0],xcoord[1],ycoord[1],xcoord[2],ycoord[2],c,f);
		} else if(facecount==4){
			DrawTriangle(xcoord[0],ycoord[0],xcoord[1],ycoord[1],xcoord[2],ycoord[2],f,f);
			DrawTriangle(xcoord[0],ycoord[0],xcoord[2],ycoord[2],xcoord[3],ycoord[3],f,f);
			if(f!=c){
				DrawLine(xcoord[0],ycoord[0],xcoord[1],ycoord[1],1,c);
				DrawLine(xcoord[1],ycoord[1],xcoord[2],ycoord[2],1,c);
				DrawLine(xcoord[2],ycoord[2],xcoord[3],ycoord[3],1,c);
				DrawLine(xcoord[0],ycoord[0],xcoord[3],ycoord[3],1,c);
			}
		} else {
			int  ymax=-1000000, ymin=1000000;
        	fill_set_pen_color((c>>16) & 0xFF, (c>>8) & 0xFF , c & 0xFF);
    		fill_set_fill_color((f>>16) & 0xFF, (f>>8) & 0xFF , f & 0xFF);
            fill_begin_fill();
			main_fill_poly_vertex_count=0;
			for(i=0;i<facecount;i++){
				main_fill_polyX[main_fill_poly_vertex_count] = (TFLOAT)xcoord[i];
				main_fill_polyY[main_fill_poly_vertex_count] = (TFLOAT)ycoord[i];
				if(main_fill_polyY[main_fill_poly_vertex_count]>ymax)ymax=main_fill_polyY[main_fill_poly_vertex_count];
				if(main_fill_polyY[main_fill_poly_vertex_count]<ymin)ymin=main_fill_polyY[main_fill_poly_vertex_count];
				main_fill_polyX[main_fill_poly_vertex_count] = (TFLOAT)xcoord[i];
				main_fill_poly_vertex_count++;
			 }
			 if(main_fill_polyY[main_fill_poly_vertex_count]!=main_fill_polyY[0] || main_fill_polyX[main_fill_poly_vertex_count] != main_fill_polyX[0]){
					main_fill_polyX[main_fill_poly_vertex_count]=main_fill_polyX[0];
					main_fill_polyY[main_fill_poly_vertex_count]=main_fill_polyY[0];
					main_fill_poly_vertex_count++;
			 }
			 fill_end_fill(main_fill_poly_vertex_count-1,ymin,ymax);
		}
	}

}


void Free3DMemory(int i){
	FreeMemorySafe((void *)&struct3d[i]->q_vertices);//array of original vertices
	FreeMemorySafe((void *)&struct3d[i]->r_vertices); //array of rotated vertices
	FreeMemorySafe((void *)&struct3d[i]->q_centroids);//array of original vertices
	FreeMemorySafe((void *)&struct3d[i]->r_centroids); //array of rotated vertices
	FreeMemorySafe((void *)&struct3d[i]->facecount); //number of vertices for each face
	FreeMemorySafe((void *)&struct3d[i]->facestart); //index into the face_x_vert table of the start of a given face
	FreeMemorySafe((void *)&struct3d[i]->fill); //fill colours
	FreeMemorySafe((void *)&struct3d[i]->line); //line colours
	FreeMemorySafe((void *)&struct3d[i]->colours);
	FreeMemorySafe((void *)&struct3d[i]->face_x_vert); //list of vertices for each face
	FreeMemorySafe((void *)&struct3d[i]->dots);
	FreeMemorySafe((void *)&struct3d[i]->depth);
	FreeMemorySafe((void *)&struct3d[i]->depthindex);
	FreeMemorySafe((void *)&struct3d[i]->normals);
	FreeMemorySafe((void *)&struct3d[i]->flags);
	FreeMemorySafe((void *)&struct3d[i]);
}
void closeall3d(void){
    int i;
    for(i = 0; i < MAX3D; i++) {
    	if(struct3d[i]!=NULL){
    		Free3DMemory(i);
    	}
    }
    for(i=1; i<4;i++){
    	camera[i].viewplane=-32767;
    }
}
void T_Mult(FLOAT3D *q1, FLOAT3D *q2, FLOAT3D *n){
    FLOAT3D a1=q1[0],a2=q2[0],b1=q1[1],b2=q2[1],c1=q1[2],c2=q2[2],d1=q1[3],d2=q2[3];
    n[0]=a1*a2-b1*b2-c1*c2-d1*d2;
    n[1]=a1*b2+b1*a2+c1*d2-d1*c2;
    n[2]=a1*c2-b1*d2+c1*a2+d1*b2;
    n[3]=a1*d2+b1*c2-c1*b2+d1*a2;
    n[4]=q1[4]*q2[4];
}

void T_Invert(FLOAT3D *q, FLOAT3D *n){
    n[0]=q[0];
    n[1]=-q[1];
    n[2]=-q[2];
    n[3]=-q[3];
    n[4]=q[4];
}

void depthsort(FLOAT3D *farray, int n, int *index){
    int i, j = n, s = 1;
    int t;
    FLOAT3D f;
	while (s) {
		s = 0;
		for (i = 1; i < j; i++) {
			if (farray[i] > farray[i - 1]) {
				f = farray[i];
				farray[i] = farray[i - 1];
				farray[i - 1] = f;
				s = 1;
				if(index!=NULL){
					t=index[i-1];
					index[i-1]=index[i];
					index[i]=t;
				}
			}
		}
		j--;
	}
}
void q_rotate(s_quaternion *in, s_quaternion rotate, s_quaternion *out){
//	PFlt(in->x);PFltComma(in->y);PFltComma(in->z);PFltComma(in->m);PRet();
	s_quaternion temp, qtemp;
	T_Mult((FLOAT3D *)&rotate, (FLOAT3D *)in, (FLOAT3D *)&temp);
	T_Invert((FLOAT3D *)&rotate, (FLOAT3D *)&qtemp);
	T_Mult((FLOAT3D *)&temp, (FLOAT3D *)&qtemp, (FLOAT3D *)out);
//	PFlt(out->x);PFltComma(out->y);PFltComma(out->z);PFltComma(out->m);PRet();
}
void normalise(s_vector *v){
	FLOAT3D n = sqrt3d((v->x) * (v->x) + (v->y) * (v->y) + (v->z) * (v->z) );
	v->x /= n;
	v->y /= n;
	v->z /= n;
}
void display3d(int n, FLOAT3D x, FLOAT3D y, FLOAT3D z, int clear, int nonormals){
	s_vector ray, lighting={0};
	s_vector p1, p2, p3, U, V;
	FLOAT3D x1, y1, z1, tmp;
	FLOAT3D at, bt, ct, t, /*A=0, B=0, */C=1, D=-camera[struct3d[n]->camera].viewplane;
	int maxH=VRes;
	int maxW=HRes;
	int vp, v, f, sortindex, csave=0, fsave=0;
	if(struct3d[n]->vmax>4){ //needed for polygon fill
		main_fill_polyX=(TFLOAT  *)GetMemory(struct3d[n]->tot_face_x_vert * sizeof(TFLOAT));
		main_fill_polyY=(TFLOAT  *)GetMemory(struct3d[n]->tot_face_x_vert * sizeof(TFLOAT));
	}
	if(struct3d[n]->xmin!=32767 && clear)DrawRectangle(struct3d[n]->xmin,struct3d[n]->ymin,struct3d[n]->xmax,struct3d[n]->ymax,0);
	struct3d[n]->xmin=32767;
	struct3d[n]->ymin=32767;
	struct3d[n]->xmax=-32767;
	struct3d[n]->ymax=-32767;
	short xcoord[MAX_POLYGON_VERTICES],ycoord[MAX_POLYGON_VERTICES];
	struct3d[n]->distance=0.0;
	for(f=0;f<struct3d[n]->nf;f++){
// calculate the surface normals for each face
		vp=struct3d[n]->facestart[f];
		p1.x=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+1]].x * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+1]].m + x;
		p1.y=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+1]].y  *struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+1]].m + y;
		p1.z=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+1]].z * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+1]].m + z;
		p2.x=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+2]].x * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+2]].m + x;
		p2.y=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+2]].y * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+2]].m + y;
		p2.z=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+2]].z * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+2]].m + z;
		p3.x=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp]].x * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp]].m + x;
		p3.y=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp]].y * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp]].m + y;
		p3.z=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp]].z * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp]].m + z;
		U.x=p2.x-p1.x;  U.y=p2.y-p1.y;  U.z=p2.z-p1.z;
		V.x=p3.x-p1.x;  V.y=p3.y-p1.y;  V.z=p3.z-p1.z;
		struct3d[n]->normals[f].x=U.y * V.z - U.z * V.y;
		struct3d[n]->normals[f].y=U.z * V.x - U.x * V.z;
		struct3d[n]->normals[f].z=U.x * V.y - U.y * V.x;
		normalise(&struct3d[n]->normals[f]);
		ray.x=p1.x - camera[struct3d[n]->camera].x;
		ray.y=p1.y - camera[struct3d[n]->camera].y;
		ray.z=p1.z - camera[struct3d[n]->camera].z;
		normalise(&ray);
		lighting.x=p1.x - struct3d[n]->light.x;
		lighting.y=p1.y - struct3d[n]->light.y;
		lighting.z=p1.z - struct3d[n]->light.z;
		normalise(&lighting);
		struct3d[n]->dots[f] = ray.x * struct3d[n]->normals[f].x + ray.y * struct3d[n]->normals[f].y + ray.z * struct3d[n]->normals[f].z;
		tmp=struct3d[n]->r_centroids[f].m;
		struct3d[n]->depth[f]=sqrt3d(
				(struct3d[n]->r_centroids[f].z * tmp + z - camera[struct3d[n]->camera].z) *
				(struct3d[n]->r_centroids[f].z * tmp + z - camera[struct3d[n]->camera].z) +
				(struct3d[n]->r_centroids[f].y * tmp + y - camera[struct3d[n]->camera].y) *
				(struct3d[n]->r_centroids[f].y * tmp + y - camera[struct3d[n]->camera].y) +
				(struct3d[n]->r_centroids[f].x * tmp + x - camera[struct3d[n]->camera].x) *
				(struct3d[n]->r_centroids[f].x * tmp + x - camera[struct3d[n]->camera].x)
				);
		struct3d[n]->depthindex[f]=f;
		struct3d[n]->distance+=struct3d[n]->depth[f];
	}
	struct3d[n]->distance/=f;
	// sort the distances from the faces to the camera
	depthsort(struct3d[n]->depth, struct3d[n]->nf, struct3d[n]->depthindex);
	// display the forward facing faces in the order of the furthest away first
	for(f=0;f<struct3d[n]->nf;f++){
		sortindex=struct3d[n]->depthindex[f];
		vp=struct3d[n]->facestart[sortindex];
		if(struct3d[n]->flags[sortindex] & 4)struct3d[n]->dots[sortindex]=-struct3d[n]->dots[sortindex];
		if(nonormals || struct3d[n]->dots[sortindex]<0){
			for(v=0;v<struct3d[n]->facecount[sortindex];v++){
				x1=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+v]].x * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+v]].m + x;
				y1=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+v]].y * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+v]].m + y;
				z1=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+v]].z * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+v]].m + z;
// We now have the coordinates in real space so project them
				at=x1-camera[struct3d[n]->camera].x;
				bt=y1-camera[struct3d[n]->camera].y;
				ct=z1-camera[struct3d[n]->camera].z;
				t=-(/*A * x1 + B * y1*/ + C * z1 + D)/(/*A * at + B * bt + */C *ct);
				xcoord[v]=x1+round3d(at*t)+(maxW>>1)-camera[struct3d[n]->camera].x-camera[struct3d[n]->camera].panx;
				ycoord[v]=maxH-round3d(y1+bt*t)-1;
				ycoord[v]-=(maxH>>1)-camera[struct3d[n]->camera].y-camera[struct3d[n]->camera].pany;
				if(clear){
					if(xcoord[v]>struct3d[n]->xmax)struct3d[n]->xmax=xcoord[v];
					if(xcoord[v]<struct3d[n]->xmin)struct3d[n]->xmin=xcoord[v];
					if(ycoord[v]>struct3d[n]->ymax)struct3d[n]->ymax=ycoord[v];
					if(ycoord[v]<struct3d[n]->ymin)struct3d[n]->ymin=ycoord[v];
				}
			}
			if((struct3d[n]->flags[sortindex] & 1) == 0) {
				if(struct3d[n]->flags[sortindex] & 10) {
					fsave=struct3d[n]->fill[sortindex];
					csave=struct3d[n]->line[sortindex];
					if(struct3d[n]->flags[sortindex] & 2)struct3d[n]->fill[sortindex]=0xFF0000;
					if(struct3d[n]->flags[sortindex] & 8){
						FLOAT3D lightratio=fabs3d(lighting.x * struct3d[n]->normals[sortindex].x + lighting.y * struct3d[n]->normals[sortindex].y + lighting.z * struct3d[n]->normals[sortindex].z);
						lightratio=(lightratio*struct3d[n]->ambient)+struct3d[n]->ambient;
						int red=(struct3d[n]->fill[sortindex] & 0xFF0000)>>16;
						int green=(struct3d[n]->fill[sortindex] & 0xFF00)>>8;
						int blue=(struct3d[n]->fill[sortindex] & 0xFF);
						red=(round3d)((FLOAT3D)red*lightratio);
						green=(round3d)((FLOAT3D)green*lightratio);
						blue=(round3d)((FLOAT3D)blue*lightratio);
						struct3d[n]->fill[sortindex]=(red<<16) | (green<<8) | blue;
						red=(struct3d[n]->line[sortindex] & 0xFF0000)>>16;
						green=(struct3d[n]->line[sortindex] & 0xFF00)>>8;
						blue=(struct3d[n]->line[sortindex] & 0xFF);
						red=(round3d)((FLOAT3D)red*lightratio);
						green=(round3d)((FLOAT3D)green*lightratio);
						blue=(round3d)((FLOAT3D)blue*lightratio);
						struct3d[n]->line[sortindex]=(red<<16) | (green<<8) | blue;
					}
				}
				DrawPolygon(n, xcoord, ycoord, sortindex);
				if(struct3d[n]->flags[sortindex] & 10){
					struct3d[n]->fill[sortindex]=fsave;
					struct3d[n]->line[sortindex]=csave;
				}
			}
		}
	}
	// Save information about how it was displayed for DRAW3D function and RESTORE command
	struct3d[n]->current.x=x;
	struct3d[n]->current.y=y;
	struct3d[n]->current.z=z;
	struct3d[n]->nonormals=nonormals;
	if(struct3d[n]->vmax>4){ //needed for polygon fill
		FreeMemory((unsigned char *)main_fill_polyX);
		FreeMemory((unsigned char *)main_fill_polyY);
	}

}

void diagnose3d(int n, FLOAT3D x, FLOAT3D y, FLOAT3D z, int sort){
	s_vector ray, normals;
	s_vector p1, p2, p3, U, V;
	FLOAT3D tmp;
	int vp, f, sortindex;
	for(f=0;f<struct3d[n]->nf;f++){
// calculate the surface normals for each face
		vp=struct3d[n]->facestart[f];
		p1.x=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+1]].x * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+1]].m + x;
		p1.y=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+1]].y  *struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+1]].m + y;
		p1.z=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+1]].z * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+1]].m + z;
		p2.x=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+2]].x * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+2]].m + x;
		p2.y=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+2]].y * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+2]].m + y;
		p2.z=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+2]].z * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp+2]].m + z;
		p3.x=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp]].x * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp]].m + x;
		p3.y=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp]].y * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp]].m + y;
		p3.z=struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp]].z * struct3d[n]->r_vertices[struct3d[n]->face_x_vert[vp]].m + z;
		U.x=p2.x-p1.x;  U.y=p2.y-p1.y;  U.z=p2.z-p1.z;
		V.x=p3.x-p1.x;  V.y=p3.y-p1.y;  V.z=p3.z-p1.z;
		normals.x=U.y * V.z - U.z * V.y;
		normals.y=U.z * V.x - U.x * V.z;
		normals.z=U.x * V.y - U.y * V.x;
		normalise(&normals);
		ray.x=p1.x - camera[struct3d[n]->camera].x;
		ray.y=p1.y - camera[struct3d[n]->camera].y;
		ray.z=p1.z/*  -camera[struct3d[n]->camera].z*/;
		normalise(&ray);
		struct3d[n]->dots[f] = ray.x * normals.x + ray.y * normals.y + ray.z * normals.z;
		tmp=struct3d[n]->r_centroids[f].m;
		struct3d[n]->depth[f]=sqrt3d(
				(struct3d[n]->r_centroids[f].z * tmp + z - camera[struct3d[n]->camera].z) *
				(struct3d[n]->r_centroids[f].z * tmp + z - camera[struct3d[n]->camera].z) +
				(struct3d[n]->r_centroids[f].y * tmp + y - camera[struct3d[n]->camera].y) *
				(struct3d[n]->r_centroids[f].y * tmp + y - camera[struct3d[n]->camera].y) +
				(struct3d[n]->r_centroids[f].x * tmp + x - camera[struct3d[n]->camera].x) *
				(struct3d[n]->r_centroids[f].x * tmp + x - camera[struct3d[n]->camera].x)
				);
		struct3d[n]->depthindex[f]=f;
	}
	// sort the dot products
	depthsort(struct3d[n]->depth, struct3d[n]->nf, struct3d[n]->depthindex);
	// display the forward facing faces in the order of the furthest away first
	for(f=0;f<struct3d[n]->nf;f++){
		if(sort)sortindex=struct3d[n]->depthindex[f];
		else sortindex=f;
		vp=struct3d[n]->facestart[sortindex];
		MMPrintString("Face ");PInt(sortindex);
		MMPrintString(" at distance ");PFlt(struct3d[n]->depth[f]);
		MMPrintString(" dot product is ");PFlt(struct3d[n]->dots[sortindex]);
		MMPrintString(" so the face is ");MMPrintString(struct3d[n]->dots[sortindex]>0 ? "Hidden" : "Showing");PRet();
	}
}
void cmd_3D(void){
	unsigned char *p;
	if((p=checkstring(cmdline, "CREATE"))) {
	   // parameters are
		// 3D object number (1 to MAX3D
		// # of vertices = nv
		// # of faces = nf
		// vertex structure (nv)
		// face array (face number, vertex number)
		// colours array
		// edge colour index array [nf]
		// fill colour index array [nf]
		// centroid structure [nf]
		// normals structure [nf]
		MMFLOAT *vertex;
		TFLOAT tmp;
		long long int *faces, *facecount, *facecountindex, *colours, *linecolour=NULL, *fillcolour=NULL;
		getargs(&p,19,",");
		if(argc<17)error("Argument count");
		int c, colourcount=0, vp, v, f, fc=0, n=getint(argv[0],1,MAX3D);
		if(struct3d[n]!=NULL)error("Object already exists");
		int nv=getinteger(argv[2]);
		if(nv<3)error("3D object must have a minimum of 3 vertices");
		int nf=getinteger(argv[4]);
		if(nf<1)error("3D object must have a minimum of 1 face");
		int cam=getint(argv[6],1,MAXCAM);
		vertex = (MMFLOAT *)findvar(argv[8], V_FIND | V_EMPTY_OK | V_NOFIND_ERR);
		if((uint32_t)vertex!=(uint32_t)vartbl[VarIndex].val.s)error("Vertex array must be a 2D floating point array");
		if(vartbl[VarIndex].type & T_NBR) {
			if(vartbl[VarIndex].dims[2] != 0) error("Vertex array must be a 2D floating point array");
			if(vartbl[VarIndex].dims[0] - OptionBase!= 2) {		// Not an array
				error("Vertex array must have 3 elements in first dimension");
			}
			if(vartbl[VarIndex].dims[1] - OptionBase < nv-1) {		// Not an array
				error("Vertex array too small");
			}
		} else error("Vertex array must be a 2D floating point array");

		facecount = (long long int *)findvar(argv[10], V_FIND | V_EMPTY_OK | V_NOFIND_ERR);
		if((uint32_t)facecount!=(uint32_t)vartbl[VarIndex].val.s)error("Vertex count array must be a 1D integer array");
		if(vartbl[VarIndex].type & T_INT) {
			if(vartbl[VarIndex].dims[1] != 0) error("Vertex count array must be a 1D integer array");
			if(vartbl[VarIndex].dims[0] - OptionBase< nf-1) {		// Not an array
				error("Vertex count array too small");
			}
		} else error("Vertex count array must be a 1D integer array");
		facecountindex=facecount;
		for(f=0;f<nf;f++)fc += (*facecountindex++);

		faces = (long long int *)findvar(argv[12], V_FIND | V_EMPTY_OK | V_NOFIND_ERR);
		if((uint32_t)faces!=(uint32_t)vartbl[VarIndex].val.s)error("Face/vertex array must be a 1D integer array");
		if(vartbl[VarIndex].type & T_INT) {
			if(vartbl[VarIndex].dims[1] != 0) error("Face/vertex array must be a 1D integer array");
			if(vartbl[VarIndex].dims[0] - OptionBase< fc-1) {		// Not an array
				error("Face/vertex array too small");
			}
		} else error("Face/vertex array must be a 1D integer array");

		colours = (long long int *)findvar(argv[14], V_FIND | V_EMPTY_OK | V_NOFIND_ERR);
		if((uint32_t)colours!=(uint32_t)vartbl[VarIndex].val.s)error("Colour array must be a 1D integer array");
		if(vartbl[VarIndex].type & T_INT) {
			if(vartbl[VarIndex].dims[1] != 0) error("Colour array must be a 1D integer array");
			colourcount=vartbl[VarIndex].dims[0] - OptionBase + 1;
		} else error("Colour array must be a 1D integer array");


		if(argc>=17 && *argv[16]){
			linecolour = (long long int *)findvar(argv[16], V_FIND | V_EMPTY_OK | V_NOFIND_ERR);
			if((uint32_t)linecolour!=(uint32_t)vartbl[VarIndex].val.s)error("Line colour array must be a 1D integer array");
			if(vartbl[VarIndex].type & T_INT) {
				if(vartbl[VarIndex].dims[1] != 0) error("Line colour  array must be a 1D integer array");
				if(vartbl[VarIndex].dims[0] - OptionBase< nf-1) {		// Not an array
					error("Line colour  array too small");
				}
			} else error("Line colour must be a 1D integer array");
		}

		if(argc==19){
			fillcolour = (long long int *)findvar(argv[18], V_FIND | V_EMPTY_OK | V_NOFIND_ERR);
			if((uint32_t)fillcolour!=(uint32_t)vartbl[VarIndex].val.s)error("Fill colour array must be a 1D integer array");
			if(vartbl[VarIndex].type & T_INT) {
				if(vartbl[VarIndex].dims[1] != 0) error("Fill colour array must be a 1D integer array");
				if(vartbl[VarIndex].dims[0] - OptionBase< nf-1) {		// Not an array
					error("Fill colour array too small");
				}
			} else error("Fill colour must be a 1D integer array");
		}
		// The data look valid so now create the object in memory
		struct3d[n]=GetMemory(sizeof(struct D3D));
		struct3d[n]->nf=nf;
		struct3d[n]->nv=nv;
		struct3d[n]->current.x=-32767;
		struct3d[n]->current.y=-32767;
		struct3d[n]->current.z=-32767;
		struct3d[n]->xmin=32767;
		struct3d[n]->ymin=32767;
		struct3d[n]->xmax=-32767;
		struct3d[n]->ymax=-32767;
		struct3d[n]->camera=cam;
		struct3d[n]->q_vertices=NULL;//array of original vertices
		struct3d[n]->r_vertices=NULL; //array of rotated vertices
		struct3d[n]->q_centroids=NULL;//array of original vertices
		struct3d[n]->r_centroids=NULL; //array of rotated vertices
		struct3d[n]->facecount=NULL; //number of vertices for each face
		struct3d[n]->facestart=NULL; //index into the face_x_vert table of the start of a given face
		struct3d[n]->fill=NULL; //fill colours
		struct3d[n]->line=NULL; //line colours
		struct3d[n]->colours=NULL;
		struct3d[n]->face_x_vert=NULL; //list of vertices for each face
		struct3d[n]->light.x=0;
		struct3d[n]->light.y=0;
		struct3d[n]->light.z=0;
		struct3d[n]->ambient=0;
		// load up things that have one entry per vertex
		struct3d[n]->q_vertices=GetMemory(struct3d[n]->nv * sizeof(struct t_quaternion));
		struct3d[n]->r_vertices=GetMemory(struct3d[n]->nv * sizeof(struct t_quaternion));
		for(v=0;v<struct3d[n]->nv;v++){
			FLOAT3D m=0.0;
			struct3d[n]->q_vertices[v].x=(FLOAT3D)(*vertex++);
			m+=struct3d[n]->q_vertices[v].x*struct3d[n]->q_vertices[v].x;
			struct3d[n]->q_vertices[v].y=*vertex++;
			m+=struct3d[n]->q_vertices[v].y*struct3d[n]->q_vertices[v].y;
			struct3d[n]->q_vertices[v].z=*vertex++;
			m+=struct3d[n]->q_vertices[v].z*struct3d[n]->q_vertices[v].z;
			if(m){
				m=sqrt(m);
				struct3d[n]->q_vertices[v].x=struct3d[n]->q_vertices[v].x/m;
				struct3d[n]->q_vertices[v].y=struct3d[n]->q_vertices[v].y/m;
				struct3d[n]->q_vertices[v].z=struct3d[n]->q_vertices[v].z/m;
				struct3d[n]->q_vertices[v].w=0.0;
				struct3d[n]->q_vertices[v].m=m;
			} else {
				struct3d[n]->q_vertices[v].x=0;
				struct3d[n]->q_vertices[v].y=0;
				struct3d[n]->q_vertices[v].z=0;
				struct3d[n]->q_vertices[v].w=0.0;
				struct3d[n]->q_vertices[v].m=1.0;
			}
			memcpy(&struct3d[n]->r_vertices[v],&struct3d[n]->q_vertices[v], sizeof(s_quaternion));
		}
		struct3d[n]->tot_face_x_vert=0;
		//load up things that have one entry per face
		struct3d[n]->vmax=0;
		struct3d[n]->facecount=GetMemory(struct3d[n]->nf * sizeof(uint16_t));
		struct3d[n]->facestart=GetMemory(struct3d[n]->nf * sizeof(uint16_t));
		struct3d[n]->fill=GetMemory(struct3d[n]->nf * sizeof(uint32_t));
		struct3d[n]->line=GetMemory(struct3d[n]->nf * sizeof(uint32_t));
		struct3d[n]->r_centroids=GetMemory(struct3d[n]->nf * sizeof(struct t_quaternion));
		struct3d[n]->q_centroids=GetMemory(struct3d[n]->nf * sizeof(struct t_quaternion));
		struct3d[n]->dots=GetMemory(struct3d[n]->nf * sizeof(MMFLOAT));
		struct3d[n]->depth=GetMemory(struct3d[n]->nf * sizeof(MMFLOAT));
		struct3d[n]->flags=GetMemory(struct3d[n]->nf * sizeof(uint8_t));
		struct3d[n]->depthindex=GetMemory(struct3d[n]->nf * sizeof(int));
		struct3d[n]->normals=GetMemory(struct3d[n]->nf * sizeof(struct SVD));
		for(f=0;f<struct3d[n]->nf;f++){
			struct3d[n]->facecount[f]=*facecount++;
			if(struct3d[n]->facecount[f]<3){
				Free3DMemory(n);
				error("Vertex count less than 3 for face %",f+OptionBase);
			}
			if(struct3d[n]->facecount[f]>struct3d[n]->vmax)struct3d[n]->vmax=struct3d[n]->facecount[f];
			struct3d[n]->facestart[f]=struct3d[n]->tot_face_x_vert;
			struct3d[n]->tot_face_x_vert+=struct3d[n]->facecount[f];
		}
		// load up the array that holds all the face vertex information
		struct3d[n]->face_x_vert=GetMemory(struct3d[n]->tot_face_x_vert * sizeof(uint16_t)); // allocate memory for the list of vertices per face
		struct3d[n]->colours=GetMemory(colourcount * sizeof(uint32_t));
		for(c=0; c<colourcount;c++){
			struct3d[n]->colours[c]=(uint32_t)*colours++;
		}
		for(f=0;f<struct3d[n]->tot_face_x_vert;f++){
			struct3d[n]->face_x_vert[f]=*faces++;
		}
		for(f=0;f<struct3d[n]->nf;f++){
			if(linecolour!=NULL){
				int index=(*linecolour++) - OptionBase;
				if(index>=colourcount || index<0){
					Free3DMemory(n);
					error("Edge colour Index %",index);
				}
				struct3d[n]->line[f]=struct3d[n]->colours[index];
			} else struct3d[n]->line[f]=gui_fcolour;
			if(fillcolour!=NULL){
				int index=(*fillcolour++) - OptionBase;
				if(index>=colourcount || index<0){
					Free3DMemory(n);
					error("Fill colour Index %",index);
				}
				struct3d[n]->fill[f]=struct3d[n]->colours[index];
			} else struct3d[n]->fill[f]=0xFFFFFFFF;
			FLOAT3D x=0, y=0, z=0, scale;
			vp=struct3d[n]->facestart[f];
// calculate the centroids of each face

			for(v=0;v<struct3d[n]->facecount[f];v++){
				tmp=struct3d[n]->q_vertices[struct3d[n]->face_x_vert[vp+v]].m;
				x+=struct3d[n]->q_vertices[struct3d[n]->face_x_vert[vp+v]].x*tmp;
				y+=struct3d[n]->q_vertices[struct3d[n]->face_x_vert[vp+v]].y*tmp;
				z+=struct3d[n]->q_vertices[struct3d[n]->face_x_vert[vp+v]].z*tmp;
			}
			x/=(FLOAT3D)struct3d[n]->facecount[f];
			y/=(FLOAT3D)struct3d[n]->facecount[f];
			z/=(FLOAT3D)struct3d[n]->facecount[f];
			struct3d[n]->q_centroids[f].x=x;
			struct3d[n]->q_centroids[f].y=y;
			struct3d[n]->q_centroids[f].z=z;
			scale=sqrt(struct3d[n]->q_centroids[f].x*struct3d[n]->q_centroids[f].x +
					struct3d[n]->q_centroids[f].y*struct3d[n]->q_centroids[f].y +
					struct3d[n]->q_centroids[f].z*struct3d[n]->q_centroids[f].z);
			struct3d[n]->q_centroids[f].x/=scale;
			struct3d[n]->q_centroids[f].y/=scale;
			struct3d[n]->q_centroids[f].z/=scale;
			struct3d[n]->q_centroids[f].m=scale;
			struct3d[n]->q_centroids[f].w=0;
			memcpy(&struct3d[n]->r_centroids[f],&struct3d[n]->q_centroids[f], sizeof(s_quaternion));
			}
		return;
	} else if((p=checkstring(cmdline, "DIAGNOSE"))) {
		getargs(&p,9,",");
		if(argc<7)error("Argument count");
		int n=getint(argv[0],1,MAX3D);
		int x=getint(argv[2],-32766,32766);
		int y=getint(argv[4],-32766,32766);
		int z=getinteger(argv[6]);
		int sort=1;
		if(argc==9)sort=getint(argv[8],0,1);
		if(struct3d[n]==NULL)error("Object % does not exist",n);
		if(camera[struct3d[n]->camera].viewplane==-32767)error("Camera position not defined");
		diagnose3d(n, x, y, z, sort);
		return;
	} else if((p=checkstring(cmdline, "LIGHT"))) {
		getargs(&p,9,",");
		if(argc!=9)error("Argument count");
		int n=getint(argv[0],1,MAX3D);
		struct3d[n]->light.x=getint(argv[2],-32766,32766);
		struct3d[n]->light.y=getint(argv[4],-32766,32766);
		struct3d[n]->light.z=getint(argv[6],-32766,32766);
		struct3d[n]->ambient=(FLOAT3D)(getint(argv[8],0,100))/100.0;
		return;
	} else if((p=checkstring(cmdline, "SHOW"))) {
		getargs(&p,9,",");
		if(argc<7)error("Argument count");
		int n=getint(argv[0],1,MAX3D);
		int x=getint(argv[2],-32766,32766);
		int y=getint(argv[4],-32766,32766);
		int z=getinteger(argv[6]);
		int nonormals=0;
		if(argc==9)nonormals=getint(argv[8],0,1);
		if(struct3d[n]==NULL)error("Object % does not exist",n);
		if(camera[struct3d[n]->camera].viewplane==-32767)error("Camera position not defined");
		display3d(n, x, y, z, 1, nonormals);
		return;
	} else if((p=checkstring(cmdline, "SET FLAGS"))) {
		int i, face, nbr;
		getargs(&p, ((MAX_ARG_COUNT-1) * 2) - 1, ",");
		if((argc & 0b11) != 0b11) error("Invalid syntax");
		int n=getint(argv[0],1,MAX3D);
		int flag=getint(argv[2],0,255);
	    // step over the equals sign and get the value for the assignment
	    for(i = 4; i < argc; i += 4) {
	        face = getinteger(argv[i]);
	        nbr = getinteger(argv[i + 2]);

	        if(nbr <= 0 || nbr>struct3d[n]->nf-face) error("Invalid argument");

	        while(--nbr>=0) {
	        	struct3d[n]->flags[face+nbr]=flag;
	        }
	    }
	} else if((p=checkstring(cmdline, "ROTATE"))) {
		void *ptr1 = NULL;
		int i, n, v, f;
		s_quaternion q1;
		MMFLOAT *q=NULL;
		getargs(&p, (MAX_ARG_COUNT * 2) - 1, ",");				// getargs macro must be the first executable stmt in a block
		if((argc & 0x01 || argc<3) == 0) error("Argument count");
		ptr1 = findvar(argv[0], V_FIND | V_EMPTY_OK | V_NOFIND_ERR);
		if(vartbl[VarIndex].type & T_NBR) {
			if(vartbl[VarIndex].dims[1] != 0) error("Invalid variable");
			if(vartbl[VarIndex].dims[0] <= 0) {		// Not an array
				error("Argument 1 must be a 5 element floating point array");
			}
			if(vartbl[VarIndex].dims[0] - OptionBase!=4)error("Argument 1 must be a 5 element floating point array");
			q = (MMFLOAT *)ptr1;
			if((uint32_t)ptr1!=(uint32_t)vartbl[VarIndex].val.s)error("Syntax");
		} else error("Argument 1 must be a 5 element floating point array");
		q1.w=(FLOAT3D)(*q++);
		q1.x=(FLOAT3D)(*q++);
		q1.y=(FLOAT3D)(*q++);
		q1.z=(FLOAT3D)(*q++);
		q1.m=(FLOAT3D)(*q);
		for(i = 2; i < argc; i += 2) {
			n=getint(argv[i],1,MAX3D);
			if(struct3d[n]==NULL)error("Object % does not exist",n);
			for(v=0;v<struct3d[n]->nv;v++){
				q_rotate(&struct3d[n]->q_vertices[v],q1,&struct3d[n]->r_vertices[v]);
			}
			for(f=0;f<struct3d[n]->nf;f++){
				q_rotate(&struct3d[n]->q_centroids[f],q1,&struct3d[n]->r_centroids[f]);
			}
		}
		return;
	} else if((p=checkstring(cmdline, "HIDE ALL"))) {
		for(int i=1;i<=MAX3D;i++){
			if(struct3d[i]!=NULL && struct3d[i]->xmin!=32767){
				DrawRectangle(struct3d[i]->xmin,struct3d[i]->ymin,struct3d[i]->xmax,struct3d[i]->ymax,0);
				struct3d[i]->xmin=32767;
				struct3d[i]->ymin=32767;
				struct3d[i]->xmax=-32767;
				struct3d[i]->ymax=-32767;
			}
		}
		return;
	} else if((p=checkstring(cmdline, "RESET"))) {
		int i, n;
		int v, f;
		getargs(&p, (MAX_ARG_COUNT * 2) - 1, ",");				// getargs macro must be the first executable stmt in a block
		if((argc & 0x01 || argc<3) == 0) error("Argument count");
		for(i = 0; i < argc; i += 2) {
			n=getint(argv[i],1,MAX3D);
			for(v=0;v<struct3d[n]->nv;v++){
				memcpy(&struct3d[n]->q_vertices[v],&struct3d[n]->r_vertices[v], sizeof(s_quaternion));
			}
			for(f=0;f<struct3d[n]->nf;f++){
				memcpy(&struct3d[n]->q_centroids[f],&struct3d[n]->r_centroids[f], sizeof(s_quaternion));
			}
		}
		return;
	} else if((p=checkstring(cmdline, "HIDE"))) {
		int i, n;
		getargs(&p, (MAX_ARG_COUNT * 2) - 1, ",");				// getargs macro must be the first executable stmt in a block
		if((argc & 0x01 || argc<3) == 0) error("Argument count");
		for(i = 0; i < argc; i += 2) {
			n=getint(argv[i],1,MAX3D);
			if(struct3d[n]==NULL)error("Object % does not exist",n);
			if(struct3d[n]->xmin==32767)return;
			DrawRectangle(struct3d[n]->xmin,struct3d[n]->ymin,struct3d[n]->xmax,struct3d[n]->ymax,0);
			struct3d[n]->xmin=32767;
			struct3d[n]->ymin=32767;
			struct3d[n]->xmax=-32767;
			struct3d[n]->ymax=-32767;
		}
		return;
	} else if((p=checkstring(cmdline, "RESTORE"))) {
		int i, n;
		getargs(&p, (MAX_ARG_COUNT * 2) - 1, ",");				// getargs macro must be the first executable stmt in a block
		if((argc & 0x01 || argc<3) == 0) error("Argument count");
		for(i = 0; i < argc; i += 2) {
			n=getint(argv[i],1,MAX3D);
			if(struct3d[n]==NULL)error("Object % does not exist",n);
			if(struct3d[n]->xmin!=32767)error("Object % is not hidden",n);
			display3d(n, struct3d[n]->current.x, struct3d[n]->current.y, struct3d[n]->current.z, 1, struct3d[n]->nonormals);
		}
		return;
	} else if((p=checkstring(cmdline, "WRITE"))) {
		getargs(&p,9,",");
		if(argc<7)error("Argument count");
		int n=getint(argv[0],1,MAX3D);
		int x=getint(argv[2],-32766,32766);
		int y=getint(argv[4],-32766,32766);
		int z=getinteger(argv[6]);
		int nonormals=0;
		if(argc==9)nonormals=getint(argv[8],0,1);
		if(struct3d[n]==NULL)error("Object % does not exist",n);
		if(camera[struct3d[n]->camera].viewplane==-32767)error("Camera position not defined");
		display3d(n, x, y, z, 0, nonormals);
		return;
	} else if((p=checkstring(cmdline, "CLOSE ALL"))) {
		closeall3d();
		return;
	} else if((p=checkstring(cmdline, "CLOSE"))) {
		int i, n;
		getargs(&p, (MAX_ARG_COUNT * 2) - 1, ",");				// getargs macro must be the first executable stmt in a block
		if((argc & 0x01 || argc<3) == 0) error("Argument count");
		for(i = 0; i < argc; i += 2) {
			n=getint(argv[i],1,MAX3D);
			if(struct3d[n]==NULL)error("Object % does not exist",n);
			if(struct3d[n]->xmin!=32767)DrawRectangle(struct3d[n]->xmin,struct3d[n]->ymin,struct3d[n]->xmax,struct3d[n]->ymax,0);
			Free3DMemory(n);
		}
		return;
	} else if((p=checkstring(cmdline, "CAMERA"))) {
		getargs(&p,11,",");
		if(argc<3)error("Argument count");
		int n=getint(argv[0],1,MAXCAM);
		camera[n].viewplane=getnumber(argv[2]);
		camera[n].x=(FLOAT3D)0;
		camera[n].y=(FLOAT3D)0;
		camera[n].panx=(FLOAT3D)0;
		camera[n].pany=(FLOAT3D)0;
		camera[n].z=0.0;
		if(argc>=5 && *argv[4])	camera[n].x=getnumber(argv[4]);
		if(camera[n].x > 32766 || camera[n].x < -32766 )error("Valid is -32766 to 32766");
		if(argc>=7 && *argv[6])	camera[n].y=getnumber(argv[6]);
		if(camera[n].y > 32766 || camera[n].x < -32766 )error("Valid is -32766 to 32766");
		if(argc>=9 && *argv[8])	camera[n].panx=getint(argv[8],-32766-camera[n].x,32766-camera[n].x);
		if(argc==11 )camera[n].pany=getint(argv[10],-32766-camera[n].y,32766-camera[n].y);
		return;
	} else {
		error("Syntax");
	}
}
void fun_3D(void){
	unsigned char *p;
	if((p=checkstring(ep, "XMIN"))) {
    	getargs(&p,1,",");
    	int n=getint(argv[0],1,MAX3D);
		if(struct3d[n]==NULL)error("Object does not exist");
    	fret=struct3d[n]->xmin;
	} else if((p=checkstring(ep, "XMAX"))) {
    	getargs(&p,1,",");
    	int n=getint(argv[0],1,MAX3D);
		if(struct3d[n]==NULL)error("Object does not exist");
    	fret=struct3d[n]->xmax;
	} else if((p=checkstring(ep, "YMIN"))) {
    	getargs(&p,1,",");
    	int n=getint(argv[0],1,MAX3D);
		if(struct3d[n]==NULL)error("Object does not exist");
    	fret=struct3d[n]->ymin;
	} else if((p=checkstring(ep, "YMAX"))) {
    	getargs(&p,1,",");
    	int n=getint(argv[0],1,MAX3D);
		if(struct3d[n]==NULL)error("Object does not exist");
    	fret=struct3d[n]->ymax;
	} else if((p=checkstring(ep, "X"))) {
    	getargs(&p,1,",");
    	int n=getint(argv[0],1,MAX3D);
		if(struct3d[n]==NULL)error("Object does not exist");
    	fret=struct3d[n]->current.x;
	} else if((p=checkstring(ep, "Y"))) {
    	getargs(&p,1,",");
    	int n=getint(argv[0],1,MAX3D);
		if(struct3d[n]==NULL)error("Object does not exist");
		fret=struct3d[n]->current.y;
	} else if((p=checkstring(ep, "DISTANCE"))) {
    	getargs(&p,1,",");
    	int n=getint(argv[0],1,MAX3D);
		if(struct3d[n]==NULL)error("Object does not exist");
    	fret=struct3d[n]->distance;
	} else if((p=checkstring(ep, "Z"))) {
    	getargs(&p,1,",");
    	int n=getint(argv[0],1,MAX3D);
		if(struct3d[n]==NULL)error("Object does not exist");
    	fret=struct3d[n]->current.z;
	} else error("Syntax");
	targ=T_NBR;
}
void closeframebuffer(void){
    if(FrameBuf!=DisplayBuf)FreeMemory(FrameBuf);
    if(DisplayBuf!=LayerBuf){
        unsigned char *temp= LayerBuf;
        LayerBuf=DisplayBuf;
        FreeMemory(temp);
    }
    FrameBuf=DisplayBuf;
    WriteBuf=DisplayBuf;
}
void cmd_framebuffer(void){
    unsigned char *p;
    if((p=checkstring(cmdline, "CREATE"))) {
        if(FrameBuf==DisplayBuf){
            FrameBuf=GetMemory(38400);
            if(checkstring(p, "R"))memcpy(FrameBuf,DisplayBuf,38400);
        }
        else error("Framebuffer already exists");
    } else if((p=checkstring(cmdline, "WRITE"))) {
        if(checkstring(p, "N"))WriteBuf=DisplayBuf;
        else if(checkstring(p, "L"))WriteBuf=LayerBuf;
        else if(checkstring(p, "F"))WriteBuf=FrameBuf;
        else error("Syntax");
    } else if((p=checkstring(cmdline, "WAIT"))) {
            while(QVgaScanLine!=480){}
    } else if((p=checkstring(cmdline, "LAYER"))) {
        if(Option.CPU_Speed!=252000)error("OPTION CPUSPEED 252000 required for layers");
        if(LayerBuf==DisplayBuf){
            LayerBuf=GetMemory(38400);
        } else error("Layer already exists");
    } else if((p=checkstring(cmdline, "CLOSE"))) {
        if(checkstring(p, "F")){
            if(WriteBuf==FrameBuf)WriteBuf=DisplayBuf;
            if(FrameBuf!=DisplayBuf)FreeMemory(FrameBuf);
            FrameBuf=DisplayBuf;
        } else if(checkstring(p, "L")){
            if(WriteBuf==LayerBuf)WriteBuf=DisplayBuf;
            if(DisplayBuf!=LayerBuf){
                unsigned char *temp= LayerBuf;
                LayerBuf=DisplayBuf;
                FreeMemory(temp);
            }
        } else  closeframebuffer();
    } else if((p=checkstring(cmdline, "COPY"))) {
        getargs(&p,5,",");
        if(!(argc==3 || argc==5))error("Syntax");
        uint8_t *s,*d;
        if(checkstring(argv[0],"N"))s=DisplayBuf;
        else if(checkstring(argv[0],"L"))s=LayerBuf;
        else if(checkstring(argv[0],"F"))s=FrameBuf;
        else error("Syntax");
        if(checkstring(argv[2],"N"))d=DisplayBuf;
        else if(checkstring(argv[2],"L"))d=LayerBuf;
        else if(checkstring(argv[2],"F"))d=FrameBuf;
        else error("Syntax");
        if(argc==5){
            if(checkstring(argv[4],"B"))while(QVgaScanLine!=480){}
            else error("Syntax");
        }
        if(d!=s)memcpy(d,s,38400);
    } else error("Syntax");
}
#endif