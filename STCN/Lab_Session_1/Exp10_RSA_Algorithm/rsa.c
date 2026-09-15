/**
 * Experiment 10: RSA Public-Key Cryptosystem Implementation
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * RSA (Rivest–Shamir–Adleman) is an asymmetric public-key cryptosystem.
 *
 * 1. Key Generation:
 *    - Select two distinct primes: p and q.
 *    - Compute modulus: n = p * q.
 *    - Compute Euler's Totient: phi(n) = (p - 1) * (q - 1).
 *    - Choose public exponent e such that: 1 < e < phi(n) and gcd(e, phi(n)) == 1.
 *    - Compute private exponent d using Extended Euclidean Algorithm:
 *        d * e = 1 (mod phi(n))  =>  d = e^(-1) mod phi(n).
 *    - Public Key  = (e, n)
 *    - Private Key = (d, n)
 *
 * 2. Encryption:
 *    - Ciphertext c = (m^e) mod n
 *
 * 3. Decryption:
 *    - Plaintext m  = (c^d) mod n
 *
 * Implementation features:
 * - Uses 64-bit unsigned integers (uint64_t) with modular exponentiation to prevent overflow.
 * - Supports encryption/decryption of both integers and text strings.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

// Extended Euclidean Algorithm: computes gcd(a, b) and finds x, y such that a*x + b*y = gcd(a, b)
int64_t ext_gcd(int64_t a, int64_t b, int64_t *x, int64_t *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int64_t x1, y1;
    int64_t gcd = ext_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return gcd;
}

int64_t gcd(int64_t a, int64_t b) {
    while (b != 0) {
        int64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Compute modular inverse: d = e^(-1) mod phi
int64_t mod_inverse(int64_t e, int64_t phi) {
    int64_t x, y;
    int64_t g = ext_gcd(e, phi, &x, &y);
    if (g != 1) return -1; // No inverse exists
    return (x % phi + phi) % phi;
}

// Modular Exponentiation: computes (base^exp) % mod safely
uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = ((__int128)result * base) % mod;
        }
        base = ((__int128)base * base) % mod;
        exp /= 2;
    }
    return result;
}

bool is_prime(int64_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    // Choose two prime numbers (p = 61, q = 53 are classic textbook primes)
    int64_t p = 61;
    int64_t q = 53;
    const char *test_msg = "NETWORKS";

    if (argc >= 3) {
        p = atoll(argv[1]);
        q = atoll(argv[2]);
    }
    if (argc >= 4) {
        test_msg = argv[3];
    }

    if (!is_prime(p) || !is_prime(q)) {
        fprintf(stderr, "Error: Both p (%" PRId64 ") and q (%" PRId64 ") must be prime numbers!\n", p, q);
        return 1;
    }

    // 1. Calculate n and phi(n)
    uint64_t n = (uint64_t)p * q;
    uint64_t phi = (uint64_t)(p - 1) * (q - 1);

    // 2. Choose public exponent e: gcd(e, phi) == 1
    uint64_t e = 17; // Common public exponent
    while (e < phi) {
        if (gcd(e, phi) == 1) break;
        e += 2;
    }

    // 3. Compute private exponent d
    int64_t d = mod_inverse(e, phi);
    if (d < 0) {
        fprintf(stderr, "Error: Modular inverse does not exist!\n");
        return 1;
    }

    printf("=================================================================\n");
    printf("                  RSA CRYPTOSYSTEM DEMONSTRATION                 \n");
    printf("=================================================================\n");
    printf("1. Key Generation:\n");
    printf("   Prime p                  = %ld\n", p);
    printf("   Prime q                  = %ld\n", q);
    printf("   Modulus n (p * q)        = %lu\n", n);
    printf("   Euler's Totient phi(n)   = %lu\n", phi);
    printf("   Public Exponent (e)      = %lu  [gcd(e, phi) = 1]\n", e);
    printf("   Private Exponent (d)     = %ld  [d * e = 1 (mod phi)]\n", d);
    printf("   -------------------------------------------------------------\n");
    printf("   PUBLIC KEY               = (e = %lu, n = %lu)\n", e, n);
    printf("   PRIVATE KEY              = (d = %ld, n = %lu)\n", d, n);
    printf("=================================================================\n");

    // Scenario A: Single Integer Number
    uint64_t int_msg = 42;
    uint64_t int_cipher = mod_exp(int_msg, e, n);
    uint64_t int_decrypted = mod_exp(int_cipher, (uint64_t)d, n);

    printf("\n2. Integer Encryption & Decryption (m = %lu):\n", int_msg);
    printf("   Encryption: c = (m^e) mod n = (%lu^%lu) mod %lu = %lu\n",
           int_msg, e, n, int_cipher);
    printf("   Decryption: m = (c^d) mod n = (%lu^%ld) mod %lu = %lu\n",
           int_cipher, d, n, int_decrypted);
    printf("   Validation Status: %s\n",
           (int_msg == int_decrypted) ? "SUCCESS (m == decrypted_m)" : "FAILED");

    // Scenario B: String Text Message
    printf("\n3. Text Message Encryption & Decryption (msg = \"%s\"):\n", test_msg);
    size_t len = strlen(test_msg);
    uint64_t cipher_tokens[len];
    char decrypted_msg[len + 1];

    printf("   Encrypted Tokens (Ciphertext): ");
    for (size_t i = 0; i < len; i++) {
        cipher_tokens[i] = mod_exp((uint8_t)test_msg[i], e, n);
        printf("%lu ", cipher_tokens[i]);
    }
    printf("\n");

    for (size_t i = 0; i < len; i++) {
        decrypted_msg[i] = (char)mod_exp(cipher_tokens[i], (uint64_t)d, n);
    }
    decrypted_msg[len] = '\0';

    printf("   Decrypted String Message     : \"%s\"\n", decrypted_msg);
    printf("   Validation Status            : %s\n",
           (strcmp(test_msg, decrypted_msg) == 0) ? "SUCCESS (Exact string match)" : "FAILED");
    printf("=================================================================\n");

    return (strcmp(test_msg, decrypted_msg) == 0) ? 0 : 1;
}
