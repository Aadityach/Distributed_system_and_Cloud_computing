// Write an MPI program called pingpong.c to run with exactly 2 processes. Process rank 0 is to send an integer
// variable called "ball" initialised with the value zero to Process rank 1. Process rank 1 will add 1 to the ball and
// send it back. This will repeat until the ball has a value of 10 in Process rank 0.
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int ball = 0;
    while (ball < 10) {
        if (rank == 0) {
            MPI_Send(&ball, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&ball, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Rank 0 received ball: %d\n", ball);
        } else {
            MPI_Recv(&ball, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            ball++;
            MPI_Send(&ball, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
