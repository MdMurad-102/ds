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
    int rank, send, recv;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        send = 100;
    else
        send = 200;

    MPI_Sendrecv(&send, 1, MPI_INT, 1-rank, 0,
                 &recv, 1, MPI_INT, 1-rank, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("P%d sent %d and received %d\n", rank, send, recv);

    if (rank == 0)
        printf("Communication completed without deadlock.\n");

    MPI_Finalize();
    return 0;
}