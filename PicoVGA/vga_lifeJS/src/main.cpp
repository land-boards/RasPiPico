
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "include.h"

// JoyStick pushbutton to Pico pin mounting
#define PB_UL 26		// Upper Left pushbutton
#define PB_RT 21		// Right direction
#define PB_FR 17		// Fire
#define PB_LT 20		// Left direction
#define PB_CF 27		// Center Fire
#define PB_DN 18		// Down direction
#define PB_UR 22		// Upper right
#define PB_UP 16		// Up direction

// copy of tiles images
u8 TilesImg_Copy[sizeof(Tiles16Img)] __attribute__ ((aligned(4)));

// game board
u8 Board[MAPSIZE];

// neighbors
u8 Neigh[MAPSIZE];

// slots
u8 Slot[MAPSIZE*SLOTNUM];
int SelSlot;

// clipboard
u8 Clip[MAPSIZE];

// cursor
int CurX, CurY;

// samples (first 2 bytes = width, height)
const u8 Samples[] = {

	// 0: Life
	5,5,
	1,1,1,1,1,
	1,1,1,1,1,
	1,1,1,1,1,
	1,1,1,1,1,
	1,1,1,1,1,

	// 1: penta-decathlon
	3,12,
	1,1,1,
	0,1,0,
	0,1,0,
	1,1,1,
	0,0,0,
	1,1,1,
	1,1,1,
	0,0,0,
	1,1,1,
	0,1,0,
	0,1,0,
	1,1,1,

	// 2: R-pentomino
	3,3,
	0,1,1,
	1,1,0,
	0,1,0,

	// 3: Diehard
	8,3,
	0,0,0,0,0,0,1,0,
	1,1,0,0,0,0,0,0,
	0,1,0,0,0,1,1,1,

	// 4. Acorn
	7,3,
	0,1,0,0,0,0,0,
	0,0,0,1,0,0,0,
	1,1,0,0,1,1,1,

	// 5: Glider
	3,3,
	0,0,1,
	1,0,1,
	0,1,1,

	// 6: spaceship
	5,4,
	1,0,0,1,0,
	0,0,0,0,1,
	1,0,0,0,1,
	0,1,1,1,1,

	// 7: pulsar
	15,15,
	0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,0,0,1,1,0,1,1,0,0,1,1,1,
	0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,
	0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,
	0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,
	1,1,1,0,0,1,1,0,1,1,0,0,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,

	// 8: Gosper glider gun
	38, 9,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,1,0,1,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,1,1,0,0,0,0,0, 0,1,1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,1,0,0,0,1,0,0,0, 0,1,1,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,1,0,0, 0,1,1,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,0, 0,1,0,0,0,1,0,1,1,0, 0,0,0,1,0,1,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,1,0,0, 0,0,0,0,0,1,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,1,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,1,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

	// 9: Simkin glider gun
	33,21,
	1,1,0,0,0,0,0,1,1,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,
	1,1,0,0,0,0,0,1,1,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,1,1,0,1,1,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,1,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,1,0, 0,1,1,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,1,1,1,0,0,0,1,0,0, 0,1,1,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,1,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 1,1,0,0,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,1,1,1,0,0,0,0,0,0, 0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,1,0,0,0,0,0,0, 0,0,0,
};

/*
// initialize videomode
void VideoInit()
{
	// setup videomode
	VgaCfgDef(&Cfg); // get default configuration
	Cfg.video = &DRV; // video timings
	Cfg.width = WIDTH; // screen width
	Cfg.height = HEIGHT; // screen height
	VgaCfg(&Cfg, &Vmode); // calculate videomode setup

	// initialize base layer 0 to tiles
	ScreenClear(pScreen);
	sStrip* t = ScreenAddStrip(pScreen, HEIGHT);
	sSegm* g = ScreenAddSegm(t, WIDTH);
	ScreenSegmTile2(g, Board, TilesImg_Copy, TILESIZE, TILESIZE, TILESIZE*TILE_NUM, MAPW);

	// initialize system clock
	set_sys_clock_pll(Vmode.vco*1000, Vmode.pd1, Vmode.pd2);

	// initialize videomode
	VgaInitReq(&Vmode);
}
*/

