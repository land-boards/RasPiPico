
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

// copy of tiles images
ALIGNED u8 TilesImg_Copy[sizeof(Tiles32Img)];

// copy of font
ALIGNED u8 Font_Copy[4096];

// text screen (character + 2x4 bit attributes)
u8 Text[TEXTSIZE*2];

// current tile size (0=text mode, -1 = uninitialized)
int CurTileSize = -1;

// initialize videomode
void VideoInit()
{
// Note: If not using overlapped layer LAYERPROG_KEY, set minimal
// frequency to 150 MHz, or else text mode will flicker.

	// setup videomode
	VgaCfgDef(&Cfg); // get default configuration
	Cfg.video = &VideoVGA; // video timings
	Cfg.width = WIDTH; // screen width
	Cfg.height = HEIGHT; // screen height
	Cfg.mode[1] = LAYERPROG_KEY; // sprite
	VgaCfg(&Cfg, &Vmode); // calculate videomode setup

	// initialize base layer 0 to tiles
	ScreenClear(pScreen);
	sStrip* t = ScreenAddStrip(pScreen, HEIGHT);
	sSegm* g = ScreenAddSegm(t, WIDTH);

	// setup layer 1 with player image
	memset(&LayerScreen[1], 0, sizeof(sLayer));

	// initialize system clock
	set_sys_clock_pll(Vmode.vco*1000, Vmode.pd1, Vmode.pd2);

	// initialize videomode
	VgaInitReq(&Vmode);
}

// set text mode
void SetTextMode()
{
	// set overlapped layer off
	LayerScreen[1].on = False;
	__dmb();

	// setup print buffer
	PrintSetup(Text, TEXTW, TEXTH, TEXTWB);

	// clear text screen
	PrintClear();

	// initialize base layer 0 to text mode
	if (CurTileSize != 0)
	{
		CurTileSize = 0;
		ScreenClear(pScreen);
		sStrip* t = ScreenAddStrip(pScreen, HEIGHT);
		sSegm* g = ScreenAddSegm(t, WIDTH);
		ScreenSegmAText(g, Text, Font_Copy, FONTH, DefPal16, TEXTWB);
	}
}

// set tile mode (must be valid variables MapW, MapH, TileSize)
void SetTileMode()
{
	if (CurTileSize != TileSize)
	{
		CurTileSize = TileSize;

		// copy tiles
		if (TileSize == 32)
			memcpy(TilesImg_Copy, Tiles32Img, sizeof(Tiles32Img));
		else if (TileSize == 28)
			memcpy(TilesImg_Copy, Tiles28Img, sizeof(Tiles28Img));
		else if (TileSize == 24)
			memcpy(TilesImg_Copy, Tiles24Img, sizeof(Tiles24Img));
		else if (TileSize == 20)
			memcpy(TilesImg_Copy, Tiles20Img, sizeof(Tiles20Img));
		else if (TileSize == 16)
			memcpy(TilesImg_Copy, Tiles16Img, sizeof(Tiles16Img));
		else if (TileSize == 12)
			memcpy(TilesImg_Copy, Tiles12Img, sizeof(Tiles12Img));
		else
			memcpy(TilesImg_Copy, Tiles8Img, sizeof(Tiles8Img));

		// initialize base layer 0 to tile mode
		ScreenClear(pScreen);
		sStrip* t = ScreenAddStrip(pScreen, MapH*TileSize);
		sSegm* g = ScreenAddSegm(t, MapW*TileSize);
		ScreenSegmTile2(g, Board, TilesImg_Copy, TileSize, TileSize, TileSize*TILES_NUM, MapW);
	}

	// shift offset
	pScreen->strip[0].seg[0].offx = OffX;
	pScreen->strip[0].seg[0].offy = OffY;

	// set overlapped layer 1 ON
	LayerScreen[1].on = True;
}

// get character from keyboard (0 = no key)
// keyboard control: right, up, left, down, 
//	upperRight=restart scene, upperLeft=previous level, fire=next level,
// keyboard control: 'H' help (solution), 'P' info
char GetChar()
{
	char c = getchar_timeout_us(0);
	if (c == (char)PICO_ERROR_TIMEOUT)
		c = 0;
	if (c == -1)
		c = 0;
	if ((c >= 'a') && (c <= 'z'))
		c -= 32;
	if (c == 0)
	{
		if (!gpio_get(PB_LT))
		{
			while (!gpio_get(PB_LT))
				sleep_ms(20);
			c =  'J';
		}
		else if (!gpio_get(PB_RT))
		{
			while (!gpio_get(PB_RT))
				sleep_ms(20);
			c =  'L';
		}
		else if (!gpio_get(PB_UP))
		{
			while (!gpio_get(PB_UP))
				sleep_ms(20);
			c =  'I';
		}
		else if (!gpio_get(PB_DN))
		{
			while (!gpio_get(PB_DN))
				sleep_ms(20);
			c =  'K';
		}
		else if (!gpio_get(PB_UL))
		{
			while (!gpio_get(PB_UL))
				sleep_ms(20);
			c =  'Q';
		}
		else if (!gpio_get(PB_FR))
		{
			while (!gpio_get(PB_FR))
				sleep_ms(20);
			c =  'W';
		}
		else if (!gpio_get(PB_UR))
		{
			while (!gpio_get(PB_UR))
				sleep_ms(20);
			c =  'R';
		}
	}
	return (c);
}

// wait for a character
char WaitChar()
{
	char c;
	while ((c = GetChar()) == 0) {}
	return c;
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

	// copy font to RAM buffer
	memcpy(Font_Copy, FontBoldB8x16, sizeof(FontBoldB8x16));

	// run VGA core
	multicore_launch_core1(VgaCore);

	// initialize videomode
	VideoInit();

	// initialize stdio
	stdio_init_all();
	initJoyPad();

	// initialize sound output
	PWMSndInit();

	// main loop
	AutInx = 0;
	while (True)
	{
		int oldaut = AutInx;

		// play demo level
		while (True)
		{
			// play demo scene
			LevNum = 0;
			Levels = IntroLevel;
			Level = 0;
			if (PlaySolve()) break;

			// play random scene
			do {
				i = RandU8Max(AutNum-1);
				AutInx = i;
				AutName = Author[i].author; // author's name
				Collect = Author[i].collect; // pointer to list of collections
				CollNum = Author[i].collnum; // number of collections

				i = RandU8Max(CollNum-1);
				CollInx = i;
				CollName = Collect[i].name; // name of collection
				Levels = Collect[i].levels; // pointer to list of levels
				LevNum = Collect[i].levnum; // number of levels

				i = RandU16Max(LevNum-1);
				Level = i;
				LevDef = Levels[i*2]; // current level definition
				LevSolve = Levels[i*2+1]; // current level solve

				BoardDim();

			} while ((LevelW > 15) || (LevelH > 12));
			if (PlaySolve()) break;
		}
		AutInx = oldaut;

		// select author
		while (AuthorSelect())
		{
			// select collection (returns True = OK, False = Esc)
			while (CollSelect())
			{
				// select level
				while (LevSelect())
				{
					// game loop
					GameLoop();
				}
			}
		}
	}
}
