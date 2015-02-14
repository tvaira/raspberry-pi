/*
 *  Copyright (C) 2009 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
   
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
  
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Version:	0.1
 *  Design:		David Gascon
 *  Implementation:		Luis Antonio Martin & Ahmad Saad
 */

/*
 * 2015 tv <tvaira@free.fr>
 * 
 * Ajouts :
 * 
 * - vitesse 100k
 * - gestion du mode étendu
 */  

#include "CAN.h"


#define DEBUGMODE 1
/***********************************************************************
 * Constructors
 ***********************************************************************/

CAN::CAN(int can_cs, int can_int, uint16_t speed) : can_cs(can_cs), can_int(can_int)
{
   #if (DEBUGMODE == 1 )
		printf("-- Constructor Can(uint16_t speed) --\n");
      printf("CS = %d INT = %d\n", can_cs, can_int);
	#endif
   
   if(speed != 0)
      begin(speed);
   
   #if (DEBUGMODE == 1)
		printf("-- End Constructor Can(uint16_t speed) --\n");
		printf("\n");
	#endif
}


/***********************************************************************
 * PUBLIC METHODS
 ***********************************************************************/

//!*************************************************************
//!	Name:	ON()													
//!	Description: 	Initialization MCP2515
//!	Param : void														
//!	Returns: "1" if no error, "0" if error							
//!*************************************************************
bool CAN::begin(uint16_t speed)
{	
	//Initialization SPI
	SPI.begin();
	//SPI.detachInterrupt();	
	SPI.setBitOrder(MSBFIRST);
	// both mode 0 & 3 should work
	SPI.setDataMode(SPI_MODE0); 
	
	//Set the SPI frequency
	SPI.setClockDivider(SPI_CLOCK_DIV32);
	
	#if (DEBUGMODE==1)
		printf("SPI configured\n");
	#endif
	
	//Configure MCP2515 pins
	pinMode(can_cs,OUTPUT);
	pinMode(can_int,INPUT);
	
	//Software resets MCP2515
	reset();
	delay(10);
	
	//After the reset enters configuration mode	
	
	//Choose the rate of CAN-bus
	switch(speed)
   {	  
    	case 1000:
		#if (DEBUGMODE==1)
			printf("Speed=1Mbps\n");
		#endif
			digitalWrite(can_cs,LOW); 	
				
			SPI.transfer(SPI_WRITE);
			SPI.transfer(CNF3);
			SPI.transfer((1<<PHSEG21));	
			SPI.transfer((1<<BTLMODE)|(1<<PHSEG11));
				
			digitalWrite(can_cs,HIGH);		
      	break;

    	case 500:
		#if (DEBUGMODE==1)
			printf("Speed=500kps\n");
		#endif
			digitalWrite(can_cs,LOW);		
      		SPI.transfer(SPI_WRITE);
			SPI.transfer(CNF3);
			SPI.transfer((1<<PHSEG21));	
			SPI.transfer((1<<BTLMODE)|(1<<PHSEG11));
			SPI.transfer((1<<BRP0));
				
			digitalWrite(can_cs,HIGH);		  
		break;

    	case 250:
		#if (DEBUGMODE==1)
			printf("Speed=250kps\n");
		#endif
			digitalWrite(can_cs,LOW);		
			SPI.transfer(SPI_WRITE);
			SPI.transfer(CNF3);
			SPI.transfer((1<<PHSEG20)|(1<<PHSEG22));	
			SPI.transfer((1<<BTLMODE)|(1<<PHSEG12)|(1<<PHSEG11)|(1<<PHSEG10));
			SPI.transfer((1<<BRP0));					
					
			digitalWrite(can_cs,HIGH);		
		break;
      
      case 100:
		#if (DEBUGMODE==1)
			printf("Speed=100kps\n");
		#endif
			digitalWrite(can_cs,LOW);		
			SPI.transfer(SPI_WRITE);
			SPI.transfer(CNF3);
         SPI.transfer((1<<PHSEG20));	
			SPI.transfer((1<<BTLMODE)|(1<<PHSEG2)|(1<<PHSEG0)|(1<<PHSEG12)|(1<<PHSEG11));
			SPI.transfer((1<<SJW0)|(1<<BRP2));					        
					
			digitalWrite(can_cs,HIGH);		
		break;

		default:
		#if (DEBUGMODE==1)
			printf("The rate requested is unavailable, is set to 125 Kbit/s by default\n");
		#endif
			digitalWrite(can_cs,LOW);		
			SPI.transfer(SPI_WRITE);
			SPI.transfer(CNF3);
			SPI.transfer((1<<PHSEG21));	
			SPI.transfer((1<<BTLMODE)|(1<<PHSEG11));
			SPI.transfer((1<<BRP2)|(1<<BRP1)|(1<<BRP0));		
			digitalWrite(can_cs,HIGH);		
		break;

  	}
	
	//Enable interrupts the Rx Buffer 
	writeRegister(CANINTE,(1<<RX1IE)|(1<<RX0IE));
		
	//Filters and masks
  	//Bufer 0: All the messages and Rollover
  	writeRegister(RXB0CTRL,(1<<RXM1)|(1<<RXM0)|(1<<BUKT)); 
	
  	//Bufer 1: All the messages
	writeRegister(RXB1CTRL,(1<<RXM1)|(1<<RXM0)); 
	
	//All bits of the mask reception delete
 	writeRegister( RXM0SIDH, 0 );
 	writeRegister( RXM0SIDL, 0 );
 	writeRegister( RXM0EID8, 0 );
  	writeRegister( RXM0EID0, 0 );
 	writeRegister( RXM1SIDH, 0 );
   writeRegister( RXM1SIDL, 0 );
  	writeRegister( RXM1EID8, 0 );
  	writeRegister( RXM1EID0, 0 );
	
	//Disable pins RXnBF pins (high impedance state) 
	writeRegister( BFPCTRL, 0 );
	
	//Set normal mode
   //setMode(LOOPBACK_MODE);
	setMode(NORMAL_MODE);
	
	//Test its correct mode
	
	if (readRegister(CANSTAT) != 0) 
   {
		printf("Failed to initialize the MCP2515, normal mode not activated\n");
		return false;
	}

   #if (DEBUGMODE==1)
      printf("\n");
	#endif

	return 1;
}

