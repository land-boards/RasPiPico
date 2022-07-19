// Near top of file
// JOYPAD pushbutton to Pico pin mounting
#define PB_UL 16		// Upper Left pushbutton
#define PB_RT 17		// Right direction
#define PB_FR 18		// Fire
#define PB_LT 20		// Left direction
#define PB_CF 21		// Center Fire
#define PB_DN 22		// Down direction
#define PB_UR 26		// Upper right
#define PB_UP 27		// Up direction

// Add before main()
// Also call initJoyPad(); in main() just after stdio_init_all();
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

// Replaces GetChar
// get character from keyboard (0 = no key)
// keyboard control: 'I' gear up, 'K' gear down, 'J' turn left, 'L' turn right
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
	return (rVal);
	
	// char c = getchar_timeout_us(0);
	// if (c == (char)PICO_ERROR_TIMEOUT) c = 0;
	// if ((c >= 'a') && (c <= 'z')) c -= 32;
	// return c;
}

