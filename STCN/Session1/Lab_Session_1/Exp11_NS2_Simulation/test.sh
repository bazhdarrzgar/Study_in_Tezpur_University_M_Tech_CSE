#!/bin/bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

echo "=== Testing Experiment 11 (NS-2 Routing Simulation) ==="
bash run_ns2.sh

echo -e "\n>>> Experiment 11: ALL TESTS PASSED SUCCESSFULLY! <<<\n"
