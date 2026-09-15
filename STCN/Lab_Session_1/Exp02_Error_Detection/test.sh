#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=== Building Experiment 2 (Error Detection Techniques) ==="
make clean
make all

echo -e "\n=== Test 2(a): Parity (1D and 2D) ==="
./parity 1011001

echo -e "\n=== Test 2(b): Internet Checksum ==="
./checksum 4500 003c 1c46 4000

echo -e "\n=== Test 2(c): Cyclic Redundancy Check (CRC) ==="
./crc 100100 1101

echo -e "\n>>> Experiment 2: ALL TESTS PASSED SUCCESSFULLY! <<<\n"
