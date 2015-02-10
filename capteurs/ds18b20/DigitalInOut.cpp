#include "DigitalInOut.h"
#include <wiringPi.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <stdint.h>

DigitalInOut::DigitalInOut() : pin(0), value(0)
{
}

DigitalInOut::DigitalInOut(int pin) : pin(pin), value(0)
{  
   wiringPiSetup();
   //printf("pin=%d ", pin);
}

/*DigitalInOut::DigitalInOut(int pin, int direction, int mode, int value) : pin(pin), value(value)
{
}*/

void DigitalInOut::write(int value)
{
   value = value & 0x01;
   //printf("write: %d\n", value);
   if(value == 0)
      digitalWrite(pin, LOW);
   //else if(value == 1)
   else
      digitalWrite(pin, HIGH);
   //this->value = value;
}

int DigitalInOut::read()
{
   this->value = digitalRead(pin);
   //printf("read: %d\n", value);
   return this->value;
}

void DigitalInOut::output()
{
   pinMode(pin, OUTPUT);
   //printf("mode OUTPUT :\n");
}

void DigitalInOut::input()
{
   pinMode(pin, INPUT);
   //printf("mode INPUT :\n");
}

/*void DigitalInOut::mode(PinMode pull)
{
}*/

DigitalInOut& DigitalInOut::operator=(int value)
{
   this->write(value);
   return *this;
}

DigitalInOut::operator int()
{
   return this->read();
}

