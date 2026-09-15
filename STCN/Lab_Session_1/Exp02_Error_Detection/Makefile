CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGETS = parity checksum crc

all: $(TARGETS)

parity: parity.c
	$(CC) $(CFLAGS) -o parity parity.c

checksum: checksum.c
	$(CC) $(CFLAGS) -o checksum checksum.c

crc: crc.c
	$(CC) $(CFLAGS) -o crc crc.c

clean:
	rm -f $(TARGETS) *.o
