/**
 * Experiment 7(a): Flow Control - Stop-and-Wait ARQ Simulation
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * In Stop-and-Wait ARQ:
 * 1. The sender transmits one frame at a time and starts a retransmission timer.
 * 2. The sender cannot send any further data until an acknowledgment (ACK) is received.
 * 3. Scenarios handled:
 *    - Normal delivery: Frame arrives -> Receiver sends ACK -> Sender receives ACK -> Next frame.
 *    - Frame loss: Frame is lost in channel -> Sender timer expires -> Frame retransmitted.
 *    - Lost ACK: ACK is lost -> Sender timer expires -> Frame retransmitted -> Receiver
 *      identifies duplicate sequence number, discards payload, and re-sends ACK.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TOTAL_FRAMES 5

void simulate_stop_and_wait(void) {
    int frame_seq = 0;
    int ack_seq = 0;

    printf("=================================================================\n");
    printf("           STOP-AND-WAIT ARQ PROTOCOL SIMULATION                 \n");
    printf("=================================================================\n");

    for (int i = 1; i <= TOTAL_FRAMES; i++) {
        frame_seq = i % 2; // Alternating 1-bit sequence number (0 or 1)
        bool frame_delivered = false;
        int attempts = 0;

        printf("\n>>> [Packet %d] Beginning transmission of Frame (Seq=%d) <<<\n", i, frame_seq);

        while (!frame_delivered) {
            attempts++;
            printf("  [Sender] Transmitting Frame [Seq=%d] (Attempt %d)... Timer started.\n",
                   frame_seq, attempts);

            // Simulate frame loss on packet 2, attempt 1
            if (i == 2 && attempts == 1) {
                printf("  [Channel] **! ERROR: Frame [Seq=%d] was LOST in transit !**\n", frame_seq);
                printf("  [Sender]  TIMEOUT expired! No ACK received.\n");
                printf("  [Sender]  Retransmitting...\n");
                continue;
            }

            // Frame reaches receiver
            printf("  [Receiver] Received Frame [Seq=%d].\n", frame_seq);

            // Simulate ACK loss on packet 4, attempt 1
            if (i == 4 && attempts == 1) {
                ack_seq = (frame_seq + 1) % 2;
                printf("  [Receiver] Frame accepted. Generated ACK [Seq=%d].\n", ack_seq);
                printf("  [Channel]  **! ERROR: ACK [Seq=%d] was LOST in transit !**\n", ack_seq);
                printf("  [Sender]   TIMEOUT expired! No ACK received.\n");
                printf("  [Sender]   Retransmitting duplicate Frame [Seq=%d]...\n", frame_seq);
                continue;
            }

            // If duplicate received after lost ACK
            if (i == 4 && attempts > 1) {
                ack_seq = (frame_seq + 1) % 2;
                printf("  [Receiver] Detected duplicate Frame [Seq=%d]! Discarding duplicate payload.\n", frame_seq);
                printf("  [Receiver] Re-sending ACK [Seq=%d] to synchronize sender.\n", ack_seq);
                printf("  [Sender]   Received ACK [Seq=%d]. Frame successfully acknowledged!\n", ack_seq);
                frame_delivered = true;
                break;
            }

            // Normal success case
            ack_seq = (frame_seq + 1) % 2;
            printf("  [Receiver] Frame verified. Sending ACK [Seq=%d]...\n", ack_seq);
            printf("  [Sender]   Received ACK [Seq=%d]. Frame successfully delivered!\n", ack_seq);
            frame_delivered = true;
        }
    }

    printf("\n=================================================================\n");
    printf("  Simulation Complete: All %d frames successfully delivered!\n", TOTAL_FRAMES);
    printf("=================================================================\n");
}

int main(void) {
    simulate_stop_and_wait();
    return 0;
}
