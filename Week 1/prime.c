#include <stdio.h>
#include <math.h>
#include <mpi.h>

int is_prime(int num) {
    if (num < 2) return 0; // 0 and 1 are not prime
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return 0; // Not a prime
    }
    return 1; // Prime number
}

int main(int argc, char **argv) {
    int rank, size;
    int nstart = 1, nfinish = 10000;
    
    MPI_Init(&argc, &argv);                 // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // Get process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // Get total number of processes

    // Compute the range for each process
    int range = (nfinish - nstart + 1) / size;
    int local_start = nstart + rank * range;
    int local_end = (rank == size - 1) ? nfinish : local_start + range - 1;

    // Print process-specific computation
    printf("Process %d: Checking range %d to %d\n", rank, local_start, local_end);

    // Each process finds primes in its subrange
    int primes[range], count = 0;
    for (int i = local_start; i <= local_end; i++) {
        if (is_prime(i)) {
            primes[count++] = i;
        }
    }

    // Process 0 gathers results
    if (rank == 0) {
        printf("Prime numbers between %d and %d:\n", nstart, nfinish);
        for (int i = 0; i < count; i++) {
            printf("%d ", primes[i]);
        }

        // Receive from other processes
        for (int p = 1; p < size; p++) {
            int recv_count;
            MPI_Recv(&recv_count, 1, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            int recv_primes[range];
            MPI_Recv(recv_primes, recv_count, MPI_INT, p, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            for (int j = 0; j < recv_count; j++) {
                printf("%d ", recv_primes[j]);
            }
        }
        printf("\n");
    } else {
        // Other processes send results to Process 0
        MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(primes, count, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize(); // Finalize MPI
    return 0;
}
