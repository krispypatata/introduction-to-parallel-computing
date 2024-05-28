#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// MARK: matrix creation
// region - Create a non-zero n x n square matrix
// Function to generate a random non-zero positive integer
int randomNonZeroPositiveInt()
{
    return (rand() % INT_MAX) + 1; // Generating numbers from 1 to 9
}

// Function to create a non-zero n x n square matrix
int **createNonZeroSquareMatrix(int n)
{
    // Allocate memory for the matrix
    int **matrix = (int **)malloc(n * sizeof(int *));
    if (matrix == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    for (int i = 0; i < n; i++)
    {
        matrix[i] = (int *)malloc(n * sizeof(int));
        if (matrix[i] == NULL)
        {
            printf("Memory allocation failed.\n");
            exit(1);
        }
    }

    // Seed the random number generator
    srand(time(NULL));

    // Fill the matrix with random non-zero positive integers
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            // Generate random non-zero positive integer
            do
            {
                matrix[i][j] = randomNonZeroPositiveInt();
            } while (matrix[i][j] == 0); // Ensure non-zero value
        }
    }

    return matrix;
}

// Function for printing an mxn 2d-array matrix
void printMatrix(int **mat, int m, int n)
{
    // print line decoration
    printf("\n");
    printf("=========================================================================================================================\n");
    // print size of the matrix
    printf("Matrix size: %d x %d\n", m, n);

    // traversing through the elements of the matrix
    for (int row = 0; row < m; row++)
    {
        for (int col = 0; col < n; col++)
        {
            printf("%d ", mat[row][col]);
        }
        printf("\n");
    }
    printf("=========================================================================================================================\n");
}

// Function to deallocate memory for the square matrix
void freeSquareMatrix(int n, int **matrix)
{
    for (int i = 0; i < n; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}
// endregion

int main(int argc, char *argv[])
{
    // MARK: - n, p, s Values
    // region - Get the values of n, p, and s from the command line arguments
    // ensure correct number of arguments are provided
    if (argc != 4)
    {
        printf("Usage: %s <n> <p> <s>\n", argv[0]);
        return 1;
    }

    // retrieve values of n, p, and s
    int n = atoi(argv[1]); // size of the matrix
    int p = atoi(argv[2]); // port number
    int s = atoi(argv[3]); // status of the instance (0: master, 1: slave)

    // for checking
    printf("n: %d, p: %d, s: %d\n", n, p, s);
    // endregion

    // region - Master Instance
    if (s == 0)
    {
        // Create a non-zero nxn square matrix M with random non-zero positive integers
        int **M = createNonZeroSquareMatrix(n);
        // print the matrix
        printMatrix(M, n, n);

        // Read the configuration file to determine the IP addresses and ports of the slaves and the number of slaves t
        // TODO: Read the configuration file

        // Divide M into t submatrices of size (n/t)xn each, m_1, m_2, ..., m_t

        // TODO: Divide M into submatrices

        // Take note of the system time time_before
        // <https://stackoverflow.com/questions/5248915/execution-time-of-c-program>
        struct timespec begin, end;
        double time_spent;
        clock_gettime(CLOCK_MONOTONIC, &begin);

        // Distribute the t submatrices to the corresponding t slaves by opening the port p and initiating communication with the IP and port of each slave
        // TODO: Distribute submatrices to slaves

        // Receive the acknowledgment "ack" from each slave, for all slaves t
        // TODO: Receive acknowledgments from slaves

        // Wait when all t slaves have sent their respective acknowledgments
        // TODO: Wait for acknowledgments from all slaves

        // Output time_elapsed at each instance's terminal
        // calculate the elapsed/execution time of the algorithm
        clock_gettime(CLOCK_MONOTONIC, &end);
        time_spent = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1e9;
        printf("==============================================\n");
        printf("\nElapsed time: %f seconds\n", time_spent);

        // Verify that each of the t slaves received the correct submatrix
        // TODO: Verify submatrices received by slaves
    }
    // endregion
    // region - Slave Instance
    else if (s == 1)
    {
        // Slave code
        // Read from the configuration file what is the IP address of the master
        // TODO: Read the configuration file

        // Wait for the master to initiate an open port communication with it by listening to the port assigned by the configuration file
        // TODO: Wait for master's communication

        // Take note of the system time time_before
        // <https://stackoverflow.com/questions/5248915/execution-time-of-c-program>
        struct timespec begin, end;
        double time_spent;
        clock_gettime(CLOCK_MONOTONIC, &begin);

        // Receive from the master the submatrix mi assigned to it
        // TODO: Receive submatrix from master

        // Send an acknowledgment "ack" to the master once the submatrix has been received fully
        // TODO: Send acknowledgment to master

        // Output time_elapsed at each instance's terminal
        // calculate the elapsed/execution time of the algorithm
        clock_gettime(CLOCK_MONOTONIC, &end);
        time_spent = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1e9;
        printf("==============================================\n");
        printf("\nElapsed time: %f seconds\n", time_spent);
    }
    // endregion

    return 0;
}
