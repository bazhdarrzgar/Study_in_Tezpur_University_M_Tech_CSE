/**
 * Experiment 1(a): Bit Stuffing and Unstuffing
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * In bit-oriented framing, frames start and end with a flag pattern: 01111110 (0x7E).
 * To prevent a pattern of six consecutive '1's in user data from being mistaken as a flag:
 * - Sender inserts a '0' bit after every five consecutive '1' bits.
 * - Receiver detects five consecutive '1' bits:
 *     - If the 6th bit is '0', it is discarded (unstuffed).
 *     - If the 6th bit is '1' followed by '0', it indicates the flag sequence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLAG "01111110"
#define MAX_BITS 1024

void bit_stuff(const char *data, char *stuffed) {
    int count = 0;
    int j = 0;

    for (int i = 0; data[i] != '\0'; i++) {
        stuffed[j++] = data[i];
        if (data[i] == '1') {
            count++;
            if (count == 5) {
                stuffed[j++] = '0'; // Stuff '0' after five 1s
                count = 0;
            }
        } else {
            count = 0;
        }
    }
    stuffed[j] = '\0';
}

void bit_unstuff(const char *stuffed, char *unstuffed) {
    int count = 0;
    int j = 0;

    for (int i = 0; stuffed[i] != '\0'; i++) {
        unstuffed[j++] = stuffed[i];
        if (stuffed[i] == '1') {
            count++;
            if (count == 5) {
                // If next bit is '0', it was stuffed -> skip it
                if (stuffed[i + 1] == '0') {
                    i++;
                }
                count = 0;
            }
        } else {
            count = 0;
        }
    }
    unstuffed[j] = '\0';
}

int main(int argc, char *argv[]) {
    char data[MAX_BITS];
    char stuffed[MAX_BITS * 2];
    char unstuffed[MAX_BITS];
    char framed[MAX_BITS * 2 + 32];

    if (argc > 1) {
        strncpy(data, argv[1], MAX_BITS - 1);
        data[MAX_BITS - 1] = '\0';
    } else {
        printf("Enter binary data bit stream (e.g. 011111101111110): ");
        if (scanf("%1023s", data) != 1) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
    }

    // Validate binary digits
    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] != '0' && data[i] != '1') {
            fprintf(stderr, "Error: Input must contain only binary digits ('0' and '1')\n");
            return 1;
        }
    }

    printf("\n--- BIT STUFFING DEMONSTRATION ---\n");
    printf("Original Data:            %s\n", data);

    bit_stuff(data, stuffed);
    printf("Stuffed Data (Payload):   %s\n", stuffed);

    // Frame with opening and closing flags
    snprintf(framed, sizeof(framed), "%s%s%s", FLAG, stuffed, FLAG);
    printf("Framed Transmission:      %s\n", framed);

    // Receiver side: Extract payload from between flags
    // In this simulation, we extract the stuffed payload directly
    bit_unstuff(stuffed, unstuffed);
    printf("Receiver Unstuffed Data:  %s\n", unstuffed);

    if (strcmp(data, unstuffed) == 0) {
        printf("\nResult: SUCCESS! Recovered bit stream matches original data.\n");
    } else {
        printf("\nResult: FAILED! Recovered bit stream does not match original data.\n");
        return 1;
    }

    return 0;
}
