/*************************************************** 
  This is a library for the Adafruit BMP085/BMP180 Barometric Pressure + Temp sensor

  Designed specifically to work with the Adafruit BMP085 or BMP180 Breakout 
  ----> http://www.adafruit.com/products/391
  ----> http://www.adafruit.com/products/1603

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

/*
 * portage vers la Raspberry Pi (wiringPi)
 * 
 * 2015 tv <tvaira@free.fr>
 * 
 */

#include "BMP085.h"

BMP085::BMP085() 
{
   m_bmp085CircuitID = -1;
   begin();
}

bool BMP085::begin(uint8_t mode) 
{
   if (mode > BMP085_ULTRAHIGHRES) 
      mode = BMP085_ULTRAHIGHRES;
   oversampling = mode;

   m_bmp085CircuitID = wiringPiI2CSetup(BMP085_I2CADDR);
   #ifdef BMP085_DEBUG
   printf("<begin> 0x%02X : %d\n", BMP085_I2CADDR, m_bmp085CircuitID);   
   #endif

   if (read8(0xD0) != 0x55) return false;

   /* read calibration data */
   ac1 = read16(BMP085_CAL_AC1);
   ac2 = read16(BMP085_CAL_AC2);
   ac3 = read16(BMP085_CAL_AC3);
   ac4 = read16(BMP085_CAL_AC4);
   ac5 = read16(BMP085_CAL_AC5);
   ac6 = read16(BMP085_CAL_AC6);
   
   b1 = read16(BMP085_CAL_B1);
   b2 = read16(BMP085_CAL_B2);
   
   mb = read16(BMP085_CAL_MB);
   mc = read16(BMP085_CAL_MC);
   md = read16(BMP085_CAL_MD);

   return true;
}

int32_t BMP085::computeB5(int32_t UT) 
{
   int32_t X1 = (UT - (int32_t)ac6) * ((int32_t)ac5) >> 15;
   int32_t X2 = ((int32_t)mc << 11) / (X1+(int32_t)md);
   
   return X1 + X2;
}

uint16_t BMP085::readRawTemperature(void) 
{
   write8(BMP085_CONTROL, BMP085_READTEMPCMD);
   delay(5);
   
   return read16(BMP085_TEMPDATA);
}

uint32_t BMP085::readRawPressure(void) 
{
   uint32_t raw;
   
   write8(BMP085_CONTROL, BMP085_READPRESSURECMD + (oversampling << 6));
   
   if (oversampling == BMP085_ULTRALOWPOWER) 
      delay(5);
   else if (oversampling == BMP085_STANDARD) 
      delay(8);
   else if (oversampling == BMP085_HIGHRES) 
      delay(14);
   else 
      delay(26);
   
   raw = read16(BMP085_PRESSUREDATA);
   
   raw <<= 8;
   raw |= read8(BMP085_PRESSUREDATA+2);
   raw >>= (8 - oversampling);

 /* this pull broke stuff, look at it later?
   if (oversampling==0) 
   {
      raw <<= 8;
      raw |= read8(BMP085_PRESSUREDATA+2);
      raw >>= (8 - oversampling);
   }
 */

   return raw;
}

