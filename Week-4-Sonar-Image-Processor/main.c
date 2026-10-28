
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"

int main(void)
{
    int matrix[MAX_SIZE][MAX_SIZE];
    int n;
    printf("Enter matrix size (2-10): ");

    if (scanf("%d", &n) != 1 || n<2 || n>MAX_SIZE) {
        printf("Invalid size. Please enter a number between 2 and 10.\n");
        return 1;
    }
    srand((unsigned int)time(NULL));
    generateMatrix(matrix, n);

    printf("Originally Generated Matrix:\n");
    displayMatrix(matrix, n);

    rotateMatrix(matrix, n);
    printf("Matrix after 90-degree clockwise rotation:\n");
    displayMatrix(matrix, n);

    applyAverageFilter(matrix, n);
    printf("Final Output After Smooth Filtering:\n");
    displayMatrix(matrix, n);

    return 0;
}
