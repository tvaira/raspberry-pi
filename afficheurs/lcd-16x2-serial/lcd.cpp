#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <stdint.h>
#include <cstring>

#include "LCDSerial.h"

/*
 * Serial Enabled 16x2 LCD - White on Black 5V
 * 
 * https://www.sparkfun.com/products/9395
 * 
 * Cable USB vers TTL :
 * 
 * Rouge -> VCC
 * Noir  -> GND
 * Vert  -> Rx
 * 
 * 2015 tv <tvaira@free.fr>
 * 
 */

LCDSerial lcd("/dev/ttyUSB0");

//-------------------------------------------------------------------------------------------
void clearScreen()
{
   lcd.clear();
   
   //clears the screen, you will use this a lot!
   //lcd.write(0xFE);
   //lcd.write(0x01); 
}
//-------------------------------------------------------------------------------------------
void selectLineOne()
{ 
   //lcd.selectLine(int num);
   //puts the cursor at line 0 char 0.
   lcd.write(0xFE); //command flag
   lcd.write(128); //position
}
//-------------------------------------------------------------------------------------------
void selectLineTwo()
{ 
   //puts the cursor at line 0 char 0.
   lcd.write(0xFE); //command flag
   lcd.write(192); //position
}
//-------------------------------------------------------------------------------------------
void moveCursorRightOne()
{
   //moves the cursor right one space
   lcd.write(0xFE); //command flag
   lcd.write(20); // 0x14
}
//-------------------------------------------------------------------------------------------
void moveCursorLeftOne()
{
   //moves the cursor left one space
   lcd.write(0xFE); //command flag
   lcd.write(16); // 0x10
}
//-------------------------------------------------------------------------------------------
void scrollRight()
{
   //same as moveCursorRightOne
   lcd.write(0xFE); //command flag
   lcd.write(20); // 0x14
}
//-------------------------------------------------------------------------------------------
void scrollLeft()
{
   //same as moveCursorLeftOne
   lcd.write(0xFE); //command flag
   lcd.write(24); // 0x18
}
//-------------------------------------------------------------------------------------------
void turnDisplayOff()
{
   //this tunrs the display off, but leaves the backlight on. 
   lcd.write(0xFE); //command flag
   lcd.write(8); // 0x08
}
//-------------------------------------------------------------------------------------------
void turnDisplayOn()
{
   //this turns the dispaly back ON
   lcd.write(0xFE); //command flag
   lcd.write(12); // 0x0C
}
//-------------------------------------------------------------------------------------------
void underlineCursorOn()
{
   //turns the underline cursor on
   lcd.write(0xFE); //command flag
   lcd.write(14); // 0x0E
}
//-------------------------------------------------------------------------------------------
void underlineCursorOff()
{
   //turns the underline cursor off
   lcd.write(0xFE); //command flag
   lcd.write(12); // 0x0C
}
//-------------------------------------------------------------------------------------------
void boxCursorOn()
{
   //this turns the box cursor on
   lcd.write(0xFE); //command flag
   lcd.write(13); // 0x0D
}
//-------------------------------------------------------------------------------------------
void boxCursorOff()
{
   //this turns the box cursor off
   lcd.write(0xFE); //command flag
   lcd.write(12); // 0x0C
}
//-------------------------------------------------------------------------------------------
void toggleSplash()
{
   //this toggles the spalsh screenif off send this to turn onif on send this to turn off
   lcd.write(0x7C); //command flag = 124 dec
   lcd.write(9); // 0x09
}
//-------------------------------------------------------------------------------------------
void backlight(int brightness)// 128 = OFF, 157 = Fully ON, everything inbetween = varied brightnbess 
{
   //this function takes an int between 128-157 and turns the backlight on accordingly
   lcd.write(0x7C); //NOTE THE DIFFERENT COMMAND FLAG = 124 dec
   lcd.write(brightness); // any value between 128 and 157 or 0x80 and 0x9D
}
//-------------------------------------------------------------------------------------------
void scrollingMarquee()
{
   printf("Test ScrollingMarquee\n");
   //This function scroll text across the screen on both lines
   clearScreen(); // it's always good to clear the screen before movonh onto a new print
   for(int j = 0; j < 17; j++)
   {
      selectLineOne();
      for(int i = 0; i < j;i++)
         moveCursorRightOne();
      lcd.print("BTS IRIS");
      selectLineTwo();
      for(int i = 0; i < j;i++)
         moveCursorRightOne();
      lcd.print("tv");
      delay(500); // you must have a delay, otherwise the screen will print and clear before you can see the text
      clearScreen();
   }
}
//-------------------------------------------------------------------------------------------
void counter()
{
   printf("Test Counter\n");
   //this function prints a simple counter that counts to 10
   clearScreen();
   for(int i = 0; i <= 10; i++)
   {
      lcd.print("Counter = ");
      lcd.print(i, DEC);
      delay(500);
      clearScreen();
   }
}
//-------------------------------------------------------------------------------------------
void tempAndHumidity()
{
   printf("Test TempAndHumidity\n");
   //this function shows how you could read the data from a temerature and humidity 
   //sensro and then print that data to the Serlcd.
   
   //these could be varaibles instead of static numbers 
   float tempF = 77.0; 
   float tempC = 25.0;
   float humidity = 67.0;
   
   clearScreen();
   selectLineOne();
   lcd.print(" Temp = ");
   lcd.print((long)tempF, DEC);
   lcd.print("F ");
   lcd.print((long)tempC, DEC);
   lcd.print("C");
   selectLineTwo();
   lcd.print(" Humidity = ");
   lcd.print((long)humidity, DEC); 
   lcd.print("%");
   delay(2500);
}
//-------------------------------------------------------------------------------------------
void backlight()
{
   printf("Test Backlight\n");
   //this function shows the different brightnesses to which the backlight can be set 
   clearScreen();
   for(int i = 128; i < 158; i+=2)// 128-157 are the levels from off to full brightness
   {
      backlight(i);
      delay(100);
      lcd.print("Backlight = ");
      lcd.print(i, DEC);
      delay(500);
      clearScreen();
   }
}
//-------------------------------------------------------------------------------------------
void cursors()
{
   printf("Test cursors\n");
   //this function shows the different cursors avaiable on the SerLCD
   clearScreen();
   
   boxCursorOn();
   lcd.print("Box On");
   delay(1500);
   clearScreen();
   
   boxCursorOff();
   lcd.print("Box Off");
   delay(1000);
   clearScreen();
   
   underlineCursorOn();
   lcd.print("Underline On");
   delay(1500);
   clearScreen();
   
   underlineCursorOff();
   lcd.print("Underline Off");
   delay(1000);
   clearScreen();
}
//-------------------------------------------------------------------------------------------
void test()
{   
   scrollingMarquee();
   tempAndHumidity();
   counter();
   backlight();
   cursors();
   //printf("\n");
}
//-------------------------------------------------------------------------------------------
int main()
{
   printf("Test LCD Serial\n");
   test();
   
   return 0;
}
