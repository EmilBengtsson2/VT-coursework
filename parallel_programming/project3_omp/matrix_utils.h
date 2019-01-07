
#ifndef MATRIX_UTILS_H_
#define MATRIX_UTILS_H_

double* make_random_matrix(int rows, int cols);

void print_matrix(double* matrix, int rows, int cols);

double* matrix_multiply(double* m1, int m1_rows, int m1_cols, double* m2, int m2_rows, int m2_cols);

void transpose_matrix(double* matrix, int* rows, int* cols);

double* read_matrix_from_file(char* filename, int* rows, int* cols);

void read_two_matrices_from_file(char* filename, double** m1, int* m1_rows, int* m1_cols, double** m2, int* m2_rows, int* m2_cols);

void write_matrix_to_file(char *filename, double *matrix, int rows, int cols);

#endif /* MATRIX_UTILS_H_ */