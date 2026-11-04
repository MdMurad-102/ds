FILE *fp = fopen("input.txt", "r");

if (fp == NULL)
{
    printf("File opening failed!\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
}

for (int i = 0; i < 4; i++)
{
    for (int j = 0; j < 4; j++)
    {
        fscanf(fp, "%d", &matrix[i][j]);
    }
}

fclose(fp);


mpicc problem2.c -o problem2
mpirun -np 4 ./problem2



#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int matrix[4][4];
    int row[4];
    int sum = 0, total = 0;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        // Take matrix input from keyboard
        printf("Enter 4x4 Matrix:\n");

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                scanf("%d", &matrix[i][j]);
            }
        }

        // Calculate P0's own row sum
        for (int j = 0; j < 4; j++)
        {
            row[j] = matrix[0][j];
            sum += row[j];
        }

        printf("P0: Row = ");
        for (int j = 0; j < 4; j++)
            printf("%d ", row[j]);

        printf("\t Sum = %d\n", sum);

        total = sum;

        // Send rows to workers
        for (int i = 1; i < size; i++)
        {
            MPI_Send(matrix[i], 4, MPI_INT,
                     i, 0, MPI_COMM_WORLD);
        }

        // Receive sums from workers
        for (int i = 1; i < size; i++)
        {
            MPI_Recv(&sum, 1, MPI_INT,
                     i, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            total += sum;
        }

        printf("Total matrix sum = %d\n", total);
    }
    else
    {
        // Receive row from root
        MPI_Recv(row, 4, MPI_INT,
                 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        // Calculate row sum
        sum = 0;

        for (int j = 0; j < 4; j++)
        {
            sum += row[j];
        }

        printf("P%d: Row = ", rank);

        for (int j = 0; j < 4; j++)
            printf("%d ", row[j]);

        printf("\t Sum = %d\n", sum);

        // Send sum to root
        MPI_Send(&sum, 1, MPI_INT,
                 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
