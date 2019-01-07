#include "matrix_utils.h"

#include "stdio.h"
#include "stdlib.h"

int main() {
    int rows;
    int cols;
    printf("Rows: \n");
    scanf("%d", &rows);
    printf("Columns: \n");
    scanf("%d", &cols);

    int* matrix = make_random_matrix(rows, cols);
    FILE* fp = fopen("matrix.txt", "w");
    fprintf(fp, "%d %d\n", rows, cols);
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            fprintf(fp, "%d ", matrix[j + i*cols]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    free(matrix);

    matrix = make_random_matrix(cols, 1);
    fp = fopen("vector.txt", "w");
    fprintf(fp, "%d %d\n", 1, cols);
    for (i = 0; i < cols; i++) {
        fprintf(fp, "%d ", matrix[i]);
    }
    fprintf(fp, "\n");
    fclose(fp);
    free(matrix);

    return 0;
}