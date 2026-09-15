#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=== Building Experiment 1 (Framing Techniques) ==="
make clean
make all

echo -e "\n=== Test 1(a): Bit Stuffing ==="
./bit_stuffing 011111101111110

echo -e "\n=== Test 1(b): Byte Stuffing ==="
./byte_stuffing ABFCED

echo -e "\n>>> Experiment 1: ALL TESTS PASSED SUCCESSFULLY! <<<\n"