//!*************************************************************
//!	Name:	getMessage()												
//!	Description: 	Take the CAN message								
//!	Param : messageCAN *rec_msje: pointer to reception buffer	
//!	Returns: "1" if no error, "0" if error							
//!*************************************************************
char CAN::getMessage(messageCAN *rec_msje)
{
	//Bought that buffer is the message
	char status = readStatus(SPI_RX_STATUS);
	char addr;
	static uint8_t previoSerialuffer;

	/*	
		if (bit_is_set(status,6)) {	    
			//If the message is in the Buffer 1
			addr = SPI_READ_RX;
		} 
		else if (bit_is_set(status,7)) 
		{	    
			//If the message is in the Buffer 2
			addr = SPI_READ_RX | 0x04;
		} 
		else {	    
			// Error, the message is not available
			return 0;
	  }*/

	if ( (((status & 0b11000000)>>6)&0b00000011) >2 )
	{
		addr=SPI_READ_RX | (previoSerialuffer++ & 0x01)<<2;
		#if (DEBUGMODE==1)
			printf("Data in buffer available\n");
		#endif
	}
	else if (bit_is_set(status,6)) 
	{		
		addr = SPI_READ_RX;
		#if (DEBUGMODE==1)
			printf("Data in buffer 0\n");
		#endif
	}
	else if (bit_is_set(status,7)) 
	{
		addr = SPI_READ_RX | 0x04;
		#if (DEBUGMODE==1)
			printf("Data in buffer 1\n");
		#endif
	}
	else {
		// Error: no message available
		#if (DEBUGMODE==1)
			printf("No messages");
		#endif
		return 0;
	}
	
	digitalWrite(can_cs,LOW);
	SPI.transfer(addr);
	
	//Read id 
   uint8_t sidh, sidl;
   sidh  = SPI.transfer(0xff);   //Read the top
	sidl  = SPI.transfer(0xff);   //Read the lower
   
	rec_msje->id  = (unsigned int) sidh << 3;
	rec_msje->id |=                sidl >> 5;
	
	//Return the Extended ID
	//SPI.transfer(0xff);
	//SPI.transfer(0xff);
   rec_msje->ext  = 0;
	
   if ( (sidl & 0x08) ==  0x08 ) 
   {
      /* extended id                  */
      rec_msje->id = (rec_msje->id << 2) + (sidl & 0x03);
      rec_msje->id = (rec_msje->id << 8) + SPI.transfer(0xff);
      rec_msje->id = (rec_msje->id << 8) + SPI.transfer(0xff);
      rec_msje->ext  = 1;
   }
   
	//Read DLC
	char length = SPI.transfer(0xff) & 0x0f;   
	if(length > 8)
      length = 8;
	rec_msje->header.length = length;
	rec_msje->header.rtr = (bit_is_set(status, 3)) ? 1 : 0;
	
	//Read data
   for (int i=0;i<length;i++) 
   {
      rec_msje->data[i] = SPI.transfer(0xFF);
   }
	    
	digitalWrite(can_cs,HIGH);
	
	//Delete the interruptions flags
	if (bit_is_set(status, 6)) 
   {
      bitModify(CANINTF, (1<<RX0IF), 0);
	}
	else 
   {
      bitModify(CANINTF, (1<<RX1IF), 0);
	}

	return status;
}



