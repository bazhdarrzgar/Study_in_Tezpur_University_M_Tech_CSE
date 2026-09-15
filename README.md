# M.Tech in Computer Science & Engineering — Academic Journey
### Department of Computer Science & Engineering | Tezpur University

[![Tezpur University](https://img.shields.io/badge/University-Tezpur%20University-blue.svg)](https://www.tezu.ernet.in/)
[![Degree](https://img.shields.io/badge/Degree-M.Tech%20CSE-success.svg)](#)
[![Language](https://img.shields.io/badge/Language-C%20%7C%20Shell%20%7C%20Tcl%2FNS2-orange.svg)](#)
[![Status](https://img.shields.io/badge/Status-Active%20Coursework-green.svg)](#)

Welcome to the repository documenting my academic coursework, lab implementations, algorithm analyses, mathematical foundations, and exam preparations for the **M.Tech in Computer Science and Engineering** program at **Tezpur University**.

---

## 📚 Curriculum & Courses

| Course Code | Course Name | Focus Areas | Key Artifacts |
|:---|:---|:---|:---|
| **MFCS** | **Mathematical Foundations of Computer Science** | Linear Algebra, Vector Spaces, Orthogonality, Gaussian Elimination, Matrix Factorizations | Detailed Chapter Notes, SVG Geometric Visualizations, Exam Solutions, Quick Memorize Sheets |
| **STCN (CSMT 502)** | **Selected Topics in Computer Networks** | Framing, Error Detection/Correction, TCP/UDP Sockets, Routing, Flow Control, Traffic Shaping, Cryptography | 11 Hands-on C & NS2 Lab Experiments, Automated Test Suite, Theory Notes, Exam Solutions |
| **LAB_SE (CSMT 503)** | **Advanced Programming Lab 1** | Advanced C Programming, Empirical Algorithm Analysis, Memory Management, File I/O | Rainfall dataset sorting benchmark (Merge vs Quick Sort), Text processing parsers |
| **AOS** | **Advanced Operating Systems** | Network IPC, POSIX Sockets, Client-Server Architectures, Systems Programming | TCP Client-Server Implementations, Socket Programming Labs |
| **DAA (CSMT 501)** | **Design & Analysis of Algorithms** | Asymptotic Analysis, Divide & Conquer, Dynamic Programming, Greedy, Graph Algorithms | Course plan, theoretical foundations |

---

## 🗂️ Detailed Directory Overview

### 1. 📐 MFCS — Mathematical Foundations of Computer Science
Comprehensive conceptual explanations, matrix theory, and vector geometry with visual aids:
* **Core Chapters**:
  * `Chapter 1.md`: Systems of linear equations, row/column pictures, Gaussian elimination, matrix operations, permutations.
  * `Chapter 2.md`: Vector spaces, subspaces, nullspace, column space, linear independence, rank, basis, and dimension.
  * `Chapter 3.md`: Orthogonality, orthogonal complements, projections onto lines and subspaces, least-squares approximation, Gram-Schmidt orthogonalization.
* **Visual Diagrams (`MFCS/images/`)**: High-quality SVG visualizations illustrating fundamental subspaces, projections, and geometric interpretations.
* **Exam Preparation & Revision (`Morning_Memorize/`)**:
  * Quick cram sheets and chapter-by-chapter essential recall formulas.
  * Solved tests and practice sessions.

---

### 2. 🌐 STCN — Selected Topics in Computer Networks (CSMT 502)
Theory, exam solutions, and complete implementations of core networking protocols and algorithms in C:

* **Theory & Exams**:
  * `Paper_Session_1/`: Overview of Physical & Data Link Layers.
  * `Exam_Session_1/`: Session 1 Test Question Paper and detailed verified solutions.
* **Hands-on Lab Experiments (`STCN/Lab_Session_1/`)**:
  * **Exp 01: Framing** — Bit Stuffing and Byte/Character Stuffing.
  * **Exp 02: Error Detection** — Longitudinal/Vertical Parity, Internet Checksum (RFC 1071), CRC (CRC-CCITT / CRC-16 / CRC-32).
  * **Exp 03: Error Correction** — Single-error correcting Hamming Code $(7, 4)$ encoding and syndrome decoding.
  * **Exp 04: TCP Sockets** — Multi-message iterative TCP Echo Server and Client.
  * **Exp 05: UDP Sockets** — Connectionless datagram communication in C.
  * **Exp 06: Routing Algorithms** — Dijkstra's Shortest Path and Bellman-Ford Distance Vector Routing.
  * **Exp 07: Flow Control** — Stop-and-Wait, Go-Back-$N$ (sliding window), and Selective Repeat ARQ protocols.
  * **Exp 08: Traffic Shaping** — Leaky Bucket and Token Bucket rate control algorithms.
  * **Exp 09: Cryptography Ciphers** — Classical Caesar and Monoalphabetic Substitution ciphers.
  * **Exp 10: Public-Key Cryptography** — RSA key generation, modular exponentiation, encryption, and decryption.
  * **Exp 11: Network Simulation** — NS-2 (Network Simulator 2) Tcl scripts for TCP/UDP bandwidth and packet loss simulation.
* **Build System & Test Automation**:
  * Includes a master `Makefile` and an automated verification script `run_all_tests.sh`.

---

### 3. 💻 LAB_SE — Advanced Programming Lab 1 (CSMT 503)
Rigorous systems-level C implementations:
* **Assignment 2.1 — Rainfall Data Sorting & Performance Analysis**:
  * Analyzes historical monthly and annual rainfall metrics across years.
  * Implements and empirically benchmarks **Merge Sort** vs. **Quick Sort** under varied pivoting strategies.
  * Tracks precise comparison counts, data movements, and wall-clock execution times with comprehensive reports.
* **Assignment 2.2 — Memory Management & Text Parsing**:
  * String tokenization, dynamic buffer management, file parsing, and rigorous edge-case testing (`test_run.sh`).

---

### 4. ⚙️ AOS — Advanced Operating Systems
* Socket programming under Linux POSIX environment.
* Iterative and concurrent TCP client-server architectures (`tcpServer.c`, `tcpClient.c`).
* References to Beej's Guide to Network Programming and OS IPC concepts.

---

### 5. 📊 DAA — Design and Analysis of Algorithms (CSMT 501)
* Course plans and curriculum covering advanced algorithm design, recurrence relations, dynamic programming, and complexity classes ($P, NP, NP\text{-complete}$).

---

## 🛠️ How to Build and Run

### STCN Lab Experiments
To compile all network programming experiments at once:
```bash
cd STCN/Lab_Session_1
make

# Run the complete automated test suite
bash run_all_tests.sh

# Clean generated binaries
make clean
```

### Advanced Programming Lab (LAB_SE)
Compile and test the rainfall sorting benchmark:
```bash
cd "LAB_SE/Assignment 2.1"
gcc -O2 -Wall -Wextra -o rainfall_sort CSMCS26A017_Lab2_1.c
./rainfall_sort A2.1_rainfall.csv
```

Compile and test Assignment 2.2:
```bash
cd "LAB_SE/Assignment 2.2"
gcc -O2 -Wall -Wextra -o lab2_2 CSMCS26A017_Lab2_2.c
bash test_run.sh
```

### AOS Socket Programming
Compile server and client:
```bash
cd "AOS/Assements 2"
gcc -Wall -o server tcpServer.c
gcc -Wall -o client tcpClient.c

# In terminal 1:
./server

# In terminal 2:
./client 127.0.0.1
```

---

## 👤 Author

* **Bazhdar Rzgar**
* M.Tech in Computer Science and Engineering
* Tezpur University, Assam, India
* GitHub: [@bazhdarrzgar](https://github.com/bazhdarrzgar)
* Email: [soyansoon9@gmail.com](mailto:soyansoon9@gmail.com)

---

## 📄 License & Notes

This repository is maintained for personal study, academic tracking, and research reference. All code and notes are authored as part of the M.Tech curriculum at Tezpur University.
