/**
 * CSMT503 - Advanced Programming Lab 1
 * CSMT533 - Data Structures and Algorithms Lab
 * Assignment 2.2: Huffman Coding (Greedy Algorithms)
 *
 * Student Roll Number: CSMCS26A017
 * File: CSMCS26A017_Lab2_2.c
 *
 * Description:
 *  - Computes character frequencies and constructs an optimal prefix Huffman Tree
 *    using a greedy approach with a Min-Priority Queue (Min-Heap).
 *  - Generates and displays variable-length prefix codes for each character.
 *  - Compares required storage bits: ASCII (8-bit) vs. Fixed-Length vs. Huffman.
 *  - Compresses text into a true binary file (.huf) packed at bit-level.
 *  - Stores header with magic identifier, file size, and preorder tree traversal.
 *  - Reads header from .huf file, reconstructs tree, decompresses, and displays contents.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>

#define MAGIC_LEN 4
static const char HUF_MAGIC[MAGIC_LEN] = {'H', 'U', 'F', '1'};
#define MAX_TREE_HT 256
#define NUM_CHARS 256
#define MAX_PATH_LEN 512

/* ========================================================================= */
/*                          1. DATA STRUCTURES                               */
/* ========================================================================= */

/**
 * Huffman Tree Node
 */
typedef struct HuffmanNode {
    unsigned char data;          /* Character symbol */
    uint64_t freq;               /* Frequency of character */
    struct HuffmanNode *left;    /* Left child (0 bit) */
    struct HuffmanNode *right;   /* Right child (1 bit) */
} HuffmanNode;

/**
 * Min Heap for Greedy Priority Queue
 */
typedef struct MinHeap {
    int size;
    int capacity;
    HuffmanNode **array;
} MinHeap;

/**
 * BitWriter: Packs individual bits into full bytes before writing to file
 */
typedef struct {
    FILE *fp;
    unsigned char buffer;
    int bitCount;                /* Bits currently placed in buffer (0 to 7) */
    uint64_t totalBytesWritten;
} BitWriter;

/**
 * BitReader: Reads bytes from file and serves one bit at a time
 */
typedef struct {
    FILE *fp;
    unsigned char buffer;
    int bitCount;                /* Bits remaining in buffer (0 to 8) */
} BitReader;

/* Structure to store generated code for each character */
typedef struct {
    char code[MAX_TREE_HT];
    int length;
} CodeEntry;

/* ========================================================================= */
/*                         2. BIT-LEVEL I/O ROUTINES                         */
/* ========================================================================= */

void bitWriterInit(BitWriter *bw, FILE *fp) {
    bw->fp = fp;
    bw->buffer = 0;
    bw->bitCount = 0;
    bw->totalBytesWritten = 0;
}

void writeBit(BitWriter *bw, int bit) {
    if (bit) {
        bw->buffer |= (unsigned char)(1 << (7 - bw->bitCount));
    }
    bw->bitCount++;
    if (bw->bitCount == 8) {
        fputc(bw->buffer, bw->fp);
        bw->totalBytesWritten++;
        bw->buffer = 0;
        bw->bitCount = 0;
    }
}

void writeBits(BitWriter *bw, uint32_t val, int numBits) {
    for (int i = numBits - 1; i >= 0; i--) {
        writeBit(bw, (int)((val >> i) & 1));
    }
}

void bitWriterFlush(BitWriter *bw) {
    if (bw->bitCount > 0) {
        fputc(bw->buffer, bw->fp);
        bw->totalBytesWritten++;
        bw->buffer = 0;
        bw->bitCount = 0;
    }
}

void bitReaderInit(BitReader *br, FILE *fp) {
    br->fp = fp;
    br->buffer = 0;
    br->bitCount = 0;
}

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

int readBits(BitReader *br, int numBits, uint32_t *val) {
    uint32_t res = 0;
    for (int i = 0; i < numBits; i++) {
        int b = readBit(br);
        if (b == -1) return -1;
        res = (res << 1) | (uint32_t)b;
    }
    *val = res;
    return 0;
}

/* ========================================================================= */
/*                 3. MIN-HEAP & GREEDY HUFFMAN TREE BUILDER                 */
/* ========================================================================= */

