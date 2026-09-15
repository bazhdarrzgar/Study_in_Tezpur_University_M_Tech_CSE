/**
 * Experiment 2(b): Error Detection - Internet Checksum (RFC 1071)
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * The Internet Checksum algorithm is used in IP, TCP, and UDP protocols:
 * 1. Sender:
 *    - Divides data into 16-bit words.
 *    - Computes the one's complement sum of all 16-bit words.
 *    - End-around carry is added back to the lowest 16 bits.
 *    - Checksum = Bitwise NOT (one's complement) of the sum.
 * 2. Receiver:
 *    - Computes the one's complement sum of all words INCLUDING the checksum.
 *    - If no transmission errors occurred, the final sum MUST be 0xFFFF (all 1s),
 *      or equivalently, the complement of the final sum must be 0x0000.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint16_t compute_checksum(const uint16_t *words, size_t count) {
    uint32_t sum = 0;

    for (size_t i = 0; i < count; i++) {
        sum += words[i];
    }

    // Fold 32-bit sum to 16 bits (add carries)
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return (uint16_t)(~sum & 0xFFFF);
}

int verify_checksum(const uint16_t *words, size_t count, uint16_t checksum) {
    uint32_t sum = 0;

    for (size_t i = 0; i < count; i++) {
        sum += words[i];
    }
    sum += checksum;

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return (sum == 0xFFFF);
}

int main(int argc, char *argv[]) {
    // Default sample data: 4 16-bit words
    uint16_t words[8] = {0x4500, 0x003c, 0x1c46, 0x4000};
    size_t count = 4;

    if (argc > 4) {
        count = (size_t)(argc - 1);
        if (count > 8) count = 8;
        for (size_t i = 0; i < count; i++) {
            words[i] = (uint16_t)strtol(argv[i + 1], NULL, 16);
        }
    }

    printf("\n--- 16-BIT INTERNET CHECKSUM DEMONSTRATION ---\n");
    printf("Sender 16-bit Words:\n");
    for (size_t i = 0; i < count; i++) {
        printf("  Word[%zu]: 0x%04X\n", i, words[i]);
    }

    uint16_t checksum = compute_checksum(words, count);
    printf("Computed Checksum: 0x%04X (~One's Complement Sum)\n", checksum);

    printf("\n--- RECEIVER VERIFICATION ---\n");
    int valid = verify_checksum(words, count, checksum);
    printf("Scenario 1 [No Error]:\n");
    printf("  Receiver Sum (Data + Checksum) check: %s (Status: %s)\n",
           valid ? "0xFFFF" : "INVALID", valid ? "VALID (Frame Accepted)" : "CORRUPT (Frame Rejected)");

    printf("\nScenario 2 [Simulated 1-bit Transmission Error]:\n");
    words[1] ^= 0x0001; // flip 1 bit in word 1
    int error_detected = !verify_checksum(words, count, checksum);
    printf("  Corrupted Word[1] to: 0x%04X\n", words[1]);
    printf("  Receiver Verification: %s\n",
           error_detected ? "ERROR DETECTED! Frame Discarded." : "Passed (Undetected error)");

    return 0;
}
