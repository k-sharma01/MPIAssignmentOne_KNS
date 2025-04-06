#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[]) {
    const int N = 5000; // Matrix size
    int rank, size, i, j, k;
    double start_time;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rowsPerProcess = N / size;

    // Allocate memory for flattened matrices
    int *matrix1 = NULL;
    int *matrix2 = (int*) malloc(N * N * sizeof(int));
    int *resultMatrix = NULL;

    if (rank == 0) {
        matrix1 = (int*) malloc(N * N * sizeof(int));
        resultMatrix = (int*) malloc(N * N * sizeof(int));

        // Fill matrices with random values 0-500
        srand(time(NULL));
        for (i = 0; i < N * N; i++) {
            matrix1[i] = rand() % 500;
            matrix2[i] = rand() % 500;
        }

        start_time = MPI_Wtime();
    }

    // Buffers for each process
    int *subMatrix1 = (int*) malloc(rowsPerProcess * N * sizeof(int));
    int *subResult = (int*) malloc(rowsPerProcess * N * sizeof(int));

    // Scatter matrix1 rows
    MPI_Scatter(matrix1, rowsPerProcess * N, MPI_INT,
                subMatrix1, rowsPerProcess * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast full matrix2 to all
    MPI_Bcast(matrix2, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Matrix multiplication
    for (i = 0; i < rowsPerProcess; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                subResult[i * N + j] += subMatrix1[i * N + k] * matrix2[k * N + j];
            }
        }
    }

    // Gather results back to root
    MPI_Gather(subResult, rowsPerProcess * N, MPI_INT,
               resultMatrix, rowsPerProcess * N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double end_time = MPI_Wtime();
        printf("Result matrix successfully calculated.\n");
        printf("Wall time: %f seconds.\n", end_time - start_time);

        // Free root memory
        free(matrix1);
        free(resultMatrix);
    }

    free(matrix2);
    free(subMatrix1);
    free(subResult);

    MPI_Finalize();
    return 0;

} // end main
