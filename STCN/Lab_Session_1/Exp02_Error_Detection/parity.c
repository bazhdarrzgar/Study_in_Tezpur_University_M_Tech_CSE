/**
 * Experiment 2(a): Error Detection - Parity Bit (1D and 2D Parity)
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * 1. Simple (1D) Parity:
 *    - Even Parity: Adds a parity bit such that the total number of 1s is EVEN.
 *    - Odd Parity:  Adds a parity bit such that the total number of 1s is ODD.
 * 2. Two-Dimensional (2D) Parity (LRC / VRC):
 *    - Organizes data into an M x N matrix of bits.
 *    - Calculates a parity bit for each row (Horizontal / VRC).
 *    - Calculates a parity bit for each column (Vertical / LRC).
 *    - Can detect all 1-bit, 2-bit, and 3-bit errors, and pinpoint single-bit errors for correction.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BITS 128
#define MAX_ROWS 16
#define MAX_COLS 16

int count_ones(const char *bits) {
    int count = 0;
    for (int i = 0; bits[i] != '\0'; i++) {
        if (bits[i] == '1') count++;
    }
    return count;
}

void demo_1d_parity(const char *data) {
    int ones = count_ones(data);
    char even_parity = (ones % 2 == 0) ? '0' : '1';
    char odd_parity  = (ones % 2 == 0) ? '1' : '0';

    printf("\n--- 1D SIMPLE PARITY ---");
    printf("\nData Bitstream:          %s (Length: %zu, '1's: %d)", data, strlen(data), ones);
    printf("\nEven Parity Bit:         %c  -> Transmitted: %s%c", even_parity, data, even_parity);
    printf("\nOdd Parity Bit:          %c  -> Transmitted: %s%c", odd_parity, data, odd_parity);

    // Receiver check with even parity
    char received[MAX_BITS + 2];
    snprintf(received, sizeof(received), "%s%c", data, even_parity);
    int rx_ones = count_ones(received);
    printf("\n[Rx Verification - No Error]:   Total 1s = %d -> %s", rx_ones,
           (rx_ones % 2 == 0) ? "VALID (No error detected)" : "CORRUPT");

    // Corrupt one bit
    received[0] = (received[0] == '1') ? '0' : '1';
    int corrupt_ones = count_ones(received);
    printf("\n[Rx Verification - 1b Error]:   Total 1s = %d -> %s", corrupt_ones,
           (corrupt_ones % 2 == 0) ? "VALID" : "ERROR DETECTED!");
    printf("\n");
}

void demo_2d_parity(int rows, int cols, char matrix[MAX_ROWS][MAX_COLS]) {
    char row_parity[MAX_ROWS];
    char col_parity[MAX_COLS];

    printf("\n--- 2D PARITY MATRIX (EVEN PARITY) ---\n");
    printf("Original Data Matrix (%d rows x %d cols):\n", rows, cols);

    // Compute row parity
    for (int r = 0; r < rows; r++) {
        int r_ones = 0;
        for (int c = 0; c < cols; c++) {
            if (matrix[r][c] == '1') r_ones++;
        }
        row_parity[r] = (r_ones % 2 == 0) ? '0' : '1';
    }

    // Compute column parity
    for (int c = 0; c < cols; c++) {
        int c_ones = 0;
        for (int r = 0; r < rows; r++) {
            if (matrix[r][c] == '1') c_ones++;
        }
        col_parity[c] = (c_ones % 2 == 0) ? '0' : '1';
    }

    // Display formatted matrix with row parities
    printf("             ");
    for (int c = 0; c < cols; c++) printf("C%d ", c);
    printf(" | Row Parity\n");
    printf("            ");
    for (int c = 0; c < cols; c++) printf("---");
    printf("-+-----------\n");

    for (int r = 0; r < rows; r++) {
        printf("Row %d:       ", r);
        for (int c = 0; c < cols; c++) {
            printf(" %c ", matrix[r][c]);
        }
        printf(" |     %c\n", row_parity[r]);
    }

    printf("            ");
    for (int c = 0; c < cols; c++) printf("---");
    printf("-+-----------\n");
    printf("Col Parity:  ");
    for (int c = 0; c < cols; c++) {
        printf(" %c ", col_parity[c]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    char data[MAX_BITS] = "1011001";

    if (argc > 1) {
        strncpy(data, argv[1], MAX_BITS - 1);
        data[MAX_BITS - 1] = '\0';
    }

    demo_1d_parity(data);

    // Sample 4x4 matrix for 2D parity
    char sample_matrix[MAX_ROWS][MAX_COLS] = {
        {'1', '0', '1', '1'},
        {'0', '1', '1', '0'},
        {'1', '1', '0', '1'},
        {'0', '0', '1', '0'}
    };
    demo_2d_parity(4, 4, sample_matrix);

    return 0;
}
