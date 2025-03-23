#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define CHUNK_SIZE 4096  

void count_letters(char *text, int length, int *letter_counts) {
    for (int i = 0; i < length; i++) {
        if (isalpha(text[i])) { 
            char lower = tolower(text[i]);
            letter_counts[lower - 'a']++; 
        }
    }
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int global_counts[ALPHABET_SIZE] = {0};
    int local_counts[ALPHABET_SIZE] = {0};
    char *buffer = NULL;
    long file_size = 0, chunk_size = 0;

    if (rank == 0) {
        FILE *file = fopen("/home/aaditya/Desktop/Sem 4/Distributed Systems and Cloud Computing 2024/Week 2/WarAndPeace.txt", "r");
        if (!file) {
            printf("Error opening file.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        rewind(file);

        chunk_size = (file_size / size) + 1;  
        buffer = (char *)malloc(file_size);
        
        fread(buffer, 1, file_size, file);
        fclose(file);
    }

    MPI_Bcast(&chunk_size, 1, MPI_LONG, 0, MPI_COMM_WORLD);

    char *local_buffer = (char *)malloc(chunk_size);

    MPI_Scatter(buffer, chunk_size, MPI_CHAR, local_buffer, chunk_size, MPI_CHAR, 0, MPI_COMM_WORLD);

    count_letters(local_buffer, chunk_size, local_counts);

    MPI_Reduce(local_counts, global_counts, ALPHABET_SIZE, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nLetter Frequency Count in 'War and Peace':\n");
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            printf("%c: %d\n", 'A' + i, global_counts[i]);
        }
    }

    if (rank == 0) free(buffer);
    free(local_buffer);

    MPI_Finalize();
    return 0;
}