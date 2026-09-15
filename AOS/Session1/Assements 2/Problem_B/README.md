# Assignment 1 - Problem (b)
Multithreaded Proxy Server for Designated Files

---

## Files

- `proxy_server.c` - Multithreaded proxy server serving designated files concurrently.
- `client.c` - Client that connects to the proxy server and requests files.
- `proxy_files/` - Directory holding designated proxy copies (`file1.txt` and `file2.txt`).
- `Makefile` - Compiles `proxy_server` and `client`.
- `test_run.sh` - Script to automatically build and test the proxy server.

---

## How to Compile

Run `make` to compile both the proxy server and client:

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

Run the automated test script which starts the proxy server and executes single and concurrent file access tests:

```bash
chmod +x test_run.sh
./test_run.sh
```

---

### Option 2: Manual Execution

#### Step 1: Start the Proxy Server (Terminal 1)

```bash
./proxy_server [port]
```
*Default port is 8000.*
Example:
```bash
./proxy_server 8000
```

#### Step 2: Run Client to Access Files (Terminal 2)

```bash
./client [server_ip] [port] [filename]
```
*Default IP: `127.0.0.1`, port: `8000`, file: `file1.txt`.*

Examples:
- Access designated `file1.txt`:
  ```bash
  ./client 127.0.0.1 8000 file1.txt
  ```
- Access designated `file2.txt`:
  ```bash
  ./client 127.0.0.1 8000 file2.txt
  ```
- Request a non-designated file (error handling):
  ```bash
  ./client 127.0.0.1 8000 unknown.txt
  ```
- Access files concurrently:
  ```bash
  ./client 127.0.0.1 8000 file1.txt file2.txt file1.txt file2.txt
  ```
  *(All clients connect to the proxy server first, synchronize, and then request files simultaneously.)*

