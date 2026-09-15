#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=================================================="
echo "  Problem A: Building server and client..."
echo "=================================================="
make clean
make

PORT=1500

echo ""
echo "=================================================="
echo "  Starting multithreaded server on port $PORT..."
echo "=================================================="
./server $PORT > server.log 2>&1 &
SERVER_PID=$!
sleep 1

cleanup() {
    echo ""
    echo "Stopping server (PID: $SERVER_PID)..."
    kill -SIGINT $SERVER_PID 2>/dev/null || true
    wait $SERVER_PID 2>/dev/null || true
    echo "Server stopped."
}
trap cleanup EXIT

echo "Server started with PID $SERVER_PID."

echo ""
echo "=================================================="
echo "  Test 1: Single client sending 'hello'"
echo "=================================================="
./client 127.0.0.1 $PORT "hello"

echo ""
echo "=================================================="
echo "  Test 2: Pool of 5 clients connecting concurrently"
echo "=================================================="
./client 127.0.0.1 $PORT "hello" 5


echo ""
echo "=================================================="
echo "  Server Logs:"
echo "=================================================="
cat server.log

echo ""
echo "=================================================="
echo "  Problem A completed successfully!"
echo "=================================================="
