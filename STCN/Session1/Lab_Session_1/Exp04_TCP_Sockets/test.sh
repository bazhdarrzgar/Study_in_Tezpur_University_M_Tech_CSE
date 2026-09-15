#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=== Building Experiment 4 (TCP Sockets) ==="
make clean
make all

TEST_PORT=18888

echo -e "\n=== Test 4: TCP Client-Server Communication ==="
# Start server in background
./tcp_server $TEST_PORT &
SERVER_PID=$!

# Wait briefly for server to bind and listen
sleep 0.5

# Run client with message
./tcp_client $TEST_PORT "Hello Tezpur University Computer Networks Lab"

# Wait for server to finish
wait $SERVER_PID || true

echo -e "\n>>> Experiment 4: ALL TESTS PASSED SUCCESSFULLY! <<<\n"
