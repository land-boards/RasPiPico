
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "include.h"

// Init the JoyPad pins
#define PB_UL 27		// Upper Left pushbutton
#define PB_RT 21		// Right direction
#define PB_FR 17		// Fire
#define PB_LT 20		// Left direction
#define PB_CF 22		// Center Fire
#define PB_DN 18		// Down direction
#define PB_UR 26		// Upper right
#define PB_UP 16		// Up direction

// copy of tiles images
ALIGNED u8 TilesImg_Copy[sizeof(TilesImg)];

// game board
u8 Board[TILESNUM];
u8 HoleX, HoleY; // hole coordinate

// new game template
u8 NewBoard[TILESNUM] = {
	TILE_1+0,  TILE_1+1,  TILE_1+2,  TILE_1+3,   TILE_BLACK, TILE_BLACK,
	TILE_1+4,  TILE_1+5,  TILE_1+6,  TILE_1+7,   TILE_BLACK, TILE_BLACK,
	TILE_1+8,  TILE_1+9,  TILE_1+10, TILE_1+11,  TILE_BLACK, TILE_BLACK,
	TILE_1+12, TILE_1+13, TILE_1+14, TILE_EMPTY, TILE_BLACK, TILE_BLACK,
};

// display help
void DispHelp()
{
	printf("\n");
	printf("%c ... right\n", KEY_R);
	printf("%c ... up\n", KEY_U);
	printf("%c ... left\n", KEY_L);
	printf("%c ... down\n", KEY_D);
	printf("%c ... new game\n", KEY_NEW);
}

// get character from keyboard (0 = no key)
// keyboard control: 'L' right, 'I' up, 'J' left, 'K' down, 'Q' new game
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
	else if (!gpio_get(PB_FR))
	{
		while (!gpio_get(PB_FR))
			sleep_ms(20);
		rVal =  'Q';
	}
	return (rVal);
	
	// char c = getchar_timeout_us(0);
	// if (c == (char)PICO_ERROR_TIMEOUT) c = 0;
	// if ((c >= 'a') && (c <= 'z')) c -= 32;
	// return c;
}

// flush characters from keyboard
void FlushChar()
{
	while (GetChar() != 0) {}
}

// shoft hole (-1..+1)
void Shift(s8 sx, s8 sy, Bool snd)
{
	// new coordinates of the hole
	s8 x = HoleX + sx;
	s8 y = HoleY + sy;

	// check coordinates
	if ((x < 0) || (x > 3) || (y < 0) || (y > 3))
	{
		// play blocking sound
		if (snd) PlaySound(BumpSnd, sizeof(BumpSnd));
		return;
	}

	// display
	Board[HoleX + HoleY*TILESWB] = Board[x + y*TILESWB];
	Board[x + y*TILESWB] = TILE_EMPTY;

	// save new coordinates
	HoleX = x;
	HoleY = y;

	// play move sound
	if (snd) PlaySound(MoveSnd, sizeof(MoveSnd));
}

// initialize new game
void NewGame()
{
	// start shuffling sound
	PlaySound(ShufflingSnd, sizeof(ShufflingSnd), True);

	// initialize new state
	memcpy(Board, NewBoard, TILESNUM);
	HoleX = 3;
	HoleY = 3;

	// shuffle board (2 seconds)
	int i, j;
	for (i = 100; i > 0; i--)
	{
		WaitVSync(); // wait for VSYNC to avoid image flickering
		for (j = 100; j > 0; j--)
		{
			if (RandU8() >= 0x80)
				Shift(RandS8MinMax(-1,1), 0, False); // shift X
			else
				Shift(0, RandS8MinMax(-1,1), False); // shift Y
		}
	}

	// stop shuffling sound
	StopSound();

	// flush keyboard
	FlushChar();
}

// check goal state
Bool Check()
{
	return memcmp(Board, NewBoard, TILESNUM) == 0;
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

// main function
int main()
{
	int i;

	// initialize random number generator
	RandInitSeed();

	// copy tiles images to RAM buffer (flash would be too slow)
	memcpy(TilesImg_Copy, TilesImg, sizeof(TilesImg));

	// initialize videomode
	Video(DEV_VGA, RES_ZX, FORM_TILE48, Board, TilesImg_Copy);
	pScreen->strip[0].seg[0].offx = -32;

	// initialize stdio
	stdio_init_all();
	initJoyPad();

	// initialize sound output
	PWMSndInit();

	// new game
	NewGame();

	// game loop
	while (True)
	{
		// key
		switch(GetChar())
		{
		case KEY_R:	// right
			Shift(-1, 0, True);
			break;

		case KEY_U:	// up
			Shift(0, 1, True);
			break;

		case KEY_L:	// left
			Shift(1, 0, True);
			break;

		case KEY_D:	// down
			Shift(0, -1, True);
			break;

		case KEY_NEW:	// new game
			NewGame();
			break;

		case 0: // no key
			break;

		default: // unknown key
			DispHelp();
			break;
		}

		// check solution
		if (Check())
		{
			// play fanfare
			PlaySound(FanfareSnd, sizeof(FanfareSnd));

			// winning animation
			for (i = 5; i > 0; i--)
			{
				sleep_ms(100);
				memset(Board, TILE_BLACK, TILESNUM);
				sleep_ms(100);
				memcpy(Board, NewBoard, TILESNUM);
			}

			// flush keyboard
			FlushChar();

			// wait for a key
			while (GetChar() == 0) {}

			// start new game
			NewGame();
		}
	}
}