HuffmanNode* createNode(unsigned char data, uint64_t freq) {
    HuffmanNode *node = (HuffmanNode *)malloc(sizeof(HuffmanNode));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for HuffmanNode\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

MinHeap* createMinHeap(int capacity) {
    MinHeap *minHeap = (MinHeap *)malloc(sizeof(MinHeap));
    if (!minHeap) {
        fprintf(stderr, "Error: Memory allocation failed for MinHeap\n");
        exit(EXIT_FAILURE);
    }
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HuffmanNode **)malloc(capacity * sizeof(HuffmanNode *));
    if (!minHeap->array) {
        fprintf(stderr, "Error: Memory allocation failed for MinHeap array\n");
        exit(EXIT_FAILURE);
    }
    return minHeap;
}

void swapNodes(HuffmanNode **a, HuffmanNode **b) {
    HuffmanNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

HuffmanNode* extractMin(MinHeap *minHeap) {
    if (minHeap->size <= 0) return NULL;
    HuffmanNode *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap *minHeap, HuffmanNode *node) {
    minHeap->size++;
    int i = minHeap->size - 1;
    while (i > 0 && node->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

void freeTree(HuffmanNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void freeMinHeap(MinHeap *minHeap) {
    if (minHeap) {
        if (minHeap->array) free(minHeap->array);
        free(minHeap);
    }
}

int isLeaf(HuffmanNode *node) {
    return (node->left == NULL && node->right == NULL);
}

/**
 * Builds the Huffman tree using Greedy Strategy (Cormen et al., Section 16.3)
 */
HuffmanNode* buildHuffmanTree(const uint64_t freqs[NUM_CHARS], int *distinctCount) {
    int distinct = 0;
    for (int i = 0; i < NUM_CHARS; i++) {
        if (freqs[i] > 0) distinct++;
    }
    *distinctCount = distinct;

    if (distinct == 0) return NULL;

    /* Handle edge case: single unique character.
     * Build full binary tree with dummy leaf on right so serialization is symmetrical. */
    if (distinct == 1) {
        for (int i = 0; i < NUM_CHARS; i++) {
            if (freqs[i] > 0) {
                HuffmanNode *root = createNode('$', freqs[i]);
                root->left = createNode((unsigned char)i, freqs[i]);
                root->right = createNode(0, 0); /* Dummy leaf node */
                return root;
            }
        }
    }

    MinHeap *minHeap = createMinHeap(distinct);
    for (int i = 0; i < NUM_CHARS; i++) {
        if (freqs[i] > 0) {
            insertMinHeap(minHeap, createNode((unsigned char)i, freqs[i]));
        }
    }

    /* Greedy loop: merge two lowest-frequency trees */
    while (minHeap->size > 1) {
        HuffmanNode *left = extractMin(minHeap);
        HuffmanNode *right = extractMin(minHeap);

        HuffmanNode *top = createNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    HuffmanNode *root = extractMin(minHeap);
    freeMinHeap(minHeap);
    return root;
}

/* ========================================================================= */
/*                     4. CODE GENERATION & TRAVERSAL                        */
/* ========================================================================= */

void generateCodesRecursive(HuffmanNode *root, char *codeBuffer, int depth, CodeEntry codeTable[NUM_CHARS]) {
    if (!root) return;

    if (isLeaf(root)) {
        if (depth == 0) {
            /* If root itself is a leaf */
            codeBuffer[0] = '0';
            codeBuffer[1] = '\0';
            strcpy(codeTable[root->data].code, codeBuffer);
            codeTable[root->data].length = 1;
        } else {
            codeBuffer[depth] = '\0';
            strcpy(codeTable[root->data].code, codeBuffer);
            codeTable[root->data].length = depth;
        }
        return;
    }

    if (root->left) {
        codeBuffer[depth] = '0';
        generateCodesRecursive(root->left, codeBuffer, depth + 1, codeTable);
    }

    if (root->right) {
        codeBuffer[depth] = '1';
        generateCodesRecursive(root->right, codeBuffer, depth + 1, codeTable);
    }
}

void generateCodes(HuffmanNode *root, CodeEntry codeTable[NUM_CHARS]) {
    for (int i = 0; i < NUM_CHARS; i++) {
        codeTable[i].code[0] = '\0';
        codeTable[i].length = 0;
    }
    if (!root) return;

    char buffer[MAX_TREE_HT];
    generateCodesRecursive(root, buffer, 0, codeTable);
}

/* Helper to format character display for console */
void formatChar(unsigned char c, char *out, size_t outSize) {
    if (c == ' ') {
        snprintf(out, outSize, "' ' (space)");
    } else if (c == '\n') {
        snprintf(out, outSize, "'\\n' (LF)");
    } else if (c == '\r') {
        snprintf(out, outSize, "'\\r' (CR)");
    } else if (c == '\t') {
        snprintf(out, outSize, "'\\t' (tab)");
    } else if (isprint(c)) {
        snprintf(out, outSize, "'%c'", c);
    } else {
        snprintf(out, outSize, "0x%02X", c);
    }
}

void printCodesTable(const uint64_t freqs[NUM_CHARS], const CodeEntry codeTable[NUM_CHARS], uint64_t totalChars) {
    printf("\n+-------------------------------------------------------------------------+\n");
    printf("|                      HUFFMAN CODES GENERATED TABLE                      |\n");
    printf("+-------+-----------------+------------+-------------+--------------------+\n");
    printf("| ASCII | Character       | Frequency  | Percentage  | Huffman Code       |\n");
    printf("+-------+-----------------+------------+-------------+--------------------+\n");

    for (int i = 0; i < NUM_CHARS; i++) {
        if (freqs[i] > 0) {
            char charName[32];
            formatChar((unsigned char)i, charName, sizeof(charName));
            double pct = (totalChars > 0) ? (100.0 * (double)freqs[i] / (double)totalChars) : 0.0;
            printf("| %-5d | %-15s | %-10llu | %9.2f%% | %-18s |\n",
                   i, charName, (unsigned long long)freqs[i], pct, codeTable[i].code);
        }
    }
    printf("+-------+-----------------+------------+-------------+--------------------+\n");
}

/* ========================================================================= */
/*                     5. BIT COMPARISON & ANALYSIS                          */
/* ========================================================================= */

void printBitComparison(uint64_t totalChars, int distinctCount,
                        const uint64_t freqs[NUM_CHARS],
                        const CodeEntry codeTable[NUM_CHARS],
                        uint64_t actualHufFileSize) {
    if (totalChars == 0) {
        printf("\nFile is empty (0 characters). No comparisons to display.\n");
        return;
    }

    /* 1. Standard ASCII Code (8 bits per character) */
    uint64_t asciiBits = totalChars * 8;
    double asciiBytes = (double)asciiBits / 8.0;

    /* 2. Fixed-Length Code: ceil(log2(distinctCount)) bits per character */
    int fixedBitsPerChar = 1;
    if (distinctCount > 1) {
        fixedBitsPerChar = (int)ceil(log2((double)distinctCount));
    }
    uint64_t fixedBits = totalChars * (uint64_t)fixedBitsPerChar;
    double fixedBytes = (double)fixedBits / 8.0;

    /* 3. Pure Huffman Code Payload (variable length prefix codes) */
    uint64_t huffmanPayloadBits = 0;
    for (int i = 0; i < NUM_CHARS; i++) {
        if (freqs[i] > 0) {
            huffmanPayloadBits += freqs[i] * (uint64_t)codeTable[i].length;
        }
    }
    double huffmanPayloadBytes = (double)huffmanPayloadBits / 8.0;

    /* 4. Total .huf File (Payload + Self-Contained Preorder Header) */
    uint64_t actualHufBits = actualHufFileSize * 8;

    double savingsVsAscii = (1.0 - ((double)huffmanPayloadBits / (double)asciiBits)) * 100.0;
    double savingsVsFixed = (1.0 - ((double)huffmanPayloadBits / (double)fixedBits)) * 100.0;
    double totalFileSavingsVsAscii = (1.0 - ((double)actualHufFileSize / (double)totalChars)) * 100.0;

    printf("\n===========================================================================\n");
    printf("                  BIT COMPARISON & COMPRESSION ANALYSIS                    \n");
    printf("===========================================================================\n");
    printf("Total Characters in File (N)           : %llu\n", (unsigned long long)totalChars);
    printf("Distinct Characters (Alphabet |Sigma|) : %d\n", distinctCount);
    printf("Fixed-Length Code Width [ceil(lg|Sigma|)]: %d bits/char\n", fixedBitsPerChar);
    printf("---------------------------------------------------------------------------\n");
    printf("%-32s | %14s | %14s | %10s\n", "Encoding Scheme", "Total Bits", "Equivalent (B)", "Ratio");
    printf("---------------------------------+----------------+----------------+-----------\n");
    printf("%-32s | %14llu | %14.2f | %9.2f%%\n",
           "1. Standard ASCII (8-bit)", (unsigned long long)asciiBits, asciiBytes, 100.0);
    printf("%-32s | %14llu | %14.2f | %9.2f%%\n",
           "2. Fixed-Length Code", (unsigned long long)fixedBits, fixedBytes,
           (double)fixedBits / (double)asciiBits * 100.0);
    printf("%-32s | %14llu | %14.2f | %9.2f%%\n",
           "3. Huffman Code (Pure Payload)", (unsigned long long)huffmanPayloadBits,
           huffmanPayloadBytes, (double)huffmanPayloadBits / (double)asciiBits * 100.0);
    if (actualHufFileSize > 0) {
        printf("%-32s | %14llu | %14llu | %9.2f%%\n",
               "4. Total .huf File (with Header)", (unsigned long long)actualHufBits,
               (unsigned long long)actualHufFileSize,
               (double)actualHufFileSize / (double)totalChars * 100.0);
    }
    printf("---------------------------------------------------------------------------\n");
    printf(">> Storage Savings by Huffman Payload over ASCII       : %6.2f%%\n", savingsVsAscii);
    printf(">> Storage Savings by Huffman Payload over Fixed-Length: %6.2f%%\n", savingsVsFixed);
    if (actualHufFileSize > 0) {
        printf(">> Net Compression Savings (including entire header)  : %6.2f%%\n", totalFileSavingsVsAscii);
    }
    printf("===========================================================================\n");
}

/* ========================================================================= */
/*               6. PREORDER TREE SERIALIZATION (HEADER FORMAT)              */
/* ========================================================================= */

/**
 * Serializes the Huffman Tree using Preorder Traversal as recommended in hints:
 *  - Internal node: write bit '0'
 *  - Leaf node    : write bit '1', followed by the 8-bit character byte
 */
void serializeTreePreorder(HuffmanNode *root, BitWriter *bw) {
    if (!root) return;

    if (isLeaf(root)) {
        writeBit(bw, 1);
        writeBits(bw, (uint32_t)root->data, 8);
    } else {
        writeBit(bw, 0);
        serializeTreePreorder(root->left, bw);
        serializeTreePreorder(root->right, bw);
    }
}

/**
 * Reconstructs the Huffman Tree from the serialized preorder bit stream.
 */
HuffmanNode* deserializeTreePreorder(BitReader *br) {
    int bit = readBit(br);
    if (bit == -1) return NULL;

    if (bit == 1) {
        /* Leaf node */
        uint32_t charVal = 0;
        if (readBits(br, 8, &charVal) == -1) return NULL;
        return createNode((unsigned char)charVal, 0);
    } else {
        /* Internal node */
        HuffmanNode *node = createNode('$', 0);
        node->left = deserializeTreePreorder(br);
        node->right = deserializeTreePreorder(br);
        return node;
    }
}

/* ========================================================================= */
/*                        7. COMPRESSION ENGINE                              */
/* ========================================================================= */

int compressFile(const char *inputFilename, const char *outputFilename) {
    FILE *in = fopen(inputFilename, "rb");
    if (!in) {
        fprintf(stderr, "Error: Unable to open input file '%s' for reading.\n", inputFilename);
        return -1;
    }

    /* Pass 1: Count character frequencies and total bytes */
    uint64_t freqs[NUM_CHARS] = {0};
    uint64_t totalChars = 0;
    int ch;
    while ((ch = fgetc(in)) != EOF) {
        freqs[(unsigned char)ch]++;
        totalChars++;
    }

    if (totalChars == 0) {
        printf("Notice: Input file '%s' is empty (0 bytes).\n", inputFilename);
    }

    /* Build Huffman Tree & Generate Codes */
    int distinctCount = 0;
    HuffmanNode *root = buildHuffmanTree(freqs, &distinctCount);
    CodeEntry codeTable[NUM_CHARS];
    generateCodes(root, codeTable);

    /* Display character codes table */
    printCodesTable(freqs, codeTable, totalChars);

    /* Open output .huf binary file */
    FILE *out = fopen(outputFilename, "wb");
    if (!out) {
        fprintf(stderr, "Error: Unable to open output file '%s' for writing.\n", outputFilename);
        fclose(in);
        freeTree(root);
        return -1;
    }

    /* Write File Header:
     * 1. 4 bytes Magic Identifier: 'H','U','F','1'
     * 2. 8 bytes: Original uncompressed file size (uint64_t big-endian)
     */
    fwrite(HUF_MAGIC, 1, MAGIC_LEN, out);

    for (int i = 7; i >= 0; i--) {
        unsigned char byte = (unsigned char)((totalChars >> (i * 8)) & 0xFF);
        fputc(byte, out);
    }

    /* If file is not empty, write preorder tree and compressed payload */
    if (totalChars > 0 && root != NULL) {
        BitWriter bw;
        bitWriterInit(&bw, out);

        /* 3. Serialize Huffman Tree in Preorder */
        serializeTreePreorder(root, &bw);

        /* 4. Pass 2: Encode payload characters bit by bit */
        rewind(in);
        while ((ch = fgetc(in)) != EOF) {
            const char *cStr = codeTable[(unsigned char)ch].code;
            for (int i = 0; cStr[i] != '\0'; i++) {
                writeBit(&bw, cStr[i] - '0');
            }
        }

        /* Flush any remaining bits in the bit writer buffer */
        bitWriterFlush(&bw);
    }

    fclose(in);
    fclose(out);

    /* Get exact output file size */
    FILE *checkFp = fopen(outputFilename, "rb");
    uint64_t outSize = 0;
    if (checkFp) {
        fseek(checkFp, 0, SEEK_END);
        outSize = (uint64_t)ftell(checkFp);
        fclose(checkFp);
    }

    /* Display Bit Comparison Analysis */
    printBitComparison(totalChars, distinctCount, freqs, codeTable, outSize);

    printf("\n[SUCCESS] File compressed successfully!\n");
    printf("   Input File : %s (%llu bytes)\n", inputFilename, (unsigned long long)totalChars);
    printf("   Output File: %s (%llu bytes)\n", outputFilename, (unsigned long long)outSize);

    freeTree(root);
    return 0;
}

/* ========================================================================= */
/*                       8. DECOMPRESSION ENGINE                             */
/* ========================================================================= */

int decompressFile(const char *inputFilename, const char *outputFilename, int displayToConsole) {
    FILE *in = fopen(inputFilename, "rb");
    if (!in) {
        fprintf(stderr, "Error: Unable to open compressed file '%s' for reading.\n", inputFilename);
        return -1;
    }

    /* 1. Verify Magic Identifier */
    char magic[MAGIC_LEN];
    if (fread(magic, 1, MAGIC_LEN, in) != MAGIC_LEN || memcmp(magic, HUF_MAGIC, MAGIC_LEN) != 0) {
        fprintf(stderr, "Error: Invalid file format! File does not begin with 'HUF1' header.\n");
        fclose(in);
        return -1;
    }

    /* 2. Read 64-bit Original File Size */
    uint64_t origSize = 0;
    for (int i = 0; i < 8; i++) {
        int byte = fgetc(in);
        if (byte == EOF) {
            fprintf(stderr, "Error: Corrupted header while reading original file size.\n");
            fclose(in);
            return -1;
        }
        origSize = (origSize << 8) | (unsigned char)byte;
    }

    printf("\n+-------------------------------------------------------------------------+\n");
    printf("|                      DECOMPRESSING .HUF ARCHIVE                         |\n");
    printf("+-------------------------------------------------------------------------+\n");
    printf("Header Verified           : HUF1 (Valid Huffman Compressed File)\n");
    printf("Original File Size Stored : %llu bytes\n", (unsigned long long)origSize);

    FILE *out = fopen(outputFilename, "wb");
    if (!out) {
        fprintf(stderr, "Error: Unable to open output file '%s' for writing.\n", outputFilename);
        fclose(in);
        return -1;
    }

    if (origSize == 0) {
        printf("Notice: Original file was empty. Created empty decompressed file.\n");
        fclose(in);
        fclose(out);
        return 0;
    }

    /* 3. Reconstruct Tree from Preorder Traversal bits in Header */
    BitReader br;
    bitReaderInit(&br, in);
    HuffmanNode *root = deserializeTreePreorder(&br);
    if (!root) {
        fprintf(stderr, "Error: Failed to reconstruct Huffman tree from file header.\n");
        fclose(in);
        fclose(out);
        return -1;
    }
    printf("Huffman Tree Status       : Successfully reconstructed from preorder header\n");

    /* 4. Decode Payload bit-by-bit until origSize bytes are decoded */
    printf("Decompressing payload...\n");
    uint64_t bytesDecoded = 0;
    HuffmanNode *curr = root;

    if (isLeaf(root)) {
        for (uint64_t i = 0; i < origSize; i++) {
            fputc(root->data, out);
        }
        bytesDecoded = origSize;
    } else {
        while (bytesDecoded < origSize) {
            int bit = readBit(&br);
            if (bit == -1) {
                fprintf(stderr, "Warning: Unexpected End-of-File reached before completing decompression.\n");
                break;
            }

            curr = (bit == 0) ? curr->left : curr->right;
            if (!curr) {
                fprintf(stderr, "Error: Invalid path encountered in Huffman tree during decoding.\n");
                break;
            }

            if (isLeaf(curr)) {
                fputc(curr->data, out);
                bytesDecoded++;
                curr = root;
            }
        }
    }

    fclose(in);
    fclose(out);
    freeTree(root);

    printf("[SUCCESS] Decompression complete! Decoded %llu / %llu bytes.\n",
           (unsigned long long)bytesDecoded, (unsigned long long)origSize);
    printf("Saved decompressed text to: %s\n", outputFilename);

    /* 5. Display Decompressed Content on Console */
    if (displayToConsole) {
        printf("\n===========================================================================\n");
        printf("                    DECOMPRESSED FILE CONTENTS                             \n");
        printf("===========================================================================\n");
        FILE *dispFp = fopen(outputFilename, "r");
        if (dispFp) {
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), dispFp)) {
                printf("%s", buffer);
            }
            fclose(dispFp);
        }
        printf("\n===========================================================================\n");
    }

    return 0;
}

/* ========================================================================= */
/*               9. AUTOMATED TEST & DEMONSTRATION SUITE                     */
/* ========================================================================= */

void runAutomatedDemo(const char *testFile) {
    char hufFile[MAX_PATH_LEN];
    char decompFile[MAX_PATH_LEN];
    snprintf(hufFile, sizeof(hufFile), "%s.huf", testFile);
    snprintf(decompFile, sizeof(decompFile), "%s_restored.txt", testFile);

    printf("\n***************************************************************************\n");
    printf("           RUNNING AUTOMATED TEST ON: %s\n", testFile);
    printf("***************************************************************************\n");

    if (compressFile(testFile, hufFile) != 0) {
        printf("[FAIL] Compression failed for %s\n", testFile);
        return;
    }

    if (decompressFile(hufFile, decompFile, 1) != 0) {
        printf("[FAIL] Decompression failed for %s\n", testFile);
        return;
    }

    /* Verification: byte-for-byte check between original and decompressed */
    FILE *f1 = fopen(testFile, "rb");
    FILE *f2 = fopen(decompFile, "rb");
    int match = 1;
    if (!f1 || !f2) {
        match = 0;
    } else {
        int c1, c2;
        do {
            c1 = fgetc(f1);
            c2 = fgetc(f2);
            if (c1 != c2) {
                match = 0;
                break;
            }
        } while (c1 != EOF && c2 != EOF);
    }
    if (f1) fclose(f1);
    if (f2) fclose(f2);

    if (match) {
        printf("\n[VERIFICATION PASSED]: '%s' and '%s' are 100%% IDENTICAL!\n", testFile, decompFile);
    } else {
        printf("\n[VERIFICATION FAILED]: Mismatch detected between original and decompressed files!\n");
    }
}

/* ========================================================================= */
/*                          10. INTERACTIVE MENU                             */
/* ========================================================================= */

void showMenu() {
    printf("\n");
    printf("===========================================================================\n");
    printf("         CSMT503 / CSMT533 - ASSIGNMENT 2.2: HUFFMAN CODING               \n");
    printf("                       Roll No: CSMCS26A017                                \n");
    printf("===========================================================================\n");
    printf(" 1. Compress a Text File (.txt -> .huf)\n");
    printf("    - Generates & Displays Huffman Codes Table\n");
    printf("    - Compares Bits: ASCII vs Fixed-Length vs Huffman\n");
    printf("    - Stores Bit-Packed File with Preorder Serialized Header\n");
    printf(" 2. Decompress a .huf File (.huf -> .txt)\n");
    printf("    - Reads Header & Rebuilds Huffman Tree from Preorder Bits\n");
    printf("    - Decodes Payload & Displays Content to Console\n");
    printf(" 3. Run Automated Demonstration Suite (sample.txt, test_short.txt)\n");
    printf(" 4. Exit\n");
    printf("===========================================================================\n");
    printf("Enter choice (1-4): ");
}

/* Helper to strip trailing newline from fgets */
void trimNewline(char *str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

int main(int argc, char *argv[]) {
    /* Command-line mode support for scripted testing and grading */
    if (argc >= 2) {
        if (strcmp(argv[1], "-c") == 0 && argc == 4) {
            return compressFile(argv[2], argv[3]);
        } else if (strcmp(argv[1], "-d") == 0 && argc == 4) {
            return decompressFile(argv[2], argv[3], 1);
        } else if (strcmp(argv[1], "--test") == 0) {
            runAutomatedDemo("sample.txt");
            runAutomatedDemo("test_short.txt");
            runAutomatedDemo("test_single.txt");
            return 0;
        } else if (argc == 2) {
            /* Quick run on specified file */
            char outHuf[MAX_PATH_LEN + 32];
            char outTxt[MAX_PATH_LEN + 32];
            snprintf(outHuf, sizeof(outHuf), "%s.huf", argv[1]);
            snprintf(outTxt, sizeof(outTxt), "%s_restored.txt", argv[1]);
            compressFile(argv[1], outHuf);
            decompressFile(outHuf, outTxt, 1);
            return 0;
        } else {
            printf("Usage:\n");
            printf("  Interactive Mode: %s\n", argv[0]);
            printf("  Compress Mode   : %s -c <input.txt> <output.huf>\n", argv[0]);
            printf("  Decompress Mode : %s -d <input.huf> <output.txt>\n", argv[0]);
            printf("  Test Mode       : %s --test\n", argv[0]);
            return 1;
        }
    }

    /* Interactive Menu Mode */
    int choice = 0;
    char inFilename[MAX_PATH_LEN];
    char outFilename[MAX_PATH_LEN + 32];

    while (1) {
        showMenu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); /* clear invalid input */
            printf("Invalid input. Please enter a number from 1 to 4.\n");
            continue;
        }
        while (getchar() != '\n'); /* clear newline */

        switch (choice) {
            case 1: {
                printf("Enter text file to compress (e.g. sample.txt): ");
                if (!fgets(inFilename, sizeof(inFilename), stdin)) break;
                trimNewline(inFilename);

                printf("Enter output compressed filename (or press Enter for '%s.huf'): ", inFilename);
                if (!fgets(outFilename, sizeof(outFilename), stdin)) break;
                trimNewline(outFilename);
                if (strlen(outFilename) == 0) {
                    snprintf(outFilename, sizeof(outFilename), "%s.huf", inFilename);
                }

                compressFile(inFilename, outFilename);
                break;
            }
            case 2: {
                printf("Enter compressed .huf file (e.g. sample.txt.huf): ");
                if (!fgets(inFilename, sizeof(inFilename), stdin)) break;
                trimNewline(inFilename);

                printf("Enter output decompressed filename (or press Enter for 'decompressed.txt'): ");
                if (!fgets(outFilename, sizeof(outFilename), stdin)) break;
                trimNewline(outFilename);
                if (strlen(outFilename) == 0) {
                    snprintf(outFilename, sizeof(outFilename), "decompressed.txt");
                }

                decompressFile(inFilename, outFilename, 1);
                break;
            }
            case 3: {
                runAutomatedDemo("sample.txt");
                runAutomatedDemo("test_short.txt");
                runAutomatedDemo("test_single.txt");
                break;
            }
            case 4:
                printf("\nExiting Huffman Coding Program. Best of luck with your lab evaluation!\n");
                return 0;
            default:
                printf("Invalid choice. Please select 1, 2, 3, or 4.\n");
        }
    }

    return 0;
}
