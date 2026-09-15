CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGETS = shortest_path_dijkstra distance_vector_routing

all: $(TARGETS)

shortest_path_dijkstra: shortest_path_dijkstra.c
	$(CC) $(CFLAGS) -o shortest_path_dijkstra shortest_path_dijkstra.c

distance_vector_routing: distance_vector_routing.c
	$(CC) $(CFLAGS) -o distance_vector_routing distance_vector_routing.c

clean:
	rm -f $(TARGETS) *.o
