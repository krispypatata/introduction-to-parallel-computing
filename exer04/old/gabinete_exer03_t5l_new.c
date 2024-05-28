#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#define MAX_SIZE 40000
#define THREAD_THRESHOLD 1000

typedef struct {
    double **matX;
    double *vectorY;
    double *correlationCoefficients;
    int startingColumn;
    int nRow;
    int nCol;
    int threadNumber;
} ThreadArgs;

int generateNonZeroInteger() {
    int random_integer;
    
    // loop until a non-zero integer is generated
    do {
        // random_integer = rand() % 2001 - 1000;
        random_integer = rand() % (INT_MAX - 1) + INT_MIN + 1;

    } while (random_integer == 0);

    // return the generated random number
    return random_integer;
}

void *performPearsonCorrelation(void *arguments) {
    ThreadArgs *args = (ThreadArgs *)arguments;

    double **submatrix = args->matX;
    double *vectorY = args->vectorY;
    int startingColumn = args->startingColumn;
    int nRow = args->nRow;
    int nCol = args->nCol;

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(args->threadNumber, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    for (int i = 0; i < nCol; i++) {
        double sum_cross_product = 0.0, sum_X = 0.0, sum_y = 0.0, sum_X_squared = 0.0, sum_y_squared = 0.0;

        for (int j = 0; j < nRow; j++) {
            sum_cross_product += submatrix[j][i] * vectorY[j];
            sum_X += submatrix[j][i];
            sum_y += vectorY[j];
            sum_X_squared += submatrix[j][i] * submatrix[j][i];
            sum_y_squared += vectorY[j] * vectorY[j];
        }

        args->correlationCoefficients[startingColumn + i] = (nRow * sum_cross_product - sum_X * sum_y) /
                                                            sqrt((nRow * sum_X_squared - sum_X * sum_X) *
                                                                 (nRow * sum_y_squared - sum_y * sum_y));
    }

    pthread_exit(NULL);
}

void computePearsonCorrelation(double **mat, double *vector, double *correlationCoefficients, int n, int numThreads) {
    if (n <= THREAD_THRESHOLD || numThreads <= 1) {
        for (int i = 0; i < n; i++) {
            double sum_cross_product = 0.0, sum_X = 0.0, sum_y = 0.0, sum_X_squared = 0.0, sum_y_squared = 0.0;

            for (int j = 0; j < n; j++) {
                sum_cross_product += mat[j][i] * vector[j];
                sum_X += mat[j][i];
                sum_y += vector[j];
                sum_X_squared += mat[j][i] * mat[j][i];
                sum_y_squared += vector[j] * vector[j];
            }

            correlationCoefficients[i] = (n * sum_cross_product - sum_X * sum_y) /
                                         sqrt((n * sum_X_squared - sum_X * sum_X) * (n * sum_y_squared - sum_y * sum_y));
        }
    } else {
        pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
        ThreadArgs *args = malloc(numThreads * sizeof(ThreadArgs));

        int columnsPerThread = n / numThreads;
        int remainingColumns = n % numThreads;

        int startColumn = 0;
        for (int i = 0; i < numThreads; i++) {
            args[i].matX = mat;
            args[i].vectorY = vector;
            args[i].correlationCoefficients = correlationCoefficients;
            args[i].startingColumn = startColumn;
            args[i].nRow = n;
            args[i].nCol = columnsPerThread + (i < remainingColumns ? 1 : 0);
            args[i].threadNumber = i;

            pthread_create(&threads[i], NULL, performPearsonCorrelation, (void *)&args[i]);

            startColumn += args[i].nCol;
        }

        for (int i = 0; i < numThreads; i++) {
            pthread_join(threads[i], NULL);
        }

        free(threads);
        free(args);
    }
}

int main() {
    int n, numThreads;
    printf("Enter the size of the square matrix (n): ");
    scanf("%d", &n);
    printf("Enter the number of threads: ");
    scanf("%d", &numThreads);

    if (n > MAX_SIZE) {
        printf("Matrix size exceeds maximum allowed size.\n");
        return 1;
    }

    double **mat = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        mat[i] = malloc(n * sizeof(double));
    }

        // traversing through the elements of the matrix
    for (int col=0; col<n; col++) {
        for (int row=0; row<n; row++) {
            // generate a random non-zero integer and put it in the currently selected cell
            mat[row][col] = generateNonZeroInteger();
        }
    }

    double *vector = malloc(n * sizeof(double));
    for (int i=0; i<n; i++) {
        // generate a random non-zero integer and put it in the currently selected cell
        vector[i] = generateNonZeroInteger();
    }

    double *correlationCoefficients = malloc(n * sizeof(double));

    // Fill matrix and vector with data

    struct timespec begin, end;
    double time_spent;
    clock_gettime(CLOCK_MONOTONIC, &begin);

    computePearsonCorrelation(mat, vector, correlationCoefficients, n, numThreads);

    // calculate the elapsed time time of the algorithm
    // calculate the elapsed/execution time of the algorithm
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_spent = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1e9;

    // display the elapsed/execution time
    printf("==============================================\n");
    printf("\nElapsed time: %f seconds\n", time_spent);

    // Output or use correlationCoefficients

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
    free(vector);
    free(correlationCoefficients);

    return 0;
}
