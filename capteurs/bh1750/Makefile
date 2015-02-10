DEBUG	= -O3
CC	= g++
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm

SRC	=	BH1750.cpp busi2c.cpp main.cpp

OBJ	=	$(SRC:.cpp=.o)

BINS	=	bh1750

bh1750:	$(OBJ)
		$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS)

.cpp.o:
		$(CC) -c $(CFLAGS) $< -o $@

clean:
		rm -f $(OBJ) *~ core $(BINS)

depend:
		makedepend -Y $(SRC)

