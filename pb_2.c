/**
Problem_2: The root process distributes different rows of a matrix to worker processes using MPI_Send. Workers calculate the sum of their assigned rows and return the results using MPI_Recv.
Sample Input
Number of processes: 4
Matrix:
1  2  3  4
5  6  7  8
9  10 11 12
13 14 15 16
Each process receives one row.
Sample Output
P0: Row = 1 2 3 4      		Sum = 10
P1: Row = 5 6 7 8      		Sum = 26
P2: Row = 9 10 11 12   	Sum = 42
P3: Row = 13 14 15 16  	Sum = 58
Total matrix sum = 136
**/

#include <stdio.h>
#include <mpi.h>

void print(int p, int r[], int sm)
{
    printf("P%d: Row = ",p);
    for (int i = 0; i < 4; i++)
    {
        printf("%d ", r[i]);
    }
    printf("\t Sum = %d\n", sm);
}

int main(int argc, char *argv[])
{
    int rank, size;
    int matrix[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 15}};
    int row[4];
    int sum = 0;
    int total = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4)
    {
        printf("Test process") ; 
        MPI_Finalize();
        return 0;
    }

    if (rank == 0)
    {
        for (int i = 1; i < 4; i++)
        {
            MPI_Send(matrix[i], 4, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        for (int j = 0; j < 4; j++)
        {
            sum += matrix[0][j];
        }
        print(rank, matrix[0], sum);
        total = sum;
        for (int i = 1; i < 4; i++)
        {
            MPI_Recv(&sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            print(i, matrix[i], sum);
            total += sum;
        }
        printf("Total sum = %d", total);
    }
    else
    {
        MPI_Recv(row, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sum = 0;
        for (int j = 0; j < 4; j++)
        {
            sum += row[j];
        }
        MPI_Send(&sum , 1, MPI_INT , 0 , 0  , MPI_COMM_WORLD) ; 
    }
    // printf("RANK %d : SIZE %d\n", rank, size);

    MPI_Finalize();
    return 0;
}
