# Assignment 2.2: Huffman Coding Compression & Decompression

**Course:** CSMT503 (Advanced Programming Lab 1) / CSMT533 (Data Structures & Algorithms Lab)  
**Student Roll Number:** `CSMCS26A017`  
**Language:** C (Standard C99/C11)

---

## 📁 Project Contents

| File | Description |
|---|---|
| **`CSMCS26A017_Lab2_2.c`** | Main C program (Tree construction, bit I/O, compression/decompression engine). |
| **`test_run.sh`** | Automated shell script to compile, run, test, and clean up. |
| **`Explanation.md`** | Comprehensive viva preparation guide (theories, trace diagrams, Q&A for 6/10 marks). |
| **`sample.txt`** | Rich English text sample (886 bytes) for testing compression. |
| **`test_short.txt`** | `"ABRACADABRA"` short test file for manual tracing during demos. |
| **`test_single.txt`** | Edge-case test file (repeated single character). |
| **`T2.2.pptx.pdf`** | Course assignment reference slide. |

---

## 🚀 Quick Start (Easiest Way)

Open your Linux terminal, navigate to this folder, and run:

```bash
cd "/home/swyanswartz/Music/STUDY/LAB_SE/Assignment 2.2"
./test_run.sh
```

`test_run.sh` will automatically compile the C code (if not already compiled) and open the **Interactive Menu**:

```text
===========================================================================
         CSMT503 / CSMT533 - ASSIGNMENT 2.2: HUFFMAN CODING               
                       Roll No: CSMCS26A017                                
===========================================================================
 1. Compress a Text File (.txt -> .huf)
    - Generates & Displays Huffman Codes Table
    - Compares Bits: ASCII vs Fixed-Length vs Huffman
    - Stores Bit-Packed File with Preorder Serialized Header
 2. Decompress a .huf File (.huf -> .txt)
    - Reads Header & Rebuilds Huffman Tree from Preorder Bits
    - Decodes Payload & Displays Content to Console
 3. Run Automated Demonstration Suite (sample.txt, test_short.txt)
 4. Exit
===========================================================================
Enter choice (1-4):
```

---

## 🧪 Automated Testing & Self-Verification

To verify that compression and decompression work losslessly on all test files with byte-for-byte fidelity:

```bash
./test_run.sh --test
```

This runs compression on `sample.txt`, `test_short.txt`, and `test_single.txt`, decompresses them, and executes `diff` to confirm **100% exact match**.

---

## 📄 Test Your Own Text File

You can test any file using the script:

```bash
# 1. Create your own test text file
echo "Hello world! This is my test file." > my_test.txt

# 2. Run compression, decompression & verification on it
./test_run.sh my_test.txt
```

---

## ⚙️ Manual Commands (Without the Script)

If you prefer compiling and running manually using `gcc`:

### 1. Compile the C Program
```bash
gcc -Wall -Wextra -O2 CSMCS26A017_Lab2_2.c -o CSMCS26A017_Lab2_2 -lm
```

### 2. Run Interactive Menu
```bash
./CSMCS26A017_Lab2_2
```

### 3. Command-Line Compression & Decompression
```bash
# Compress a text file to .huf
./CSMCS26A017_Lab2_2 -c sample.txt sample.txt.huf

# Decompress a .huf file and display contents on console
./CSMCS26A017_Lab2_2 -d sample.txt.huf restored.txt

# Verify with diff
diff -s sample.txt restored.txt
```

---

## 🧹 Cleaning Up Generated Files

To remove all generated files (`*.huf`, `*_restored.txt`, etc.) and keep the folder clean:

```bash
# Remove only generated output files
./test_run.sh --clean-outputs

# Remove generated output files AND compiled executable
./test_run.sh --clean
```

---

## 🎓 Tips for Demo with Instructor / TA

1. **Output Demo (3 Marks):**
   - Run `./test_run.sh` and select option **1** to compress `sample.txt`.
   - Point out the **Huffman Codes Table** and the **Bit Comparison Analysis** table.
   - Show that Huffman coding achieves significant space savings compared to standard 8-bit ASCII.
   - Select option **2** to decompress `sample.txt.huf` and show that the file contents are displayed directly in the terminal without loss.

2. **Code Explanation & Viva (6 Marks):**
   - Review [`Explanation.md`](file:///home/swyanswartz/Music/STUDY/LAB_SE/Assignment%202.2/Explanation.md).
   - Be ready to explain:
     - **Greedy approach:** Why we pick the 2 nodes with lowest frequency at each step using a Min-Heap.
     - **Prefix property:** Why no code is a prefix of another (enables unambiguous left-to-right decoding).
     - **Preorder Header:** How the tree is stored in the header using preorder bit traversal (`0` for internal node, `1` + 8-bit char for leaf).
     - **BitWriter / BitReader:** How individual variable-length bits are packed into full bytes.
