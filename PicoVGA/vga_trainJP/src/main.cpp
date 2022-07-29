
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "include.h"

// JOYPAD pushbutton to Pico pin mounting
#define PB_UL 16		// Upper Left pushbutton
#define PB_RT 17		// Right direction
#define PB_FR 18		// Fire
#define PB_LT 20		// Left direction
#define PB_CF 21		// Center Fire
#define PB_DN 22		// Down direction
#define PB_UR 26		// Upper right
#define PB_UP 27		// Up direction

u8 Text[TEXTMAX*2]; // 2 rows of text (2nd row is empty, it is used to center line vertically)
u8 TextCol[TEXTMAX*2*8] __attribute__ ((aligned(4))); // text color gradient

// copy of tiles images
u8 TilesImg_Copy[sizeof(TilesImg)] __attribute__ ((aligned(4)));

// copy of font
u8 Font_Copy[2048] __attribute__ ((aligned(4)));

// info box
u8 Box[BOXSIZE] __attribute__ ((aligned(4)));
sCanvas BoxCanvas; // box canvas

// initialize videomode
void VideoInit()
{
	// setup videomode
	VgaCfgDef(&Cfg); // get default configuration
	Cfg.video = &VideoVGA; // video timings
	Cfg.width = WIDTH; // screen width
	Cfg.height = HEIGHT; // screen height
	Cfg.mode[INFO_LAYER] = LAYERMODE_COLOR; // info box
	VgaCfg(&Cfg, &Vmode); // calculate videomode setup

	// initialize base layer 0 to tiles + state row
	ScreenClear(pScreen);
	sStrip* t = ScreenAddStrip(pScreen, TILEH);
	sSegm* g = ScreenAddSegm(t, WIDTH);
	ScreenSegmTile(g, Board, TilesImg_Copy, TILESIZE, TILESIZE, MAPW); // tiles

	t = ScreenAddStrip(pScreen, STATEH);
	g = ScreenAddSegm(t, WIDTH);
	ScreenSegmDText(g, Text, Font_Copy, 8, COL_BLACK, TextCol, TEXTMAX); // text
	g->dbly = True;
	g->offy = -8;
	g->wrapy = STATEH/2;

	// setup layer 1 with info box
	LayerSetup(INFO_LAYER, Box, &Vmode, BOXW, BOXH);
	LayerSetX(INFO_LAYER, BOXX);
	LayerSetY(INFO_LAYER, BOXY);

	// prepare box canvas
	BoxCanvas.img = Box;
	BoxCanvas.w = BOXW;
	BoxCanvas.h = BOXH;
	BoxCanvas.wb = BOXW;
	BoxCanvas.format = CANVAS_8;

	// initialize system clock
	set_sys_clock_pll(Vmode.vco*1000, Vmode.pd1, Vmode.pd2);

	// initialize videomode
	VgaInitReq(&Vmode);
}

// display help
// JoyPad control: right, up, left, down, centerFire=OK, upperLeft=escape, 
// fire=backspace, upperRight=help (solution)
void DispHelp()
{
	printf("\n");
	printf("%right\n");
	printf("up\n");
	printf("left\n");
	printf("down\n");
	printf("upper right = help solve scene\n", KEY_HELP);
	printf("center fire = OK\n");
	printf("upper left = escape, restart\n");
}

