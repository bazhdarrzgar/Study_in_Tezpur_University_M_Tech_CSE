/**
 * Experiment 9(b): Substitution Cipher (Monoalphabetic Substitution)
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * In a monoalphabetic substitution cipher, each character of the plain alphabet
 * is uniquely mapped to a character in a 26-character permuted substitution alphabet.
 *
 * Example:
 * Plain Alphabet : A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
 * Cipher Alphabet: Q W E R T Y U I O P A S D F G H J K L Z X C V B N M
 *
 * Properties:
 * - Key space: 26! ≈ 4.03 x 10^26 possible keys (resistant to brute-force).
 * - Vulnerable to frequency analysis.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TEXT 1024

// Default substitution alphabet key (QWERTY permutation)
#define DEFAULT_KEY "QWERTYUIOPASDFGHJKLZXCVBNM"

bool validate_key(const char *key) {
    if (strlen(key) != 26) return false;
    bool seen[26] = {false};
    for (int i = 0; i < 26; i++) {
        if (!isalpha((unsigned char)key[i])) return false;
        int idx = toupper((unsigned char)key[i]) - 'A';
        if (seen[idx]) return false; // Duplicate letter in key
        seen[idx] = true;
    }
    return true;
}

void encrypt_substitution(const char *plaintext, char *ciphertext, const char *key) {
    for (int i = 0; plaintext[i] != '\0'; i++) {
        char ch = plaintext[i];
        if (isupper((unsigned char)ch)) {
            ciphertext[i] = toupper((unsigned char)key[ch - 'A']);
        } else if (islower((unsigned char)ch)) {
            ciphertext[i] = tolower((unsigned char)key[ch - 'a']);
        } else {
            ciphertext[i] = ch;
        }
    }
    ciphertext[strlen(plaintext)] = '\0';
}

void decrypt_substitution(const char *ciphertext, char *plaintext, const char *key) {
    // Build reverse decryption table
    char reverse_key[26];
    for (int i = 0; i < 26; i++) {
        int target_idx = toupper((unsigned char)key[i]) - 'A';
        reverse_key[target_idx] = 'A' + i;
    }

    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char ch = ciphertext[i];
        if (isupper((unsigned char)ch)) {
            plaintext[i] = reverse_key[ch - 'A'];
        } else if (islower((unsigned char)ch)) {
            plaintext[i] = tolower((unsigned char)reverse_key[ch - 'a']);
        } else {
            plaintext[i] = ch;
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

int main(int argc, char *argv[]) {
    char text[MAX_TEXT] = "Selected Topics in Computer Networks";
    const char *key = DEFAULT_KEY;

    if (argc >= 2) {
        strncpy(text, argv[1], MAX_TEXT - 1);
        text[MAX_TEXT - 1] = '\0';
    }
    if (argc >= 3) {
        key = argv[2];
    }

    if (!validate_key(key)) {
        fprintf(stderr, "Error: Substitution key must be exactly 26 unique alphabetical letters!\n");
        return 1;
    }

    char encrypted[MAX_TEXT];
    char decrypted[MAX_TEXT];

    printf("===============================================================\n");
    printf("              SUBSTITUTION CIPHER DEMONSTRATION                \n");
    printf("===============================================================\n");
    printf("Plain Alphabet : ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
    printf("Cipher Key     : %s\n", key);
    printf("Original Text  : %s\n", text);

    encrypt_substitution(text, encrypted, key);
    printf("Encrypted Text : %s\n", encrypted);

    decrypt_substitution(encrypted, decrypted, key);
    printf("Decrypted Text : %s\n", decrypted);

    if (strcmp(text, decrypted) == 0) {
        printf("\nResult: SUCCESS! Decrypted text matches original plaintext.\n");
    } else {
        printf("\nResult: FAILED! Decryption error.\n");
        return 1;
    }

    return 0;
}
