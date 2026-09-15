/**
 * Experiment 6(b): Distance Vector Routing Algorithm (Bellman-Ford)
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * Distance Vector Routing is a distributed routing algorithm used in protocols
 * such as RIP (Routing Information Protocol).
 * Every router maintains a routing table containing:
 *   (Destination, Next Hop, Cost / Metric).
 *
 * Each router periodically shares its distance vector with direct neighbors.
 * Upon receiving a vector from neighbor v, router x updates its table via the
 * Bellman-Ford equation:
 *   D_x(y) = min_v { c(x, v) + D_v(y) }
 * Updates continue until the network reaches convergence (no further distance reductions).
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_NODES 10
#define INF 9999

struct RoutingEntry {
    int cost;
    int next_hop;
};

struct Router {
    struct RoutingEntry table[MAX_NODES];
};

void display_tables(int n, struct Router routers[], char names[], int iteration) {
    printf("\n--- ROUTING TABLES (Iteration %d) ---\n", iteration);
    for (int i = 0; i < n; i++) {
        printf("Routing Table for Node %c:\n", names[i]);
        printf("  Dest | Cost | Next Hop\n");
        printf("  -----+------+---------\n");
        for (int j = 0; j < n; j++) {
            if (routers[i].table[j].cost == INF) {
                printf("    %c  |  INF |   --\n", names[j]);
            } else if (i == j) {
                printf("    %c  |    0 | %c (Local)\n", names[j], names[j]);
            } else {
                printf("    %c  |  %3d |    %c\n",
                       names[j], routers[i].table[j].cost, names[routers[i].table[j].next_hop]);
            }
        }
        printf("\n");
    }
}

int main(void) {
    int n = 4;
    char names[MAX_NODES] = {'A', 'B', 'C', 'D'};

    // Direct link cost matrix between adjacent routers
    // Links: A-B: 1, B-C: 3, A-C: 7, C-D: 1, B-D: 8
    int cost_matrix[MAX_NODES][MAX_NODES] = {
        {0,   1,   7,   INF},
        {1,   0,   3,   8  },
        {7,   3,   0,   1  },
        {INF, 8,   1,   0  }
    };

    struct Router routers[MAX_NODES];

    // Initialization: each router only knows direct neighbor link costs
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            routers[i].table[j].cost = cost_matrix[i][j];
            if (cost_matrix[i][j] != INF && i != j) {
                routers[i].table[j].next_hop = j;
            } else {
                routers[i].table[j].next_hop = -1;
            }
        }
    }

    printf("--- DISTANCE VECTOR ROUTING SIMULATION ---\n");
    printf("Nodes: 4 (A, B, C, D)\n");
    printf("Initial link costs: A-B=1, B-C=3, A-C=7, C-D=1, B-D=8\n");

    display_tables(n, routers, names, 0);

    // Iterative distance vector exchange
    bool updated = true;
    int iteration = 0;

    while (updated) {
        updated = false;
        iteration++;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) continue;

                // Check all neighbors k of router i
                for (int k = 0; k < n; k++) {
                    if (cost_matrix[i][k] != INF && cost_matrix[i][k] > 0) {
                        int candidate_cost = cost_matrix[i][k] + routers[k].table[j].cost;
                        if (candidate_cost < routers[i].table[j].cost) {
                            routers[i].table[j].cost = candidate_cost;
                            routers[i].table[j].next_hop = k;
                            updated = true;
                        }
                    }
                }
            }
        }

        if (updated) {
            printf(">>> Network updated during round %d. Propagating new vectors...\n", iteration);
        }
    }

    printf("\n>>> CONVERGENCE REACHED in %d iterations! <<<\n", iteration);
    display_tables(n, routers, names, iteration);

    return 0;
}