//!*************************************************************
//!	Name:	sendMessage()												
//!	Description: 	Send the CAN message
//!	Param: messageCAN *send_msje: pointer to transmission buffer
//!	Returns: "1" if no error, "0" if error							
//!*************************************************************
char CAN::sendMessage(messageCAN *send_msje)
{
	char status = readStatus(SPI_READ_STATUS);
	
	/* Status char:
	 * 
	 * Bit	Funcion
	 *  2	TXB0CNTRL.TXREQ
	 *  4	TXB1CNTRL.TXREQ
	 *  6	TXB2CNTRL.TXREQ
	 */
	
	char address;
	
	if (bit_is_clear(status, 2)) 
   {
		address = 0x00;
	} 
   else if (bit_is_clear(status, 4)) 
   {
		address = 0x02;
	} 
   else if (bit_is_clear(status, 6)) 
   {
		address = 0x04;
	} 
   else 
   {
		//All buffers are used can not send messages (returns 0)
      #if (DEBUGMODE==1)
      printf("All buffers are used can not send messages (returns 0) !\n");
      #endif
		return 0;
	}
	
	digitalWrite(can_cs,LOW);
	SPI.transfer(SPI_WRITE_TX | address);
   
   if(send_msje->ext == 0)
   {
      //First, send the top ID10....ID3
      SPI.transfer(send_msje->id >> 3);
      //After sending the lower ID2....ID0
      SPI.transfer((send_msje->id & 0x07) << 5);
      //As we will not use the Extended ID you set it to 0
      SPI.transfer(0);
      SPI.transfer(0);
   }
   else
   {     
      uint16_t canid;
      
      canid = (uint16_t)(send_msje->id & 0x0FFFF);
      
      txbuf[3] = (uint8_t) (canid & 0xFF);
      txbuf[2] = (uint8_t) (canid >> 8);
      canid = (uint16_t)(send_msje->id >> 16);
      txbuf[1] = (uint8_t) (canid & 0x03);
      txbuf[1] += (uint8_t) ((canid & 0x1C) << 3);
      txbuf[1] |= (1<<EXIDE);
      txbuf[0] = (uint8_t) (canid >> 5 );
      
      write16(4);	
   }
	
	char length = send_msje->header.length & 0x0F;
	
   if (send_msje->header.rtr) 
   {
      SPI.transfer((1<<RTR) | length);
   } 
   else 
   {
      //Send the message length
      SPI.transfer(length);		
      //Send data
      for (int i=0;i<length;i++) 
      {
         SPI.transfer(send_msje->data[i]);
      }
   }
	
	digitalWrite(can_cs,HIGH);
	
	//Send message
	digitalWrite(can_cs,LOW);
	address = (address == 0) ? 1 : address;
	SPI.transfer(SPI_RTS | address);
	digitalWrite(can_cs,HIGH);
   
   delay(10);
	
	return address;
}


//!*************************************************************
//!	Name:	printMessage()											
//!	Description: 	CAN message print out the serial port			
//!	Param : messageCAN *msje: pointer to the message				
//!	Returns: "1" if no error, "0" if error							
//!*************************************************************
void CAN::printMessage(messageCAN *msje)
{
	printf("  id: 0x%08X", msje->id);
   printf("  ext: %d",msje->ext);
	printf("  rtr: %d",msje->header.rtr);
	printf("  length: %d",msje->header.length);
	printf(" => ");
    
	if (!msje->header.rtr) 
   {
		//Data
		printf(" data:  ");
      for(int i = 0; i<msje->header.length; i++)
         printf("0x%02X ", msje->data[i]);
		printf("\n");
	}
}


//!*************************************************************
//!	Name:	setMode()													
//!	Description: 	Configure the MCP2515							
//!	Param : uint8_t mode: The work mode							
//!	Returns: void														
//!*************************************************************
void CAN::setMode(uint8_t mode)
{
	uint8_t mode_register = 0;
	
	if (mode == LISTEN_ONLY_MODE) 
   {
		mode_register = (0<<REQOP2)|(1<<REQOP1)|(1<<REQOP0);
	}
	else if (mode == LOOPBACK_MODE) 
   {
		mode_register = (0<<REQOP2)|(1<<REQOP1)|(0<<REQOP0);
	}
	else if (mode == SLEEP_MODE) 
   {
		mode_register = (0<<REQOP2)|(0<<REQOP1)|(1<<REQOP0);
	}
	else if (mode == NORMAL_MODE) 
   {
		mode_register = (0<<REQOP2)|(0<<REQOP1)|(0<<REQOP0);
	}
		
	//Set the new mode
	bitModify(CANCTRL, (1<<REQOP2)|(1<<REQOP1)|(1<<REQOP0), mode_register);
	
	//Wait until the mode has been changed
	while ((readRegister(CANSTAT) & 0xe0) != mode_register) {	}
}


