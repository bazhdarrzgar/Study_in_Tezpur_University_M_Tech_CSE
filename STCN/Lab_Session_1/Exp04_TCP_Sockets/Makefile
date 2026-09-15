CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGETS = tcp_server tcp_client

all: $(TARGETS)

tcp_server: tcp_server.c
	$(CC) $(CFLAGS) -o tcp_server tcp_server.c

tcp_client: tcp_client.c
	$(CC) $(CFLAGS) -o tcp_client tcp_client.c

clean:
	rm -f $(TARGETS) *.o
