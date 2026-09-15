CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGETS = udp_server udp_client

all: $(TARGETS)

udp_server: udp_server.c
	$(CC) $(CFLAGS) -o udp_server udp_server.c

udp_client: udp_client.c
	$(CC) $(CFLAGS) -o udp_client udp_client.c

clean:
	rm -f $(TARGETS) *.o
