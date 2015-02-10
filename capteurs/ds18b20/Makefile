DEBUG	= -O3
CC	= g++
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm

SRC	=	DigitalInOut.cpp  DS18B20.cpp  main.cpp  OneWireCRC.cpp  OneWireThermomete.cpp

OBJ	=	$(SRC:.cpp=.o)

BINS	=	ds18b20

ds18b20:	$(OBJ)
		$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS)

.cpp.o:
		$(CC) -c $(CFLAGS) $< -o $@

clean:
		rm -f $(OBJ) *~ core $(BINS)

depend:
		makedepend -Y $(SRC)

