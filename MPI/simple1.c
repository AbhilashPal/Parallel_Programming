#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char** argv)
{
    float value;
    int size, rank;
    MPI_Status s;
    MPI_Init(&argc, &argv);
    double then = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &size); // total worldsize is 4
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // our own rank in this world
    value=3.1415;
    printf("MPI Process %d of %d (value=%f)\n", rank, size, value); // output from every process
    if (rank>0) // for ranks 1,2,3 we receive value, 1 instance of MPI_DOUBLE from rank-1 i.e 0,1,2 with tag 0 pertaining to MPI_COMM_WORLD and return the status message s.
        MPI_Recv(&value, 1, MPI_FLOAT, rank-1, 0, MPI_COMM_WORLD, &s);
    if (rank<size-1) // for rank 0,1,2 send the same
        MPI_Send(&value, 1, MPI_FLOAT, rank+1, 0, MPI_COMM_WORLD);
    if (rank==size-1) // for rank 3 i.e 4-1, print the value propagated.
        printf("Value from MPI Process 0: %f\n",value);

    double now = MPI_Wtime();
    printf("Total Time elapsed in process %d : %f\n",rank,now-then); // arbitrary! Not that proc n time > proc n-1 time even though n depends on proc n-1 's send for data.

    MPI_Finalize();
}
