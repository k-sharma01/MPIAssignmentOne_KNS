#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[]) {

	const int N = 5000;
	int i, j, k;

	// Start measuring wall time
	clock_t start_time = clock();

	// Allocate space for matrices, checking for any errors
	int **matrix1 = (int**) malloc(N * sizeof(int*));
	int **matrix2 = (int**) malloc(N * sizeof(int*));
	int **resultMatrix = (int**) malloc(N * sizeof(int*));
	if(matrix1 == NULL || matrix2 == NULL || resultMatrix == NULL) {
		printf("Memory allocation failed.\n");
		return -1;
	}

	for(i = 0; i < N; i++) {
		matrix1[i] = (int*) malloc(N * sizeof(int));
		matrix2[i] = (int*) malloc(N * sizeof(int));
		resultMatrix[i] = (int*) malloc(N * sizeof(int));
		// check for memory allocation errors
		if(matrix1[i] == NULL || matrix2[i] == NULL || resultMatrix[i] == NULL) {
			printf("Memory allocation failed in a row.\n");
			return -1;
		}
	}

	// Populate matrices with random numbers 0 - 500
	srand(time(NULL));
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			matrix1[i][j] = (rand() % 500);
			matrix2[i][j] = (rand() % 500);
		}
	}

	// Compute result matrix
	int tempResult;
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			tempResult = 0;
			for(k = 0; k < N; k++) {
				tempResult += matrix1[i][k] * matrix2[k][j];
			}
			resultMatrix[i][j] = tempResult;
		}
	} // end outer for

	printf("Finished computing result matrix serially.\n");

    	// Free allocated memory
    	for(i = 0; i < N; i++) {
        	free(matrix1[i]);
		free(matrix2[i]);
        	free(resultMatrix[i]);
    	}
    	free(matrix1);
    	free(matrix2);
   	free(resultMatrix);

	// Print elapsed wall time
	clock_t end_time = clock();
	double elapsed_time = (double)(end_time - start_time)/ CLOCKS_PER_SEC;
	printf("Wall time: %f seconds.\n", elapsed_time);

    	return 0;

} // end main
