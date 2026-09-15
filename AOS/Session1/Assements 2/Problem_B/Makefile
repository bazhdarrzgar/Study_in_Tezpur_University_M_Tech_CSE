CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread

all: proxy_server client

proxy_server: proxy_server.c
	$(CC) $(CFLAGS) -o proxy_server proxy_server.c

client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f proxy_server client *.o *.log

.PHONY: all clean
