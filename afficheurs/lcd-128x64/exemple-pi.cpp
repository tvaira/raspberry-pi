#include "LCDSPI.h"
#include "logo.xbm" // -> logo
//#include "bmp.h"  // -> dfrobot_logo

/*
 * This LCD module uses a 128x64 liquid crystal display that support Chinese character, English characters and even graphics. 
 * It can exhibit 4 lines and 12 English characters/6 Chinese characters per line. It is suitable for interactive work with Arduino. 
 * 
 * URL : http://www.dfrobot.com/wiki/index.php?title=SPI_LCD_Module_%28SKU:DFR0091%29
 * 
 * 
 * Raspberry Pi to Arduino shields connection bridge :
 *
 * http://www.cooking-hacks.com/documentation/tutorials/raspberry-pi-to-arduino-shields-connection-bridge
 */

/*
 * Cablage
 * 
 * Module SPI LCD          Shield Pi
 *
 * SCK            --->     SCK
 * CS             --->     8
 * SID            --->     MOSI
 *
 * 5V
 * GND
 */

/*
 * 2015 tv <tvaira@free.fr>
 */ 

int main ()
{
   unsigned char message1[] = "  BTS IRIS/SN   ";
   unsigned char message2[] = "** 2014/2015  **";
   unsigned char message3[] = "tvaira \x40 free.fr";
   unsigned char message4[] = "  Raspberry Pi  ";
   LCDSPI lcd;
   
   lcd.Clear();
   
	lcd.DrawXBM(logo); 
   //lcd.DrawFullScreen(dfrobot_logo); 
   delay(2000);   
   lcd.Clear();
   
   lcd.DisplayString(0, 0, message1, 16);
   lcd.DisplayString(1, 0, message2, 16);
   lcd.DisplayString(2, 0, message3, 16);
   lcd.DisplayString(3, 0, message4, 16);
   delay(5000);      
   lcd.Clear();
 
   //lcd.Close();
   
	return (0);
}
