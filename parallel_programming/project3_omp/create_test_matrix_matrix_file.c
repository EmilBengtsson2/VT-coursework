#include "matrix_utils.h"

#include "stdio.h"
#include "stdlib.h"

int main() {
    int rows1;
    int cols1;
    int cols2;
    printf("Rows of matrix 1: \n");
    scanf("%d", &rows1);
    printf("Columns of matrix 1: \n");
    scanf("%d", &cols1);
    printf("Columns of matrix 2: \n");
    scanf("%d", &cols2);

    double* m1 = make_random_matrix(rows1, cols1);
    double* m2 = make_random_matrix(cols1, cols2);

    FILE* fp = fopen("matrix1.txt", "w");
    fprintf(fp, "%d %d\n", rows1, cols1);
    int i, j;
    for (i = 0; i < rows1; i++) {
        for (j = 0; j < cols1; j++) {
            fprintf(fp, "%lf ", m1[j + i*cols1]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n%d %d\n", cols1, cols2);
    for (i = 0; i < cols1; i++) {
        for (j = 0; j < cols2; j++) {
            fprintf(fp, "%lf ", m2[j + i*cols2]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    free(m1);
    free(m2);

    return 0;
}