/*
Problem_10: Design a producer–consumer system where one or more MPI processes generate data and another process receives and processes the data using point-to-point communication.
Sample Input
Number of processes: 3
P0 = Producer
P1 = Consumer
P2 = Consumer
Number of items = 8
Items: 10 20 30 40 50 60 70 80
Sample Output
P0 produced: 10
P1 consumed: 10
P0 produced: 20
P2 consumed: 20
P0 produced: 30
P1 consumed: 30
P0 produced: 40
P2 consumed: 40
...
All 8 items processed successfully.
*/

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{

    int rank, size, item, i;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        int data[] = {10, 20, 30, 40, 50, 60, 70, 80};
        for (i = 0; i < 8; i++)
        {
            int dest = (i % 2) + 1;
            printf("P0 produced: %d\n", data[i]);
            fflush(stdout);

            MPI_Send(&data[i], 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            MPI_Recv(&item, 1, MPI_INT, dest, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        item = -1;
        MPI_Send(&item, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&item, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    }
    else
    {
        while (1)
        {
            MPI_Recv(&item, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (item == -1)
                break;
            printf("P%d consumed: %d\n", rank, item);
            fflush(stdout);
            MPI_Send(&item, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}