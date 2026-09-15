/**
 * Experiment 7(b): Flow Control - Go-Back-N (GBN) ARQ Simulation
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * In Go-Back-N ARQ:
 * - Sender maintains a sliding window of size N (Window Size W).
 * - Sender can transmit up to W frames without waiting for an ACK.
 * - Receiver accepts frames ONLY strictly in sequence (Receiver window size = 1).
 * - If a frame is lost or corrupted, all subsequent out-of-order frames reaching
 *   the receiver are DISCARDED.
 * - When the sender's timer for the unacknowledged frame expires, the sender
 *   retransmits ALL frames currently in the window starting from the lost frame
 *   ("Goes back N").
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WINDOW_SIZE 4
#define TOTAL_FRAMES 8

void simulate_go_back_n(void) {
    int next_frame_to_send = 0;
    int ack_expected = 0;
    bool error_simulated = false;

    printf("=================================================================\n");
    printf("              GO-BACK-N (GBN) ARQ SIMULATION                     \n");
    printf("  Window Size (W) = %d | Total Frames to Transmit = %d           \n", WINDOW_SIZE, TOTAL_FRAMES);
    printf("=================================================================\n");

    while (ack_expected < TOTAL_FRAMES) {
        printf("\n--- CURRENT WINDOW STATE: [%d to %d] ---\n",
               ack_expected,
               (ack_expected + WINDOW_SIZE - 1 < TOTAL_FRAMES) ? (ack_expected + WINDOW_SIZE - 1) : (TOTAL_FRAMES - 1));

        // Transmit frames within sender window
        while (next_frame_to_send < ack_expected + WINDOW_SIZE && next_frame_to_send < TOTAL_FRAMES) {
            printf("  [Sender] Transmitting Frame %d...\n", next_frame_to_send);
            next_frame_to_send++;
        }

        // Simulate transmission and receiver behavior
        // Inject loss on Frame 1 during the first pass
        if (!error_simulated && ack_expected == 0) {
            printf("\n  [Channel] Frame 0 arrived successfully.\n");
            printf("  [Receiver] Frame 0 accepted -> Sent ACK 1.\n");
            printf("  [Sender] Received ACK 1. Window sliding forward.\n");
            ack_expected = 1;

            printf("\n  [Channel] **! ERROR: Frame 1 was LOST in transit !**\n");

            // Subsequent frames in the window reach the receiver
            for (int f = 2; f < next_frame_to_send; f++) {
                printf("  [Channel] Frame %d arrived at receiver.\n", f);
                printf("  [Receiver] Out of order! Expected Frame %d, received Frame %d.\n", ack_expected, f);
                printf("  [Receiver] ** Frame %d DISCARDED! ** Resending cumulative ACK %d.\n", f, ack_expected);
            }

            // Sender timeout
            printf("\n  [Sender] ** TIMEOUT for Frame 1! **\n");
            printf("  [Sender] Go-Back-N triggered! Rolling back transmission pointer to Frame 1.\n");
            next_frame_to_send = 1; // Roll back
            error_simulated = true;
            continue;
        }

        // Normal error-free reception for remaining frames
        while (ack_expected < next_frame_to_send) {
            printf("  [Receiver] Received Frame %d in order -> Accepted! Sent ACK %d.\n",
                   ack_expected, ack_expected + 1);
            ack_expected++;
            printf("  [Sender]   Received ACK %d. Window slid to %d.\n", ack_expected, ack_expected);
        }
    }

    printf("\n=================================================================\n");
    printf("  Simulation Complete: All %d frames delivered successfully via GBN!\n", TOTAL_FRAMES);
    printf("=================================================================\n");
}

int main(void) {
    simulate_go_back_n();
    return 0;
}
