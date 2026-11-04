/*
Problem_18: Divide a large dataset among MPI processes. Each process generates a local histogram, and MPI_Allreduce combines all local histograms into a global histogram.
Sample Input
Number of processes: 4
Values range from 0 to 9.
P0: 1 2 2 3
P1: 2 3 4 4
P2: 1 4 5 5
P3: 0 2 5 9
Sample Output
Final histogram after MPI_Allreduce:
Value    Frequency
0        1
1        2
2        4
3        2
4        3
5        3
6        0
7        0
8        0
9        1

Total number of elements = 20
*/

#include <mpi.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
    int rank, size;
    int local[4], hist[10] = {0}, global[10] = {};

    int data[16] = {1, 2, 2, 3, 2, 3, 4, 4, 1, 4, 5, 5, 0, 2, 5, 9};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Scatter(data, 4, MPI_INT, local, 4, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < 4; i++)
    {
        hist[local[i]]++;
    }
    MPI_Allreduce(hist, global, 10, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("Value    Frequency\n");

        for (int i = 0; i < 10; i++)
            printf("%d        %d\n", i, global[i]);

        printf("Total number of elements = 16\n");
    }
    MPI_Finalize();
    return 0;
}
