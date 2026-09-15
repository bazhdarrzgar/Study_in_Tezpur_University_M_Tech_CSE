#!/usr/bin/env bash
# ==============================================================================
#  Script: test_run.sh
#  Purpose: Compile, test, and run Assignment 2.2 (Huffman Coding in C)
#  Roll No: CSMCS26A017
# ==============================================================================

set -e

# Change to script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

SRC="CSMCS26A017_Lab2_2.c"
BIN="CSMCS26A017_Lab2_2"

# ANSI Colors
GREEN="\033[1;32m"
BLUE="\033[1;34m"
YELLOW="\033[1;33m"
RED="\033[1;31m"
CYAN="\033[1;36m"
NC="\033[0m" # No Color

# Function: Clean generated artifacts
clean_artifacts() {
    echo -e "${YELLOW}[CLEAN] Removing generated files (*.huf, *_restored.txt, decompressed.txt)...${NC}"
    rm -f *.huf *_restored.txt decompressed.txt
    if [ "$1" == "--all" ]; then
        echo -e "${YELLOW}[CLEAN] Removing compiled binary ($BIN)...${NC}"
        rm -f "$BIN"
    fi
    echo -e "${GREEN}[CLEAN] Done!${NC}"
}

# Function: Compile C code
compile_code() {
    echo -e "${BLUE}[BUILD] Compiling $SRC with GCC...${NC}"
    gcc -Wall -Wextra -O2 "$SRC" -o "$BIN" -lm
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[BUILD SUCCESS] Executable '$BIN' is ready.${NC}\n"
    else
        echo -e "${RED}[BUILD FAILED] Compilation errors encountered.${NC}"
        exit 1
    fi
}

# Always ensure binary is up to date
if [ ! -f "$BIN" ] || [ "$SRC" -nt "$BIN" ]; then
    compile_code
fi

# Argument Handling
case "$1" in
    --clean|-c)
        clean_artifacts --all
        exit 0
        ;;
    --clean-outputs)
        clean_artifacts
        exit 0
        ;;
    --test|-t)
        echo -e "${CYAN}======================================================${NC}"
        echo -e "${CYAN}   RUNNING AUTOMATED TEST SUITE (SAMPLE FILES)        ${NC}"
        echo -e "${CYAN}======================================================${NC}"
        ./"$BIN" --test
        echo ""
        echo -e "${GREEN}[ALL TESTS COMPLETED SUCCESSFULLY]${NC}"
        exit 0
        ;;
    --help|-h)
        echo -e "${CYAN}Usage: ./test_run.sh [OPTION | FILENAME]${NC}"
        echo "Options:"
        echo "  (no argument)       Run in interactive menu mode for demo"
        echo "  -t, --test          Run automated test suite on sample test files"
        echo "  -c, --clean         Clean generated test files and compiled binary"
        echo "  --clean-outputs     Clean only generated .huf and restored files"
        echo "  <filename>          Compress, decompress, and verify a specific text file"
        echo "  -h, --help          Show this help message"
        exit 0
        ;;
    "")
        # Interactive mode
        echo -e "${GREEN}[STARTING] Launching interactive demo mode...${NC}"
        ./"$BIN"
        ;;
    *)
        # Single file test mode
        TARGET_FILE="$1"
        if [ ! -f "$TARGET_FILE" ]; then
            echo -e "${RED}Error: File '$TARGET_FILE' does not exist.${NC}"
            exit 1
        fi
        echo -e "${CYAN}======================================================${NC}"
        echo -e "${CYAN}   RUNNING COMPRESSION & DECOMPRESSION ON: $TARGET_FILE${NC}"
        echo -e "${CYAN}======================================================${NC}"
        ./"$BIN" "$TARGET_FILE"
        echo ""
        echo -e "${BLUE}[VERIFICATION] Checking byte-for-byte fidelity with diff...${NC}"
        if diff -s "$TARGET_FILE" "${TARGET_FILE}_restored.txt"; then
            echo -e "${GREEN}[PASS] Restored file matches '$TARGET_FILE' exactly!${NC}"
        else
            echo -e "${RED}[FAIL] Mismatch detected!${NC}"
            exit 1
        fi
        ;;
esac
