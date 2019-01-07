#include "matrix_utils.h"

#include "mpi.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "errno.h"

int main() {
    MPI_Init(NULL, NULL);
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int m1_rows, m1_cols;
    int* m1;

    int m2_rows, m2_cols;
    int* m2;

    int* result;

    int m1_sendcounts[comm_size], m1_send_displacements[comm_size];
    int m2_sendcounts[comm_size], m2_send_displacements[comm_size];
    int local_m1_cols_size, local_m2_rows_size;

    double local_start_time, local_finish_time, local_elapsed_time, global_elapsed_time;

    //if rank 0; read matrices
    if (rank == 0) {
        read_two_matrices_from_file("matrix1.txt", &m1, &m1_rows, &m1_cols, &m2, &m2_rows, &m2_cols);
        if (m1 == NULL || m2 == NULL) {
            MPI_Abort(MPI_COMM_WORLD, ENOENT);
        }
    }

    //Start of algorithm
    MPI_Barrier(MPI_COMM_WORLD);
    local_start_time = MPI_Wtime();

    //allocate memory for result matrix
    //initialize some variables
    if (rank == 0) {
        //create arrays with displacements and sendcounts for MPI_Scatterv
        //Sendcounts and displacements for m2
        float rows_cols_per_core = m2_rows/(float)comm_size;
        int i;
        for (i = 0; i < comm_size; i++) {
            m2_send_displacements[i] = ceil(i*rows_cols_per_core) * m2_cols;
            m2_sendcounts[i] = ceil((i+1)*rows_cols_per_core) * m2_cols - m2_send_displacements[i];
        }
        //Sendcounts and displacements for a transponated m1
        for (i = 0; i < comm_size; i++) {
            m1_send_displacements[i] = ceil(i*rows_cols_per_core) * m1_rows;
            m1_sendcounts[i] = ceil((i+1)*rows_cols_per_core) * m1_rows - m1_send_displacements[i];
        }

        result = malloc(m1_rows * m2_cols * sizeof(int));

        //Transpose m1 (to make it possible to scatter it correctly)
        transpose_matrix(m1, &m1_rows, &m1_cols);
    }

    //Distribute information among processing units
    MPI_Bcast(&m1_cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m2_cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(m1_sendcounts, 1, MPI_INT, &local_m1_cols_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(m2_sendcounts, 1, MPI_INT, &local_m2_rows_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //allocate memory for local rows and cols
    int* local_m1_cols = malloc(local_m1_cols_size * sizeof(int));
    int* local_m2_rows = malloc(local_m2_rows_size * sizeof(int));

    //receive local m1 cols and m2 rows
    MPI_Scatterv(m1, m1_sendcounts, m1_send_displacements, MPI_INT, local_m1_cols, local_m1_cols_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(m2, m2_sendcounts, m2_send_displacements, MPI_INT, local_m2_rows, local_m2_rows_size, MPI_INT, 0, MPI_COMM_WORLD);

    //Transpose local m1
    int nbr_local_m1_rows = local_m1_cols_size / m1_cols;
    int nbr_local_m1_cols = m1_cols;
    transpose_matrix(local_m1_cols, &nbr_local_m1_rows, &nbr_local_m1_cols);

    //multiply local m1 cols with local m2 rows
    int* local_result = matrix_multiply(local_m1_cols, nbr_local_m1_rows, nbr_local_m1_cols, local_m2_rows, local_m2_rows_size / m2_cols, m2_cols);

    //Send all local result matrixes to master
    if (rank != 0) {
        MPI_Send(local_result, nbr_local_m1_rows * m2_cols, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    //gather all the results
    if (rank == 0) {
        //m1_cols instead of m1_rows because m1 is still transposed.
        int result_size = m1_cols * m2_cols;
        int i, j;
        for (i = 0; i < result_size; i++) {
            result[i] = local_result[i];
        }
        for (i = 1; i < comm_size; i++) {
            MPI_Recv(local_result, result_size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (j = 0; j < result_size; j++) {
                result[j] += local_result[j];
            }
        }
    }

    //end of algorithm
    local_finish_time = MPI_Wtime();
    local_elapsed_time = local_finish_time - local_start_time;
    MPI_Reduce(&local_elapsed_time, &global_elapsed_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Multiplication done\nIt took %f seconds\n", global_elapsed_time);
        write_matrix_to_file("matrix_matrix_result.txt", result, m1_cols, m2_cols);
    }

    //free local m1 cols, local m2 rows, and local result memory
    free(local_m1_cols);
    free(local_m2_rows);
    free(local_result);

    //if rank 0; free m1, m2, and result matrix memory
    if (rank == 0) {
        free(m1);
        free(m2);
        free(result);
    }
    
    MPI_Finalize();
    return 0;
}