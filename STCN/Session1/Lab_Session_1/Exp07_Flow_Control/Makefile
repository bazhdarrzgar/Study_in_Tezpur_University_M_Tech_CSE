CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGETS = stop_and_wait go_back_n selective_repeat

all: $(TARGETS)

stop_and_wait: stop_and_wait.c
	$(CC) $(CFLAGS) -o stop_and_wait stop_and_wait.c

go_back_n: go_back_n.c
	$(CC) $(CFLAGS) -o go_back_n go_back_n.c

selective_repeat: selective_repeat.c
	$(CC) $(CFLAGS) -o selective_repeat selective_repeat.c

clean:
	rm -f $(TARGETS) *.o
