DEBUG	= -O3
CC	= g++
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm

SRC	=	TSL2561.cpp main.cpp

OBJ	=	$(SRC:.cpp=.o)

BINS	=	main

main:	$(OBJ)
		$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS)

.cpp.o:
		$(CC) -c $(CFLAGS) $< -o $@

clean:
		rm -f $(OBJ) *~ core $(BINS)

depend:
		makedepend -Y $(SRC)

