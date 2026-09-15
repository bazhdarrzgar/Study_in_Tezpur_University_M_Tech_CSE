CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGETS = caesar_cipher substitution_cipher

all: $(TARGETS)

caesar_cipher: caesar_cipher.c
	$(CC) $(CFLAGS) -o caesar_cipher caesar_cipher.c

substitution_cipher: substitution_cipher.c
	$(CC) $(CFLAGS) -o substitution_cipher substitution_cipher.c

clean:
	rm -f $(TARGETS) *.o
