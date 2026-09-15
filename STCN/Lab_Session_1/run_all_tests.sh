#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=========================================================================="
echo "          STCN LAB EXPERIMENTS (SESSION 1) - MASTER TEST SUITE            "
echo "      Course: CSMT 502 (Selected Topics in Computer Networks)             "
echo "=========================================================================="
echo ""

EXPERIMENTS=(
    "Exp01_Framing:Framing Techniques (Bit & Byte Stuffing)"
    "Exp02_Error_Detection:Error Detection (Parity, Checksum, CRC)"
    "Exp03_Error_Correction:Error Correction (Hamming Code)"
    "Exp04_TCP_Sockets:TCP Client-Server Sockets"
    "Exp05_UDP_Sockets:UDP Client-Server Sockets"
    "Exp06_Routing_Algorithms:Routing Algorithms (Dijkstra & Distance Vector)"
    "Exp07_Flow_Control:Flow Control (Stop & Wait, GBN, Selective Repeat)"
    "Exp08_Traffic_Shaping:Traffic Shaping (Leaky Bucket & Token Bucket)"
    "Exp09_Cryptography_Ciphers:Classical Ciphers (Caesar & Substitution)"
    "Exp10_RSA_Algorithm:Public-Key Cryptography (RSA Algorithm)"
    "Exp11_NS2_Simulation:Network Simulation (NS-2 Routing Protocol)"
)

PASSED=0
TOTAL=${#EXPERIMENTS[@]}

for item in "${EXPERIMENTS[@]}"; do
    FOLDER="${item%%:*}"
    DESC="${item##*:}"

    echo "--------------------------------------------------------------------------"
    echo ">>> Running $FOLDER: $DESC ..."
    echo "--------------------------------------------------------------------------"

    if [ -d "$FOLDER" ] && [ -f "$FOLDER/test.sh" ]; then
        (cd "$FOLDER" && bash test.sh)
        echo "[PASSED] $FOLDER"
        PASSED=$((PASSED + 1))
    else
        echo "[FAILED] $FOLDER: Missing folder or test.sh"
        exit 1
    fi
    echo ""
done

echo "=========================================================================="
echo "                         TEST EXECUTION SUMMARY                           "
echo "=========================================================================="
echo "  Total Experiments Executed : $TOTAL"
echo "  Total Experiments Passed   : $PASSED"
echo "  Status                     : ALL TESTS PASSED WITH 100% SUCCESS!"
echo "=========================================================================="
