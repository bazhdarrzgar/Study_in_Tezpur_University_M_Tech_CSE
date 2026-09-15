# CSMT 502: Selected Topics in Computer Networks (STCN)
## Laboratory Practical Manual & Implementation Repository — Session 1

This repository contains full, working implementations for all 11 experiments outlined in the syllabus of **CSMT 502: Selected Topics in Computer Networks**, implemented cleanly in C and NS-2 OTcl.

---

## Directory Structure

```text
Lab_Session_1/
├── Makefile                               # Master build file for all C experiments
├── run_all_tests.sh                       # Automated test suite running all 11 experiments
├── README.md                              # Complete lab manual with theory & instructions
├── Exp01_Framing/                         # 1. Framing Techniques
│   ├── bit_stuffing.c                     # Bit stuffing & unstuffing (01111110 flag)
│   ├── byte_stuffing.c                    # Byte/character stuffing & unstuffing (FLAG/ESC)
│   ├── Makefile
│   └── test.sh
├── Exp02_Error_Detection/                 # 2. Error Detection Techniques
│   ├── parity.c                           # 1D Simple & 2D Parity (LRC/VRC matrix)
│   ├── checksum.c                         # 16-bit Internet Checksum (RFC 1071)
│   ├── crc.c                              # Cyclic Redundancy Check (Modulo-2 polynomial division)
│   ├── Makefile
│   └── test.sh
├── Exp03_Error_Correction/                # 3. Error Correction Technique
│   ├── hamming.c                          # General (n, k) Hamming Code (single-bit correction)
│   ├── Makefile
│   └── test.sh
├── Exp04_TCP_Sockets/                     # 4. TCP Client-Server Sockets
│   ├── tcp_server.c                       # Reliable stream server
│   ├── tcp_client.c                       # Stream client
│   ├── Makefile
│   └── test.sh
├── Exp05_UDP_Sockets/                     # 5. UDP Client-Server Sockets
│   ├── udp_server.c                       # Connectionless datagram server
│   ├── udp_client.c                       # Datagram client with timeout handling
│   ├── Makefile
│   └── test.sh
├── Exp06_Routing_Algorithms/              # 6. Routing Algorithms
│   ├── shortest_path_dijkstra.c           # Dijkstra's single-source shortest path algorithm
│   ├── distance_vector_routing.c          # Bellman-Ford distributed distance vector routing
│   ├── Makefile
│   └── test.sh
├── Exp07_Flow_Control/                    # 7. Flow Control Techniques
│   ├── stop_and_wait.c                    # Stop-and-Wait ARQ simulation
│   ├── go_back_n.c                        # Go-Back-N ARQ simulation (sliding window)
│   ├── selective_repeat.c                 # Selective Repeat ARQ simulation
│   ├── Makefile
│   └── test.sh
├── Exp08_Traffic_Shaping/                 # 8. Traffic Shaping
│   ├── leaky_bucket.c                     # Leaky Bucket algorithm (constant leak rate)
│   ├── token_bucket.c                     # Token Bucket algorithm (burst-tolerant rate limit)
│   ├── Makefile
│   └── test.sh
├── Exp09_Cryptography_Ciphers/            # 9. Classical Ciphers
│   ├── caesar_cipher.c                    # Caesar shift cipher + Brute-force cryptanalysis
│   ├── substitution_cipher.c              # Monoalphabetic substitution cipher
│   ├── Makefile
│   └── test.sh
├── Exp10_RSA_Algorithm/                   # 10. Public-Key Cryptography
│   ├── rsa.c                              # RSA keygen, modular exponentiation & cryptosystem
│   ├── Makefile
│   └── test.sh
└── Exp11_NS2_Simulation/                  # 11. NS-2 Simulation
    ├── routing_simulation.tcl             # Dynamic routing topology simulation script
    ├── run_ns2.sh                         # NS-2 execution & trace analysis script
    ├── README_NS2.md                      # Detailed NS-2 execution guide
    └── test.sh
```

---

## Quick Start: Building & Running

### 1. Build All Programs
From `Lab_Session_1/`:
```bash
make all
```

### 2. Run All Automated Tests
```bash
bash run_all_tests.sh
```

### 3. Clean All Binaries
```bash
make clean
```

---

## Detailed Experiment Summaries

### Experiment 1: Framing Techniques
- **1(a) Bit Stuffing (`bit_stuffing.c`)**:
  - Uses flag sequence `01111110`.
  - When five consecutive `1`s occur in data, inserts `0`.
  - Receiver checks for five `1`s: if next bit is `0`, it is discarded.
  - Run: `./Exp01_Framing/bit_stuffing 011111101111110`

- **1(b) Byte Stuffing (`byte_stuffing.c`)**:
  - Delimiters: `FLAG = 'F'`, `ESCAPE = 'E'`.
  - Any internal occurrence of `FLAG` or `ESC` is preceded by an `ESC` byte.
  - Run: `./Exp01_Framing/byte_stuffing ABFCED`

---

### Experiment 2: Error Detection Techniques
- **2(a) Parity Bit (`parity.c`)**:
  - Computes Even/Odd parity bits for 1D streams.
  - Generates 2D LRC/VRC parity matrix across rows and columns.
  - Run: `./Exp02_Error_Detection/parity 1011001`

- **2(b) Internet Checksum (`checksum.c`)**:
  - Computes 16-bit one's complement sum with end-around carry (RFC 1071).
  - Validates sum + checksum equals `0xFFFF`.
  - Run: `./Exp02_Error_Detection/checksum 4500 003c 1c46 4000`

