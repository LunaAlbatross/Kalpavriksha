#include "matrix.h"

void rotateMatrix(int matrix[MAX_SIZE][MAX_SIZE], int n)
{
    //Transpose the matrix
    for (int row = 0; row < n; row++) {
        for (int col = row + 1; col < n; col++) {
            int *element1 = *(matrix + row) + col;
            int *element2 = *(matrix + col) + row;

            int temp = *element1;
            *element1 = *element2;
            *element2 = temp;
        }
    }
    //Reverse each row
    for (int row = 0; row < n; row++) {
        int left = 0;
        int right = n - 1;
        while (left < right) {
            int *element1 = *(matrix + row) + left;
            int *element2 = *(matrix + row) + right;

            int temp = *element1;
            *element1 = *element2;
            *element2 = temp;

            left++;
            right--;
        }
    }
}
