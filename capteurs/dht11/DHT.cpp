#include "DHT.h"

DHT::DHT(uint8_t pin, uint8_t type, uint8_t count) 
{
   _pin = pin;
   _type = type;
   _count = count;
   firstreading = true;
   
   if(wiringPiSetup()==-1)
      return;
   
   pinMode(_pin, INPUT);
   digitalWrite(_pin, HIGH);
   
   _lastreadtime = 0;
}

void DHT::start(void)
{
   /*pinMode(_pin, OUTPUT);
   digitalWrite(_pin, LOW);
   delay(18);
   digitalWrite(_pin, HIGH);
   delayMicroseconds(40);
   pinMode(_pin, INPUT);*/
   
   // pull the pin high and wait 250 milliseconds
   digitalWrite(_pin, HIGH);
   delay(250);
   
   // now pull it low for ~20 milliseconds
   pinMode(_pin, OUTPUT);
   digitalWrite(_pin, LOW);
   delay(20);

   digitalWrite(_pin, HIGH);
   delayMicroseconds(40);
   pinMode(_pin, INPUT);
  
   _lastreadtime = 0;
}

float DHT::readTemperature(bool celcius) 
{
   float f;
   
   if (read()) 
   {
      switch (_type) 
      {
         case DHT11:
            f = data[2];
            if(!celcius)
               f = convertCtoF(f);               
            return f;
         case DHT22:
         case DHT21:
            f = data[2] & 0x7F;
            f *= 256;
            f += data[3];
            f /= 10;
            if (data[2] & 0x80)
            f *= -1;
            if(!celcius)
               f = convertCtoF(f);            
            return f;
      }
   }
   return NAN;
}

float DHT::convertCtoF(float c) 
{
	return c * 9. / 5. + 32.;
}

float DHT::convertFtoC(float f) 
{
  return (f - 32.) * 5. / 9.; 
}

float DHT::readHumidity(void) 
{
   float f;
   
   if (read()) 
   {
    switch (_type) 
    {
       case DHT11:
         f = data[0];
         return f;
       case DHT22:
       case DHT21:
         f = data[0];
         f *= 256;
         f += data[1];
         f /= 10;
         return f;
    }
   }
   return NAN;
}

float DHT::computeHeatIndex(float tempFahrenheit, float percentHumidity) 
{
  // Adapted from equation at: https://github.com/adafruit/DHT-sensor-library/issues/9 and
  // Wikipedia: http://en.wikipedia.org/wiki/Heat_index
  return -42.379 + 
           2.04901523 * tempFahrenheit + 
          10.14333127 * percentHumidity +
          -0.22475541 * tempFahrenheit*percentHumidity +
          -0.00683783 * pow(tempFahrenheit, 2) +
          -0.05481717 * pow(percentHumidity, 2) + 
           0.00122874 * pow(tempFahrenheit, 2) * percentHumidity + 
           0.00085282 * tempFahrenheit*pow(percentHumidity, 2) +
          -0.00000199 * pow(tempFahrenheit, 2) * pow(percentHumidity, 2);
}

bool DHT::read(void) 
{
   uint8_t laststate = HIGH;
   uint8_t counter = 0;
   uint8_t j = 0, i;
   unsigned long currenttime;
   
   // Check if sensor was read less than two seconds ago and return early
   // to use last reading.
   currenttime = millis();
   if (currenttime < _lastreadtime) 
   {
      // ie there was a rollover
      _lastreadtime = 0;
   }
   if (!firstreading && ((currenttime - _lastreadtime) < 2000)) 
   {
      return true; // return last correct measurement
      //delay(2000 - (currenttime - _lastreadtime));
   }
   firstreading = false;

   _lastreadtime = millis();
   
   data[0] = data[1] = data[2] = data[3] = data[4] = 0;
   
   start();
   
   // read in timings
   for ( i=0; i < MAX_TIME; i++) 
   {
      counter = 0;
      while (digitalRead(_pin) == laststate) 
      {
         counter++;
         delayMicroseconds(1);
         if (counter == 255) 
         {
           break;
         }
      }
      laststate = digitalRead(_pin);
      
      if (counter == 255) break;
      
      // ignore first 3 transitions
      if ((i >= 4) && (i%2 == 0)) 
      {
         // shove each bit into the storage bytes
         data[j/8] <<= 1;
         if (counter > _count)
            data[j/8] |= 1;
         j++;
      }   
   }
   
   // check we read 40 bits and that the checksum matches
   if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) 
   {
      //printf("true !\n");
      return true;
   }   
   
   //printf("false !\n");
   return false;
}

void DHT::readVal(void)
{
   uint8_t lststate = HIGH;
   uint8_t counter = 0;
   uint8_t j = 0, i;
   float farenheit;
  
   for(i=0;i<6;i++)
      data[i] = 0;
      
   start();
   
   for(i=0;i<MAX_TIME;i++)
   {
      counter = 0;
      while(digitalRead(_pin) == lststate)
      {
         counter++;
         delayMicroseconds(1);
         if(counter == 255)
            break;
      }
      lststate = digitalRead(_pin);
      if(counter == 255)
         break;
    
      // top 3 transistions are ignored
      if( (i>=4) && (i%2==0) )
      {
         data[j/8] <<= 1;
         if(counter > 16)
            data[j/8] |= 1;
         j++;
      }
   }
   // verify cheksum and print the verified data
   if( (j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) )
   {
      farenheit = data[2]*9./5.+32;
      printf("Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n", data[0], data[1], data[2], data[3], farenheit);
   }
   else
      printf("Invalid Data !\n");
}
