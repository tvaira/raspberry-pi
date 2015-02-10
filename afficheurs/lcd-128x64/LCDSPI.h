#ifndef LCDSPI_h
#define LCDSPI_h

#include "arduPi.h"

#define DEFAULTTIME 80 // ms

#define FUNCTION_SET       0x30 // basic instruction
#define FUNCTION_SET_EXT   0x34 // extended instruction
#define ENTRY_MODE_SET     0x04
#define MODE_I_D           0x02 // cursor moves right, address counter (AC) is increased by 1
#define MODE_S             0x01
#define DISPLAY_CLEAR      0x01
#define RETURN_HOME        0x02
#define DISPLAY_CONTROL    0x08
#define DISPLAY_ALL_ON     0x0F
#define DISPLAY_ON         0x04
#define CURSOR_ON          0x02
#define BLINK_ON           0x01

#define LCD_CLR                 1    // clear display
#define LCD_HOME                2    // return to home position

#define LCD_ENTRY_MODE          4    // set entry mode
#define LCD_ENTRY_INC           2    // 1=increment, 0=decrement
#define LCD_ENTRY_SHIFT         1    // 1=display shift on

#define LCD_DISPLAYMODE         8    // 
#define LCD_DISPLAYMODE_ON      4    // turn display on
#define LCD_DISPLAYMODE_CURSOR  2    // turn cursor on
#define LCD_DISPLAYMODE_BLINK   1    // blinking cursor

typedef unsigned char uchar;

class LCDSPI 
{
   public:
      LCDSPI(int latch=8);

      void Initialise(void);
      void Close();

      void WriteByte(int dat);
      void WriteCommand(int cmd);
      void WriteData(int data);
      void WriteReverseData(int data);

      void Clear(void);
      void ReturnHome(void);
      void DisplayString(int X,int Y,uchar *ptr,int dat);
      void DisplaySig(int M,int N,int sig);
      void DrawFullScreen(uchar *p);
      void DrawXBM(uchar *p);
      void img1(uchar img[]);
      void img2(uchar img[]);

   private:
      int latchPin; 
      int clockPin;  
      int dataPin;  
      int delaytime;
};

#endif
