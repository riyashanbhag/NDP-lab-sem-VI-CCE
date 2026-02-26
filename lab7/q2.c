#include <stdio.h>

int main() {
    float r = 10.0;       // Replenishment rate (KBps)
    float b = 50.0;       // Bucket size (KB)
    float tokens = 50.0;  // Initially full
    float packet_size = 15.0;
    float time = 0.0;
    float interval = 0.5;

    int i;

    printf("Time\tTokens Before\tStatus\t\tTokens After\n");
    printf("------------------------------------------------------\n");

    for(i = 1; i <= 10; i++) {

        time += interval;

        // Add tokens
        tokens += r * interval;
        if(tokens > b)
            tokens = b;

        printf("%.1f\t%.2f\t\t", time, tokens);

        // Check if packet can be sent
        if(tokens >= packet_size) {
            tokens -= packet_size;
            printf("Sent\t\t%.2f\n", tokens);
        } else {
            printf("Queued\t\t%.2f\n", tokens);
        }
    }

    return 0;
}