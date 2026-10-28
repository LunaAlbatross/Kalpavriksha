#include "matrix.h"
void applyAverageFilter(int matrix[MAX_SIZE][MAX_SIZE], int n)
{
int previous[10];
int current[10];
int next[10];

//Initialize the row buffers
for (int col = 0; col<n; col++) {
    *(previous + col) = 0;
    *(current + col) = *(*(matrix + 0) + col);
    *(next + col) = *(*(matrix + 1) + col);
}
for (int row = 0; row<n; row++) {
    int temp[10];
    //Calculate averages using the original row buffers
    for (int col = 0; col<n; col++) {
        int sum = 0;
        int count = 0;

        for (int dr = -1;dr <= 1;dr++) {
            for (int dc = -1;dc <= 1;dc++) {
                int neighborRow = row + dr;
                int neighborCol = col + dc;

                if (neighborRow >= 0 && neighborRow < n &&
                    neighborCol >= 0 && neighborCol < n) {
                    if (dr == -1) {
                        sum += *(previous + neighborCol);
                    } else if (dr == 0) {
                        sum += *(current + neighborCol);
                    } else {
                        sum += *(next + neighborCol);
                    }
                    count++;
                }
            }
        }
        *(temp+col) = sum / count;
    }
    //Preserve the current original row and advance the buffers
    for (int col=0;col<n;col++) {
        *(previous+col) = *(current+col);
        *(current+col) = *(next+col);
    }
    //Load the next untouched row before writing filtered values
    if (row + 2 <n) {
        for (int col = 0;col<n;col++) {
            *(next + col) = *(*(matrix + row + 2) + col);
        }
    }
    //Write the filtered row back into the matrix
    for (int col = 0; col<n;col++) {
        *(*(matrix+row)+col) = *(temp + col);
    }
}
}