#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=== Building Experiment 8 (Traffic Shaping) ==="
make clean
make all

echo -e "\n=== Test 8(a): Leaky Bucket Algorithm ==="
./leaky_bucket

echo -e "\n=== Test 8(b): Token Bucket Algorithm ==="
./token_bucket

echo -e "\n>>> Experiment 8: ALL TESTS PASSED SUCCESSFULLY! <<<\n"
