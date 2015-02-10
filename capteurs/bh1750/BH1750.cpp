#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <stdint.h>
#include <cstring>

#include "wiringPi.h"
#include "wiringPiI2C.h"

#include "busi2c.h"
#include "BH1750.h"

BH1750::BH1750(uint8_t adresse) 
{
   _adresse = adresse;
   
   #ifdef BUSI2C
   busi2c = new BusI2C;
   busi2c->set_i2c_bus(1);
   #ifdef BH1750_DEBUG   
   printf("adresse : 0x%02X\n", _adresse);
   #endif
   #else
   _bh1750CircuitID = wiringPiI2CSetup(adresse); 
   #ifdef BH1750_DEBUG   
   printf("adresse : 0x%02X (%d)\n", _adresse, _bh1750CircuitID);
   #endif
   #endif
}

void BH1750::begin(uint8_t mode) 
{
   //disable();
   configure(mode);  
}

void BH1750::configure(uint8_t mode) 
{
    //reset();
    //enable();
    switch (mode) 
    {
        case BH1750_CONTINUOUS_HIGH_RES_MODE:
        case BH1750_CONTINUOUS_HIGH_RES_MODE_2:
        case BH1750_CONTINUOUS_LOW_RES_MODE:
        case BH1750_ONE_TIME_HIGH_RES_MODE:
        case BH1750_ONE_TIME_HIGH_RES_MODE_2:
        case BH1750_ONE_TIME_LOW_RES_MODE:
            // apply a valid mode change            
            #ifdef BUSI2C
            busi2c->write(_adresse, mode, 0, NULL);
            #ifdef BH1750_DEBUG
            printf("0x%02X <- 0x%02X\n", _adresse, mode);
            #endif
            busi2c->delay_ms(180);            
            #else
            wiringPiI2CWrite(_bh1750CircuitID, mode);
            #ifdef BH1750_DEBUG
            printf("0x%02X <- 0x%02X (%d)\n", _adresse, mode, _bh1750CircuitID);
            #endif
            delay(180); // ms
            #endif
            break;
        default:
            // Invalid measurement mode
            #ifdef BH1750_DEBUG
            printf("Invalid measurement mode !\n");
            #endif
            break;
    }
}

float BH1750::readLightLevel(void) 
{   
   uint16_t level = 0;
   float lux = 0.;

   #ifdef BUSI2C
   unsigned char datas[2];
   if (busi2c->read(_adresse, 2, datas))
      return 0.;
   #ifdef BH1750_DEBUG   
   printf("0x%02X -> 0x%02X 0x%02X\n", _adresse, datas[0], datas[1]);   
   #endif
   level = datas[0] << 8;   
   level |= datas[1];   
   #else
   uint8_t datas[2];
   ::read(_bh1750CircuitID, datas, 2);
   #ifdef BH1750_DEBUG
   printf("0x%02X -> 0x%02X 0x%02X\n", _adresse, datas[0], datas[1]);   
   #endif
   level = datas[0] << 8;   
   level |= datas[1];   
   #endif

   lux = (float)level/1.2; // convert to lux

   #ifdef BH1750_DEBUG
   printf("Luminosite : %.2f lux (%d : 0x%04X)\n", lux, level, level);
   #endif

   return lux;
}

/*********************************************************************/

void BH1750::enable(void)
{
   #ifdef BUSI2C
   busi2c->write(_adresse, BH1750_POWER_ON, 0, NULL);
   #ifdef BH1750_DEBUG
   printf("0x%02X <- 0x%02X\n", _adresse, BH1750_POWER_ON);
   #endif
   #else
   wiringPiI2CWrite(_bh1750CircuitID, BH1750_POWER_ON);
   #ifdef BH1750_DEBUG
   printf("0x%02X <- 0x%02X (%d)\n", _adresse, BH1750_POWER_ON, _bh1750CircuitID);
   #endif
   #endif
}

void BH1750::disable(void)
{
   #ifdef BUSI2C
   busi2c->write(_adresse, BH1750_POWER_DOWN, 0, NULL);
   #ifdef BH1750_DEBUG
   printf("0x%02X <- 0x%02X\n", _adresse, BH1750_POWER_DOWN);
   #endif
   #else
   wiringPiI2CWrite(_bh1750CircuitID, BH1750_POWER_DOWN);
   #ifdef BH1750_DEBUG
   printf("0x%02X <- 0x%02X (%d)\n", _adresse, BH1750_POWER_DOWN, _bh1750CircuitID);
   #endif
   #endif
}

void BH1750::reset(void)
{
   enable();
   #ifdef BUSI2C
   busi2c->write(_adresse, BH1750_RESET, 0, NULL);
   printf("0x%02X <- 0x%02X\n", _adresse, BH1750_RESET);
   #else
   wiringPiI2CWrite(_bh1750CircuitID, BH1750_RESET);
   printf("0x%02X <- 0x%02X (%d)\n", _adresse, BH1750_RESET, _bh1750CircuitID);
   #endif
}
