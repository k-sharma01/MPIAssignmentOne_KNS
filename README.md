# Problem 1: Hello World

The hello.c file successfully implements MPI to print hello world from the number of processors specified by the command-line arguments.

# Problem 2: Summing an Array

The arraySum.c file splits up an array among a specified number of processors and computes the sum. I verified that it produced the correct result using Gauss' formula, and found that more processes did not indicate
significant improvement in wall time, suggesting that communication among processes accounted for some time as well.

# Problem 3: Matrix Multiplication

The matrixMultSerial.c file contains code to serially compute the result of matrix multiplication. I attempted to run this on a 5000x5000 matrix of random numbers, and it took over 90 minutes before I decided
to kill it, though it works fine with smaller matrices. The matrixMultParallel.c file does the same computation but on a flattened matrix that is distributed among processors. This took approximately 14
minutes to complete on the 5000x5000 matrix, and significantly less time than the serial program on smaller matrices.
