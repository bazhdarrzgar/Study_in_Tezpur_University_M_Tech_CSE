CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGETS = hamming

all: $(TARGETS)

hamming: hamming.c
	$(CC) $(CFLAGS) -o hamming hamming.c

clean:
	rm -f $(TARGETS) *.o
