/**
 * Experiment 6(a): Shortest Path Routing using Dijkstra's Algorithm
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * Dijkstra's algorithm computes the shortest path from a single source router
 * to all other routers in a weighted network graph (non-negative weights).
 * It forms the basis of Link-State routing protocols like OSPF (Open Shortest Path First).
 *
 * Steps:
 * 1. Initialize dist[source] = 0, and dist[v] = INFINITY for all other nodes v.
 * 2. Maintain a set of unvisited nodes.
 * 3. In each iteration, select unvisited node u with the minimum distance.
 * 4. Mark u as visited.
 * 5. For each neighbor v of u:
 *      if dist[u] + cost(u, v) < dist[v]:
 *          dist[v] = dist[u] + cost(u, v)
 *          prev[v] = u
 * 6. Repeat until all reachable nodes are visited.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_NODES 20
#define INF 999999

void print_path(int parent[], int j, char node_names[]) {
    if (parent[j] == -1) {
        printf("%c", node_names[j]);
        return;
    }
    print_path(parent, parent[j], node_names);
    printf(" -> %c", node_names[j]);
}

int get_next_hop(int parent[], int dest, int source) {
    if (dest == source) return source;
    int curr = dest;
    while (parent[curr] != -1 && parent[curr] != source) {
        curr = parent[curr];
    }
    return curr;
}

void dijkstra(int n, int cost[MAX_NODES][MAX_NODES], int src, char node_names[]) {
    int dist[MAX_NODES];
    bool visited[MAX_NODES];
    int parent[MAX_NODES];

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }
    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int min_dist = INF;
        int u = -1;

        for (int v = 0; v < n; v++) {
            if (!visited[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                u = v;
            }
        }

        if (u == -1) break; // Remaining nodes are unreachable
        visited[u] = true;

        for (int v = 0; v < n; v++) {
            if (!visited[v] && cost[u][v] != INF && dist[u] + cost[u][v] < dist[v]) {
                dist[v] = dist[u] + cost[u][v];
                parent[v] = u;
            }
        }
    }

    // Output formal Routing Table for Source Node
    printf("\n===============================================================\n");
    printf("     ROUTING TABLE FOR SOURCE NODE '%c' (DIJKSTRA ALGORITHM)    \n", node_names[src]);
    printf("===============================================================\n");
    printf(" Destination | Shortest Cost | Next Hop | Full Path\n");
    printf("-------------+---------------+----------+----------------------\n");

    for (int i = 0; i < n; i++) {
        if (i == src) {
            printf("      %c      |       0       |    --    | %c (Local)\n", node_names[i], node_names[i]);
            continue;
        }

        if (dist[i] == INF) {
            printf("      %c      |   UNREACHABLE |    --    | No path\n", node_names[i]);
        } else {
            int next_hop = get_next_hop(parent, i, src);
            printf("      %c      |      %3d      |    %c     | ",
                   node_names[i], dist[i], node_names[next_hop]);
            print_path(parent, i, node_names);
            printf("\n");
        }
    }
    printf("===============================================================\n");
}

int main(void) {
    int n = 6;
    char node_names[MAX_NODES] = {'A', 'B', 'C', 'D', 'E', 'F'};

    // Sample network topology graph:
    // A-B: 2, A-C: 4
    // B-C: 1, B-D: 7
    // C-E: 3
    // D-E: 2, D-F: 1
    // E-F: 5
    int cost[MAX_NODES][MAX_NODES];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cost[i][j] = (i == j) ? 0 : INF;
        }
    }

    cost[0][1] = cost[1][0] = 2; // A - B (2)
    cost[0][2] = cost[2][0] = 4; // A - C (4)
    cost[1][2] = cost[2][1] = 1; // B - C (1)
    cost[1][3] = cost[3][1] = 7; // B - D (7)
    cost[2][4] = cost[4][2] = 3; // C - E (3)
    cost[3][4] = cost[4][3] = 2; // D - E (2)
    cost[3][5] = cost[5][3] = 1; // D - F (1)
    cost[4][5] = cost[5][4] = 5; // E - F (5)

    printf("--- SHORTEST PATH ROUTING (DIJKSTRA) ---\n");
    printf("Network Topology: 6 Nodes (A, B, C, D, E, F)\n");
    printf("Links: (A-B: 2), (A-C: 4), (B-C: 1), (B-D: 7), (C-E: 3), (D-E: 2), (D-F: 1), (E-F: 5)\n");

    dijkstra(n, cost, 0, node_names); // Compute routing table for node A

    return 0;
}
