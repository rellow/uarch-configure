CC = gcc
CFLAGS = -O2 -Wall
LFLAGS = -lm

all:	intel-prefetch-disable

intel-prefetch-disable:	intel-prefetch-disable.o
	$(CC) -o intel-prefetch-disable intel-prefetch-disable.o $(LFLAGS)

intel-prefetch-disable.o:	intel-prefetch-disable.c
	$(CC) $(CFLAGS) -c intel-prefetch-disable.c

clean:	
	rm -f *.o *~ intel-prefetch-disable

