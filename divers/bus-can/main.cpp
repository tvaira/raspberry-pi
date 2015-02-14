/* 
 *        ------ CAN Bus Basic Example --------
 * 
 *  This sketch shows how to send data through CAN Bus standard.
 *  
 *  Copyright (C) 2014 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Version:          0.1
 *  Implementation:   Luis Antonio Martin  & Ahmad Saad
 */

// 2015 tv <tvaira@free.fr>

#include "CAN.h"
#include "arduPi.h"

/* Model seeestudio (bleu) */

#define CAN_CS_SEEEDSTUDIO     9
#define CAN_INT_SEEEDSTUDIO    3

/* Model sparkfun (rouge) */

#define CAN_CS_SPARKFUN        10
#define CAN_INT_SPARKFUN       2

// Create an instance of the object
CAN myCAN = CAN(CAN_CS_SPARKFUN, CAN_INT_SPARKFUN);

// Setting up our devices and I/Os
void setup() 
{
  // 250: 250Kbp etc ...
  myCAN.begin(500);
}

void showDatas(CAN::messageCAN *message)
{
   int i;

   for(i = 0; i<message->header.length; i++)
   {
      printf("0x%02X ", message->data[i]);
   }
   putchar('\n');
}

void initMessage(CAN::messageCAN *message)
{
   int i;

   //reset datas
   for(i=0;i<8;i++)
   {
      message->data[i] = 0x00;
   }
   message->header.length = 0;
}

void loop() 
{
  //****************************************
  // 1. Receive data
  //****************************************
  
  if (myCAN.messageAvailable() == 1) 
  {
    // Read the last message received.
    myCAN.getMessage(&myCAN.messageRx);
    // Print in the serial monitor the received message
    myCAN.printMessage(&myCAN.messageRx);
  }
  
  // A time delay
  delay(250);
}

int main ()
{
   setup();
   
   while(1)
   {
      loop();
   }
   
   return 0;
}

