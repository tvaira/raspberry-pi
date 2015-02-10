#ifndef DHT_H
#define DHT_H

/* DHT library 

MIT license
written by Adafruit Industries
*/

/* 2015 tv <tvaira@free.fr> */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <wiringPi.h>

#define MAX_TIME 85

#define DHT11 11
#define DHT22 22
#define DHT21 21
#define AM2301 21

#define DHT11PIN 7

class DHT 
{
   private:      
      uint8_t _pin, _type, _count;
      bool firstreading;
      unsigned long _lastreadtime;      
      uint8_t data[6];
   
      void start(void);
      bool read(void);
   
   public:
      DHT(uint8_t pin=DHT11PIN, uint8_t type=DHT11, uint8_t count=16);
      float readTemperature(bool celcius=true);
      float convertCtoF(float);
      float convertFtoC(float);
      float computeHeatIndex(float tempFahrenheit, float percentHumidity);
      float readHumidity(void);
      
      void readVal(void);
};

#endif
