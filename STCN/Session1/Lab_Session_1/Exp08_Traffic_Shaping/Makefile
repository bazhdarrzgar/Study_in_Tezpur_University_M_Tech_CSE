CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGETS = leaky_bucket token_bucket

all: $(TARGETS)

leaky_bucket: leaky_bucket.c
	$(CC) $(CFLAGS) -o leaky_bucket leaky_bucket.c

token_bucket: token_bucket.c
	$(CC) $(CFLAGS) -o token_bucket token_bucket.c

clean:
	rm -f $(TARGETS) *.o
