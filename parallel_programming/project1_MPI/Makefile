
all: sequential_matrix_vector_multiply sequential_matrix_matrix_multiply create_test_matrix_vector_pair create_test_matrix_matrix_file

parallel: parallel_matrix_vector_multiply parallel_matrix_matrix_multiply

parallel_matrix_vector_multiply: matrix_utils.o parallel_matrix_vector_multiply.c
	mpicc -lm -o parallel_matrix_vector_multiply parallel_matrix_vector_multiply.c matrix_utils.o

parallel_matrix_matrix_multiply: matrix_utils.o parallel_matrix_matrix_multiply.c
	mpicc -lm -o parallel_matrix_matrix_multiply parallel_matrix_matrix_multiply.c matrix_utils.o

create_test_matrix_vector_pair: matrix_utils.o create_test_matrix_vector_pair.c
	gcc -o create_test_matrix_vector_pair create_test_matrix_vector_pair.c matrix_utils.o

create_test_matrix_matrix_file: matrix_utils.o create_test_matrix_matrix_file.c
	gcc -o create_test_matrix_matrix_file create_test_matrix_matrix_file.c matrix_utils.o

matrix_utils.o: matrix_utils.c matrix_utils.h
	gcc -c -o matrix_utils.o matrix_utils.c

clean:
	rm -f parallel_matrix_vector_multiply
	rm -f parallel_matrix_matrix_multiply
	rm -f create_test_matrix_vector_pair
	rm -f create_test_matrix_matrix_file
	rm -f matrix_utils.o