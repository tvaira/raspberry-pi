#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <stdint.h>

#include "TSL2561.h"

/*
 * http://www.adafruit.com/product/439
 * http://www.gotronic.fr/art-capteur-de-luminosite-tsl2561-19569.htm
 * 
 * 2015 tv <tvaira@free.fr>
 */

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type
*/
/**************************************************************************/
void displaySensorDetails(TSL2561 &tsl)
{
  sensor_t sensor;

  tsl.getSensor(&sensor);

  printf("------------------------------------\n");
  printf("Sensor :       %s\n", sensor.name);
  printf("Driver Ver :   %d\n", sensor.version);
  printf("Unique ID :    %d\n", sensor.sensor_id);
  printf("Max Value :    %0.2f lux\n", sensor.max_value);
  printf("Min Value :    %0.2f lux\n", sensor.min_value);
  printf("Resolution :   %0.2f lux\n", sensor.resolution);
  printf("------------------------------------\n");
  printf("\n");

  usleep(500*1000);
}

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureSensor(TSL2561 &tsl)
{
  /* You can also manually set the gain or enable auto-gain support */
  tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  //tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  //tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */

  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
}

/**************************************************************************/
/*
    Setup function
*/
/**************************************************************************/
void setup(TSL2561 &tsl)
{
  printf("Light Sensor Test\n");

  /* Initialise the sensor */
  if(!tsl.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    printf("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR !\n");
    //while(1);
    return;
  }

  /* Display some basic information on this sensor */
  displaySensorDetails(tsl);

  /* Setup the sensor gain and integration time */
  configureSensor(tsl);

  /* We're ready to go! */
  printf("\n");
}


int main( int argc, char **argv )
{
   TSL2561 tsl = TSL2561(TSL2561_ADDR_LOW, 12345);

   printf("-> Début du programme de test.\n");

   setup(tsl);

   /* Get a new sensor event */
   sensors_event_t event;
   while(1)
   {
      tsl.getEvent(&event);

      /* Display the results (light is measured in lux) */
      if (event.light)
      {
         printf("-> %0.2f lux\n", event.light);
      }
      else
      {
         /* If event.light = 0 lux the sensor is probably saturated
        and no reliable data could be generated! */
         printf("Sensor overload ! \n");
      }
      printf("\n");
      sleep(2);
   }

   printf("-> Fin du programme de test.\n");
   
   return 0;
}

