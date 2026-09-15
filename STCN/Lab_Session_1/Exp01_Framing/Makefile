CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGETS = bit_stuffing byte_stuffing

all: $(TARGETS)

bit_stuffing: bit_stuffing.c
	$(CC) $(CFLAGS) -o bit_stuffing bit_stuffing.c

byte_stuffing: byte_stuffing.c
	$(CC) $(CFLAGS) -o byte_stuffing byte_stuffing.c

clean:
	rm -f $(TARGETS) *.o
