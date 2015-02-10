#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "DS18B20.h"
#include "OneWireDefs.h"

/* 2015 tv <tvaira@free.fr> */

#define Thermometre DS18B20
#define DS18B20PIN 7  
 
int main()
{    
    Thermometre device(true, true, false, DS18B20PIN); // device( crcOn, useAddress, parasitic, int pin )
    
    while (!device.initialize()); 
    
    while (true)
    {
        // changing the resolutions only affects the DS18B20. The DS18S20 is fixed.
        device.setResolution(nineBit);
        device.readTemperature(); 
        sleep(2);
        //device.setResolution(tenBit);
        //device.readTemperature(); 
        //sleep(2);
        //device.setResolution(elevenBit);
        //device.readTemperature(); 
        //sleep(2);
        //device.setResolution(twelveBit);
        //device.readTemperature(); 
        //sleep(2);
    }
     
    return EXIT_SUCCESS;
}
