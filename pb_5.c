/**
Problem_5: Distribute matrix rows from the root process to other processes using MPI_Send. Each process performs its portion of matrix multiplication and sends the result back to the root.
Sample Input
Number of processes: 2
Matrix A:
1 2
3 4
Matrix B:
5 6
7 8
Sample Output
Matrix C = A × B
19 22
43 50

C[0][0] = 1×5 + 2×7 = 19
C[0][1] = 1×6 + 2×8 = 22
C[1][0] = 3×5 + 4×7 = 43
C[1][1] = 3×6 + 4×8 = 50
*/

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{

    int A[2][2] = {{1, 2}, {3, 4}};
    int B[2][2] = {{5, 6}, {7, 8}};
    int C[2][2];
    int row[2], reslut[2];

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        row[0] = A[0][0];
        row[1] = A[0][1];
        for (int i = 0; i < 2; i++)
        {
            C[0][i] = 0;
            for (int j = 0; j < 2; j++)
            {
                C[0][i] += row[j] * B[j][i];
            }
        }

        MPI_Send(A[1], 2, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(reslut, 2, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        C[1][0] = reslut[0];
        C[1][1] = reslut[1];
        printf("Metrix C = A x B\n");
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    }
    else
    {
        MPI_Recv(row, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < 2; i++)
        {
            reslut[i] = 0;
            for (int j = 0; j < 2; j++)
            {
                reslut[i] += row[j] * B[j][i];
            }
        }
        MPI_Send(reslut, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}