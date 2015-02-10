#ifndef DIGITALINOUT_H
#define DIGITALINOUT_H
 
class DigitalInOut
{
public:
   DigitalInOut();
   DigitalInOut(int pin); //  	Create a DigitalInOut connected to the specified pin.
  	//DigitalInOut(int pin, int direction, int mode, int value); //  	Create a DigitalInOut connected to the specified pin.

   void write(int value); //  	Set the output, specified as 0 or 1 (int)
   int read(); //  	Return the output setting, represented as 0 or 1 (int)
   void output(); //  	Set as an output.
   void input(); //  	Set as an input.
   //void mode(PinMode pull); //  	Set the input pin mode.
   DigitalInOut& operator=(int value); //  	A shorthand for write()
  	operator int(); //  	A shorthand for read() 

private:
   int pin;
   int value; 
};
 
#endif
