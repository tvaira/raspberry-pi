#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "BH1750.h"

/*
 * http://www.zartronic.fr/capteur-dintensit%C3%A9-lumineuse-lux-bh1750-p-421.html
 * 
 * 2015 tv <tvaira@free.fr>
 */

int main()
{
   BH1750 bh1750(BH1750_I2CADDR_L); // cf. doc
   float luminosite = 0.;
   
   bh1750.begin();
   while (true)
   {      
      luminosite = bh1750.readLightLevel();
      printf("Luminosite : %.2f lux\n", luminosite);
      sleep(2);        
   }
     
   return EXIT_SUCCESS;
} 