// display help
void DispHelp()
{
	printf("\n");
	printf("%c ... right\n", KEY_R);
	printf("%c ... up\n", KEY_U);
	printf("%c ... left\n", KEY_L);
	printf("%c ... down\n", KEY_D);
	printf("space ... flip state\n");
	printf("Enter .. run life\n");
	printf("0..9 ... select slot\n");
	printf("%c ... copy to clipboard\n", KEY_COPY);
	printf("%c ... paste from clipboard\n", KEY_PASTE);
	printf("%c ... clear board\n", KEY_CLEAR);
	printf("Selected slot: %u\n", SelSlot);
	printf("Rules: 3->new, 2->stable, other->dead\n");
}

// get character from keyboard (0 = no key)
char GetChar()
{
	char c = getchar_timeout_us(0);
	if (c == (char)PICO_ERROR_TIMEOUT) c = 0;
	if ((c >= 'a') && (c <= 'z')) c -= 32;
	return c;
}

// flush characters from keyboard
void FlushChar()
{
	while (GetChar() != 0) {}
}

// set cursor on
void CurOn()
{
	u8* d = &Board[CurX + CurY*MAPW];
	u8 b = *d;
	if (b == TILE_FULL)
		b = TILE_SELFULL;
	else if (b == TILE_EMPTY)
		b = TILE_SELEMPTY;
	*d = b;
}

// set cursor off
void CurOff()
{
	u8* d = &Board[CurX + CurY*MAPW];
	u8 b = *d;
	if (b == TILE_SELFULL)
		b = TILE_FULL;
	else if (b == TILE_SELEMPTY)
		b = TILE_EMPTY;
	*d = b;
}

// flip cursor
void CurFlip()
{
	u8* d = &Board[CurX + CurY*MAPW];
	u8 b = *d;
	if (b == TILE_SELFULL)
		b = TILE_SELEMPTY;
	else if (b == TILE_SELEMPTY)
		b = TILE_SELFULL;
	*d = b;
}

// get cell on coordinates (0 or 1)
int Cell(int x, int y)
{
	if ((x < 0) || (x >= MAPW) || (y < 0) || (y >= MAPH)) return 0;
	u8* d = &Board[x + y*MAPW];
	u8 b = *d;
	return (b == TILE_FULL) ? 1 : 0;
}

// save to current slot
void SaveSlot()
{
	memcpy(&Slot[MAPSIZE*SelSlot], Board, MAPSIZE);
}

// load from current slot
void LoadSlot()
{
	memcpy(Board, &Slot[MAPSIZE*SelSlot], MAPSIZE);
}

// run life
void Run()
{
	int x, y;
	u8 b;
	u8* d;
	u8* s;

	FlushChar();

	// break with a key
	while (GetChar() == 0)
	{
		if (!gpio_get(PB_UR))
		{
			while (!gpio_get(PB_UR))
				sleep_ms(20);
			return;
		}
		// calculate neighbors
		d = Neigh;
		for (y = 0; y < MAPH; y++)
		{
			for (x = 0; x < MAPW; x++)
			{
				*d++ =	Cell(x-1, y-1) +
					Cell(x  , y-1) +
					Cell(x+1, y-1) +

					Cell(x-1, y  ) +
					Cell(x+1, y  ) +

					Cell(x-1, y+1) +
					Cell(x  , y+1) +
					Cell(x+1, y+1);
			}
		}

		// update cells
		s = Neigh;
		d = Board;
		for (x = MAPSIZE; x > 0; x--)
		{
			b = *s++;

			// 3 -> new cell
			if (b == 3) 
				*d = TILE_FULL;

			// 2 -> unchanged
			else if (b == 2)
				;
			// else -> dead
			else
				*d = TILE_EMPTY;
			d++;
		}

		// delay
		sleep_ms(SPEED);
	}
}

