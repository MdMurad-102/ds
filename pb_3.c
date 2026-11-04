/** 
Problem_3: Implement a ring topology in which every MPI process sends a message to its next process and receives a message from its previous process. Measure the communication time for different message sizes.
Sample Input
Number of processes = 4
Initial value at P0 = 100
Communication: P0 → P1 → P2 → P3 → P0
Sample Output
P0 sends 100 to P1
P1 receives 100 from P0
P1 sends 100 to P2
P2 receives 100 from P1
P2 sends 100 to P3
P3 receives 100 from P2
P3 sends 100 to P0
P0 receives 100 from P3
Ring communication completed.
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
    int data  = 100 ; 
    int next , prev ; 
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    next = (rank+1)%size ; 
    prev = (rank-1+size)%size ; 
    
    if(rank == 0 )
    {
       printf("P0 sends 100 to P1\n");
    }
    if(rank==0)
    {
        MPI_Send(&data , 1 , MPI_INT ,next , 0 , MPI_COMM_WORLD) ; 
        MPI_Recv(&data , 1 , MPI_INT,prev , 0 , MPI_COMM_WORLD,MPI_STATUS_IGNORE) ; 
        printf("P0 receives %d from P%d\n",data,prev) ; 
    }
    else
    {
        MPI_Recv(&data , 1 , MPI_INT,prev , 0 , MPI_COMM_WORLD,MPI_STATUS_IGNORE) ; 
        printf("P%d receives %d from P%d\n",rank,data,prev) ; 
        printf("P%d sends %d to P%d\n",rank,data,next) ; 
        MPI_Send(&data , 1, MPI_INT , next , 0 , MPI_COMM_WORLD) ; 
    }
    
    if(rank==0)
    {
       printf("Ring communication completed\n");
    }
    

    MPI_Finalize() ; 
    return 0;
}
