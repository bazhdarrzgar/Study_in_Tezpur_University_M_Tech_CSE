/**
 * Experiment 7(c): Flow Control - Selective Repeat (SR) ARQ Simulation
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * In Selective Repeat ARQ:
 * - Sender and receiver both maintain sliding windows of size W (W <= 2^(m-1)).
 * - The receiver accepts and BUFFERS valid out-of-order frames that fall within
 *   its window instead of discarding them.
 * - Negative Acknowledgment (NAK) or selective ACK requests retransmission of
 *   ONLY the damaged or lost frame.
 * - Sender retransmits ONLY the specific unacknowledged frame.
 * - Once the missing frame arrives, all contiguous buffered frames are delivered
 *   in sequential order, and both windows slide forward.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WINDOW_SIZE 4
#define TOTAL_FRAMES 8

struct FrameState {
    int seq_num;
    bool sent;
    bool received;
    bool acked;
};

void simulate_selective_repeat(void) {
    struct FrameState frames[TOTAL_FRAMES];
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        frames[i].seq_num = i;
        frames[i].sent = false;
        frames[i].received = false;
        frames[i].acked = false;
    }

    int send_base = 0;
    int rcv_base = 0;

    printf("=================================================================\n");
    printf("           SELECTIVE REPEAT (SR) ARQ SIMULATION                  \n");
    printf("  Window Size (W) = %d | Total Frames = %d                       \n", WINDOW_SIZE, TOTAL_FRAMES);
    printf("=================================================================\n");

    // Phase 1: Send initial window [0, 1, 2, 3]
    printf("\n>>> Phase 1: Transmitting initial window frames [0 .. 3] <<<\n");
    for (int i = 0; i < WINDOW_SIZE && i < TOTAL_FRAMES; i++) {
        printf("  [Sender] Transmitting Frame %d...\n", i);
        frames[i].sent = true;
    }

    // Frame 0 arrives ok
    printf("\n  [Channel] Frame 0 received successfully.\n");
    printf("  [Receiver] Accepted Frame 0 -> Sent ACK 0.\n");
    frames[0].received = true;
    frames[0].acked = true;
    rcv_base++;
    send_base++;

    // Frame 1 is lost!
    printf("\n  [Channel] **! ERROR: Frame 1 was LOST in transit !**\n");

    // Frames 2 and 3 arrive
    for (int i = 2; i < WINDOW_SIZE; i++) {
        printf("  [Channel] Frame %d arrived at receiver.\n", i);
        printf("  [Receiver] Frame %d is out of order but within Rx window [%d .. %d].\n",
               i, rcv_base, rcv_base + WINDOW_SIZE - 1);
        printf("  [Receiver] ** Frame %d BUFFERED ** in memory -> Sent ACK %d.\n", i, i);
        frames[i].received = true;
        frames[i].acked = true;
    }

    // Sender times out ONLY on Frame 1
    printf("\n>>> Phase 2: Sender Timeout & Selective Retransmission <<<\n");
    printf("  [Sender] Frame 0 acknowledged. Frames 2 & 3 acknowledged.\n");
    printf("  [Sender] ** TIMEOUT for Frame 1 only! **\n");
    printf("  [Sender] Selectively retransmitting ONLY Frame 1 (Frames 2 & 3 NOT retransmitted).\n");

    // Retransmitted Frame 1 arrives
    printf("  [Channel] Retransmitted Frame 1 arrives at receiver.\n");
    printf("  [Receiver] Received missing Frame 1!\n");
    frames[1].received = true;
    frames[1].acked = true;

    printf("  [Receiver] Combining Frame 1 with buffered Frames 2 and 3.\n");
    printf("  [Receiver] Delivering in-order block [Frames 1, 2, 3] to Network Layer!\n");
    rcv_base = 4;
    send_base = 4;
    printf("  [Receiver] Rx Window slid forward to [%d .. %d].\n", rcv_base, rcv_base + WINDOW_SIZE - 1);
    printf("  [Sender]   Tx Window slid forward to [%d .. %d].\n", send_base, send_base + WINDOW_SIZE - 1);

    // Remaining frames [4 .. 7]
    printf("\n>>> Phase 3: Transmitting remaining frames [4 .. 7] <<<\n");
    for (int i = send_base; i < TOTAL_FRAMES; i++) {
        printf("  [Sender] Transmitting Frame %d...\n", i);
        printf("  [Receiver] Accepted Frame %d -> Sent ACK %d.\n", i, i);
        frames[i].sent = true;
        frames[i].received = true;
        frames[i].acked = true;
    }

    // Verify all frames acknowledged
    int delivered_count = 0;
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        if (frames[i].acked) delivered_count++;
    }

    printf("\n=================================================================\n");
    printf("  Simulation Complete: All %d/%d frames delivered successfully via SR!\n",
           delivered_count, TOTAL_FRAMES);
    printf("=================================================================\n");
}

int main(void) {
    simulate_selective_repeat();
    return 0;
}
