/**
 * Experiment 2(c): Error Detection - Cyclic Redundancy Check (CRC)
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * CRC is a polynomial-based error detection technique using modulo-2 arithmetic (XOR).
 * Let Data = D(x) of length m, Generator = G(x) of length r.
 * 1. Sender:
 *    - Appends (r - 1) zeros to data: D'(x) = D(x) * 2^(r-1).
 *    - Performs modulo-2 division D'(x) / G(x) to find remainder R(x) (CRC/FCS).
 *    - Transmits Codeword T(x) = D(x) with R(x) appended.
 * 2. Receiver:
 *    - Divides received T'(x) by G(x).
 *    - If remainder == 0: No detectable error, accept frame.
 *    - If remainder != 0: Error detected, reject frame.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 2048

void xor_op(char *dividend, const char *divisor, int start, int div_len) {
    for (int i = 0; i < div_len; i++) {
        dividend[start + i] = (dividend[start + i] == divisor[i]) ? '0' : '1';
    }
}

void compute_crc(const char *data, const char *generator, char *remainder) {
    int data_len = strlen(data);
    int gen_len  = strlen(generator);

    // Temp buffer for dividend: data + (gen_len - 1) zeros
    char temp[MAX_LEN];
    strcpy(temp, data);
    for (int i = 0; i < gen_len - 1; i++) {
        temp[data_len + i] = '0';
    }
    temp[data_len + gen_len - 1] = '\0';

    int total_len = data_len + gen_len - 1;

    for (int i = 0; i <= total_len - gen_len; i++) {
        if (temp[i] == '1') {
            xor_op(temp, generator, i, gen_len);
        }
    }

    // Remainder is last (gen_len - 1) bits
    strncpy(remainder, temp + data_len, gen_len - 1);
    remainder[gen_len - 1] = '\0';
}

int check_crc(const char *codeword, const char *generator) {
    int code_len = strlen(codeword);
    int gen_len  = strlen(generator);

    char temp[MAX_LEN];
    strcpy(temp, codeword);

    for (int i = 0; i <= code_len - gen_len; i++) {
        if (temp[i] == '1') {
            xor_op(temp, generator, i, gen_len);
        }
    }

    // Check if remaining bits are all '0'
    for (int i = code_len - gen_len + 1; i < code_len; i++) {
        if (temp[i] != '0') {
            return 0; // Error detected
        }
    }
    return 1; // Valid (Remainder == 0)
}

int main(int argc, char *argv[]) {
    char data[MAX_LEN] = "100100";          // Default data
    char generator[MAX_LEN] = "1101";       // Default divisor (x^3 + x^2 + 1)
    char remainder[MAX_LEN];
    char codeword[MAX_LEN];

    if (argc >= 3) {
        strncpy(data, argv[1], MAX_LEN - 1);
        strncpy(generator, argv[2], MAX_LEN - 1);
    } else {
        printf("Using default data='%s', generator='%s'\n", data, generator);
    }

    printf("\n--- CYCLIC REDUNDANCY CHECK (CRC) DEMONSTRATION ---\n");
    printf("Original Data bits (D) : %s\n", data);
    printf("Generator polynomial(G): %s\n", generator);

    compute_crc(data, generator, remainder);
    printf("Calculated Remainder   : %s (CRC bits)\n", remainder);

    // Construct transmitted codeword = Data + Remainder
    snprintf(codeword, sizeof(codeword), "%s%s", data, remainder);
    printf("Transmitted Codeword (T): %s\n", codeword);

    printf("\n--- RECEIVER VERIFICATION ---\n");
    // Test Case 1: Error-free transmission
    int status1 = check_crc(codeword, generator);
    printf("[Scenario 1 - No Channel Error]:\n");
    printf("  Received Codeword     : %s\n", codeword);
    printf("  CRC Validation Result : %s\n", status1 ? "PASSED (Remainder = 0, No Error)" : "FAILED");

    // Test Case 2: 1-bit error introduction
    char corrupt_codeword[MAX_LEN];
    strcpy(corrupt_codeword, codeword);
    corrupt_codeword[0] = (corrupt_codeword[0] == '1') ? '0' : '1'; // Flip first bit
    int status2 = check_crc(corrupt_codeword, generator);
    printf("\n[Scenario 2 - 1-bit Channel Error Injected]:\n");
    printf("  Corrupted Codeword    : %s\n", corrupt_codeword);
    printf("  CRC Validation Result : %s\n", status2 ? "Passed" : "ERROR DETECTED! Frame Discarded.");

    return (status1 && !status2) ? 0 : 1;
}
