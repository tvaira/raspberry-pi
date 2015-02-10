/***************************************************
  This is an example sketch for the Adafruit 1.8" SPI display.
  This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
  as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

//Use these pins for the shield!
#define CS		10
#define MOSI	11
#define MISO	12
#define SCK		13
#define DC   8
#define RST  0

//#include "arduPi.h"
//#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7735.h" // Hardware-specific library

// use the hardware SPI pins
Adafruit_ST7735 tft = Adafruit_ST7735(CS, DC, RST);

void rotateText() 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.setCursor(0, 30);
    tft.setTextColor(ST7735_RED);
    tft.setTextSize(1);
    tft.println("Hello World!");
    tft.setTextColor(ST7735_YELLOW);
    tft.setTextSize(2);
    tft.println("Hello World!");
    tft.setTextColor(ST7735_GREEN);
    tft.setTextSize(3);
    tft.println("Hello World!");
    tft.setTextColor(ST7735_BLUE);
    tft.setTextSize(4);
    tft.print(1234.567);
    
    delay(500);
  
    tft.setRotation(tft.getRotation()+1);
  }
}

void rotateFillcircle(void) 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.fillCircle(10, 30, 10, ST7735_YELLOW);

    delay(500);

    tft.setRotation(tft.getRotation()+1);
  }
}

void rotateDrawcircle(void) 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.drawCircle(10, 30, 10, ST7735_YELLOW);
 
    delay(500);
  
    tft.setRotation(tft.getRotation()+1);
  }
}

void rotateFillrect(void) 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.fillRect(10, 20, 10, 20, ST7735_GREEN);
 
    delay(500);

    tft.setRotation(tft.getRotation()+1);
  }
}

void rotateDrawrect(void) 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.drawRect(10, 20, 10, 20, ST7735_GREEN);
 
    delay(500);

    tft.setRotation(tft.getRotation()+1);
  }
}

void rotateFastline(void) 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.drawFastHLine(0, 20, tft.width(), ST7735_RED);
    tft.drawFastVLine(20, 0, tft.height(), ST7735_BLUE);

    delay(500);

    tft.setRotation(tft.getRotation()+1);
  }
}

void rotateLine(void) 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.drawLine(tft.width()/2, tft.height()/2, 0, 0, ST7735_RED);
    
    delay(500);

    tft.setRotation(tft.getRotation()+1);
  }
}

void rotatePixel(void) 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.drawPixel(10,20, ST7735_WHITE);
    
    delay(500);

    tft.setRotation(tft.getRotation()+1);
  }
}

void rotateTriangle(void) 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.drawTriangle(20, 10, 10, 30, 30, 30, ST7735_GREEN);
    
    delay(500);

    tft.setRotation(tft.getRotation()+1);
  }
}

void rotateFillTriangle(void) 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.fillTriangle(20, 10, 10, 30, 30, 30, ST7735_RED);
    
    delay(500);

    tft.setRotation(tft.getRotation()+1);
  }
}

void rotateRoundRect(void) 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.drawRoundRect(20, 10, 25, 15, 5, ST7735_BLUE);
    delay(500);

    tft.setRotation(tft.getRotation()+1);
  }
}

void rotateFillRoundRect(void) 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.fillRoundRect(20, 10, 25, 15, 5, ST7735_CYAN);
    delay(500);

    tft.setRotation(tft.getRotation()+1);
  }
}

void rotateChar(void) 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.drawChar(25, 15, 'A', ST7735_WHITE, ST7735_WHITE, 1);
    delay(500);

    tft.setRotation(tft.getRotation()+1);
  }
}

void rotateString(void) 
{
  for (uint8_t i=0; i<4; i++) 
  {
    tft.fillScreen(ST7735_BLACK);
    printf("Rotation %d\n", tft.getRotation());

    tft.setCursor(8, 25);
    tft.setTextSize(1);
    tft.setTextColor(ST7735_WHITE);
    tft.print("Adafruit Industries");
    delay(500);

    tft.setRotation(tft.getRotation()+1);
  }
}


int main() 
{
  printf("Test ST7735 %dx%d\n", tft.width(), tft.height());
   
  // If your TFT's plastic wrap has a Black Tab, use the following:
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab 
  
  tft.setTextWrap(false); // Allow text to run off right edge
  tft.fillScreen(ST7735_BLACK);
  
  rotateLine();
  rotateText();
  rotatePixel();
  rotateFastline();
  rotateDrawrect();
  rotateFillrect();
  rotateDrawcircle();
  rotateFillcircle();
  rotateTriangle();
  rotateFillTriangle();
  rotateRoundRect();
  rotateFillRoundRect();
  rotateChar();
  rotateString();
  
  return 0;
}


