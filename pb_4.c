/**
Problem_4: Implement a ping-pong program between two MPI processes using MPI_Send and MPI_Recv. Test message sizes from a few bytes to several megabytes and analyze communication latency and bandwidth.
Sample Input
Number of processes: 2
Number of iterations = 5
Message size = 1024 bytes
Sample Output
Message size      	: 1024 bytes
Iterations         	: 5
Total round trips	: 5
Average round-trip	: 0.000018 seconds
Average one-way    	: 0.000009 seconds
Repeat the experiment for 64 bytes, 256 bytes, 1 KB, 4 KB, 16 KB, 64 KB, and 1 MB.
*/

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{

    int rank, size;
    int ite = 5;
    int sizes[] = {64, 256, 1024, 4096, 1024 * 16, 1024 * 64, 1024 * 1024};
    char data[1024 * 1024];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < 7; i++)
    {
        int msg = sizes[i];
        double start, end, rtt, one, band;
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0)
        {
            start = MPI_Wtime();

            for (int i = 0; i < ite; i++)
            {
                MPI_Send(data, msg, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(data, msg, MPI_CHAR, 1, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            end = MPI_Wtime();

            double total_time = end - start;

            rtt = total_time / ite;
            one = rtt / 2;
            band = msg / one;
            printf("Message size.       : %d\n",msg) ; 
            printf("Total time          : %f seconds\n", total_time);
            printf("Average round-trip  : %f seconds\n", rtt);
            printf("Average one-way     : %f seconds\n", one);
            printf("Bandwidth           : %f Bytes/sec\n", band);
        }
        else
        {
            for (int j = 0; j < ite; j++)
            {
                MPI_Recv(data, msg, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(data, msg, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            }
        }
        printf(" \n\n");
    }
    MPI_Finalize();
    return 0;
}