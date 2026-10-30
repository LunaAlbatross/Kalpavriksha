#include <stdio.h>
#include "matrix.h"

void displayMatrix(int matrix[MAX_SIZE][MAX_SIZE], int n)
{
    for (int row = 0; row<n; row++) {
        for (int col = 0; col<n; col++) {
            printf("%4d ", *(*(matrix + row) + col));
        }
        printf("\n");
    }
}
