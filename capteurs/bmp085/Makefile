DEBUG	= -O3
CC	= g++
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm -lrt

SRC	=	BMP085.cpp bmp085test.cpp

OBJ	=	$(SRC:.cpp=.o)

BINS	=	bmp085test

bmp085test:	$(OBJ)
		$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS)

.cpp.o:
		$(CC) -c $(CFLAGS) $< -o $@

clean:
		rm -f $(OBJ) *~ core $(BINS)

depend:
		makedepend -Y $(SRC)