- **2(c) Cyclic Redundancy Check (`crc.c`)**:
  - Implements binary polynomial modulo-2 division using XOR operations.
  - Detects single-bit and burst transmission corruptions.
  - Run: `./Exp02_Error_Detection/crc 100100 1101`

---

### Experiment 3: Error Correction Technique (Hamming Code)
- **Hamming Code (`hamming.c`)**:
  - Calculates redundant bits $r$ satisfying $2^r \ge k + r + 1$.
  - Places parity bits at positions $1, 2, 4, 8, \dots$.
  - Computes syndrome value $S$ on the receiver side. If $S > 0$, flips bit $S$ to correct transmission errors and restores original message bits.
  - Run: `./Exp03_Error_Correction/hamming 1011 3`

---

### Experiment 4: Client–Server Communication Using TCP Sockets
- **`tcp_server.c` & `tcp_client.c`**:
  - Connection-oriented byte stream sockets (`SOCK_STREAM`).
  - Implements full socket lifecycle: `socket() -> bind() -> listen() -> accept() -> recv()/send()`.
  - Run server in terminal 1: `./Exp04_TCP_Sockets/tcp_server 8888`
  - Run client in terminal 2: `./Exp04_TCP_Sockets/tcp_client 8888 "Hello Server"`

---

### Experiment 5: UDP Client–Server Communication
- **`udp_server.c` & `udp_client.c`**:
  - Connectionless datagram sockets (`SOCK_DGRAM`).
  - Utilizes `sendto()` and `recvfrom()` without prior connection establishment.
  - Includes `SO_RCVTIMEO` timeout protection on the client.
  - Run server: `./Exp05_UDP_Sockets/udp_server 9999`
  - Run client: `./Exp05_UDP_Sockets/udp_client 9999 "Test packet"`

---

### Experiment 6: Routing Algorithms
- **6(a) Shortest Path Routing (`shortest_path_dijkstra.c`)**:
  - Dijkstra's single-source shortest path algorithm on weighted graphs.
  - Generates comprehensive routing table with costs, next hops, and complete hop paths.
  - Run: `./Exp06_Routing_Algorithms/shortest_path_dijkstra`

- **6(b) Distance Vector Routing (`distance_vector_routing.c`)**:
  - Distributed Bellman-Ford algorithm: $D_x(y) = \min_v \{ c(x,v) + D_v(y) \}$.
  - Simulates iterative vector exchanges until convergence.
  - Run: `./Exp06_Routing_Algorithms/distance_vector_routing`

---

### Experiment 7: Flow Control Techniques
- **7(a) Stop & Wait ARQ (`stop_and_wait.c`)**:
  - 1-bit alternating sequence numbers, timeout handling, lost packet retransmissions, and duplicate discarding.
  - Run: `./Exp07_Flow_Control/stop_and_wait`

- **7(b) Go-Back-N ARQ (`go_back_n.c`)**:
  - Sliding window of size $W$. Discards out-of-order frames when earlier frame is lost; retransmits entire window from the lost frame.
  - Run: `./Exp07_Flow_Control/go_back_n`

- **7(c) Selective Repeat ARQ (`selective_repeat.c`)**:
  - Sender and receiver windows of size $W$. Buffers out-of-order packets at receiver; retransmits only the specific lost frame.
  - Run: `./Exp07_Flow_Control/selective_repeat`

---

### Experiment 8: Traffic Shaping
- **8(a) Leaky Bucket (`leaky_bucket.c`)**:
  - Buffer capacity $C$, constant leak rate $R$. Buffers bursty arrivals and discards overflows.
  - Run: `./Exp08_Traffic_Shaping/leaky_bucket`

- **8(b) Token Bucket (`token_bucket.c`)**:
  - Token accumulation at rate $r$ up to capacity $C$. Allows short-term bursts when tokens are available while bounding long-term average rate.
  - Run: `./Exp08_Traffic_Shaping/token_bucket`

---

### Experiment 9: Classical Ciphers
- **9(a) Caesar Cipher (`caesar_cipher.c`)**:
  - Shifts characters by key $k$: $C = (P + k) \pmod{26}$.
  - Includes brute-force cryptanalysis across all 25 shifts.
  - Run: `./Exp09_Cryptography_Ciphers/caesar_cipher "Computer Networks" 3`

- **9(b) Substitution Cipher (`substitution_cipher.c`)**:
  - Permuted 26-letter substitution alphabet with bijective mapping.
  - Run: `./Exp09_Cryptography_Ciphers/substitution_cipher "Network Security" "QWERTYUIOPASDFGHJKLZXCVBNM"`

---

### Experiment 10: RSA Cryptosystem
- **`rsa.c`**:
  - Asymmetric public-key encryption:
    1. Key generation: $n = p \times q$, $\phi(n) = (p-1)(q-1)$, chooses $e$, computes $d \equiv e^{-1} \pmod{\phi(n)}$ via Extended Euclidean Algorithm.
    2. Square-and-multiply modular exponentiation: $c = m^e \pmod{n}$, $m = c^d \pmod{n}$.
  - Encrypts and decrypts both numerical data and ASCII strings.
  - Run: `./Exp10_RSA_Algorithm/rsa 61 53 "NETWORKS"`

---

### Experiment 11: NS-2 Simulation of Routing Algorithms
- **`routing_simulation.tcl` & `run_ns2.sh`**:
  - Simulates Distance Vector dynamic routing protocol (`$ns rtproto DV`) on a 7-node network topology.
  - Simulates dynamic link failure at $t = 2.0\text{ s}$ and recovery at $t = 3.5\text{ s}$.
  - Generates `out.tr` and `out.nam` traces for analysis and visualization.
  - Run: `bash Exp11_NS2_Simulation/run_ns2.sh`
