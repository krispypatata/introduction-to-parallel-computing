#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "generateMatrix.h"

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

// Function to divide a square matrix into t submatrices
int ***divideSquareMatrix(int n, int t, int **matrix)
{
    // Allocate memory for the submatrices
    int ***submatrices = (int ***)malloc(t * sizeof(int **));
    if (submatrices == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    for (int i = 0; i < t; i++)
    {
        submatrices[i] = (int **)malloc(n * sizeof(int *));
        if (submatrices[i] == NULL)
        {
            printf("Memory allocation failed.\n");
            exit(1);
        }
        for (int j = 0; j < n; j++)
        {
            submatrices[i][j] = (int *)malloc(n * sizeof(int));
            if (submatrices[i][j] == NULL)
            {
                printf("Memory allocation failed.\n");
                exit(1);
            }
        }
    }


    for (int subMatrix = 0; subMatrix < t; subMatrix++)
    {
        // Calculate submatrix size
        int submatrixSize = (subMatrix == t - 1) ? (n / t + n % t) : (n / t);

        for (int row = 0; row < submatrixSize; row++)
        {
            for (int col = 0; col < n; col++)
            {
                submatrices[subMatrix][row][col] = matrix[subMatrix * (n / t) + row][col];
            }
        }
    }

    return submatrices;
}

// Function to deallocate memory for the submatrices (handle excess rows)
void freeSubMatrices(int t, int n, int ***submatrices)
{
    // if there are excess rows, free them as well
    int excessRows = n % t;

    for (int i = 0; i < t; i++)
    {
        int submatrixSize = (i == t - 1) ? (n / t + n % t) : (n / t);

        for (int j = 0; j < submatrixSize; j++)
        {
            free(submatrices[i][j]);
        }
        free(submatrices[i]);
    }
}



// Main function
int main()
{
    int n;
    printf("Enter the size of the square matrix: ");
    scanf("%d", &n);

    // Creating non-zero square matrix
    int **matrix = createNonZeroSquareMatrix(n);

    // Displaying the matrix
    printf("Generated non-zero %d x %d matrix:\n", n, n);
    printMatrix(matrix, n, n);

    // Divide the matrix into t submatrices
    int t;
    printf("Enter the number of submatrices to divide the matrix into: ");
    scanf("%d", &t);

    // Creating submatrices
    int ***submatrices = divideSquareMatrix(n, t, matrix);

    // Displaying the submatrices
    printf("Divided matrix into %d submatrices:\n", t);
    for (int i = 0; i < t; i++)
    {
        int submatrixSize = (i == t - 1) ? (n / t + n % t) : (n / t);
        printf("Submatrix %d:\n", i + 1);
        printMatrix(submatrices[i], submatrixSize, n);
    }

    // Free allocated memory
    freeSquareMatrix(n, matrix);

    // Free allocated memory
    freeSubMatrices(t, n, submatrices);

    return 0;
}


