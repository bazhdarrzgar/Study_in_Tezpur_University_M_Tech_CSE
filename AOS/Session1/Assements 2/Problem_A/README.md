# Assignment 1 - Problem (a)
Multithreaded Concurrent Client-Server with Worker Thread Pool

---

## Files

- `server.c` - Multithreaded TCP server with a pool of 5 worker threads (`Thread 1` to `Thread 5`).
- `client.c` - Client that connects, sends a request (e.g. `"hello"`), and prints the server response.
- `Makefile` - Compiles `server` and `client`.
- `test_run.sh` - Script to automatically build and test the client-server system.

---

## How to Compile

Run `make` to compile both the server and client:

```bash
make
```

To remove compiled binaries:

```bash
make clean
```

---

## How to Run

### Option 1: Automated Test Script (Recommended)

Run the automated test script which starts the server and runs single and concurrent client tests:

```bash
chmod +x test_run.sh
./test_run.sh
```

---

### Option 2: Manual Execution

#### Step 1: Start the Server (Terminal 1)

```bash
./server [port]
```
*Default port is 1500.*
Example:
```bash
./server 1500
```

#### Step 2: Run Client(s) (Terminal 2)

```bash
./client [server_ip] [port] [message] [pool_size]
```
*Default IP: `127.0.0.1`, port: `1500`, message: `"hello"`, pool_size: `1`.*

Example (Single Client):
```bash
./client 127.0.0.1 1500 "hello"
```

Example (Pool of 5 Concurrent Clients):
```bash
./client 127.0.0.1 1500 "hello" 5
```
*(All 5 clients will connect to the server first, synchronize, and then send requests simultaneously.)*

