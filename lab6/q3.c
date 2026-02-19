/*
------------------------------------------------------------
Implement the Hamming Code method to detect and correct
a single-bit error in transmitted data.

------------------------------------------------------------
HAMMING CODE - ERROR DETECTION AND CORRECTION
------------------------------------------------------------
*/

#include <stdio.h>
#include <math.h>

int main() {

    int data[20], hamming[30];
    int m, r = 0, i, j, k;

    printf("Enter number of data bits: ");
    scanf("%d", &m);

    printf("Enter data bits (one by one):\n");
    for(i = 1; i <= m; i++)
        scanf("%d", &data[i]);

    // Calculate number of parity bits
    while(pow(2, r) < (m + r + 1))
        r++;

    int total = m + r;

    // Initialize Hamming code array
    for(i = 1, j = 1, k = 1; i <= total; i++) {

        if(i == pow(2, j-1)) {
            hamming[i] = 0;   // parity bit placeholder
            j++;
        } else {
            hamming[i] = data[k];
            k++;
        }
    }

    // Calculate parity bits
    for(i = 0; i < r; i++) {

        int pos = pow(2, i);
        int parity = 0;

        for(j = 1; j <= total; j++) {
            if(j & pos)
                parity ^= hamming[j];
        }

        hamming[pos] = parity;
    }

    printf("\nHamming Code Generated:\n");
    for(i = 1; i <= total; i++)
        printf("%d ", hamming[i]);

    printf("\n\nEnter received bits:\n");

    int received[30];
    for(i = 1; i <= total; i++)
        scanf("%d", &received[i]);

    // Check for error
    int error_pos = 0;

    for(i = 0; i < r; i++) {

        int pos = pow(2, i);
        int parity = 0;

        for(j = 1; j <= total; j++) {
            if(j & pos)
                parity ^= received[j];
        }

        if(parity != 0)
            error_pos += pos;
    }

    if(error_pos == 0) {
        printf("\nNo Error in Transmission\n");
    }
    else {
        printf("\nError at position: %d\n", error_pos);

        // Correct the error
        received[error_pos] ^= 1;

        printf("Corrected Data:\n");
        for(i = 1; i <= total; i++)
            printf("%d ", received[i]);
    }

    return 0;
}
