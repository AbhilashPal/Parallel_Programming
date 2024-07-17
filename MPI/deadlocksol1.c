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

        if  ( rank == 0 ){
            // receive that stuff into tmp, from 0+4-1=3%4=3 i.e  3 -> 0, 0 -> 1, 1 -> 2, 2 -> 3
            MPI_Recv(&tmp, 1, MPI_INT, (rank+size-1)%size, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            // send rank, 1 MPI_INT, to 0+1%4 = 1..2..3..0 i.e circular comms hocche!
            MPI_Send(&rank, 1, MPI_INT, (rank+1)%size, 0, MPI_COMM_WORLD);
        }
        else {
            // send rank, 1 MPI_INT, to 0+1%4 = 1..2..3..0 i.e circular comms hocche!
            MPI_Send(&rank, 1, MPI_INT, (rank+1)%size, 0, MPI_COMM_WORLD);
            // receive that stuff into tmp, from 0+4-1=3%4=3 i.e  3 -> 0, 0 -> 1, 1 -> 2, 2 -> 3
            MPI_Recv(&tmp, 1, MPI_INT, (rank+size-1)%size, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }


        printf("MPI Process %d of %d (value=%d)\n", rank, size, tmp); // CAN BE IN ANY ORDER

        MPI_Finalize();
        return 0;
    }
