
#ifndef MATRIX_UTILS_H_
#define MATRIX_UTILS_H_

int* make_random_matrix(int rows, int cols);

void print_matrix(int* matrix, int rows, int cols);

int* matrix_multiply(int* m1, int m1_rows, int m1_cols, int* m2, int m2_rows, int m2_cols);

void transpose_matrix(int* matrix, int* rows, int* cols);

int* read_matrix_from_file(char* filename, int* rows, int* cols);

void read_two_matrices_from_file(char* filename, int** m1, int* m1_rows, int* m1_cols, int** m2, int* m2_rows, int* m2_cols);

void write_matrix_to_file(char *filename, int *matrix, int rows, int cols);

#endif /* MATRIX_UTILS_H_ */