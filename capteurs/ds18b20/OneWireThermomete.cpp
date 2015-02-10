#include "OneWireThermomete.h"
#include "OneWireDefs.h"

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>  
#include <stdint.h>
#include <unistd.h>
 
// constructor specifies standard speed for the 1-Wire comms
OneWireThermomete::OneWireThermomete(bool crcOn, bool useAddr, bool parasitic, int pin, int device_id) :
    useCRC(crcOn), useAddress(useAddr), useParasiticPower(parasitic), 
    oneWire(pin, STANDARD), deviceId(device_id), resolution(twelveBit)
{
    // NOTE: the power-up resolution of a DS18B20 is 12 bits. The DS18S20's resolution is always
    // 9 bits + enhancement, but we treat the DS18S20 as fixed to 12 bits for calculating the
    // conversion time Tconv.
}
 
bool OneWireThermomete::initialize()
{
    // get the device address for use in selectROM() when reading the temperature
    // - not really needed except for device validation if using skipROM()
    if (useAddress)
    {
        //printf("\r\n");
        //printf("New Scan\r\n");
        printf("."); fflush(stdout);
 
        oneWire.resetSearch();    
        if (!oneWire.search(address))   // search for 1-wire device address
        {            
            //printf("No more addresses.\r\n");
            sleep(2);
            return false;
        }
 
        printf("\n\nAdresse = ");
        for (int i = 0; i < ADDRESS_SIZE; i++) 
        {
            printf("%02X ", (int)address[i]);
        }
        printf("\n");
        
        if (OneWireCRC::crc8(address, ADDRESS_CRC_BYTE) != address[ADDRESS_CRC_BYTE])   // check address CRC is valid
        {
            printf("CRC non valide !\n");
            sleep(2);
            return false;
        }
 
        if (address[0] != deviceId)
        {                    
            // Make sure it is a one-wire thermometer device
            if (DS18B20_ID == deviceId)
                printf("DS1820/DS18S20 non detecte !\r\n");
            else if (DS18S20_ID == deviceId)
                printf("DS18B20 non detecte !\n");
            else
              printf("DS18B20/DS1820/DS18S20 ?\n");
            
            sleep(2);
            return false;   
        }
        else
        {
            if (DS18B20_ID == deviceId) printf("DS18B20 detecte !\r\n");
            if (DS18S20_ID == deviceId) printf("DS1820/DS18S20 detecte !\r\n");            
        }
    }
    
    return true;
}
 
// NOTE ON USING SKIP ROM: ok to use before a Convert command to get all
// devices on the bus to do simultaneous temperature conversions. BUT can 
// only use before a Read Scratchpad command if there is only one device on the
// bus. For purpose of this library it is assumed there is only one device
// on the bus.
void OneWireThermomete::resetAndAddress()
{
    oneWire.reset();                // reset device 
    if (useAddress)
    {
        oneWire.matchROM(address);  // select which device to talk to
    }
    else
    {
        oneWire.skipROM();          // broadcast
    }
}
 
bool OneWireThermomete::readAndValidateData(BYTE* data)
{
    bool dataOk = true;
    
    resetAndAddress();
    oneWire.writeByte(READSCRATCH);    // read Scratchpad
 
    //printf("read = ");
    for (int i = 0; i < THERMOM_SCRATCHPAD_SIZE; i++) 
    {               
        // we need all bytes which includes CRC check byte
        data[i] = oneWire.readByte();
        //printf("%02X ", (int)data[i]);
    }
    //printf("\n");
 
    // Check CRC is valid if you want to
    if (useCRC && !(OneWireCRC::crc8(data, THERMOM_CRC_BYTE) == data[THERMOM_CRC_BYTE]))  
    {  
        // CRC failed
        printf("Erreur CRC ! ");
        printf("%02X != %02X\n", OneWireCRC::crc8(data, THERMOM_CRC_BYTE), data[THERMOM_CRC_BYTE]);
        dataOk = false;
    }
   
    if(dataOk != false) 
    {
      printf("read = ");
      for (int i = 0; i < THERMOM_SCRATCHPAD_SIZE; i++) 
      {               
         printf("%02X ", (int)data[i]);
      }
      printf("\n");
    }

    return dataOk;
}
 
float OneWireThermomete::readTemperature()
{
    BYTE data[THERMOM_SCRATCHPAD_SIZE];
    float realTemp = -999;
 
    resetAndAddress();
    oneWire.writeByte(CONVERT);     // issue Convert command
    
    if (useParasiticPower)
    {
        // delay while converting - Tconv (according to resolution of reading)
        delay(CONVERSION_TIME[resolution]);
    }
    else
    {
        // TODO
        // after the Convert command, the device should respond by transmitting 0
        // while the temperature conversion is in progress and 1 when the conversion is done
        // - as were are not checking this (TODO), we use Tconv, as we would do for
        // parasitic power 
        delay(CONVERSION_TIME[resolution]);
    }
 
    if (readAndValidateData(data))    // issue Read Scratchpad commmand and get data
    {
        realTemp = calculateTemperature(data);
    }
    
    return realTemp; 
}
