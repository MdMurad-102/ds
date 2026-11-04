/*
Problem_7: Implement a distributed vector computation using MPI_Isend and MPI_Irecv. Compare its execution time with the equivalent blocking MPI_Send/MPI_Recv implementation.
Sample Input
Number of processes: 2
Array size = 8
P0: 10 20 30 40 50 60 70 80
P1: 1 2 3 4 5 6 7 8
Each process exchanges its array using MPI_Isend and MPI_Irecv.
Sample Output
P0 received: 1 2 3 4 5 6 7 8
P1 received: 10 20 30 40 50 60 70 80
Nonblocking communication completed.
Sample performance:
Blocking communication time    = 0.000021 s
Nonblocking communication time = 0.000015 s
*/



#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int A[8], R[8];
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < 8; i++)
    {
        if (rank == 0)
            A[i] = (i + 1) * 10;
        else
            A[i] = i + 1;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();
    MPI_Send(A, 8, MPI_INT, 1 - rank, 0, MPI_COMM_WORLD);
    MPI_Recv(R, 8, MPI_INT, 1 - rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    double blocking_time = MPI_Wtime() - start;

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    MPI_Request req[2];
    MPI_Isend(A, 8, MPI_INT, 1 - rank, 1, MPI_COMM_WORLD, &req[0]);
    MPI_Irecv(R, 8, MPI_INT, 1 - rank, 1, MPI_COMM_WORLD, &req[1]);
    MPI_Waitall(2, req, MPI_STATUSES_IGNORE);
    double nonBlocking_time = MPI_Wtime() - start;

    printf("P%d Received : ", rank);
    for (int i = 0; i < 8; i++)
        printf("%d ", R[i]);
    printf("\n");

    printf("P%d Blocking time    = %f s\n", rank, blocking_time);
    printf("P%d Nonblocking time = %f s\n", rank, nonBlocking_time);

    if (rank == 0)
        printf("Nonblocking communication completed.\n");

    MPI_Finalize();
    return 0;
}