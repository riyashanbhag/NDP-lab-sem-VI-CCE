#include <stdio.h>

int main() {
    int arrival_times[] = {1,2,3,5,6,8,11,12,15,16,19};
    int n = 11;

    int bucket_size = 10;      // X = 10 bytes
    int output_rate = 1;       // 1 byte per second
    int packet_size = 4;       // 4 bytes each packet

    int bucket = 0;
    int last_time = 0;
    int i;

    printf("Time\tBucket\tStatus\n");
    printf("-----------------------------\n");

    for(i = 0; i < n; i++) {

        int current_time = arrival_times[i];

        // Leak the bucket
        bucket -= (current_time - last_time) * output_rate;

        if(bucket < 0)
            bucket = 0;

        printf("t=%d\t", current_time);

        // Check if packet can be added
        if(bucket + packet_size <= bucket_size) {
            bucket += packet_size;
            printf("%d\tConforming\n", bucket);
        } else {
            printf("%d\tNon-Conforming (Dropped)\n", bucket);
        }

        last_time = current_time;
    }

    return 0;
}
