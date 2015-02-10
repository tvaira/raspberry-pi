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

#define BUTTON_NONE 0
#define BUTTON_DOWN 1
#define BUTTON_RIGHT 2
#define BUTTON_SELECT 3
#define BUTTON_UP 4
#define BUTTON_LEFT 5

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(FILE * bmpFile) 
{
  uint16_t result;
  
  //((uint8_t *)&result)[0] = f.read(); // LSB
  //((uint8_t *)&result)[1] = f.read(); // MSB
  fread(&result, sizeof(uint8_t), 2, bmpFile);
  return result;
}

uint32_t read32(FILE * bmpFile) 
{
  uint32_t result;
  
  //((uint8_t *)&result)[0] = f.read(); // LSB
  //((uint8_t *)&result)[1] = f.read();
  //((uint8_t *)&result)[2] = f.read();
  //((uint8_t *)&result)[3] = f.read(); // MSB
  fread(&result, sizeof(uint8_t), 4, bmpFile);
  return result;
}


// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).

#define BUFFPIXEL 128

void bmpDraw(char *filename, uint8_t x, uint8_t y) 
{
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t bmpImagesize;          // Image size
  uint32_t bmpFilesize;           // File size
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint32_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
  int32_t position;

  if((x >= tft.width()) || (y >= tft.height())) return;

  printf("Loading image %s\n", filename);

  // Open requested file
  FILE * bmpFile = fopen(filename, "r");
  if (bmpFile == NULL) 
  {
    printf("File not found");
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) // BMP signature ?
  { 
    bmpFilesize = read32(bmpFile);
    printf("File size: %d\n", bmpFilesize);
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    printf("Image Offset: %d\n", bmpImageoffset);
    // Read DIB header
    printf("Header size: %d\n", read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) 
    { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      printf("Bit Depth: %d\n", bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) 
      { // 0 = uncompressed
        goodBmp = true; // Supported BMP format -- proceed!
        bmpImagesize = read32(bmpFile);
        printf("Image size: %dx%d (%d octets)\n", bmpWidth, bmpHeight, bmpImagesize);        

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;
        printf("Row size: %d\n", rowSize); 
        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) 
        {
          bmpHeight = -bmpHeight;
          flip      = false;
        }
        //printf("Flip: %d\n", flip); 
        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;
        printf("Crop size: %dx%d\n", w, h);         
        //uint32_t bytesPerLine = ((w * bmpDepth / 8) + 3) & ~3;
        //printf("Bytes per line: %d\n", bytesPerLine);         
        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) 
        { // For each scanline...
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;          
          position = ftell(bmpFile);
          if(position != (int32_t)pos) 
          {
            fseek(bmpFile, pos, SEEK_SET/*SEEK_CUR*/);
          }

          fread(sdbuffer, sizeof(sdbuffer), 1, bmpFile);
          buffidx = 0;
          for (col=0; col<w; col++) // For each pixel...
          { 
            // Convert pixel from BMP to TFT format, push to display
            r = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            b = sdbuffer[buffidx++];
            
            tft.pushColor(tft.Color565(r,g,b));
          } // end pixel
        } // end scanline
        printf("Loaded in %ld ms\n", millis() - startTime);
      } // end goodBmp
    }
  }

  fclose(bmpFile);
  if(!goodBmp) printf("BMP format not recognized !\n");
}

uint8_t readButton(void) 
{
  float a = analogRead(3);
  
  a *= 5.0;
  a /= 1024.0;  
  printf("Button read analog = %.1f V\n", a);

  if (a < 0.2) return BUTTON_DOWN;
  if (a < 1.0) return BUTTON_RIGHT;
  if (a < 1.5) return BUTTON_SELECT;
  if (a < 2.0) return BUTTON_UP;
  if (a < 3.2) return BUTTON_LEFT;
  else return BUTTON_NONE;
}

uint8_t buttonhistory = 0;

void loop() 
{
   bool fini = false;
   
   while(!fini)
   {
     uint8_t b = readButton();
     tft.setTextSize(3);     
     if (b == BUTTON_DOWN) 
     {       
       tft.setTextColor(ST7735_RED); 
       tft.setCursor(0, 10);
       tft.print("Down ");
       buttonhistory |= 1;
     }
     if (b == BUTTON_LEFT) 
     {
       tft.setTextColor(ST7735_RED); 
       tft.setCursor(0, 35);
        tft.print("Left ");
       buttonhistory |= 2;
       delay(1000);    
       bmpDraw("tv.bmp", 0, 0);
       //delay(5000);    
     }
     if (b == BUTTON_UP) 
     {
       tft.setTextColor(ST7735_RED); 
       tft.setCursor(0, 60);
       tft.print("Up"); 
       buttonhistory |= 4;
       fini = true;
     }
     if (b == BUTTON_RIGHT) 
     {       
       tft.setTextColor(ST7735_RED); 
       tft.setCursor(0, 85);
       tft.print("Right");
       buttonhistory |= 8;
       delay(1000);    
       bmpDraw("parrot.bmp", 0, 0);
       //delay(5000);    
     }
     //if ((b == BUTTON_SELECT) && (buttonhistory == 0xF)) 
     if (b == BUTTON_SELECT) 
     {
       tft.setTextColor(ST7735_BLUE);
       tft.setCursor(0, 110);
       tft.print("SELECT");
       buttonhistory |= 8;
       delay(1000);    
       bmpDraw("pi.bmp", 0, 0);
       //delay(5000);    
     }
     delay(100);
  }
}


int main() 
{
  printf("Test ST7735 %dx%d\n", tft.width(), tft.height());

   /*while(1)
   {
      TestJoystick();
      delay(100);
   }*/
   
  // If your TFT's plastic wrap has a Black Tab, use the following:
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab 
  
  tft.fillScreen(0x0000);
  
  loop();
  
  return 0;
}
