# Comprehensive Lab Demo & Explanation Guide: Huffman Coding

**Course:** CSMT503 (Advanced Programming Lab 1) / CSMT533 (Data Structures and Algorithms Lab)  
**Assignment:** 2.2 — Greedy Algorithms: Huffman Coding  
**Student Roll Number:** `CSMCS26A017`  
**Primary Source Code:** `CSMCS26A017_Lab2_2.c`  
**Evaluation Breakdown (10 Marks Total):**
- **1 Mark:** Timely submission
- **3 Marks:** Program Output & Verification
- **6 Marks:** Viva / Code Explanation & Theory

---

## Table of Contents
1. [Overview & Project Objectives](#1-overview--project-objectives)
2. [Greedy Algorithms & Huffman Theory](#2-greedy-algorithms--huffman-theory)
3. [Architecture & Data Structures](#3-architecture--data-structures)
4. [Bitwise Operations & The Bit-Level I/O Engine](#4-bitwise-operations--the-bit-level-io-engine)
5. [The `.huf` Binary File Header Specification](#5-the-huf-binary-file-header-specification)
6. [Mathematical Bit Comparison (ASCII vs Fixed-Length vs Huffman)](#6-mathematical-bit-comparison-ascii-vs-fixed-length-vs-huffman)
7. [Step-by-Step Manual Trace: `"ABRACADABRA"`](#7-step-by-step-manual-trace-abracadabra)
8. [Code Walkthrough by Function](#8-code-walkthrough-by-function)
9. [How to Compile, Run, and Demo to Instructor](#9-how-to-compile-run-and-demo-to-instructor)
10. [Top Viva Questions & Answers (Securing the 6 Marks)](#10-top-viva-questions--answers-securing-the-6-marks)

---

## 1. Overview & Project Objectives

Huffman coding is an optimal **lossless data compression** technique developed by David A. Huffman (1952).

### Assignment Requirements Checklist
| Requirement | Status | Implementation in `CSMCS26A017_Lab2_2.c` |
|---|:---:|---|
| **Accept user-given text file** |  | Interactive prompt or command-line argument (`-c`, `-d`, or filename) |
| **Produce Huffman codes** |  | Built using Min-Heap priority queue via greedy merging |
| **Display codes for each character** |  | Clean ASCII table showing character, ASCII code, frequency, percentage, and Huffman bit string |
| **Compare bits: ASCII vs Fixed vs Huffman** |  | Full comparison table with exact bit counts, equivalent bytes, and percentage savings |
| **Store compressed text in binary file (`.huf`)** |  | Real bit packing (`writeBit`, `writeBits`) writing true binary bytes to disk |
| **Self-contained header in `.huf` file** |  | Stores magic ID (`HUF1`), 64-bit uncompressed size, and preorder tree traversal |
| **Read header & decompress to display content** |  | Parses header, rebuilds tree, decodes bits, restores text file, and prints to console |
| **Roll number naming standard** |  | Named `CSMCS26A017_Lab2_2.c` matching student roll number `CSMCS26A017` |

---

## 2. Greedy Algorithms & Huffman Theory

### What is a Greedy Algorithm?
A greedy algorithm makes the **locally optimal choice** at each stage with the hope of finding a **global optimum**.

In Huffman Coding:
- **Local choice:** At each step, extract the **two tree nodes with the smallest frequencies** and combine them into an internal parent node whose frequency is their sum.
- **Global result:** A binary prefix tree that minimizes the Weighted External Path Length:
  $$\text{Cost}(T) = \sum_{c \in C} \text{freq}(c) \times \text{depth}_T(c)$$
  This minimizes the total number of bits required to encode the entire file.

### What is a Prefix Code?
A prefix code (more accurately, a **prefix-free code**) is a code in which **no code word is a prefix of any other code word**.
- For example, if `'A'` is encoded as `0`, no other character's code can start with `0` (e.g. `01` is forbidden).
- **Benefit:** Allows unambiguous, instantaneous decoding from left to right **without needing delimiters or separators** between symbols.

---

## 3. Architecture & Data Structures

```text
+-----------------------------------------------------------------------------------+
|                               COMPRESSION PIPELINE                                |
+-----------------------------------------------------------------------------------+
  [Input Text File]
          |
          v  Pass 1: Frequency Analysis
  [Frequency Table (256 uint64)]
          |
          v  Insert non-zero leaves
  [Min-Heap Priority Queue]  <-- Greedily extract 2 smallest, insert merged parent
          |
          v
  [Huffman Tree]
          |
          +-----------------------------+
          |                             |
          v Preorder Serialization      v DFS Tree Traversal
  [Header Bitstream]            [Code Table (char -> "011...")]
          |                             |
          |                             v Pass 2: Encode Characters
          |                     [Payload Bitstream]
          |                             |
          +--------------+--------------+
                         |
                         v
              [Compressed .huf Binary File]
```

### Key Structs in C

#### 1. `HuffmanNode`
```c
typedef struct HuffmanNode {
    unsigned char data;          /* Character symbol (for leaf nodes) */
    uint64_t freq;               /* Occurrence frequency */
    struct HuffmanNode *left;    /* Left child represents bit 0 */
    struct HuffmanNode *right;   /* Right child represents bit 1 */
} HuffmanNode;
```

#### 2. `MinHeap` (Priority Queue)
```c
typedef struct MinHeap {
    int size;                    /* Current number of elements */
    int capacity;                /* Maximum capacity (at most 256 for bytes) */
    HuffmanNode **array;         /* Array of pointers to tree nodes */
} MinHeap;
```
- A binary min-heap guarantees that the minimum frequency node is always at `array[0]`.
- Extraction takes $O(\log k)$ time, and insertion takes $O(\log k)$ time.

---

## 4. Bitwise Operations & The Bit-Level I/O Engine

Computers read and write files in units of **bytes (8 bits)**. Because Huffman codes are variable-length (e.g., `'A'` may be 1 bit, `'z'` may be 10 bits), we cannot write individual bits directly using `fputc` or `fwrite`.

### The `BitWriter` Solution
We maintain an 8-bit accumulator (`buffer`) and a counter (`bitCount`):
```c
typedef struct {
    FILE *fp;
    unsigned char buffer;
    int bitCount;                /* 0 to 7 */
    uint64_t totalBytesWritten;
} BitWriter;
```

#### Writing a Bit (`writeBit`):
```c
void writeBit(BitWriter *bw, int bit) {
    if (bit) {
        /* Set bit at position (7 - bitCount) */
        bw->buffer |= (unsigned char)(1 << (7 - bw->bitCount));
    }
    bw->bitCount++;
    if (bw->bitCount == 8) {
        fputc(bw->buffer, bw->fp);  /* Buffer full: write byte to file */
        bw->totalBytesWritten++;
        bw->buffer = 0;
        bw->bitCount = 0;
    }
}
```

#### Flushing Remaining Bits (`bitWriterFlush`):
If the file stream finishes with 3 bits inside `buffer`, `bitWriterFlush` writes the final byte (padded with trailing 0s).

### The `BitReader` Solution
`BitReader` reads one byte from disk whenever its internal 8-bit buffer is exhausted, and delivers bits one by one:
```c
int readBit(BitReader *br) {
    if (br->bitCount == 0) {
        int ch = fgetc(br->fp);
        if (ch == EOF) return -1;
        br->buffer = (unsigned char)ch;
        br->bitCount = 8;
    }
    int bit = (br->buffer >> (br->bitCount - 1)) & 1;
    br->bitCount--;
    return bit;
}
```

---

## 5. The `.huf` Binary File Header Specification

To make our `.huf` file 100% self-contained and universally decodable, the file begins with a compact header:

```text
+-----------------------+-----------------------+-----------------------------+---------------------+
| Magic Identifier (4B) | File Size (8 Bytes)   | Preorder Tree Bitstream     | Compressed Payload  |
| "HUF1"                | uint64_t (Big-Endian) | Traversal bits + leaf chars | Variable-length bits|
+-----------------------+-----------------------+-----------------------------+---------------------+
```

### Preorder Tree Serialization
As requested in the assignment hint (*"The Huffman code tree can be stored as a preorder or postorder traversal"*):
- **Internal Node:** Write bit `0`. Then recurse on Left and Right subtrees.
- **Leaf Node:** Write bit `1`. Followed immediately by the 8 bits of the character.

#### Example:
For 2 characters `'A'` and `'B'`:
- Root is internal $\to$ write `0`
- Left child is `'A'` $\to$ write `1` followed by `'A'` (`01000001`)
- Right child is `'B'` $\to$ write `1` followed by `'B'` (`01000010`)
Total header tree size: only **19 bits** (~2.4 bytes)!

### Why Store the 64-Bit File Size?
When writing bits, the last byte may contain 1 to 7 padding bits. By storing the exact original file size in the header:
- The decompressor stops decoding the instant all original characters are restored.
- Prevents trailing garbage characters caused by padding bits.

---

## 6. Mathematical Bit Comparison (ASCII vs Fixed-Length vs Huffman)

Given:
- $N$: Total number of characters in the file ($\sum \text{freq}[c]$)
- $D$: Number of distinct characters (alphabet size $|\Sigma|$)

### 1. Standard ASCII Code
Every character uses a fixed 8-bit byte:
$$\text{ASCII Bits} = N \times 8$$

### 2. Fixed-Length Code
The minimum number of bits needed to uniquely encode $D$ unique symbols is:
$$k = \lceil \log_2(D) \rceil$$
$$\text{Fixed-Length Bits} = N \times k$$

### 3. Huffman Variable-Length Code
Each character $c$ has code length $L(c)$:
$$\text{Huffman Bits} = \sum_{c \in \Sigma} (\text{freq}(c) \times L(c))$$

### 4. Storage Savings Formulas
$$\text{Savings over ASCII (\%)} = \left(1 - \frac{\text{Huffman Bits}}{\text{ASCII Bits}}\right) \times 100\%$$
$$\text{Savings over Fixed (\%)} = \left(1 - \frac{\text{Huffman Bits}}{\text{Fixed Bits}}\right) \times 100\%$$

---

## 7. Step-by-Step Manual Trace: `"ABRACADABRA"`

Let's trace string: `"ABRACADABRA\n"` (Length = 12 characters).

### Step 1: Frequency Count
| Character | Count |
|---|:---:|
| `'A'` | 5 |
| `'B'` | 2 |
| `'R'` | 2 |
| `'C'` | 1 |
| `'D'` | 1 |
| `'\n'` | 1 |

### Step 2: Min-Heap Initialization
Heap array initially contains 6 leaf nodes:
`[ ('\n', 1), ('C', 1), ('D', 1), ('B', 2), ('R', 2), ('A', 5) ]`

### Step 3: Greedy Merging Iterations
1. **Merge 1:** Extract `'\n'` (1) and `'C'` (1) $\to$ Create internal node $N_1$ with weight $1+1 = 2$.
   Heap: `[ ('D', 1), ('B', 2), ('R', 2), (N_1, 2), ('A', 5) ]`
2. **Merge 2:** Extract `'D'` (1) and `'B'` (2) $\to$ Create internal node $N_2$ with weight $1+2 = 3$.
   Heap: `[ (N_1, 2), ('R', 2), (N_2, 3), ('A', 5) ]`
3. **Merge 3:** Extract $N_1$ (2) and `'R'` (2) $\to$ Create internal node $N_3$ with weight $2+2 = 4$.
   Heap: `[ (N_2, 3), (N_3, 4), ('A', 5) ]`
4. **Merge 4:** Extract $N_2$ (3) and $N_3$ (4) $\to$ Create internal node $N_4$ with weight $3+4 = 7$.
   Heap: `[ ('A', 5), (N_4, 7) ]`
5. **Merge 5:** Extract `'A'` (5) and $N_4$ (7) $\to$ Create root node with weight $5+7 = 12$.

```text
                        Root (12)
                       /         \
                   0  /           \  1
                   'A' (5)         N_4 (7)
                                 /         \
                             0  /           \  1
                              N_2 (3)        N_3 (4)
                             /      \        /      \
                         0  /      1 \   0  /      1 \
                          'D' (1)  'B' (2) N_1 (2)   'R' (2)
                                           /     \
                                       0  /     1 \
                                      '\n' (1)    'C' (1)
```

### Resulting Prefix Codes
- `'A'`: `0` (1 bit — most frequent symbol gets shortest code!)
- `'B'`: `101` (3 bits)
- `'D'`: `100` (3 bits)
- `'R'`: `111` (3 bits)
- `'\n'`: `1100` (4 bits)
- `'C'`: `1101` (4 bits)

### Bit Count Calculation
- **ASCII:** $12 \times 8 = \mathbf{96 \text{ bits}}$
- **Fixed-Length:** $D = 6 \implies \lceil \log_2 6 \rceil = 3 \text{ bits/char} \implies 12 \times 3 = \mathbf{36 \text{ bits}}$
- **Huffman:** $(5 \times 1) + (2 \times 3) + (1 \times 3) + (2 \times 3) + (1 \times 4) + (1 \times 4) = 5 + 6 + 3 + 6 + 4 + 4 = \mathbf{28 \text{ bits}}$!
- **Savings over ASCII:** $(1 - 28/96) = \mathbf{70.83\%}$!

---

## 8. Code Walkthrough by Function

### 1. Priority Queue / Min-Heap
- `minHeapify(MinHeap *minHeap, int idx)`: Restores heap property downwards in $O(\log n)$.
- `extractMin(MinHeap *minHeap)`: Removes `array[0]`, moves last element to top, and heapifies down.
- `insertMinHeap(MinHeap *minHeap, HuffmanNode *node)`: Places node at the end and bubbles up.

### 2. Tree Construction
- `buildHuffmanTree(freqs, &distinctCount)`:
  - Counts distinct characters.
  - Builds min-heap with leaf nodes.
  - Runs the greedy loop while `heap->size > 1`, extracting 2 nodes, merging into parent with freq = left + right, and inserting parent back into heap.
  - Returns final remaining node as root.

### 3. Tree Traversals
- `generateCodesRecursive(root, codeBuffer, depth, codeTable)`:
  - DFS traversal: adds `'0'` for left branches, `'1'` for right branches.
  - When reaching a leaf, records the code string into `codeTable[character]`.
- `serializeTreePreorder(root, BitWriter *bw)`:
  - If leaf: writes bit `1` followed by 8 bits of character.
  - If internal: writes bit `0` and recurses left and right.
- `deserializeTreePreorder(BitReader *br)`:
  - Reads 1 bit. If `1`, reads 8-bit character and returns leaf node.
  - If `0`, allocates internal node and recursively parses left and right children.

### 4. Compression & Decompression
- `compressFile(inputFile, outputFile)`:
  - Pass 1: Analyzes frequencies.
  - Builds tree and code table.
  - Prints generated codes table and comparisons.
  - Writes magic `HUF1`, 64-bit size, and preorder serialized tree.
  - Pass 2: Reads characters and writes variable-length Huffman bit codes using `BitWriter`.
  - Flushes buffer and closes files.
- `decompressFile(inputFile, outputFile, displayToConsole)`:
  - Verifies magic `HUF1` signature.
  - Reads 64-bit original size.
  - Reconstructs tree from preorder bits using `BitReader`.
  - Loops bit-by-bit from root to leaves, emitting characters until original size is reached.
  - Displays decompressed text to console.

---

## 9. How to Compile, Run, and Demo to Instructor

### Compilation
```bash
gcc -Wall -Wextra -O2 CSMCS26A017_Lab2_2.c -o CSMCS26A017_Lab2_2 -lm
```

### Interactive Menu Mode
```bash
./CSMCS26A017_Lab2_2
```
Presents a clear 4-option menu:
1. Compress a file (`.txt` $\to$ `.huf`)
2. Decompress a file (`.huf` $\to$ `.txt` and view on console)
3. Run automated demo suite on sample files
4. Exit

### Command-Line Arguments Mode
```bash
# Compress
./CSMCS26A017_Lab2_2 -c sample.txt sample.txt.huf

# Decompress and display
./CSMCS26A017_Lab2_2 -d sample.txt.huf sample_restored.txt

# Run full test suite with diff verification
./CSMCS26A017_Lab2_2 --test
```

---

## 10. Top Viva Questions & Answers (Securing the 6 Marks)

### Q1: Why is Huffman Coding called a "Greedy" algorithm?
> **Answer:** Because at every step, it makes the locally optimal choice: it greedily merges the two subtrees with the absolute lowest frequencies. This choice requires no backtracking and is proven mathematically to yield an optimal prefix-free tree with the minimal weighted path length.

### Q2: What is the Prefix Property and why is it essential?
> **Answer:** The prefix property ensures that no character's binary code is a prefix (initial segment) of any other character's code. Because of this property, a bitstream can be uniquely and instantaneously decoded from left to right without any delimiters or spaces between characters.

### Q3: What is the time complexity of building a Huffman Tree?
> **Answer:**
> - If there are $d$ distinct characters, inserting them into a min-heap takes $O(d)$.
> - The greedy loop runs $d-1$ times. In each iteration, 2 `extractMin` and 1 `insert` operations take $O(\log d)$ each.
> - Total time complexity: **$O(d \log d)$**.
> - For ASCII text where $d \le 256$, $d$ is a constant, so tree construction takes $O(1)$ constant time relative to the text length $N$. Reading the file takes $O(N)$.

### Q4: How is the `.huf` file decompressed if the receiver doesn't have the original table?
> **Answer:** The `.huf` file is completely self-contained. The header stores the tree using a preorder bit traversal (`0` for internal nodes, `1` followed by 8-bit character for leaf nodes) along with the original file size. The decompressor reads this header first, recreates the exact binary tree, and then traverses it using the payload bits to decode each character.

### Q5: Why do we store the uncompressed file size in the header?
> **Answer:** Because the bit writer packages bits into 8-bit bytes, the final byte may have 1 to 7 unused padding bits. Without knowing the exact original character count (or using an EOF marker), the decoder would interpret those extra bits as trailing garbage characters. Storing the exact byte count stops decoding at the exact original byte count.

### Q6: Can Huffman compression ever make a file larger?
> **Answer:** Yes! For very small files (like 5-10 bytes) or uniformly distributed random/encrypted data, the overhead of storing the header (magic bytes + file size + tree) can exceed the small savings from the payload bits. However, as file size grows and character frequencies become skewed, Huffman coding achieves significant net compression.
