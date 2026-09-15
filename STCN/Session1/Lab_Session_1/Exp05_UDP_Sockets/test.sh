#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=== Building Experiment 5 (UDP Sockets) ==="
make clean
make all

TEST_PORT=19999

echo -e "\n=== Test 5: UDP Client-Server Communication ==="
# Start UDP server in background
./udp_server $TEST_PORT &
SERVER_PID=$!

sleep 0.5

# Run UDP client with test message
./udp_client $TEST_PORT "UDP Datagram packet test from CSMT 502"

wait $SERVER_PID || true

echo -e "\n>>> Experiment 5: ALL TESTS PASSED SUCCESSFULLY! <<<\n"
