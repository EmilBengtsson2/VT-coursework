#include "stdio.h"
#include "mpi.h"

int BUFSIZE = 100;

int main() {
    MPI_Init(NULL, NULL);
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char msg[BUFSIZE];
    if (rank == 0) {
        printf("Master: Hello slaves give me your messages\n");
        MPI_Status status;
        int i;
        for (i = 1; i < size; i++) {
            MPI_Recv(&msg, BUFSIZE, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
            printf("Message received from process %d: %s\n", i, msg);
        }
    } else {
        sprintf(msg, "Hello back from process %d", rank);
        MPI_Send(msg, BUFSIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
