/*
	Author: Gabinete, Keith Ginoel S.
	Section: CMSC 180 - T-5L

    Lab Exercise 01
    Date Created: February 18, 2024

	Program Description: 
		This is a simple program that computes the Pearson Correlation Coefficient of a matrix and a vector.
*/

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// ======================================================================================================
/*
    a function for printing a square matrix that is made with 1d array
*/
void printMatrix(double **mat, int m, int n, int nDecimal) {
    // total field width including decimal point and sign
    int fieldWidth = 8; 

    // number of digits after the decimal point
    int precision = nDecimal;  

    // traversing through the elements of the matrix
    for (int row=0; row<m; row++) {
        for (int col=0; col<n; col++) {
            printf("%*.*f ", fieldWidth, precision, mat[row][col]);
        }
        printf("\n");
    }
}


// ======================================================================================================
/*
    a function for printing a vector
*/
void printVector(double vector[], int n, int nDecimal) {
    // total field width including decimal point and sign
    int fieldWidth = 5; 

    // number of digits after the decimal point
    int precision = nDecimal;

    // traverse through the elements of the vector
    printf("[ ");
    for (int i = 0; i < n; i++) {
        printf("%*.*f ", fieldWidth, precision, vector[i]);

        // adding commas
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf("]");
    printf("\n");
}


// ======================================================================================================
/*
    a function to compute the Pearson Correlation Coefficient vector of an nxn square matrix X 
    with a nx1 vector y

    parameters:
        matrix X
        vector y, and
        value of n, and

    returns:
        vector v (pearson correlation coefficients)
*/
double* pearson_cor (double **mat, double *vector, int nRow, int nCol) {
    // for checking (submatrix)
    // printMatrix(mat, nRow, nCol, 2);

    // for code simplicity/clarity
    double **X = mat;
    double *y = vector;
    int m = nRow;
    int n = nCol;

    // vector for the Pearson Correlation coefficients
    double *v = (double*) malloc(n * sizeof(double));

    // ==================================================================================================
    /* 
        Step 1: Calculate the sums of x and y
    */
    // ==================================================================================================
    double *sum_X = (double *) malloc (n * sizeof(double));
    // double sum_X[n]; // alternative (if encountering segfault)
    double sum_y = 0;

    // traverse through the columns of matrix x
    for (int j=0; j<n; j++) {
        double sum_col = 0;
        for (int i=0; i<m; i++) {
            sum_col += X[i][j];
        }

        // add the computed sum of the column to sum_X
        sum_X[j] = sum_col;
    }

    // traverse through the elements of vector y
    for (int i=0; i<m; i++) {
        sum_y += y[i];
    }

    // --------------------------------------------------------------------------------------------------
    // for checking
    // printf("\nSUM OF X:\n");
    // printVector(sum_X, n, 0);

    // printf("\n\nSUM OF Y:");
    // printf("\n%.2f\n", sum_y);

    // printf("\n");
    // --------------------------------------------------------------------------------------------------
    
    // ==================================================================================================
    /*
        Step 2: Calculate x2 and y2 and their sums
    */
    // ==================================================================================================
    double *sum_X_squared = (double *) malloc (n * sizeof(double));
    // double sum_X_squared[n];  // alternative (if encountering segfault)
    double sum_y_squared = 0;

    // traverse through the columns of matrix x, square all elements and get their sum
    for (int j=0; j<n; j++) {
        double sum_col_squared = 0;
        for (int i=0; i<m; i++) {
            sum_col_squared += X[i][j]*X[i][j];
        }

        // add the computed sum of the column to sum_X_squared
        sum_X_squared[j] = sum_col_squared;
    }

    // traverse through the elements of vector y, square all elements and get their sum
    for (int i=0; i<m; i++) {
        sum_y_squared += y[i]*y[i];
    }

    // --------------------------------------------------------------------------------------------------
    // for checking
    // printf("\nSUM OF X SQUARES:\n");
    // printVector(sum_X_squared, n, 0);
    
    // printf("\n\nSUM OF Y SQUARES:");
    // printf("\n%.0f\n", sum_y_squared);
    // printf("\n");

    // --------------------------------------------------------------------------------------------------

    // ==================================================================================================
    /*
        Step 3: Calculate the cross product and its sum
    */
    // ==================================================================================================
    double *sum_cross_product = (double *) malloc (n * sizeof(double));
    // double sum_cross_product[n];  // alternative (if encountering segfault)

    // traverse through the columns of matrix x, compute for the cross product of each column and vector y
    for (int j=0; j<n; j++) {
        double sum_col_cross_product = 0;
        for (int i=0; i<m; i++) {
            sum_col_cross_product += X[i][j] * y[i];
        }

        // add the computed sum of the column to sum_X_squared
        sum_cross_product[j] = sum_col_cross_product;
    }

    // --------------------------------------------------------------------------------------------------
    // for checking
    // printf("\n\nSUM OF CROSS PRODUCTS:\n");
    // printVector(sum_cross_product, n, 0);
    // printf("\n");
    // --------------------------------------------------------------------------------------------------

    // ==================================================================================================
    /*
        Step 4: Calculate r
    */
    // ==================================================================================================
    for (int i=0; i<n; i++) {
        double r = 0;
        r = ( m*sum_cross_product[i] - sum_X[i] * sum_y ) / sqrt( (m*sum_X_squared[i] - sum_X[i]*sum_X[i]) * (m*sum_y_squared - sum_y*sum_y) );
        v[i] = r;
    }

    // --------------------------------------------------------------------------------------------------
    // for checking
    printf("\n\nPEARSON CORRELATION COEFFICIENTS:\n");
    printVector(v, n, 4);
    // printf("===========================================================================\n");
    // printf("===========================================================================\n");
    // printf("===========================================================================\n");
    // --------------------------------------------------------------------------------------------------

    // ==================================================================================================
    /*
        Cleanup
    */
    // ==================================================================================================
    // deallocating memory
    free(sum_X);
    free(sum_X_squared);
    free(sum_cross_product);

    return v;
}


// ======================================================================================================
/*
    testing the implementation of the created Pearson Correlation Coefficient algorithm with the test
    cases provided in the lab handout
*/
void testAlgorithm () {
    // creating a 10x10 matrix
    int n = 10;
    double **mat = (double **)malloc(n * sizeof(double *));
    
    for (int i = 0; i < n; i++) {
        mat[i] = (double *)malloc(n * sizeof(double));
    }

    if (mat == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    // test case (provided in the lab handout)
    double weights[] = {3.63, 3.02, 3.82, 3.42, 3.59, 2.87, 3.03, 3.46, 3.36, 3.3};
    double lengths[] = {53.1, 49.7, 48.4, 54.2, 54.9, 43.7, 47.2, 45.2, 54.4, 50.4};

    // initializing each column of the matrix with the values from the weights vector
    for (int row=0; row<n; row++){
		for (int col=0; col<n; col++){
            mat[col][row] = weights[col];
		}
	}

    // for checking (printing)
    // printMatrix(mat, n, 2);

    // measure execution time
    // <https://stackoverflow.com/questions/5248915/execution-time-of-c-program>
    clock_t begin = clock();

    // compute for the Pearson Correlation Coefficient of the matrix and the given vector
    pearson_cor(mat, lengths, n, n);

    // calculate the execution time of the algorithm
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    // display the execution time
    printf("\nExecution time: %fs\n", time_spent);

    // free the matrix to avoid memory leaks
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);

    // force exit the program
    exit(0);
}


// ======================================================================================================
/*
    a function that generates a random non-zero integer
*/
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


// ======================================================================================================
/*
    Start of the main program
*/
int main() {
    // ==================================================================================================
    /*
        Testing the algorithm by using the test cases provided in the lab handout
    */
    // ==================================================================================================
    testAlgorithm();
    // ==================================================================================================

    // for asking input from the user
    char fgetsInput[128], fgetsInputTrimmed[128];
    char dump[128];
    int n;

    // for the while loop
    bool isInvalidInput = true;

    // seed the random number generator
    // makes sure that random numbers are actually generated every time this program is executed
    srand(time(NULL)); 

    // ==================================================================================================
    /*
        ask an input from the user
    */
    // will run until a valid input is entered
    while (isInvalidInput) {
        // message prompt
        printf("Please enter the size of the square matrix: ");

        // fetch input stream contents
        fgets(fgetsInput, 127, stdin);

        // remove whitespaces from the input string
        char *inputPtr = fgetsInput;
        char *trimmedPtr = fgetsInputTrimmed;
        
        while (*inputPtr != '\0') {
            if (!isspace(*inputPtr)) {
                *trimmedPtr = *inputPtr;
                trimmedPtr++;
            }
            inputPtr++;
            
        }

        // put termination character on the trimmed string
        *trimmedPtr = '\0';

        // store the fetched contents on some variables (for input validity checking)
        int fetchCount = sscanf(fgetsInputTrimmed, " %d%s ", &n, dump);

        // check if only one integer is read and if there are no leftover characters
        if (fetchCount == 1 && dump[0] == '\0') {
            // valid input is found; break the loop
            isInvalidInput = false;

            // show the entered value of n
            printf("\n==================");
            printf("\nValue of n: %d", n);
            printf("\n==================\n");
        } else {
            // prompt an error message
            printf("\n==================");
            printf("\nInvalid input.");
            printf("\n==================");
            printf("\n\n");
        }
        // clear dump contents for the next iteration
        for (int i = 0; i < 128; i++) dump[i] = '\0';

        // for checking
        // printf("Fecth count: %d\n", fetchCount);
    }
    // ==================================================================================================
    /*
        Testing the algorithm by using the test cases provided in the lab handout
    */
    // ==================================================================================================
    // testAlgorithm();

    // ==================================================================================================
    /*
        Creating an nxn double matrix
    */
    // ==================================================================================================
    double **mat = (double **) malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        mat[i] = (double *)malloc(n * sizeof(double));
    }

    // ==================================================================================================
    /*
        Filling in the generated nxn matrix with random values
    */
    // ==================================================================================================
    // traversing through the elements of the matrix
    for (int col=0; col<n; col++) {
        for (int row=0; row<n; row++) {
            // generate a random non-zero integer and put it in the currently selected cell
            mat[row][col] = generateNonZeroInteger();
        }
    }

    double *vector = (double *) malloc(n * sizeof(double));
    for (int i=0; i<n; i++) {
        // generate a random non-zero integer and put it in the currently selected cell
        vector[i] = generateNonZeroInteger();
    }

    // for checking (printing)
    // printf("\nVector:");
    // printVector(vector, n, 0);
    // printf("\n");
    
    // printMatrix(mat, n, 0);

    // measure execution time
    // <https://stackoverflow.com/questions/5248915/execution-time-of-c-program>
    clock_t begin = clock();

    // compute for the Pearson Correlation Coefficient of the matrix and the given vector
    double *pearson_correlation_coefficients = pearson_cor(mat, vector, n, n);

    // calculate the execution time of the algorithm
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    // display the execution time
    printf("\nExecution time: %fs\n", time_spent);

    // free the matrix to avoid memory leaks
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
    free(vector);
    free(pearson_correlation_coefficients);


    return 0;
}

