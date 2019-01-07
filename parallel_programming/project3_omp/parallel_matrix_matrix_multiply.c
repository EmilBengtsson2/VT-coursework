#include "matrix_utils.h"

#include "stdlib.h"
#include "stdio.h"
#include "sys/time.h"
#include "omp.h"

void ijk();
void jik();
void ikj();
void jki();
void kij();
void kji();
void nested();
void matrix_matrix_multiply(void (*algorithm)(), char *filename);

int m1_rows, m1_cols;
double *m1;

int m2_rows, m2_cols;
double *m2;

int thread_count;

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Error: too few arguments\n");
        return -1;
    }
    thread_count = strtol(argv[1], NULL, 10);

    //read matrices
    read_two_matrices_from_file("matrix1.txt", &m1, &m1_rows, &m1_cols, &m2, &m2_rows, &m2_cols);
    if (m1 == NULL || m2 == NULL) {
        printf("Something went wrong when reading the matrices from file.\n");
        return -1;
    }

    ///////    run algorithms    ///////
    printf("---ijk---\n");
    matrix_matrix_multiply(ijk, "ijk_matrix_matrix_result.txt");
    printf("---------\n\n");
    printf("---jik---\n");
    matrix_matrix_multiply(jik, "jik_matrix_matrix_result.txt");
    printf("---------\n\n");
    printf("---ikj---\n");
    matrix_matrix_multiply(ikj, "ikj_matrix_matrix_result.txt");
    printf("---------\n\n");
    printf("---jki---\n");
    matrix_matrix_multiply(jki, "jki_matrix_matrix_result.txt");
    printf("---------\n\n");
    printf("---kij---\n");
    matrix_matrix_multiply(kij, "kij_matrix_matrix_result.txt");
    printf("---------\n\n");
    printf("---kji---\n");
    matrix_matrix_multiply(kji, "kji_matrix_matrix_result.txt");
    printf("---------\n\n");
    printf("---nested---\n");
    matrix_matrix_multiply(nested, "nested_matrix_matrix_result.txt");
    printf("------------\n\n");

    //free m1 and m2 matrix memory
    free(m1);
    free(m2);
    return 0;
}

double *result;

void reset_result_matrix() {
    int i, j;
    for (i = 0; i < m1_rows; i++) {
        for (j = 0; j < m2_cols; j++) {
            result[i * m2_cols + j] = 0;
        }
    }
}

void matrix_matrix_multiply(void (*algorithm)(), char *filename) {
    result = malloc(m1_rows * m2_cols * sizeof(double));
    reset_result_matrix();

    struct timeval tv_start, tv_end;
    gettimeofday(&tv_start, NULL);
    algorithm();
    gettimeofday(&tv_end, NULL);
    long comp_time = (tv_end.tv_sec-tv_start.tv_sec)*1000000L + tv_end.tv_usec - tv_start.tv_usec;

    printf("Multiplication done\nIt took %ld microseconds\n", comp_time);
    write_matrix_to_file(filename, result, m1_rows, m2_cols);
    free(result);
}

void ijk() {
    int i, j, k;
#   pragma omp parallel for num_threads(thread_count) \
    default(none) shared(m1_rows, m1_cols, m2_cols, m1, m2, result) private(i, j, k)
    for (i = 0; i < m1_rows; i++) {
        for (j = 0; j < m2_cols; j++) {
            for (k = 0; k < m1_cols; k++) {
                result[i * m2_cols + j] += m1[i * m1_cols + k] * m2[j + k * m2_cols];
            }
        }
    }
}

void jik() {
    int j, i, k;
#   pragma omp parallel for num_threads(thread_count) \
    default(none) shared(m1_rows, m1_cols, m2_cols, m1, m2, result) private(j, i, k)
    for (j = 0; j < m2_cols; j++) {
        for (i = 0; i < m1_rows; i++) {
            for (k = 0; k < m1_cols; k++) {
                result[i * m2_cols + j] += m1[i * m1_cols + k] * m2[j + k * m2_cols];
            }
        }
    }
}

