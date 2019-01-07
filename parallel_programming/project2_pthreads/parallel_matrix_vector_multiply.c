#include "matrix_utils.h"

#include "pthread.h"
#include "stdlib.h"
#include "stdio.h"
#include "sys/time.h"

int _matrix_rows, _matrix_cols;
double* _matrix;

int _vector_rows, _vector_cols;
double* _vector;

double* _result;

int _thread_count;

pthread_mutex_t _barrier_mutex;
pthread_cond_t _barrier_cond;
int _barrier_counter;
long *_comp_times;

void *local_matrix_vector_multiply(void* rank);

int main(int argc, char **argv) {

    _barrier_counter = 0;

    if (argc < 2) {
        printf("Error: too few arguments\n");
        return -1;
    }

    pthread_t* thread_handles;

    //read matrices
    _matrix = read_matrix_from_file("matrix.txt", &_matrix_rows, &_matrix_cols);
    _vector = read_matrix_from_file("vector.txt", &_vector_cols, &_vector_rows);
    //the variable vector_cols is always equal to 1
    if (_matrix == NULL || _vector == NULL) {
        printf("Something went wrong when reading the matrices from files.\n");
        return -1;
    }

    //allocate memory for result
    _result = malloc(_matrix_rows * sizeof(double));

    //create threads
    long rank;
    _thread_count = strtol(argv[1], NULL, 10);
    _comp_times = malloc(_thread_count * sizeof(long));
    thread_handles = malloc(_thread_count * sizeof(pthread_t));
    for (rank = 0; rank < _thread_count; rank++) {
        int ret = pthread_create(&thread_handles[rank], NULL, local_matrix_vector_multiply, (void*) rank);
        if (ret != 0) {
            printf("pthread_create returned nonzero value\n");
            return -1;
        }
    }

    //join threads
    for (rank = 0; rank < _thread_count; rank++) {
        int ret = pthread_join(thread_handles[rank], NULL);
        if (ret != 0) {
            printf("pthread_join returned nonzero value\n");
            return -1;
        }
    }

    // Get longest computation time
    int i;
    long longest_comp_time = 0;
    for (i = 0; i < _thread_count; i++) {
        if (_comp_times[i] > longest_comp_time)
            longest_comp_time = _comp_times[i];
    }

    printf("Multiplication done\nIt took %ld microseconds\n", longest_comp_time);
    write_matrix_to_file("matrix_vector_result.txt", _result, _matrix_rows, 1);

    //free memory
    free(_vector);
    free(_result);
    free(_matrix);
    free(_comp_times)
    return 0;
}

void *local_matrix_vector_multiply(void* rank) {
    pthread_mutex_lock(&_barrier_mutex);
    _barrier_counter++;
    if (_barrier_counter == _thread_count) {
        pthread_cond_broadcast(&_barrier_cond);
    } else {
        while (pthread_cond_wait(&_barrier_cond, &_barrier_mutex) != 0);
    }
    pthread_mutex_unlock(&_barrier_mutex);
    struct timeval tv_start, tv_end;
    gettimeofday(&tv_start, NULL);
    long local_rank = (long) rank;
    double rows_per_thread = _matrix_rows/(double)_thread_count;
    int first_row = (int)(local_rank * rows_per_thread);
    int last_row = (int)((local_rank+1) * rows_per_thread);
    int i, j;
    for (i = first_row; i < last_row; i++) {
        _result[i] = 0;
        for (j = 0; j < _matrix_cols; j++) {
            _result[i] += _matrix[j + i*_matrix_cols] * _vector[j];
        }
    }
    gettimeofday(&tv_end, NULL);
    long local_comp_time = (tv_end.tv_sec-tv_start.tv_sec)*1000000L + tv_end.tv_usec - tv_start.tv_usec;
    _comp_times[local_rank] = local_comp_time;
}