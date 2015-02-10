#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <stdint.h>

#include "BMP085.h"

/*
 * http://www.amazon.fr/gp/product/B00QPBHM9A/ref=pd_lpo_sbs_dp_ss_1
 * https://www.adafruit.com/product/391
 * https://www.sparkfun.com/products/11824
 *
 * 2015 tv <tvaira@free.fr>
 */

int main() 
{
   BMP085 bmp;

   while(1)
   {
      printf("Temperature : %.1f *C\n", bmp.readTemperature());   
      
      printf("Pression atmospherique : %.2f hPa\n", (float)bmp.readPressure()/100.);   
      
      // Calculate altitude assuming 'standard' barometric
      // pressure of 1013.25 millibar = 101325 Pascal
      //printf("Altitude = %.1f m\n", bmp.readAltitude());   
      
      //printf("Pressure at sealevel (calculated) = %.2f hPa\n", (float)bmp.readSealevelPressure()/100.);
      
      // you can get a more precise measurement of altitude
      // if you know the current sea level pressure which will
      // vary with weather and such.
      printf("Altitude : %.1f m\n", bmp.readAltitude(102500));   
   
      delay(2000);
      printf("\n\n");
   }
   
   return 0;
}
