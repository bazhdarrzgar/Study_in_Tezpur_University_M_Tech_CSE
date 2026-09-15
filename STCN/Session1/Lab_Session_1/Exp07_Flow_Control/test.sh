#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=== Building Experiment 7 (Flow Control Techniques) ==="
make clean
make all

echo -e "\n=== Test 7(a): Stop & Wait ARQ ==="
./stop_and_wait

echo -e "\n=== Test 7(b): Go-Back-N ARQ ==="
./go_back_n

echo -e "\n=== Test 7(c): Selective Repeat ARQ ==="
./selective_repeat

echo -e "\n>>> Experiment 7: ALL TESTS PASSED SUCCESSFULLY! <<<\n"
