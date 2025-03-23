#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    int size, rank, tag = 0;
    int count;
    MPI_Status status;
    int *data = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL) + rank);

    if (rank == 0) {
        for (int i = 0; i < size - 1; i++) {
            MPI_Recv(&count, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
            
            data = (int *)malloc(count * sizeof(int));
            if (data == NULL) {
                printf("Process 0: Memory allocation failed\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }

            MPI_Recv(data, count, MPI_INT, status.MPI_SOURCE, tag, MPI_COMM_WORLD, &status);
            
            int received_count;
            MPI_Get_count(&status, MPI_INT, &received_count);
            
            printf("Node ID: %d; tag: %d; received %d integers\n",
                   status.MPI_SOURCE, status.MPI_TAG, received_count);

            free(data);
            data = NULL;
        }
    } else {
        count = rand() % 100;  
        
        data = (int *)malloc(count * sizeof(int));
        if (data == NULL) {
            printf("Process %d: Memory allocation failed\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        for (int i = 0; i < count; i++) {
            data[i] = rank * 100 + i;  
        }

        MPI_Send(&count, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        
        MPI_Send(data, count, MPI_INT, 0, tag, MPI_COMM_WORLD);
        
        printf("Process %d sent %d integers to Process 0\n", rank, count);


        free(data);
        data = NULL;
    }

    MPI_Finalize();
    return 0;
}