#ifndef ONEWIRETHERMOMETE_H
#define ONEWIRETHERMOMETE_H
 
#include "OneWireCRC.h"
#include "OneWireDefs.h"
 
typedef unsigned char BYTE;    // something a byte wide
 
class OneWireThermomete
{
public:
    OneWireThermomete(bool crcOn, bool useAddr, bool parasitic, int pin, int device_id);
    
    bool initialize();
    float readTemperature();
    virtual void setResolution(eResolution resln) = 0; 
 
protected:
    const bool useCRC;
    const bool useAddress;
    const bool useParasiticPower;
    OneWireCRC oneWire;
    const int deviceId;
    eResolution resolution; 
    BYTE address[8];
    
    void resetAndAddress();
    bool readAndValidateData(BYTE* data);
    virtual float calculateTemperature(BYTE* data) = 0;    // device specific
};
 
#endif
