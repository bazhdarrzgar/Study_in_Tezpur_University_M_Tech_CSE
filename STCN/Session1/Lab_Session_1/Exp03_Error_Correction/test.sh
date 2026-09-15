#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=== Building Experiment 3 (Hamming Code Error Correction) ==="
make clean
make all

echo -e "\n=== Test 3: Hamming Code (7,4) Single-Bit Correction ==="
./hamming 1011 3

echo -e "\n>>> Experiment 3: ALL TESTS PASSED SUCCESSFULLY! <<<\n"
