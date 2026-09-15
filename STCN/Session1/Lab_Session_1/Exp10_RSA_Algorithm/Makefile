CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGETS = rsa

all: $(TARGETS)

rsa: rsa.c
	$(CC) $(CFLAGS) -o rsa rsa.c

clean:
	rm -f $(TARGETS) *.o
