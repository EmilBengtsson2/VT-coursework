
all: create_test_matrix_matrix_file parallel_matrix_matrix_multiply

parallel_matrix_matrix_multiply: matrix_utils.o parallel_matrix_matrix_multiply.c
	gcc -g -Wall -fopenmp -o parallel_matrix_matrix_multiply parallel_matrix_matrix_multiply.c matrix_utils.o

create_test_matrix_matrix_file: matrix_utils.o create_test_matrix_matrix_file.c
	gcc -g -Wall -o create_test_matrix_matrix_file create_test_matrix_matrix_file.c matrix_utils.o

matrix_utils.o: matrix_utils.c matrix_utils.h
	gcc -g -Wall -c -o matrix_utils.o matrix_utils.c

clean:
	rm -f parallel_matrix_matrix_multiply
	rm -f create_test_matrix_matrix_file
	rm -f matrix_utils.o