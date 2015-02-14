/*! \file CAN.h
    \brief  Library for managing CAN-bus modules
    
    Copyright (C) 2011 Libelium Comunicaciones Distribuidas S.L.
    http://www.libelium.com
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.
   
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
  
    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
    Version:		0.1
    Design:		David Gascón
    Implementation:	Luis Antonio Martín Nuez

 */

/*! \def WaspCAN_h
    \brief The library flag
 */

/*
 * 2015 tv <tvaira@free.fr>
 * 
 * Ajouts :
 * 
 * - vitesse 100k
 * - gestion du mode étendu
 */ 

#ifndef CAN_h
#define CAN_h


/******************************************************************************
 * Includes
 ******************************************************************************/

#include "arduPi.h"

/******************************************************************************
 * Definitions & Declarations
 ******************************************************************************/

//CAN-bus connection pins

#define MODEL SEEEDSTUDIO

#if (MODEL == SEEEDSTUDIO ) /* bleu */

#define CAN_CS        9
#define CAN_INT       3

#elif (MODEL == SPARKFUN ) /* rouge */

#define CAN_CS        10
#define CAN_INT       2

#endif

//MCP2515 Opcodes 

#define SPI_RESET               0xC0
#define SPI_READ                0x03
#define SPI_READ_RX             0x90
#define SPI_WRITE               0x02
#define SPI_WRITE_TX            0x40
#define SPI_RTS                 0x80
#define SPI_READ_STATUS         0xA0
#define SPI_RX_STATUS           0xB0
#define SPI_BIT_MODIFY          0x05

#define LISTEN_ONLY_MODE	0x01
#define LOOPBACK_MODE		0x02
#define SLEEP_MODE		0x03
#define NORMAL_MODE		0x04

#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr,bit) \
	(!(_SFR_BYTE(sfr) & _BV(bit))) 

#define _BV(bit) (1 << (bit))
#define _SFR_BYTE(sfr) _MMIO_BYTE(_SFR_ADDR(sfr))
#define _MMIO_BYTE(mem_addr) (*(volatile uint8_t *)(mem_addr))
#define _SFR_ADDR(sfr) _SFR_MEM_ADDR(sfr)
#define _SFR_MEM_ADDR(sfr) ( &(sfr))



/****************************************************************************
                                MCP2515 REGISTERS
 ****************************************************************************/

//Message acceptance filters registers - Buffer 0

#define RXF0SIDH                0x00
#define RXF0SIDL                0x01
#define RXF0EID8                0x02
#define RXF0EID0                0x03
#define RXF1SIDH                0x04
#define RXF1SIDL                0x05
#define RXF1EID8                0x06
#define RXF1EID0                0x07
#define RXF2SIDH                0x08
#define RXF2SIDL                0x09
#define RXF2EID8                0x0A
#define RXF2EID0                0x0B

//Digital outputs/inputs registers

#define BFPCTRL                 0x0C
#define TXRTSCTRL               0x0D

//CAN-bus control and status registers

#define CANSTAT                 0x0E
#define CANCTRL                 0x0F

//Message acceptance filters registerts - Buffer 1

#define RXF3SIDH                0x10
#define RXF3SIDL                0x11
#define RXF3EID8                0x12
#define RXF3EID0                0x13
#define RXF4SIDH                0x14
#define RXF4SIDL                0x15
#define RXF4EID8                0x16
#define RXF4EID0                0x17
#define RXF5SIDH                0x18
#define RXF5SIDL                0x19
#define RXF5EID8                0x1A
#define RXF5EID0                0x1B

//Error counters registers

#define TEC                     0x1C
#define REC                     0x1D

//Message acceptance masks registers

#define RXM0SIDH                0x20
#define RXM0SIDL                0x21
#define RXM0EID8                0x22
#define RXM0EID0                0x23
#define RXM1SIDH                0x24
#define RXM1SIDL                0x25
#define RXM1EID8                0x26
#define RXM1EID0                0x27

//Bit timing registers