// get character from keyboard (0 = no key)
// keyboard control: 'L' right, 'I' up, 'J' left, 'K' down, 
//	'H' help (solution), Enter OK, Esc escape, BS backspace
char GetChar()
{
	char rVal = 0;
	if (!gpio_get(PB_LT))
	{
		while (!gpio_get(PB_LT))
			sleep_ms(20);
		rVal =  'J';
	}
	else if (!gpio_get(PB_RT))
	{
		while (!gpio_get(PB_RT))
			sleep_ms(20);
		rVal =  'L';
	}
	else if (!gpio_get(PB_UP))
	{
		while (!gpio_get(PB_UP))
			sleep_ms(20);
		rVal =  'I';
	}
	else if (!gpio_get(PB_DN))
	{
		while (!gpio_get(PB_DN))
			sleep_ms(20);
		rVal =  'K';
	}
	else if (!gpio_get(PB_CF))
	{
		while (!gpio_get(PB_CF))
			sleep_ms(20);
		rVal =  KEY_OK;
	}
	else if (!gpio_get(PB_UL))
	{
		while (!gpio_get(PB_UL))
			sleep_ms(20);
		rVal =  KEY_ESC;
	}
	else if (!gpio_get(PB_FR))
	{
		while (!gpio_get(PB_FR))
			sleep_ms(20);
		rVal =  KEY_BS;
	}
	else if (!gpio_get(PB_UR))
	{
		while (!gpio_get(PB_UR))
			sleep_ms(20);
		rVal =  'K';
	}
	return (rVal);
	// char c = getchar_timeout_us(0);
	// if (c == (char)PICO_ERROR_TIMEOUT)
		// c = 0;
	// return c;
}

// flush characters from keyboard
void FlushChar()
{
	while (GetChar() != 0) {}
}

// Init the JoyPad pins
// #define PB_UL 16		// Upper Left pushbutton
// #define PB_RT 17		// Right direction
// #define PB_FR 18		// Fire
// #define PB_LT 20		// Left direction
// #define PB_CF 21		// Center Fire
// #define PB_DN 22		// Down direction
// #define PB_UR 26		// Upper right
// #define PB_UP 27		// Up direction
void initJoyPad()
{
	gpio_init(PB_UL);
	gpio_set_dir(PB_UL, GPIO_IN);
	gpio_pull_up(PB_UL);
	
	gpio_init(PB_FR);
	gpio_set_dir(PB_FR, GPIO_IN);
	gpio_pull_up(PB_FR);
	
	gpio_init(PB_UP);
	gpio_set_dir(PB_UP, GPIO_IN);
	gpio_pull_up(PB_UP);
	
	gpio_init(PB_UR);
	gpio_set_dir(PB_UR, GPIO_IN);
	gpio_pull_up(PB_UR);
	
	gpio_init(PB_DN);
	gpio_set_dir(PB_DN, GPIO_IN);
	gpio_pull_up(PB_DN);
	
	gpio_init(PB_LT);
	gpio_set_dir(PB_LT, GPIO_IN);
	gpio_pull_up(PB_LT);
	
	gpio_init(PB_RT);
	gpio_set_dir(PB_RT, GPIO_IN);
	gpio_pull_up(PB_RT);
	
	gpio_init(PB_CF);
	gpio_set_dir(PB_CF, GPIO_IN);
	gpio_pull_up(PB_CF);
}

int main()
{
	int i;

	// initialize random number generator
	RandInitSeed();

	// copy tiles images to RAM buffer (flash would be too slow)
	memcpy(TilesImg_Copy, TilesImg, sizeof(TilesImg));

	// copy font to RAM buffer
	memcpy(Font_Copy, FontBold8x8, sizeof(FontBold8x8));

	// run VGA core
	multicore_launch_core1(VgaCore);

	// clear info row
	InfoClear();

	// initialize videomode
	VideoInit();

	// initialize stdio
	stdio_init_all();
	initJoyPad();

	// initialize sound output
	PWMSndInit();

	// set first scene
	Level = LEVFIRST;
	Score = 0;

	// loop with demo scene
	while (True)
	{
		// demo
		i = Level;
		while (True)
		{
			// play demo scene 0
			Level = 0;
			if (HelpLevel()) break; // break

#ifdef AUTOMODE // automode - run levels to check solutions (help completes scene)
			break;
#endif

			// play random scene
			Level = RandU8MinMax(1, LEVNUM);
			if (HelpLevel()) break; // break
		}
		Level = i;
		InitLevel();

		// main loop
		while (True)
		{
			// game loop - called every single game step
			if (GameLoop()) break;

			// delay
			sleep_ms(GAMESPEED);
		}
	}
}
