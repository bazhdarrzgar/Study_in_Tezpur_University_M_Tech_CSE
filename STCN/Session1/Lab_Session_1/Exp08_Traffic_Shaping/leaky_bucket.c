/**
 * Experiment 8(a): Traffic Shaping - Leaky Bucket Algorithm
 * Course: Selected Topics in Computer Networks (CSMT 502)
 *
 * Theory:
 * The Leaky Bucket algorithm shapes bursty network traffic into a uniform,
 * constant-rate stream.
 * - The bucket has a finite capacity C.
 * - Packets arrive at irregular, bursty rates and fill the bucket.
 * - If arriving packets exceed the available bucket capacity, the excess packets
 *   are dropped (packet loss).
 * - Packets leak out of the bucket at a strictly CONSTANT output rate R per second.
 */

#include <stdio.h>
#include <stdlib.h>

#define BUCKET_CAPACITY 10 // Max capacity of bucket (packets)
#define LEAK_RATE       3  // Constant leak rate (packets per second)

struct TimeSlot {
    int time;
    int incoming;
};

int main(void) {
    // Trace of incoming packet arrivals over 6 seconds
    struct TimeSlot trace[] = {
        {1, 4},
        {2, 8},
        {3, 6},
        {4, 2},
        {5, 0},
        {6, 5}
    };
    int total_slots = sizeof(trace) / sizeof(trace[0]);

    int bucket_content = 0;

    printf("=================================================================================\n");
    printf("                  LEAKY BUCKET TRAFFIC SHAPING SIMULATION                        \n");
    printf("  Bucket Capacity (C) = %d packets | Constant Leak Rate (R) = %d pkts/sec        \n",
           BUCKET_CAPACITY, LEAK_RATE);
    printf("=================================================================================\n");
    printf(" Time(s) | Incoming | Buffer Before | Sent(Leak) | Dropped(Overflow) | Buffer After \n");
    printf("---------+----------+---------------+------------+-------------------+--------------\n");

    int time = 1;
    int trace_idx = 0;

    while (trace_idx < total_slots || bucket_content > 0) {
        int incoming = 0;
        if (trace_idx < total_slots && trace[trace_idx].time == time) {
            incoming = trace[trace_idx].incoming;
            trace_idx++;
        }

        int buffer_before = bucket_content;
        int dropped = 0;

        // Check if incoming packets overflow capacity
        if (bucket_content + incoming > BUCKET_CAPACITY) {
            dropped = (bucket_content + incoming) - BUCKET_CAPACITY;
            bucket_content = BUCKET_CAPACITY;
        } else {
            bucket_content += incoming;
        }

        // Leak packets at constant rate
        int sent = 0;
        if (bucket_content >= LEAK_RATE) {
            sent = LEAK_RATE;
            bucket_content -= LEAK_RATE;
        } else {
            sent = bucket_content;
            bucket_content = 0;
        }

        printf("   %2d    |    %2d    |       %2d      |     %2d     |        %2d         |      %2d     \n",
               time, incoming, buffer_before, sent, dropped, bucket_content);

        time++;
    }

    printf("=================================================================================\n");
    printf("  Simulation Finished: Traffic successfully shaped to <= %d pkts/sec.\n", LEAK_RATE);
    printf("=================================================================================\n");

    return 0;
}
