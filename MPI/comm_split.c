#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char** argv)
{
    int size, rank, tmp;
    MPI_Status s;
    MPI_Comm comm_even_odd;
    int value;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Split comm world into even odd. ie 0,2,4... on one and 1,3,5... on another
    MPI_Comm_split(MPI_COMM_WORLD, rank % 2, rank, &comm_even_odd);

    // Set value based on whether the rank is even or odd
    if (rank % 2 == 0) {
        value = 16;
    } else {
        value = 17;
    }

    // Send the value to the next process in the even/odd communicator 0 -> 2, 2 -> 0,
    // and receive from the previous process in the even/odd communicator
    int comm_rank, comm_size;
    MPI_Comm_rank(comm_even_odd, &comm_rank); // get rank of proc in containing communicator 0,1, 0,1
    MPI_Comm_size(comm_even_odd, &comm_size); // get size of communicator 2

    // Send to next rank in the split communicator 0 -> 1, 1 -> 0
    // Receive from previous rank in split communicator 0 <- 1, 1 <- 0
    MPI_Sendrecv(&value, 1, MPI_INT, (comm_rank + 1) % comm_size, 0, comm_even_odd,
                &tmp, 1, MPI_INT, (comm_rank + comm_size - 1) % comm_size, 0,
                comm_even_odd, MPI_STATUS_IGNORE);

    printf("MPI Process %d of %d (received value=%d)\n", rank, size, tmp); // again random order

    MPI_Comm_free(&comm_even_odd);  // Free the split communicator
    MPI_Finalize();

    return 0;
}
