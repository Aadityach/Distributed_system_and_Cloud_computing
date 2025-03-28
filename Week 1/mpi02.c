/*
  - Process 0 sends x to process 1 and y to processes 2. "1, MPI_INT" 
    indicates that the message consists of one integer.
  - Processes other than rank 0 wait to receive a message using MPI_Recv. 
    The "0, 0" indicates that the message is expected from process 0 and 
    should have the tag 0. The result is stored in the number variable. 
*/


#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) { // Process 0 sends messages
        int x = 9;
        int y = 17;
        
        MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&y, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    } 
    else if (rank == 1 || rank == 2) { // Processes 1 and 2 receive messages
        int number;
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received %d\n", rank, number);
    }

    MPI_Finalize();
    return 0;
}
