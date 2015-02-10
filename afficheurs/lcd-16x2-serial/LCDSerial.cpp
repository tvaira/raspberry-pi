/*
 LCDSerial.cpp - Library for controlling a SparkFun LCDSerial
              module.
	      Code written for firmware version 2.5

 Created by Cody B. Null, September 9, 2011
 Modified by Jordan Hochenbaum, Dec 5, 2011. (Version 1.5)

 Version 1.4 - This version wrote specifically for 16x2
               Display.
               More display support coming in later version.

 Version 1.5 - Since mid 2011 the Arduino IDE has implemented the NewSoftSerial 
                library updates in the standard SoftwareSerial library and so 
                using NewSoftSerial is no longer needed. LCDSerial has been updated 
                to use the Standard SoftwareSerial library. Also, Arduino 1.0
                support has been added. Backwards compatible with previous versions.

 
 Note -	This library requires NewSoftSerial library
 The latest version of NewSoftSerial library can 
 always be found at http://arduiniana.org. -> NO LONGER NECESSARY. See V1.5 notes above
*/
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <stdint.h>
#include <cstring>

#include "LCDSerial.h"

//	PUBLIC FUNCTIONS

// Contstructor
// defaults to 16x2 display
LCDSerial::LCDSerial(const char *serialPort) : SerialPi(serialPort)
{
   #ifdef LCDSerial_DEBUG
   printf("<LCDSerial::LCDSerial> %s\n", serialPort);   
   #endif  
	begin(9600);
	_numlines = LCD_2LINE;
	_numchars = LCD_16CHAR;
	_rowoffset = 0;
}

// Set brightness value range 1-30 1=OFF 30=FULL
void LCDSerial::setBrightness(int val)
{
	if(val >= 1 && val <= 30)
   {
		specialCommand(LCD_BACKLIGHT | (val - 1));
	}
}

// Clears screen and returns cursor to home position
void LCDSerial::clear()
{
	command(LCD_CLEARDISPLAY);
}

// Clears a single line by writing blank spaces then returning
// cursor to beginning of line
void LCDSerial::clearLine(int num)
{
	if(num > 0 && num <= _numlines)
   {
		setCursor(num, 1);
		print("                ");
		setCursor(num, 1);
	}
}

// Moves cursor to the beginning of selected line
void LCDSerial::selectLine(int num)
{
	if(num > 0 && num <= _numlines)
   {
		setCursor(num, 1);
	}
}

// returns cursor to home position
void LCDSerial::home()
{
	command(LCD_RETURNHOME);
}

// Saves first 2 lines of txt to splash screen memory
void LCDSerial::setSplash()
{
	specialCommand(LCD_SETSPLASHSCREEN);
}

// Toggles splashscreen on and off
void LCDSerial::toggleSplash()
{
	specialCommand(LCD_SPLASHTOGGLE);
}

//  This is for text that flows Left to Right
void LCDSerial::leftToRight()
{
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LCDSerial::rightToLeft() 
{
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// Blinking cursor on/off
void LCDSerial::blink()
{
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LCDSerial::noBlink()
{
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Underline cursor on/off
void LCDSerial::cursor()
{
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LCDSerial::noCursor()
{
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Display on/off
void LCDSerial::display()
{
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LCDSerial::noDisplay()
{
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Set cursor to specific row and col values start at 1 not 0
void LCDSerial::setCursor(int row, int col)
{
	int row_offsets[2][4] = 
   {
		{ 0x00, 0x40, 0x10, 0x50 },
		{ 0x00, 0x40, 0x14, 0x54 }
	};
	
   if((row > 0 && row < 3) && (col > 0 && col < 17))
   {
      command(LCD_SETDDRAMADDR | ((col - 1) + row_offsets[_rowoffset][(row - 1)]));
	}
}

// Creates custom characters 8 char limit
// Input values start with 1
void LCDSerial::createChar(int location, uint8_t charmap[])
{
	location -= 1;
	location &= 0x07;
   
   for (int i=0; i<8; i++)
   {
      command(LCD_SETCGRAMADDR | (location << 3) | i);
      write(charmap[i]);
   }
}

// Prints custom character
// Input values start with 1
void LCDSerial::printCustomChar(int num)
{
	write((num - 1));
}

// PRIVATE FUNCTIONS

// Functions for sending the special command values
void LCDSerial::command(uint8_t value)
{
	write(0xFE);
	write(value);
	delay(5);
}

void LCDSerial::specialCommand(uint8_t value)
{
	write(0x7C);
	write(value);
	delay(5);
}
