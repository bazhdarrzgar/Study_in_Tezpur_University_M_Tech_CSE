/**
 * Experiment 1(b): Byte Stuffing (Character Stuffing) and Unstuffing
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * In byte-oriented framing, frames start and end with a designated FLAG byte.
 * If the FLAG byte or an ESCAPE (ESC) byte appears naturally inside user data:
 * - Sender inserts an ESC byte immediately before that character.
 * - Receiver detects an ESC byte, removes it, and treats the next byte as raw data.
 *
 * Classroom Convention:
 * FLAG = 'F', ESCAPE = 'E'
 * Example:
 * Data:       A B F C E D
 * Stuffed:    A B E F C E E D
 * Framed:   F A B E F C E E D F
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_FLAG 'F'
#define DEFAULT_ESC  'E'
#define MAX_BUF 1024

void byte_stuff(const char *data, char *stuffed, char flag, char esc) {
    int j = 0;
    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] == flag || data[i] == esc) {
            stuffed[j++] = esc; // Insert escape byte before flag or esc
        }
        stuffed[j++] = data[i];
    }
    stuffed[j] = '\0';
}

void byte_unstuff(const char *stuffed, char *unstuffed, char flag, char esc) {
    (void)flag; // flag is stripped before calling unstuff
    int j = 0;
    for (int i = 0; stuffed[i] != '\0'; i++) {
        if (stuffed[i] == esc) {
            i++; // Skip escape character, take next literal character
        }
        unstuffed[j++] = stuffed[i];
    }
    unstuffed[j] = '\0';
}

int main(int argc, char *argv[]) {
    char data[MAX_BUF];
    char stuffed[MAX_BUF * 2];
    char unstuffed[MAX_BUF];
    char framed[MAX_BUF * 2 + 3];
    char flag = DEFAULT_FLAG;
    char esc = DEFAULT_ESC;

    if (argc > 1) {
        strncpy(data, argv[1], MAX_BUF - 1);
        data[MAX_BUF - 1] = '\0';
    } else {
        printf("Enter string data payload (e.g. ABFCED): ");
        if (scanf("%1023s", data) != 1) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
    }

    printf("\n--- BYTE STUFFING DEMONSTRATION ---\n");
    printf("Framing Delimiters:       FLAG='%c', ESC='%c'\n", flag, esc);
    printf("Original Data:            %s\n", data);

    byte_stuff(data, stuffed, flag, esc);
    printf("Stuffed Data (Payload):   %s\n", stuffed);

    // Frame with opening and closing FLAG bytes
    snprintf(framed, sizeof(framed), "%c%s%c", flag, stuffed, flag);
    printf("Transmitted Frame:        %s\n", framed);

    // Receiver side: verify outer flags and strip them
    int framed_len = strlen(framed);
    if (framed[0] != flag || framed[framed_len - 1] != flag) {
        fprintf(stderr, "Error: Invalid framing flags detected!\n");
        return 1;
    }

    // Extract payload inside flags
    char received_payload[MAX_BUF * 2];
    strncpy(received_payload, framed + 1, framed_len - 2);
    received_payload[framed_len - 2] = '\0';

    byte_unstuff(received_payload, unstuffed, flag, esc);
    printf("Receiver Unstuffed Data:  %s\n", unstuffed);

    if (strcmp(data, unstuffed) == 0) {
        printf("\nResult: SUCCESS! Recovered payload exactly matches original input.\n");
    } else {
        printf("\nResult: FAILED! Recovered payload does not match.\n");
        return 1;
    }

    return 0;
}
