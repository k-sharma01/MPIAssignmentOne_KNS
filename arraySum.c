#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
	const int NUMINTS = 1000000;
	int rank, size, i;
	long long local_sum = 0, total_sum = 0;
	int  *data = NULL;
	int chunk_size;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	// Start a timer to compute wall time at end
	double start_time = 0.0;
	double end_time = 0.0;
	if(rank == 0) {
		start_time = MPI_Wtime();
	}
	
	// Calculate the sizes and displacements to scatter data based on the number of processors
	// Contains logic to handle if the number is not evenly divisible
	int base_size = NUMINTS / size;
	int remainder = NUMINTS % size;
	
	int *sendcounts = malloc(size * sizeof(int));
	int *displacements = malloc(size * sizeof(int));

	// Calculate sendcounts and displacements for each process
	int offset = 0;
	for(i = 0; i < size; i++) {
		sendcounts[i] = base_size;
		if(i < remainder) {
			sendcounts[i] += 1;
		}
		displacements[i] = offset;
		offset += sendcounts[i]; // Update the offset for the next process
	}
	
	// Determine how much data the current process will receive
	int my_chunk = sendcounts[rank];
	int *localArray = (int *)malloc(my_chunk * sizeof(int));
	
	if(rank == 0) {
		data = (int *)malloc(NUMINTS * sizeof(int));
		for(i = 0; i < NUMINTS; i++) data[i] = i + 1;
	}
	
	// Scatter data
	MPI_Scatterv(data, sendcounts, displacements, MPI_INT, localArray, my_chunk, MPI_INT, 0, MPI_COMM_WORLD);
	for(i = 0; i < my_chunk; i++) {
		local_sum += localArray[i];
	}
	
	MPI_Reduce(&local_sum, &total_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

	if(rank == 0) {
		end_time = MPI_Wtime();
		printf("The total sum is: %lld\n", total_sum);
		printf("Wall time: %f seconds.\n", end_time-start_time);
		free(data);
	}

	free(localArray);
	
	MPI_Finalize();

	return 0;
} /* main */
