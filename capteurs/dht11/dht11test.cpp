#include "DHT.h"

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

/*
 * http://www.dfrobot.com/index.php?route=product/product&product_id=174
 * http://www.adafruit.com/product/386
 * 
 * 2015 tv <tvaira@free.fr>
 */

int main() 
{
   DHT dht;
   
   while(1)
   {
      // Wait a few seconds between measurements.
      delay(3000);
      //dht.readVal();
   
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float h = dht.readHumidity();
      // Read temperature as Celsius
      float tc = dht.readTemperature();
      // Read temperature as Fahrenheit : 
      float tf = dht.readTemperature(false);
   
      // Compute heat index
      // Must send in temp in Fahrenheit!
      float hi;
      if (!isnan(tf) && !isnan(h)) 
         hi = dht.computeHeatIndex(tf, h);
      else
         hi = NAN;
   
      if (!isnan(h)) 
         printf("Humidity: %.1f %%\n", h); 
      if (!isnan(tc)) 
         printf("Temperature: %.1f *C (%.1f *F)\n", tc, dht.convertCtoF(tc)); 
      if (!isnan(tf)) 
         printf("Temperature: %.1f *F (%.1f *C)\n", tf, dht.convertFtoC(tf)); 
      if (!isnan(hi)) 
      {
         printf("Heat index: %.1f *F\n", hi);
         printf("Ressenti de température de l'air : %.1f *C\n", dht.convertFtoC(hi)); 
      }
   }
   
   return 0;
}