#define CNF3                    0x28
#define CNF2                    0x29
#define CNF1                    0x2A

//Interrups registers

#define CANINTE                 0x2B
#define CANINTF                 0x2C

//Error flags registers

#define EFLG                    0x2D

//Message transmission registers - buffer --> 0

#define TXB0CTRL                0x30
#define TXB0SIDH                0x31
#define TXB0SIDL                0x32
#define TXB0EID8                0x33
#define TXB0EID0                0x34
#define TXB0DLC                 0x35
#define TXB0D0                  0x36
#define TXB0D1                  0x37
#define TXB0D2                  0x38
#define TXB0D3                  0x39
#define TXB0D4                  0x3A
#define TXB0D5                  0x3B
#define TXB0D6                  0x3C
#define TXB0D7                  0x3D

//Message transmission registers - buffer --> 1

#define TXB1CTRL                0x40
#define TXB1SIDH                0x41
#define TXB1SIDL                0x42
#define TXB1EID8                0x43
#define TXB1EID0                0x44
#define TXB1DLC                 0x45
#define TXB1D0                  0x46
#define TXB1D1                  0x47
#define TXB1D2                  0x48
#define TXB1D3                  0x49
#define TXB1D4                  0x4A
#define TXB1D5                  0x4B
#define TXB1D6                  0x4C
#define TXB1D7                  0x4D

//Message transmission registers - buffer --> 2

#define TXB2CTRL                0x50
#define TXB2SIDH                0x51
#define TXB2SIDL                0x52
#define TXB2EID8                0x53
#define TXB2EID0                0x54
#define TXB2DLC                 0x55
#define TXB2D0                  0x56
#define TXB2D1                  0x57
#define TXB2D2                  0x58
#define TXB2D3                  0x59
#define TXB2D4                  0x5A
#define TXB2D5                  0x5B
#define TXB2D6                  0x5C
#define TXB2D7                  0x5D

//Message reception registers - buffer --> 0

#define RXB0CTRL                0x60
#define RXB0SIDH                0x61
#define RXB0SIDL                0x62
#define RXB0EID8                0x63
#define RXB0EID0                0x64
#define RXB0DLC                 0x65
#define RXB0D0                  0x66
#define RXB0D1                  0x67
#define RXB0D2                  0x68
#define RXB0D3                  0x69
#define RXB0D4                  0x6A
#define RXB0D5                  0x6B
#define RXB0D6                  0x6C
#define RXB0D7                  0x6D

//Message reception registers - buffer --> 1

#define RXB1CTRL                0x70
#define RXB1SIDH                0x71
#define RXB1SIDL                0x72
#define RXB1EID8                0x73
#define RXB1EID0                0x74
#define RXB1DLC                 0x75
#define RXB1D0                  0x76
#define RXB1D1                  0x77
#define RXB1D2                  0x78
#define RXB1D3                  0x79
#define RXB1D4                  0x7A
#define RXB1D5                  0x7B
#define RXB1D6                  0x7C
#define RXB1D7                  0x7D

//Control registers 

/** BFPCTRL **/
#define B1BFS                   5
#define B0BFS                   4
#define B1BFE                   3
#define B0BFE                   2
#define B1BFM                   1
#define B0BFM                   0

/** TXRTSCTRL **/
#define B2RTS                   5
#define B1RTS                   4
#define B0RTS                   3
#define B2RTSM                  2
#define B1RTSM                  1
#define B0RTSM                  0

/** CANSTAT **/
#define OPMOD2                  7
#define OPMOD1                  6
#define OPMOD0                  5
#define ICOD2                   3
#define ICOD1                   2
#define ICOD0                   1

/** CANCTRL **/
#define REQOP2                  7
#define REQOP1                  6
#define REQOP0                  5
#define ABAT                    4
#define CLKEN                   2
#define CLKPRE1                 1
#define CLKPRE0                 0

/** CNF3 **/
#define SOF                     7
#define WAKFIL                  6
#define PHSEG22                 2
#define PHSEG21                 1
#define PHSEG20                 0

