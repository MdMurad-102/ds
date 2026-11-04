/*
Problem_8: Create a two-process communication program that can cause deadlock when blocking sends are used. Modify the program to avoid deadlock using appropriate MPI communication techniques.
Sample Input
Number of processes: 2
P0 message = 100
P1 message = 200
Sample Output
Unsafe version:
P0: Waiting to send 100
P1: Waiting to send 200
Potential deadlock detected.
Corrected version:
P0 sent 100
P1 received 100
P1 sent 200
P0 received 200
Communication completed without deadlock.
*/

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{

    int rank, data;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        printf("PO message : ");
        scanf("%d", &data);
        printf("\nP0: Waiting to send %d \n", data);
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else
    {
        printf("P1 message : ");
        scanf("%d", &data);
        printf("\nP1: Waiting to send %d \n", data);

        MPI_Send(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Finalize();
    return 0;
}