#include "matrix_utils.h"

#include "mpi.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "errno.h"

int main() {
    MPI_Init(NULL, NULL);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int matrix_rows, matrix_cols;
    int* matrix;

    int vector_rows, vector_cols;
    int* vector;

    int* result;

    int sendcounts[size], send_displacements[size];
    int local_rows_size;

    double local_start_time, local_finish_time, local_elapsed_time, global_elapsed_time;

    //if rank 0; read matrices
    if (rank == 0) {
        //set matrix and vector sizes
        matrix = read_matrix_from_file("matrix.txt", &matrix_rows, &matrix_cols);
        vector = read_matrix_from_file("vector.txt", &vector_cols, &vector_rows);
        //the variable vector_cols is always equal to 1 and never used.
        if (matrix == NULL || vector == NULL) {
            MPI_Abort(MPI_COMM_WORLD, ENOENT);
        }
    }

    //Start of algorithm
    MPI_Barrier(MPI_COMM_WORLD);
    local_start_time = MPI_Wtime();

    //allocate memory for result vector
    //initialize some variables
    if (rank == 0) {
        //create arrays with displacements and sendcounts for MPI_Scatterv
        float rows_per_core = matrix_rows/(float)size;
        int i;
        for (i = 0; i < size; i++) {
            send_displacements[i] = ceil(i*rows_per_core) * matrix_cols;
            sendcounts[i] = ceil((i+1)*rows_per_core) * matrix_cols - send_displacements[i];
        }

        result = malloc(matrix_rows * sizeof(int));
    }

    //Distribute information among processing units
    MPI_Bcast(&matrix_cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&vector_rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(sendcounts, 1, MPI_INT, &local_rows_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) {
        vector = malloc(vector_rows * sizeof(int));
    }

    //allocate memory for local rows
    int* local_rows = malloc(local_rows_size * sizeof(int));

    //receive local matrix rows and vector
    MPI_Scatterv(matrix, sendcounts, send_displacements, MPI_INT, local_rows, local_rows_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector, vector_rows, MPI_INT, 0, MPI_COMM_WORLD);

    //multiply local matrix rows with vector
    int* local_result = matrix_multiply(local_rows, local_rows_size/matrix_cols, matrix_cols, vector, vector_rows, 1);

    //compute receivecounts and displacements for MPI_Gatherv
    int receivecounts[size];
    int receive_displacements[size];
    if (rank == 0) {
        int i;
        for (i = 0; i < size; i++) {
            receivecounts[i] = sendcounts[i]/matrix_cols;
            receive_displacements[i] = send_displacements[i]/matrix_cols;
        }
    }

    //gather all the results
    MPI_Gatherv(local_result, local_rows_size/matrix_cols, MPI_INT, result, receivecounts, receive_displacements, MPI_INT, 0, MPI_COMM_WORLD);

    //end of algorithm
    local_finish_time = MPI_Wtime();
    local_elapsed_time = local_finish_time - local_start_time;
    MPI_Reduce(&local_elapsed_time, &global_elapsed_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Multiplication done\nIt took %f seconds\n", global_elapsed_time);
        write_matrix_to_file("matrix_vector_result.txt", result, matrix_rows, 1);
    }

    //free local rows and vector memory
    free(local_rows);
    free(vector);
    free(local_result);

    //if rank 0; free test matrix and result vector memory
    if (rank == 0) {
        free(matrix);
        free(result);
    }
    
    MPI_Finalize();
    return 0;
}