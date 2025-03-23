#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size, parcel;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >=3) {
        if (rank == 0) {
            parcel = 1;
            MPI_Send(&parcel, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&parcel, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Parcel value is %d\n", parcel);
        }else {
            MPI_Recv(&parcel, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            parcel++;
            int next_rank;
            if (rank + 1 == size) {
                next_rank = 0;
            } else {
                next_rank = rank + 1;
            }
            MPI_Send(&parcel, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
        }
    }
    else{
        printf("This program requires at least 3 processes\n");
        MPI_Finalize();
    }

    
    MPI_Finalize();
    return 0;
}