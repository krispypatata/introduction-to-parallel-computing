/*
	Author: Gabinete, Keith Ginoel S.
	Section: CMSC 180 - T-5L

    Lab Exercise 01
    Date Created: February 18, 2024

	Program Description: 
		This is a simple program that computes the Pearson Correlation Coefficient of a matrix and a vector.
*/

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// ======================================================================================================
/*
    a function for printing a square matrix that is made with 1d array
*/
// ======================================================================================================
void printMatrix(double mat[], int n) {
    // total field width including decimal point and sign
    int fieldWidth = 5; 

    // number of digits after the decimal point
    int precision = 2;  

    // traversing through the elements of the matrix
    for (int row=0; row<n; row++) {
        for (int col=0; col<n; col++) {
            printf("%*.*f ", fieldWidth, precision, mat[row*n+col]);
        }
        printf("\n");
    }

    // another format:
    // for (int row=0; row<n; row++){
	// 	for (int col=0; col<n; col++){
    //         printf("%5d ", mat[row*n+col]);
	// 		// printf("[%d]:\t%.2f\t", row*n+col, mat[row*n+col]);
    //         if (col==n-1) printf("\n");
	// 	}
	// }
}


// ======================================================================================================
/*
    a function for printing a vector
*/
// ======================================================================================================
void printVector(double vector[], int n) {
    // total field width including decimal point and sign
    int fieldWidth = 5; 

    // number of digits after the decimal point
    int precision = 2;

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
// ======================================================================================================
double* pearson_cor (double *mat, double *vector, int nRow, int nCol) {
    double *X = mat;
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
    double sum_y = 0;

    // traverse through the columns of matrix x
    for (int i=0; i<m; i++) {
        double sum_col = 0;
        for (int j=0; j<n; j++) {
            sum_col += X[j*n+i];
        }

        // add the computed sum of the column to sum_X
        sum_X[i] = sum_col;
    }

    // traverse through the elements of vector y
    for (int i=0; i<m; i++) {
        sum_y += y[i];
    }

    // --------------------------------------------------------------------------------------------------
    // for checking
    printf("\nSUM OF X:\n");
    printVector(sum_X, n);

    printf("\n\nSUM OF Y:");
    printf("\n%.2f\n", sum_y);

    printf("\n");
    // --------------------------------------------------------------------------------------------------

    // ==================================================================================================
    /*
        Step 2: Calculate x2 and y2 and their sums
    */
    // ==================================================================================================
    double *sum_X_squared = (double *) malloc (n * sizeof(double));
    double sum_y_squared = 0;

    // traverse through the columns of matrix x, square all elements and get their sum
    for (int i=0; i<m; i++) {
        double sum_col_squared = 0;
        for (int j=0; j<n; j++) {
            sum_col_squared += X[j*n+i]*X[j*n+i];
        }

        // add the computed sum of the column to sum_X_squared
        sum_X_squared[i] = sum_col_squared;
    }

    // traverse through the elements of vector y, square all elements and get their sum
    for (int i=0; i<m; i++) {
        sum_y_squared += y[i]*y[i];
    }

    // --------------------------------------------------------------------------------------------------
    // for checking
    printf("\nSUM OF X SQUARES:\n");
    printVector(sum_X_squared, n);
    
    printf("\n\nSUM OF Y SQUARES:");
    printf("\n%.2f\n", sum_y_squared);
    printf("\n");

    // --------------------------------------------------------------------------------------------------

    // ==================================================================================================
    /*
        Step 3: Calculate the cross product and its sum
    */
    // ==================================================================================================
    double *sum_cross_product = (double *) malloc (n * sizeof(double));

    // traverse through the columns of matrix x, compute for the cross product of each column and vector y
    for (int i=0; i<m; i++) {
        double sum_col_cross_product = 0;
        for (int j=0; j<n; j++) {
            sum_col_cross_product += X[j*n+i] * y[j];
        }

        // add the computed sum of the column to sum_X_squared
        sum_cross_product[i] = sum_col_cross_product;
    }

    // --------------------------------------------------------------------------------------------------
    // for checking
    printf("\n\nSUM OF CROSS PRODUCTS:\n");
    printVector(sum_cross_product, n);
    printf("\n");
    // --------------------------------------------------------------------------------------------------

    // ==================================================================================================
    /*
        Step 4: Calculate r
    */
    // ==================================================================================================
    

    for (int i=0; i<n; i++) {
        double r = 0;
        r = ( n*sum_cross_product[i] - sum_X[i] * sum_y ) / sqrt( (n*sum_X_squared[1] - sum_X[i]*sum_X[i]) * (n*sum_y_squared - sum_y*sum_y) );
        v[i] = r;
    }

    // --------------------------------------------------------------------------------------------------
    // for checking
    printf("\n\nPEARSON CORRELATION COEFFICIENTS:\n");
    printVector(v, n);
    printf("\n");
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
// ======================================================================================================
void testAlgorithm () {
    // creating a 10x10 matrix
    int n = 10;
    double *mat = (double*)malloc(n * n * sizeof(double));
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
            mat[col*n+row] = weights[col];
		}
	}

    // for checking (printing)
    printMatrix(mat, n);

    // for monitoring the execution time
    clock_t begin = clock();

    // compute for the Pearson Correlation Coefficient of the matrix and the given vector
    pearson_cor(mat, lengths, n, n);

    // calculate the execution time of the algorithm
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    // display the execution time
    printf("\nExecution time: %f\n", time_spent);

    // free the matrix to avoid memory leaks
    free(mat);

    // force exit the program
    exit(0);
}


// ======================================================================================================
/*
    Start of the main program
*/
int main() {
    char fgetsInput[128];
    char dump[128];
    int n;
    // for the while loop
    bool isInvalidInput = true;

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

        // store the fetched contents on some variables (for input validity checking)
        int fetchCount = sscanf(fgetsInput, " %d%s ", &n, dump);

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
        
    */
    testAlgorithm();


    // ==================================================================================================
    
    /*
        creating an nxn double matrix
    */
    double *mat = (double*) malloc(n * n * sizeof(double));

    // measure execution time
    // <https://stackoverflow.com/questions/5248915/execution-time-of-c-program>


    return 0;
}

