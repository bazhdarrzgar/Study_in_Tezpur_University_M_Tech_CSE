/**
 * Experiment 9(a): Caesar Cipher (Shift Cipher)
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * The Caesar Cipher is one of the earliest monoalphabetic substitution ciphers.
 * Each letter in the plaintext is shifted by a fixed number of positions k:
 *   Encryption: C = (P + k) mod 26
 *   Decryption: P = (C - k + 26) mod 26
 *
 * Features:
 * - Handles both uppercase and lowercase letters.
 * - Non-alphabetic characters (spaces, punctuation) remain unchanged.
 * - Includes a Brute-Force Cryptanalysis tool testing all 25 possible keys.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXT 1024

void encrypt_caesar(const char *plaintext, char *ciphertext, int key) {
    key = ((key % 26) + 26) % 26; // Normalize key to [0, 25]

    for (int i = 0; plaintext[i] != '\0'; i++) {
        char ch = plaintext[i];
        if (isupper((unsigned char)ch)) {
            ciphertext[i] = ((ch - 'A' + key) % 26) + 'A';
        } else if (islower((unsigned char)ch)) {
            ciphertext[i] = ((ch - 'a' + key) % 26) + 'a';
        } else {
            ciphertext[i] = ch; // Preserve spaces/punctuation
        }
    }
    ciphertext[strlen(plaintext)] = '\0';
}

void decrypt_caesar(const char *ciphertext, char *plaintext, int key) {
    key = ((key % 26) + 26) % 26;

    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char ch = ciphertext[i];
        if (isupper((unsigned char)ch)) {
            plaintext[i] = ((ch - 'A' - key + 26) % 26) + 'A';
        } else if (islower((unsigned char)ch)) {
            plaintext[i] = ((ch - 'a' - key + 26) % 26) + 'a';
        } else {
            plaintext[i] = ch;
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

void brute_force_attack(const char *ciphertext) {
    printf("\n--- BRUTE-FORCE CRYPTANALYSIS (ALL 25 CANDIDATE KEYS) ---\n");
    char candidate[MAX_TEXT];
    for (int k = 1; k < 26; k++) {
        decrypt_caesar(ciphertext, candidate, k);
        printf("  Key %2d: %s\n", k, candidate);
    }
}

int main(int argc, char *argv[]) {
    char text[MAX_TEXT] = "Computer Networks and Cryptography";
    int shift = 3; // Classic Caesar shift

    if (argc >= 2) {
        strncpy(text, argv[1], MAX_TEXT - 1);
        text[MAX_TEXT - 1] = '\0';
    }
    if (argc >= 3) {
        shift = atoi(argv[2]);
    }

    char encrypted[MAX_TEXT];
    char decrypted[MAX_TEXT];

    printf("===============================================================\n");
    printf("                  CAESAR CIPHER DEMONSTRATION                  \n");
    printf("===============================================================\n");
    printf("Plaintext       : %s\n", text);
    printf("Shift Key (k)   : %d\n", shift);

    encrypt_caesar(text, encrypted, shift);
    printf("Encrypted Cipher: %s\n", encrypted);

    decrypt_caesar(encrypted, decrypted, shift);
    printf("Decrypted Plain : %s\n", decrypted);

    if (strcmp(text, decrypted) == 0) {
        printf("\nResult: SUCCESS! Decrypted text matches original plaintext.\n");
    } else {
        printf("\nResult: FAILED! Decrypted text mismatch.\n");
        return 1;
    }

    brute_force_attack(encrypted);

    return 0;
}
