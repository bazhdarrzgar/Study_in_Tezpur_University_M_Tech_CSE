#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=== Building Experiment 10 (RSA Algorithm) ==="
make clean
make all

echo -e "\n=== Test 10: RSA Key Generation, Encryption, Decryption ==="
./rsa 61 53 "NETWORKS"

echo -e "\n>>> Experiment 10: ALL TESTS PASSED SUCCESSFULLY! <<<\n"
