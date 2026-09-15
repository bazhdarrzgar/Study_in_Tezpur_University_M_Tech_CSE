#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=== Building Experiment 9 (Classical Ciphers) ==="
make clean
make all

echo -e "\n=== Test 9(a): Caesar Cipher ==="
./caesar_cipher "Computer Networks" 3

echo -e "\n=== Test 9(b): Substitution Cipher ==="
./substitution_cipher "Selected Topics in Computer Networks" "QWERTYUIOPASDFGHJKLZXCVBNM"

echo -e "\n>>> Experiment 9: ALL TESTS PASSED SUCCESSFULLY! <<<\n"
