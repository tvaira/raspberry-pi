DEBUG	= -O3
CC	= g++
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lpthread -lm -lrt

SRC	=	arduPi.cpp Adafruit_GFX.cpp Print.cpp Adafruit_ST7735.cpp graphicstest.cpp

OBJ	=	$(SRC:.cpp=.o)

BINS	=	$(SRC:.cpp=)

graphicstest:	$(OBJ)
		$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS)

.cpp.o:
		$(CC) -c $(CFLAGS) $< -o $@

clean:
		rm -f $(OBJ) *~ core $(BINS)

depend:
		makedepend -Y $(SRC)