void ikj() {
    int i, k, j;
#   pragma omp parallel for num_threads(thread_count) \
    default(none) shared(m1_rows, m1_cols, m2_cols, m1, m2, result) private(i, k, j)
    for (i = 0; i < m1_rows; i++) {
        for (k = 0; k < m1_cols; k++) {
            for (j = 0; j < m2_cols; j++) {
                result[i * m2_cols + j] += m1[i * m1_cols + k] * m2[j + k * m2_cols];
            }
        }
    }
}

void jki() {
    int i, k, j;
#   pragma omp parallel for num_threads(thread_count) \
    default(none) shared(m1_rows, m1_cols, m2_cols, m1, m2, result) private(i, k, j)
    for (j = 0; j < m2_cols; j++) {
        for (k = 0; k < m1_cols; k++) {
            for (i = 0; i < m1_rows; i++) {
                result[i * m2_cols + j] += m1[i * m1_cols + k] * m2[j + k * m2_cols];
            }
        }
    }
}

void kij() {
    int i, k, j;
    double *local_result;
#   pragma omp parallel num_threads(thread_count) \
    default(none) shared(m1_rows, m1_cols, m2_cols, m1, m2, result, thread_count) private(i, k, j, local_result)
    {
        local_result = malloc(m1_rows * m2_cols * sizeof(double));
        //set local matrix to all 0's
        for (i = 0; i < m1_rows; i++) {
            for (j = 0; j < m2_cols; j++) {
                local_result[i * m2_cols + j] = 0;
            }
        }
#       pragma omp for
        for (k = 0; k < m1_cols; k++) {
            for (i = 0; i < m1_rows; i++) {
                for (j = 0; j < m2_cols; j++) {
                    local_result[i * m2_cols + j] += m1[i * m1_cols + k] * m2[j + k * m2_cols];
                }
            }
        }
#       pragma omp critical
        {
            for (i = 0; i < m1_rows; i++) {
                for (j = 0; j < m2_cols; j++) {
                    result[i * m2_cols + j] += local_result[i * m2_cols + j];
                }
            }
        }
    }
}

void kji() {
    int i, k, j;
    double *local_result;
#   pragma omp parallel num_threads(thread_count) \
    default(none) shared(m1_rows, m1_cols, m2_cols, m1, m2, result) private(i, k, j, local_result)
    {
        local_result = malloc(m1_rows * m2_cols * sizeof(double));
        //set local matrix to all 0's
        for (i = 0; i < m1_rows; i++) {
            for (j = 0; j < m2_cols; j++) {
                local_result[i * m2_cols + j] = 0;
            }
        }
#       pragma omp for
        for (k = 0; k < m1_cols; k++) {
            for (j = 0; j < m2_cols; j++) {
                for (i = 0; i < m1_rows; i++) {
                    local_result[i * m2_cols + j] += m1[i * m1_cols + k] * m2[j + k * m2_cols];
                }
            }
        }
#       pragma omp critical
        {
            for (i = 0; i < m1_rows; i++) {
                for (j = 0; j < m2_cols; j++) {
                    result[i * m2_cols + j] += local_result[i * m2_cols + j];
                }
            }
        }
        free(local_result);
    }
}

void nested() {
    int i, j, k;
#   pragma omp parallel num_threads(thread_count) \
    default(none) shared(m1_rows, m1_cols, m2_cols, m1, m2, result, thread_count) private(i, j, k)
    {
        # pragma omp for collapse(2)
        for (i = 0; i < m1_rows; i++) {
            for (j = 0; j < m2_cols; j++) {
                for (k = 0; k < m1_cols; k++) {
                    result[i * m2_cols + j] += m1[i * m1_cols + k] * m2[j + k * m2_cols];
                }
            }
        }
    }
}
