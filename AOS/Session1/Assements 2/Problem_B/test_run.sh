#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=================================================="
echo "  Problem B: Building proxy_server and client..."
echo "=================================================="
make clean
make

PORT=8000

echo ""
echo "=================================================="
echo "  Starting multithreaded proxy server on port $PORT..."
echo "=================================================="
./proxy_server $PORT > proxy_server.log 2>&1 &
SERVER_PID=$!
sleep 1

cleanup() {
    echo ""
    echo "Stopping proxy server (PID: $SERVER_PID)..."
    kill -SIGINT $SERVER_PID 2>/dev/null || true
    wait $SERVER_PID 2>/dev/null || true
    echo "Proxy server stopped."
}
trap cleanup EXIT

echo "Proxy server started with PID $SERVER_PID."

echo ""
echo "=================================================="
echo "  Test 1: Accessing designated file1.txt"
echo "=================================================="
./client 127.0.0.1 $PORT file1.txt

echo ""
echo "=================================================="
echo "  Test 2: Accessing designated file2.txt"
echo "=================================================="
./client 127.0.0.1 $PORT file2.txt

echo ""
echo "=================================================="
echo "  Test 3: Accessing non-designated file unknown.txt"
echo "=================================================="
./client 127.0.0.1 $PORT unknown.txt

echo ""
echo "=================================================="
echo "  Test 4: Concurrent clients requesting files"
echo "=================================================="
./client 127.0.0.1 $PORT file1.txt file2.txt file1.txt file2.txt


echo ""
echo "=================================================="
echo "  Proxy Server Logs:"
echo "=================================================="
cat proxy_server.log

echo ""
echo "=================================================="
echo "  Problem B completed successfully!"
echo "=================================================="
