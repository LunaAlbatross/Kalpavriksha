#include <stdlib.h>
#include "matrix.h"
void generateMatrix(int matrix[MAX_SIZE][MAX_SIZE], int n)
{
    for (int row = 0;row<n;row++) {
        for (int col = 0; col<n;col++) {
            *(*(matrix+row) + col) = rand() % 256;
        }
    }
}