// initialize slots by samples
void InitSlot()
{
	int slot;
	u8* d;
	const u8* s = Samples;
	int w, h;
	for (slot = 0; slot < SLOTNUM; slot++)
	{
		// destination slot
		d = &Slot[MAPSIZE*slot];

		// sample dimension
		w = *s++;
		h = *s++;

		// center image
		d += (MAPW-w)/2 + (MAPH-h)/2*MAPW;

		// copy sample
		for (; h > 0; h--)
		{
			memcpy(d, s, w);
			d += MAPW;
			s += w;
		}
	}
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
	char ch;

	// copy tiles images to RAM buffer (flash would be too slow)
	memcpy(TilesImg_Copy, Tiles16Img, sizeof(Tiles16Img));

	// clear buffer
	memset(Board, TILE_EMPTY, MAPSIZE);
	memset(Slot, TILE_EMPTY, MAPSIZE*SLOTNUM);
	memset(Clip, TILE_EMPTY, MAPSIZE);
	CurX = MAPW/2;
	CurY = MAPH/2;
	SelSlot = 1;

	// fill slots with samples
	InitSlot();

	// load current slot
	LoadSlot();

	// run VGA core
//	multicore_launch_core1(VgaCore);

	// initialize videomode
//	VideoInit();
	Video(DEV_VGA, RES_VGA, FORM_TILE16, Board, TilesImg_Copy);

	// initialize stdio
	stdio_init_all();
	initJoyPad();

	// display help
	DispHelp();

	// set cursor on
	CurOn();

	// loop with demo scene
	// keyboard control: (L) right = right, (I) up=up, (J) left=left, (K) down=down, SPACE Fire=flip state, 
	//	(C) = copy, (V) = paste, 13 fire=run life, 
	//  'D'=clear, 0..9 select slot
	while (True)
	{
		// keyboard and JoyPad
		if (!gpio_get(PB_UP))
		{
			while (!gpio_get(PB_UP))
				sleep_ms(20);
			ch =  'I';
		}
		else if (!gpio_get(PB_DN))
		{
			while (!gpio_get(PB_DN))
				sleep_ms(20);
			ch =  'K';
		}
		else if (!gpio_get(PB_LT))
		{
			while (!gpio_get(PB_LT))
				sleep_ms(20);
			ch =  'J';
		}
		else if (!gpio_get(PB_RT))
		{
			while (!gpio_get(PB_RT))
				sleep_ms(20);
			ch =  'L';
		}
		else if (!gpio_get(PB_FR))
		{
			while (!gpio_get(PB_FR))
				sleep_ms(20);
			ch =  ' ';
		}
		else
		{
			ch = GetChar();
		}
		if (ch != 0)
		{
			switch (ch)
			{
			// right
			case KEY_R:
				CurOff(); // set cursor off
				CurX++;
				if (CurX >= MAPW) CurX = 0;
				CurOn(); // set cursor on
				break;

			// left
			case KEY_L:
				CurOff(); // set cursor off
				CurX--;
				if (CurX < 0) CurX = MAPW-1;
				CurOn(); // set cursor on
				break;

			// down
			case KEY_D:
				CurOff(); // set cursor off
				CurY++;
				if (CurY >= MAPH) CurY = 0;
				CurOn(); // set cursor on
				break;

			// up
			case KEY_U:
				CurOff(); // set cursor off
				CurY--;
				if (CurY < 0) CurY = MAPH-1;
				CurOn(); // set cursor on
				break;
		
			// toggle
			case ' ':
				CurFlip(); // flip cursor
				break;

			// run
			case 13:
				CurOff(); // set cursor off
				SaveSlot(); // save state
				Run(); // run life
				LoadSlot(); // restore state
				CurOn(); // set cursor on
				break;

			// select slot
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				CurOff(); // set cursor off
				SaveSlot();
				SelSlot = ch - '0';
				LoadSlot();
				CurOn(); // set cursor on
				printf("Selected slot: %u\n", SelSlot);
				break;

			// copy to clipboard
			case KEY_COPY:
				CurOff(); // set cursor off
				memcpy(Clip, Board, MAPSIZE);
				CurOn(); // set cursor on
				break;

			// paste from clipboard
			case KEY_PASTE:
				CurOff(); // set cursor off
				memcpy(Board, Clip, MAPSIZE);
				CurOn(); // set cursor on
				break;

			// clear board
			case KEY_CLEAR:
				CurOff(); // set cursor off
				memset(Board, TILE_EMPTY, MAPSIZE);
				CurOn(); // set cursor on
				break;

			// help
			default:
				DispHelp();
				break;
			}
		}
	}
}