/** CNF2 **/
#define BTLMODE                 7
#define SAM                     6
#define PHSEG12                 5
#define PHSEG11                 4
#define PHSEG10                 3
#define PHSEG2                  2
#define PHSEG1                  1
#define PHSEG0                  0

/** CNF1 **/
#define SJW1                    7
#define SJW0                    6
#define BRP5                    5
#define BRP4                    4
#define BRP3                    3
#define BRP2                    2
#define BRP1                    1
#define BRP0                    0

/** CANINTE **/
#define MERRE                   7
#define WAKIE                   6
#define ERRIE                   5
#define TX2IE                   4
#define TX1IE                   3
#define TX0IE                   2
#define RX1IE                   1
#define RX0IE                   0

/** CANINTF */
#define MERRF                   7
#define WAKIF                   6
#define ERRIF                   5
#define TX2IF                   4
#define TX1IF                   3
#define TX0IF                   2
#define RX1IF                   1
#define RX0IF                   0

/** EFLG **/
#define RX1OVR                  7
#define RX0OVR                  6
#define TXB0                    5
#define TXEP                    4
#define RXEP                    3
#define TXWAR                   2
#define RXWAR                   1
#define EWARN                   0

/** TXBnCTRL (n = 0, 1, 2) **/
#define ABTF                    6
#define MLOA                    5
#define TXERR                   4
#define TXREQ                   3
#define TXP1                    1
#define TXP0                    0

/** RXB0CTRL **/
#define RXM1                    6
#define RXM0                    5
#define RXRTR                   3
#define BUKT                    2
#define BUKT1                   1
#define FILHIT0                 0

/** RXB1CTRL **/
#define RSM1                    6
#define FILHIT2                 2
#define FILHIT1                 1

/** TXBnSIDL (n = 0, 1) **/
#define EXIDE                   3 // 0x08

/** RXBnSIDL (n = 0, 1) **/
#define SRR                     4
#define IDE                     3

/** RXBnDLC (n = 0, 1) / TXBnDLC **/
#define RTR                     6
#define DLC3                    3
#define DLC2                    2
#define DLC1                    1
#define DLC0                    0

#include <inttypes.h>

/******************************************************************************
 * Class
 ******************************************************************************/

  //! CAN Class
/*!
	CAN Class defines all the variables and functions used to manage CAN-bus modules
 */

class CAN
{

	private:
		// Write a register of the MCP2515 using SPI
		void writeRegister( char direction, char data );
		// Read a register of the MCP2515 using SPI
		char readRegister(char direction);		
		// Modify a bit of the MCP2515 using SPI
		void bitModify(char direction, char mask, char data);
		// Check the status of the MCP2515 registers 
		char readStatus(char type);
		// Check if the buffers are empty
		bool checkFreeBuffer(void);	
		// Reset MCP2515
		void reset(void);
		
      int can_cs;
      int can_int;
		char txbuf[5];
		char rxbuf[4];
		void write16(int num);

	public:
	
		// Data structure 
		typedef struct
      {
				unsigned int id;
            unsigned int ext;
				struct
				{ 
				  char rtr : 1;
				  char length : 4;
				}header;
			
				uint8_t data[8];
		} messageCAN;
	
		// Receive buffer
		messageCAN messageRx;
		// Trasmit buffer
		messageCAN messageTx;
		// Empty constructor
		CAN(int can_cs=CAN_CS, int can_int=CAN_INT, uint16_t speed=0);
		// Powers the CANBus module and opens the SPI
		bool begin(uint16_t speed);
		// Check if there is any message
		uint8_t messageAvailable(void);
		// Take the CAN message
		char getMessage(messageCAN *msje);
		// Send the CAN message
		char sendMessage(messageCAN *msje);
		// CAN message print out the serial port
		void printMessage(messageCAN *msje);
		// Configure the MCP2515
		void setMode(uint8_t mode);
};

#endif