//!*************************************************************
//!	Name:	messageAvailable()										
//!	Description: 	Check if there is any message					
//!	Param : void							
//!	Returns: 1 if available, 0 if not. 														
//!*************************************************************
uint8_t CAN::messageAvailable(void)
{
	char status = readStatus(SPI_RX_STATUS);

	if ( (((status & 0b11000000)>>6)&0b00000011) >2 )
	{		
		return 1;	
	} 
   else if (bit_is_set(status,6))  
   {
		return 1;
	} 
   else if (bit_is_set(status,7)) 
   {
		return 1;
	} 
   else  
   {	
		#if (DEBUGMODE==1)
			printf("No data available (0x%02X)\n", (unsigned char)status);
		#endif
      return 0;		
	}
}

/***********************************************************************
 * PRIVATE METHODS
 ***********************************************************************/

//Write a MCP2515 register
void CAN::writeRegister( char direction, char data )
{
	//CS low to select the MCP2515 
	digitalWrite(can_cs,LOW);

	/*SPI.transfer(SPI_WRITE);
	SPI.transfer(direction);
	SPI.transfer(data);*/
	
	txbuf[0] = SPI_WRITE;
	txbuf[1] = direction;	
	txbuf[2] = data;
	write16(3);	
	
	//CS line again to release 
	digitalWrite(can_cs,HIGH);
}


//**********************************************************************

//Read a MCP2515 register
char CAN::readRegister(char direction)
{
	//char data;
	
	//CS low to select the MCP2515 
	digitalWrite(can_cs,LOW);
	
	/*SPI.transfer(SPI_READ);
	SPI.transfer(direction);*/
	
	txbuf[0] = SPI_READ;
	txbuf[1] = direction;
	txbuf[2] = 0xFF;
	write16(3);	


	//Read data SPI
	//data = SPI.transfer(0xff);	

	
	//CS line again to release 
	digitalWrite(can_cs,HIGH);
	return rxbuf[2];
	//return data;
}


//***********************************************************************

//Modify a bit of the MCP2515 registers
void CAN::bitModify(char direction, char mask, char data)
{
	//CS low to select the MCP2515 
	digitalWrite(can_cs,LOW);
	
	/*SPI.transfer(SPI_BIT_MODIFY);
	SPI.transfer(direction);
	SPI.transfer(mask);
	SPI.transfer(data);*/
	
	txbuf[0] = SPI_BIT_MODIFY;
	txbuf[1] = direction;	
	txbuf[2] = mask;
	txbuf[3] = data;
	write16(4);	
	
	
	//CS line again to release 
	digitalWrite(can_cs,HIGH);
}


//***********************************************************************

//Check the status of the MCP2515 registers (SPI_RX_STATUS / SPI_READ_STATUS)
char CAN::readStatus(char type)
{
	//char data;
	
	//CS low to select the MCP2515 
	digitalWrite(can_cs,LOW);
		
	/*SPI.transfer(type);	
	//Read data SPI 
	data = SPI.transfer(0xFF); */
	 	
	txbuf[0] = type;
	txbuf[1] = 0xFF;
	write16(2);	
	 	
	 	
	//CS line again to release 
	digitalWrite(can_cs,HIGH);
	
	return rxbuf[1];

	//return data;
}


//***********************************************************************

//Check if the buffers are empty
bool CAN::checkFreeBuffer(void)
{
	char status = readStatus(SPI_READ_STATUS);
	
	
	if ((status & 0x54) == 0x54) 
	{	  
		//All buffers used
		return false;
	}
	
	return true;
}


//***********************************************************************

//Reset MCP2515
void CAN::reset(void)
{
	//CS low to select the MCP2515 
	digitalWrite(can_cs,LOW);	
	SPI.transfer(SPI_RESET);	
	//CS line again to release 
	digitalWrite(can_cs,HIGH);

	//Wait a bit to be stabilized after the reset MCP2515
	delay(10);
	
	#if (DEBUGMODE==1)
		printf("The MCP2515 has been successfully reset, configuration mode activated\n");
	#endif
}

void CAN::write16(int num)
{
	digitalWrite(can_cs,LOW);
	SPI.transfernb(txbuf, rxbuf, num);
	digitalWrite(can_cs,HIGH);	
}

