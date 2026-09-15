/**
 * Experiment 3: Error Correction - Hamming Code
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * Hamming Code is an error-detecting and single-bit error-correcting linear code.
 * For k data bits, r redundant (parity) bits are added such that:
 *   2^r >= k + r + 1
 * Parity bits are placed at bit positions that are powers of 2 (1, 2, 4, 8, ...).
 * The remaining positions hold data bits.
 *
 * Even parity rule:
 * - Parity bit P_i checks all bit positions whose binary representation has the i-th bit set.
 *
 * Error Correction:
 * - Receiver recalculates the parity bits. The binary value formed by the syndrome
 *   bits (S_r ... S_2 S_1) directly indicates the 1-based index of the corrupted bit.
 * - If syndrome == 0, no error occurred.
 * - If syndrome > 0, flipping that bit corrects the transmission!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_BITS 64

bool is_power_of_two(int n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

int calculate_r(int k) {
    int r = 1;
    while ((1 << r) < (k + r + 1)) {
        r++;
    }
    return r;
}

void encode_hamming(const char *data_bits, int k, int *codeword, int *n_out, int *r_out) {
    int r = calculate_r(k);
    int n = k + r;

    // 1-indexed codeword array (indices 1 to n)
    for (int i = 1; i <= n; i++) {
        codeword[i] = 0;
    }

    // Place data bits into non-power-of-two positions
    int data_idx = 0;
    for (int i = 1; i <= n; i++) {
        if (!is_power_of_two(i)) {
            codeword[i] = data_bits[data_idx++] - '0';
        }
    }

    // Calculate parity bits
    for (int i = 0; i < r; i++) {
        int pos = (1 << i);
        int ones = 0;
        for (int j = 1; j <= n; j++) {
            if (j & pos) {
                if (j != pos && codeword[j] == 1) {
                    ones++;
                }
            }
        }
        codeword[pos] = (ones % 2 == 0) ? 0 : 1; // Even parity
    }

    *n_out = n;
    *r_out = r;
}

int decode_and_correct(int *received, int n, int r, char *recovered_data) {
    int syndrome = 0;

    for (int i = 0; i < r; i++) {
        int pos = (1 << i);
        int ones = 0;
        for (int j = 1; j <= n; j++) {
            if (j & pos) {
                if (received[j] == 1) {
                    ones++;
                }
            }
        }
        if (ones % 2 != 0) {
            syndrome += pos;
        }
    }

    if (syndrome > 0 && syndrome <= n) {
        // Correct the single bit error
        received[syndrome] ^= 1;
    }

    // Extract data bits
    int d_idx = 0;
    for (int i = 1; i <= n; i++) {
        if (!is_power_of_two(i)) {
            recovered_data[d_idx++] = received[i] + '0';
        }
    }
    recovered_data[d_idx] = '\0';

    return syndrome;
}

int main(int argc, char *argv[]) {
    char data[MAX_BITS] = "1011"; // Default 4-bit data for (7,4) Hamming code
    int error_pos = 3;            // Default error bit position to inject

    if (argc >= 2) {
        strncpy(data, argv[1], MAX_BITS - 1);
        data[MAX_BITS - 1] = '\0';
    }
    if (argc >= 3) {
        error_pos = atoi(argv[2]);
    }

    int k = strlen(data);
    for (int i = 0; i < k; i++) {
        if (data[i] != '0' && data[i] != '1') {
            fprintf(stderr, "Error: Data must be binary ('0' or '1')\n");
            return 1;
        }
    }

    int codeword[MAX_BITS];
    int n, r;
    encode_hamming(data, k, codeword, &n, &r);

    printf("\n--- HAMMING CODE (%d, %d) ENCODING ---\n", n, k);
    printf("Original Data (k=%d bits)      : %s\n", k, data);
    printf("Calculated Parity Bits (r)     : %d redundant bits (2^r >= k + r + 1)\n", r);
    printf("Encoded Codeword (n=%d bits)   : ", n);
    for (int i = 1; i <= n; i++) {
        printf("%d", codeword[i]);
    }
    printf("\nBit Positions Breakdown:\n");
    for (int i = 1; i <= n; i++) {
        if (is_power_of_two(i)) {
            printf("  Bit %2d: %d [Parity P%d]\n", i, codeword[i], i);
        } else {
            printf("  Bit %2d: %d [Data Bit]\n", i, codeword[i]);
        }
    }

    printf("\n--- CHANNEL TRANSMISSION & ERROR CORRECTION ---\n");

    // Case 1: Error-free transmission
    int rx_clean[MAX_BITS];
    memcpy(rx_clean, codeword, sizeof(codeword));
    char extracted_clean[MAX_BITS];
    int syn_clean = decode_and_correct(rx_clean, n, r, extracted_clean);
    printf("[Case 1: No Error Injected]\n");
    printf("  Syndrome = %d -> Result: %s\n", syn_clean,
           syn_clean == 0 ? "NO ERROR DETECTED" : "CORRUPT");
    printf("  Extracted Data: %s\n", extracted_clean);

    // Case 2: Error injection
    if (error_pos >= 1 && error_pos <= n) {
        int rx_corrupt[MAX_BITS];
        memcpy(rx_corrupt, codeword, sizeof(codeword));
        rx_corrupt[error_pos] ^= 1; // Inject single-bit error

        printf("\n[Case 2: Error Injected at Bit Position %d]\n", error_pos);
        printf("  Corrupted Codeword Received  : ");
        for (int i = 1; i <= n; i++) printf("%d", rx_corrupt[i]);
        printf("\n");

        char extracted_corrected[MAX_BITS];
        int syn = decode_and_correct(rx_corrupt, n, r, extracted_corrected);

        printf("  Computed Syndrome Value      : %d (Binary position: %d)\n", syn, syn);
        printf("  Action Taken                 : Flipped bit %d back to %d\n", syn, rx_corrupt[syn]);
        printf("  Corrected Codeword           : ");
        for (int i = 1; i <= n; i++) printf("%d", rx_corrupt[i]);
        printf("\n");
        printf("  Recovered Original Data      : %s\n", extracted_corrected);

        if (strcmp(data, extracted_corrected) == 0) {
            printf("  Verification Status          : SUCCESS! Bit perfectly corrected.\n");
        } else {
            printf("  Verification Status          : FAILED!\n");
            return 1;
        }
    }

    return 0;
}
