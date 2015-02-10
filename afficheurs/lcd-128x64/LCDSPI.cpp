#include "LCDSPI.h"

//#include <cstdio>
//#include <cstring>

LCDSPI::LCDSPI(int latch) : latchPin(latch), clockPin(3), dataPin(9), delaytime(DEFAULTTIME)
{
   SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_CLOCK_DIV128);
   Initialise();
}

void LCDSPI::Initialise()
{
   pinMode(latchPin, OUTPUT);     
	digitalWrite(latchPin, LOW);

	delayMicroseconds(delaytime);

	WriteCommand(FUNCTION_SET);   
	WriteCommand(DISPLAY_CONTROL | DISPLAY_ON);   
	WriteCommand(DISPLAY_CLEAR);
	WriteCommand(ENTRY_MODE_SET | MODE_I_D);
}

void LCDSPI::Close()
{
	WriteCommand(FUNCTION_SET);   
	WriteCommand(DISPLAY_CONTROL);   
	WriteCommand(DISPLAY_CLEAR);
	WriteCommand(ENTRY_MODE_SET | MODE_I_D);
}

void LCDSPI::WriteByte(int dat)
{  
   digitalWrite(latchPin, HIGH);
	delayMicroseconds(delaytime);
	SPI.transfer(dat);
	digitalWrite(latchPin, LOW);
}

void LCDSPI::WriteCommand(int cmd)
{
   int H_data, L_data;
   
	H_data = cmd;
	H_data &= 0xf0;
	L_data = cmd;
	L_data &= 0x0f;
	L_data <<= 4;
	WriteByte(0xf8);
	WriteByte(H_data);
	WriteByte(L_data);
}

void LCDSPI::WriteData(int data)
{
   int H_data, L_data;
   
	H_data = data;
	H_data &= 0xf0;
	L_data = data;
	L_data &= 0x0f;
	L_data <<= 4;
	WriteByte(0xfa);
	WriteByte(H_data);
	WriteByte(L_data);
}

void LCDSPI::WriteReverseData(int data)
{
   int H_data, L_data;
   
//mov al, Source      //al = 76543210
   H_data = data;   
//mov bl, al          //bl = 76543210
   L_data = data;
//shr al, 4           //al = xxxx7654
   H_data >>= 4;
//shl bl, 4           //bl = 3210xxxx
   L_data <<= 4;
//or  al, bl          //al = 32107654
   H_data |= L_data;
//mov bl, al          //bl = 32107654
   L_data = H_data;
//and al, 0xCC        //al = 32xx76xx
   H_data &= 0xCC;
//and bl, 0x33        //bl = xx10xx54
   L_data &= 0x33;
//shr al, 2           //al = xx32xx76
   H_data >>= 2;
//shl bl, 2           //bl = 10xx54xx
   L_data <<= 2;
//or  al, bl          //al = 10325476
   H_data |= L_data;
//mov bl, al          //bl = 10325476
   L_data = H_data;
//and al, 0xAA        //al = 1x3x5x7x
   H_data &= 0xAA;
//and bl, 0x55        //bl = x0x2x4x6
   L_data &= 0x55;
//shr al, 1           //al = x1x3x5x7
   H_data >>= 1;
//shl bl, 1           //bl = 0x2x4x6x
   L_data <<= 1;
//or al, bl           //al = 01234567 
   H_data |= L_data;   
   data = H_data;
   
   WriteData(data);
}

void LCDSPI::Clear(void)
{  
    WriteCommand(FUNCTION_SET);
    WriteCommand(DISPLAY_CLEAR);
}

void LCDSPI::ReturnHome(void)
{  
    WriteCommand(FUNCTION_SET);
    WriteCommand(RETURN_HOME);
}

void LCDSPI::DisplayString(int X, int Y, uchar *ptr, int dat)
{
   int i;

	switch(X)
   {
		case 0:  Y |= 0x80; break;
		case 1:  Y |= 0x90; break;
		case 2:  Y |= 0x88; break;
		case 3:  Y |= 0x98; break;
		default: return;
	}

	WriteCommand(Y); // Set DDRAM Address

	for(i=0;i<dat;i++)
   { 
		WriteData(ptr[i]);
	}
}

void LCDSPI::DisplaySig(int M, int N, int sig)
{
   switch(M)
   {
      case 0:  N |= 0x80; break;
      case 1:  N |= 0x90; break;
      case 2:  N |= 0x88; break;
      case 3:  N |= 0x98; break;
      default: break;
   }
   
   WriteCommand(N);
   WriteData(sig);
}

void LCDSPI::DrawFullScreen(uchar *p)
{
   int ygroup,x,y,i;
   int temp;
   int tmp;
          
   for(ygroup=0;ygroup<64;ygroup++)
   {                           
      if(ygroup<32)
      {
         x = 0x80;
         y = ygroup + 0x80;
      }
      else 
      {
         x = 0x88;
         y = ygroup - 32 + 0x80;    
      }         
      WriteCommand(0x34); // extended instruction set, graphic display OFF
      WriteCommand(y); // Set the vertical address first
      WriteCommand(x); // and followed the horizontal address by consecutive writings
      WriteCommand(0x30); // basic instruction set

      tmp = ygroup * 16;
      for(i=0;i<16;i++)
      {        
         temp = p[tmp];
         WriteData(temp);                 
         tmp += 1;
      }
   }
   WriteCommand(0x34); // extended instruction set, graphic display OFF
   WriteCommand(0x36); // extended instruction set, graphic display ON
}

void LCDSPI::DrawXBM(uchar *p)
{
   int ygroup,x,y,i;
   int temp;
   int tmp;
          
   for(ygroup=0;ygroup<64;ygroup++)
   {                           
      if(ygroup<32)
      {
         x = 0x80;
         y = ygroup + 0x80;
      }
      else 
      {
         x = 0x88;
         y = ygroup - 32 + 0x80;    
      }         
      WriteCommand(0x34); // extended instruction set, graphic display OFF
      WriteCommand(y); // Set the vertical address first
      WriteCommand(x); // and followed the horizontal address by consecutive writings
      WriteCommand(0x30); // basic instruction set

      tmp = ygroup * 16;
      for(i=0;i<16;i++)
      {        
         temp = p[tmp];
         WriteReverseData(temp);                 
         tmp += 1;
      }
   }
   WriteCommand(0x34); // extended instruction set, graphic display OFF
   WriteCommand(0x36); // extended instruction set, graphic display ON
}

void LCDSPI::img1(uchar img[])
{
   unsigned int i;
   unsigned char page, column;  
   
   for(page=0xB0;page<0xB4;page++)  
   {  
      WriteCommand(page);  //set page address   
      WriteCommand(0x10);  //set Column address MSB   
      WriteCommand(0x04);  //set column address LSB 
      
      i = (0xB3-page)*128;
      
      for(column=0;column<128;column++)  
      {  
         WriteData(~img[i+column]);  
      } 
   } 
   WriteCommand(0x34);
   WriteCommand(0x36);
}

void LCDSPI::img2(uchar img[])
{
   unsigned int i;
   unsigned char page, column;  
   
   for(page=0xB4;page<0xB8;page++)  
   {  
      WriteCommand(page);  //set page address   
      WriteCommand(0x10);  //set Column address MSB   
      WriteCommand(0x04);  //set column address LSB 
      
      i = (0xB7-page)*128;
      
      for(column=0;column<128;column++)  
      {  
         WriteData(~img[i+column]);  
      } 
   }  
   WriteCommand(0x34);
   WriteCommand(0x36);
}
