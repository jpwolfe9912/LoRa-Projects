/** @file 		lcd.c
 *  @brief
 *
 *
 *  @author 	jeremywolfe
 *  @date 		Aug 13, 2022
 */
#include "lcd.h"

uint8_t _addr;
uint8_t _displayfunction;
uint8_t _displaycontrol;
uint8_t _displaymode;
uint8_t _cols = COLS;
uint8_t _rows = ROWS;
uint8_t _charsize;
uint8_t _backlightval;

/* Function Prototypes */
static void send(uint8_t, uint8_t);
static void write4bits(uint8_t);
static void expanderWrite(uint8_t);
static void pulseEnable(uint8_t);

static inline void command(uint8_t value);
static inline size_t write(uint8_t value);

void lcdInit()
{
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (ROWS > 1)
	{
		_displayfunction |= LCD_2LINE;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	delay(50);

	// Now we pull both RS and R/W low to begin commands
	expanderWrite(LCD_BACKLIGHT); // reset expanderand turn backlight off (Bit 8 =1)
	delay(1000);

	// put the LCD into 4 bit mode
	//  this is according to the hitachi HD44780 datasheet
	//  figure 24, pg 46

	// we start in 8bit mode, try to set 4 bit mode
	write4bits(0x03 << 4);
	delayMicroseconds(4500); // wait min 4.1ms

	// second try
	write4bits(0x03 << 4);
	delayMicroseconds(4500); // wait min 4.1ms

	// third go!
	write4bits(0x03 << 4);
	delayMicroseconds(150);

	// finally, set to 4-bit interface
	write4bits(0x02 << 4);

	// set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON;
	lcdDisplay();

	// clear it off
	lcdClear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);

	lcdHome();
	
	for (int i = 0; i < 3; i++)
	{
		lcdBacklight();
		delay(250);
		lcdNoBacklight();
		delay(250);
	}
	lcdBacklight(); // finish with backlight on
}

/********** high level commands, for the user! */
void lcdClear()
{
	command(LCD_CLEARDISPLAY); // clear display, set cursor position to zero
	delayMicroseconds(2000);   // this command takes a long time!
}

void lcdHome()
{
	command(LCD_RETURNHOME); // set cursor position to zero
	delayMicroseconds(2000); // this command takes a long time!
}

void lcdSetCursor(uint8_t col, uint8_t row)
{
	int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	if (row > ROWS)
	{
		row = ROWS - 1; // we count rows starting w/0
	}
	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void lcdNoDisplay()
{
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void lcdDisplay()
{
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void lcdNoCursor()
{
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void lcdCursor()
{
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void lcdNoBlink()
{
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void lcdBlink()
{
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void lcdScrollDisplayLeft(void)
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void lcdScrollDisplayRight(void)
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void lcdLeftToRight(void)
{
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void lcdRightToLeft(void)
{
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void lcdAutoscroll(void)
{
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void lcdNoAutoscroll(void)
{
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// Turn the (optional) backlight off/on
void lcdNoBacklight(void)
{
	_backlightval = LCD_NOBACKLIGHT;
	expanderWrite(0);
}

void lcdBacklight(void)
{
	_backlightval = LCD_BACKLIGHT;
	expanderWrite(0);
}

void lcdSendString(char *str)
{
	while (*str)
		write(*str++);
}

void lcdSendChar(char c)
{
	write(c);
}

/*********** mid level commands, for sending data/cmds */

static inline void command(uint8_t value)
{
	send(value, 0);
}

static inline size_t write(uint8_t value)
{
	send(value, Rs);
	return 1;
}

/************ low level data pushing commands **********/

// write either command or data
static void send(uint8_t value, uint8_t mode)
{
	uint8_t highnib = value & 0xf0;
	uint8_t lownib = (value << 4) & 0xf0;
	write4bits((highnib) | mode);
	write4bits((lownib) | mode);
}

static void write4bits(uint8_t value)
{
	expanderWrite(value);
	pulseEnable(value);
}

static void expanderWrite(uint8_t _data)
{
	i2c2CommandDMA((int)(_data) | LCD_BACKLIGHT);
}

static void pulseEnable(uint8_t _data)
{
	expanderWrite(_data | En); // En high
	delayMicroseconds(1);	   // enable pulse must be >450ns

	expanderWrite(_data & ~En); // En low
	delayMicroseconds(50);		// commands need > 37us to settle
}

static void setBacklight(uint8_t new_val)
{
	if (new_val)
	{
		lcdBacklight(); // turn backlight on
	}
	else
	{
		lcdNoBacklight(); // turn backlight off
	}
}

/*
void
lcdInit(void)
{
	LL_mDelay(50);
	// 4 bit initialisation
	LL_mDelay(50);  // wait for >40ms
	lcdSendCmd (0x30);
	LL_mDelay(5);  // wait for >4.1ms
	lcdSendCmd (0x30);
	LL_mDelay(1);  // wait for >100us
	lcdSendCmd (0x30);
	LL_mDelay(10);
	lcdSendCmd (0x20);  // 4bit mode
	LL_mDelay(10);

	// dislay initialisation
	lcdSendCmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	LL_mDelay(1);
	lcdSendCmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	LL_mDelay(1);
	lcdSendCmd (0x01);  // clear display
	LL_mDelay(1);
	LL_mDelay(1);
	lcdSendCmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	LL_mDelay(1);
	lcdSendCmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)

}

void
lcdSendCmd(uint8_t cmd)
{
	char data_u, data_l;
	uint8_t data[4];
	data_u = cmd & 0xF0;
	data_l = (cmd << 4) & 0xF0;
	data[0] = data_u|0x0C;  //en=1, rs=0
	data[1] = data_u|0x08;  //en=0, rs=0
	data[2] = data_l|0x0C;  //en=1, rs=0
	data[3] = data_l|0x08;  //en=0, rs=0
	i2c2LcdCmd(data, 4);
}

void
lcdSendData(char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	i2c2LcdCmd(data_t, 4);
}

void
lcdClear(void)
{
	lcdSendCmd(0x80);
	for (int i=0; i<70; i++)
	{
		lcdSendData(' ');
	}
}

void
lcdPutCur(uint8_t row, uint8_t col)
{
	switch (row)
	{
		case 0:
			col |= 0x80;
			break;
		case 1:
			col |= 0xC0;
			break;
	}

	lcdSendCmd(col);
}

void
lcdSendString(char *str)
{
	while (*str)lcdSendData(*str++);
}
*/