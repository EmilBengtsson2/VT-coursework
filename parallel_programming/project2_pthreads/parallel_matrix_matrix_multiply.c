#include "matrix_utils.h"

#include "pthread.h"
#include "stdlib.h"
#include "stdio.h"
#include "sys/time.h"

int _m1_rows, _m1_cols;
double* _m1;

int _m2_rows, _m2_cols;
double* _m2;

double* _result;

int _thread_count;

pthread_mutex_t _barrier_mutex;
pthread_cond_t _barrier_cond;
int _barrier_counter;
long *_comp_times;

void *local_matrix_matrix_multiply(void* rank);

int main(int argc, char **argv) {

    _barrier_counter = 0;

    if (argc < 2) {
        printf("Error: too few arguments\n");
        return -1;
    }

    pthread_t* thread_handles;

    //read matrices
    read_two_matrices_from_file("matrix1.txt", &_m1, &_m1_rows, &_m1_cols, &_m2, &_m2_rows, &_m2_cols);
    if (_m1 == NULL || _m2 == NULL) {
        printf("Something went wrong when reading the matrices from file.\n");
        return -1;
    }

    _result = malloc(_m1_rows * _m2_cols * sizeof(double));

    //create threads
    long rank;
    _thread_count = strtol(argv[1], NULL, 10);
    _comp_times = malloc(_thread_count * sizeof(long));
    thread_handles = malloc(_thread_count * sizeof(pthread_t));
    for (rank = 0; rank < _thread_count; rank++) {
        int ret = pthread_create(&thread_handles[rank], NULL, local_matrix_matrix_multiply, (void*) rank);
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

    // Get longest comp time
    int i;
    long longest_comp_time = 0;
    for (i = 0; i < _thread_count; i++) {
        if (_comp_times[i] > longest_comp_time)
            longest_comp_time = _comp_times[i];
    }

    printf("Multiplication done\nIt took %ld microseconds\n", longest_comp_time);
    write_matrix_to_file("matrix_matrix_result.txt", _result, _m1_cols, _m2_cols);

    //free _m1, _m2, and result matrix memory
    free(_m1);
    free(_m2);
    free(_result);
    free(_comp_times);
    return 0;
}

void *local_matrix_matrix_multiply(void* rank) {
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
    double m1_rows_per_thread = _m1_rows/(double)_thread_count;
    int first_row = (int)(local_rank * m1_rows_per_thread);
    int last_row = (int)((local_rank+1) * m1_rows_per_thread);
    int i, j, k;
    for (i = first_row; i < last_row; i++) {
        for (j = 0; j < _m2_cols; j++) {
            _result[i * _m2_cols + j] = 0;
            for (k = 0; k < _m1_cols; k++) {
                _result[i * _m2_cols + j] += _m1[i * _m1_cols + k] * _m2[j + k * _m2_cols];
            }
        }
    }
    gettimeofday(&tv_end, NULL);
    long local_comp_time = (tv_end.tv_sec-tv_start.tv_sec)*1000000L + tv_end.tv_usec - tv_start.tv_usec;
    _comp_times[local_rank] = local_comp_time;
}