int32_t BMP085::readPressure(void) 
{
   int32_t UT, UP, B3, B5, B6, X1, X2, X3, p;
   uint32_t B4, B7;
   
   UT = readRawTemperature();
   UP = readRawPressure();

  // use datasheet numbers!
  /*UT = 27898;
  UP = 23843;
  ac6 = 23153;
  ac5 = 32757;
  mc = -8711;
  md = 2868;
  b1 = 6190;
  b2 = 4;
  ac3 = -14383;
  ac2 = -72;
  ac1 = 408;
  ac4 = 32741;
  oversampling = 0;*/

   B5 = computeB5(UT);

#ifdef BMP085_DEBUG_CALCUL
  printf("X1 = %d\n", X1);
  printf("X2 = %d\n", X2);
  printf("B5 = %d\n", B5);
#endif

   // do pressure calcs
   B6 = B5 - 4000;
   X1 = ((int32_t)b2 * ( (B6 * B6)>>12 )) >> 11;
   X2 = ((int32_t)ac2 * B6) >> 11;
   X3 = X1 + X2;
   B3 = ((((int32_t)ac1*4 + X3) << oversampling) + 2) / 4;

#ifdef BMP085_DEBUG_CALCUL
  printf("B6 = %d\n", B6);
  printf("X1 = %d\n", X1);
  printf("X2 = %d\n", X2);
  printf("B3 = %d\n", B3);
#endif

   X1 = ((int32_t)ac3 * B6) >> 13;
   X2 = ((int32_t)b1 * ((B6 * B6) >> 12)) >> 16;
   X3 = ((X1 + X2) + 2) >> 2;
   B4 = ((uint32_t)ac4 * (uint32_t)(X3 + 32768)) >> 15;
   B7 = ((uint32_t)UP - B3) * (uint32_t)( 50000UL >> oversampling );

#ifdef BMP085_DEBUG_CALCUL
  printf("X1 = %d\n", X1);
  printf("X2 = %d\n", X2);
  printf("B4 = %d\n", B4);
  printf("B7 = %d\n", B7);
#endif

   if (B7 < 0x80000000) 
   {
      p = (B7 * 2) / B4;
   } 
   else 
   {
      p = (B7 / B4) * 2;
   }
   X1 = (p >> 8) * (p >> 8);
   X1 = (X1 * 3038) >> 16;
   X2 = (-7357 * p) >> 16;

#ifdef BMP085_DEBUG_CALCUL
  printf("p = %d\n", p);
  printf("X1 = %d\n", X1);
  printf("X2 = %d\n", X2);
#endif

   p = p + ((X1 + X2 + (int32_t)3791)>>4);
#ifdef BMP085_DEBUG_CALCUL
  printf("p = %d\n", p);
#endif
   return p;
}

int32_t BMP085::readSealevelPressure(float altitude_meters) 
{
   float pressure = readPressure();
   
   return (int32_t)(pressure / pow(1.0-altitude_meters/44330, 5.255));
}

float BMP085::readTemperature(void) 
{
   int32_t UT, B5;     // following ds convention
   float temp;

   UT = readRawTemperature();

   // use datasheet numbers!
   /*UT = 27898;
   ac6 = 23153;
   ac5 = 32757;
   mc = -8711;
   md = 2868;*/

   B5 = computeB5(UT);
   temp = (B5+8) >> 4;
   temp /= 10;
  
   return temp;
}

float BMP085::readAltitude(float sealevelPressure) 
{
   float altitude;

   float pressure = readPressure();

   altitude = 44330 * (1.0 - pow(pressure /sealevelPressure,0.1903));

   return altitude;
}

/*********************************************************************/
uint8_t BMP085::read8(uint8_t a) 
{
   uint8_t data = 0;
   
   if(m_bmp085CircuitID != -1)
   {
      wiringPiI2CWrite(m_bmp085CircuitID, a);   
      ::read(m_bmp085CircuitID, &data, 1);   
      #ifdef BMP085_DEBUG 
      printf("<read8> 0x%02X -> 0x%02X\n", a, data);   
      #endif
   }
   
   return data;
}

uint16_t BMP085::read16(uint8_t a) 
{
   uint16_t data = 0;      
   
   if(m_bmp085CircuitID != -1)
   {
      wiringPiI2CWrite(m_bmp085CircuitID, a);
      uint8_t datas[2];
      ::read(m_bmp085CircuitID, datas, 2);
      data = datas[0] << 8;   
      data |= datas[1];     
      #ifdef BMP085_DEBUG 
      printf("<read16> 0x%02X -> 0x%04X\n", a, data);   
      #endif
   }

   return data;
}

void BMP085::write8(uint8_t a, uint8_t d) 
{
   if(m_bmp085CircuitID != -1)
   {
      uint8_t buffer[2];
      buffer[0] = a;
      buffer[1] = d;
      ::write(m_bmp085CircuitID, buffer, 2);	
      #ifdef BMP085_DEBUG
      printf("<write8> 0x%02X <- 0x%02X\n", buffer[0], buffer[1]); 
      #endif
   }
}
