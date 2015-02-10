#ifndef ONEWIRECRC_H
#define ONEWIRECRC_H

#include "DigitalInOut.h"

// Select the table-lookup method of computing the 8-bit CRC by setting this to 1
#ifndef ONEWIRE_CRC8_TABLE
#define ONEWIRE_CRC8_TABLE 1
#endif
 
typedef unsigned char BYTE;    // used to be uint8_t : something a byte wide, whatever ....
 
enum eSpeed {OVERDRIVE, STANDARD};
 
class OneWireCRC
{
public:
    OneWireCRC(int oneWire, eSpeed);
    
    // reset, read, write functions
    int reset();
    void writeByte(int data);
    int readByte();
    int touchByte(int data);
    void block(BYTE* data, int data_len);
    int overdriveSkip(BYTE* data, int data_len);
    
    // address functions
    void matchROM(BYTE rom[8]);
    void skipROM();
 
    // address search functions
    void resetSearch();
    BYTE search(BYTE* newAddr);
 
    // CRC check functions
    static BYTE crc8(BYTE* addr, BYTE len);
    static unsigned short crc16(unsigned short* data, unsigned short len);
 
private:
    const int* timing;
    
    BYTE address[8];
    int searchJunction;        // so we can set to it -1 somewhere
    bool searchExhausted;
    
    DigitalInOut oneWirePort;
    
    // read/write bit functions
    void writeBit(int bit);
    int readBit();
};
 
#endif
