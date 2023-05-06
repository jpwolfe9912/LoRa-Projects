/*
 * lcd.h
 *
 *  Created on: Aug 13, 2022
 *      Author: jeremywolfe
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "main.h"

#define LCD_ADDRESS		0x27

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

#define ROWS 4U
#define COLS 20U


void lcdInit();
void lcdClear();
void lcdHome();
void lcdSetCursor(uint8_t col, uint8_t row);
void lcdNoDisplay();
void lcdDisplay();
void lcdNoCursor();
void lcdCursor();
void lcdNoBlink();
void lcdBlink();
void lcdScrollDisplayLeft(void);
void lcdScrollDisplayRight(void);
void lcdLeftToRight(void);
void lcdRightToLeft(void);
void lcdAutoscroll(void);
void lcdNoAutoscroll(void);
void lcdNoBacklight(void);
void lcdBacklight(void);
void lcdSendString(char *str);
void lcdSendChar(char c);

/*
void lcdInit(void);
void lcdSendCmd(uint8_t cmd);
void lcdClear(void);
void lcdPutCur(uint8_t row, uint8_t col);
void lcdSendString(char *str);
*/

#endif /* INC_LCD_H_ */
