#include "matrix_utils.h"

#include "stdio.h"
#include "stdlib.h"
#include "time.h"

//Creates a rows x cols matrix with "random" positive entries between 0 and 1
//The matrix is put in an array with the first "cols" elements being the first row,
//the second "cols" elements being the second row, and so on.
int* make_random_matrix(int rows, int cols) {
    srand(rows+cols+time(0));
    int* matrix = malloc(rows * cols * sizeof(int));
    int i, j;
    for (i = 0; i < rows * cols; i++) {
        matrix[i] = rand() % 2;
    }
    return matrix;
}

void print_matrix(int* matrix, int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", matrix[i*cols + j]);
        }
        printf("\n");
    }
}

//multiplies m1 with m2 and returns the result as a new matrix, returns NULL if it
//is not possible to multiply the matrixes
int* matrix_multiply(int* m1, int m1_rows, int m1_cols, int* m2, int m2_rows, int m2_cols) {
    if (m1_cols != m2_rows) //not possible to multiply
        return NULL;
    int* matrix = malloc(m1_rows * m2_cols * sizeof(int*));
    int i, j;
    for (i = 0; i < m1_rows; i++) {
        for (j = 0; j < m2_cols; j++) {
            int sum = 0;
            int k;
            for (k = 0; k < m1_cols; k++) {
                sum += m1[i * m1_cols + k] * m2[k * m2_cols + j];
            }
            matrix[i * m2_cols + j] = sum;
        }
    }
    return matrix;
}

void transpose_matrix(int* matrix, int* rows, int* cols) {
    int* temp = malloc((*rows) * (*cols) * sizeof(int));
    int i, j;
    for (i = 0; i < *rows; i++) {
        for (j = 0; j < *cols; j++) {
            temp[j * (*rows) + i] = matrix[i * (*cols) + j];
        }
    }
    for (i = 0; i < (*cols) * (*rows); i++) {
        matrix[i] = temp[i];
    }
    free(temp);
    int temp_rows = *rows;
    *rows = *cols;
    *cols = temp_rows;
}

int* read_matrix_from_file(char* filename, int* rows, int* cols) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("ERROR, no file with name \"%s\" exists\n", filename);
        return NULL;
    }

    fscanf(fp, "%d %d", rows, cols);
    int* matrix = malloc((*rows) * (*cols) * sizeof(int));

    int i, j;
    for (i = 0; i < *rows; i++) {
        for (j = 0; j < *cols; j++) {
            fscanf(fp, "%d", &matrix[j + i*(*cols)]);
        }
    }
    fclose(fp);
    return matrix;
}

void read_two_matrices_from_file(char* filename, int** m1, int* m1_rows, int* m1_cols, int** m2, int* m2_rows, int* m2_cols) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("ERROR, no file with name \"%s\" exists\n", filename);
        return;
    }

    fscanf(fp, "%d %d", m1_rows, m1_cols);
    int *matrix = malloc((*m1_rows) * (*m1_cols) * sizeof(int));

    int i, j;
    for (i = 0; i < *m1_rows; i++) {
        for (j = 0; j < *m1_cols; j++) {
            fscanf(fp, "%d", &matrix[j + i*(*m1_cols)]);
        }
    }

    *m1 = matrix;

    fscanf(fp, "%d %d", m2_rows, m2_cols);
    matrix = malloc((*m2_rows) * (*m2_cols) * sizeof(int));

    for (i = 0; i < *m2_rows; i++) {
        for (j = 0; j < *m2_cols; j++) {
            fscanf(fp, "%d", &matrix[j + i*(*m2_cols)]);
        }
    }

    *m2 = matrix;
}

void write_matrix_to_file(char *filename, int *matrix, int rows, int cols) {
    FILE *fp = fopen(filename, "w");

    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            fprintf(fp, "%d ", matrix[j + i*cols]);
        }
        fprintf(fp, "\n");
    }
}