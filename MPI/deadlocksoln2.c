
    #include <stdio.h>
    #include <stdlib.h>
    #include <mpi.h>

    int main (int argc, char** argv)
    {
        int size, rank, tmp;
        MPI_Status s;
        MPI_Init(&argc, &argv);

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        //NOT GUARANTEED TO DEADLOCK BUT MIGHT

        // send rank, 1 MPI_INT, to 0+1%4 =>  0->1..1->2..2->3..3->0 i.e circular comms hocche!
        // receive that stuff into tmp, from 0+4-1=3%4=3 i.e  3 -> 0, 0 -> 1, 1 -> 2, 2 -> 3
        MPI_Sendrecv(&rank, 1, MPI_INT, (rank+1)%size, 0,
                    &tmp, 1, MPI_INT, (rank+size-1)%size, 0,
                    MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        printf("MPI Process %d of %d (value=%d)\n", rank, size, tmp); // again random order

        MPI_Finalize();
    }
