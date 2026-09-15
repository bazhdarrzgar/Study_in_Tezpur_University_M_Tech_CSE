#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=== Building Experiment 6 (Routing Algorithms) ==="
make clean
make all

echo -e "\n=== Test 6(a): Dijkstra's Shortest Path Routing ==="
./shortest_path_dijkstra

echo -e "\n=== Test 6(b): Distance Vector Routing (Bellman-Ford) ==="
./distance_vector_routing

echo -e "\n>>> Experiment 6: ALL TESTS PASSED SUCCESSFULLY! <<<\n"
