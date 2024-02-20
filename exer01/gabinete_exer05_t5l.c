/*
	Author: Gabinete, Keith Ginoel S.
	Section: CMSC 180 - T-5L

    Lab Exercise 01
    Date Created: February 18, 2024

	Program Description: 
		This is a simple program that computes the Pearson Correlation Coefficient of a matrix and a vector.
*/


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


// ======================================================================================================
/*
    a fucntion to compute the Pearson Correlation Coefficient vector of an nxn square matrix X 
    with a nx1 vector y

    parameters:
        matrix X
        vector y, and
        value of n, and

    returns:
        vector v (pearson correlation coefficients)
*/
double* pearson_cor (double *mat, double *vector, int nRow, int nCol) {
    double *X = mat;
    double *y = vector;
    int m = nRow;
    int n = nCol;

    // vector for the Pearson Correlation coefficients
    double *r = (double*) malloc(n * sizeof(double));

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
    printf("\n\nSUM OF Y:");
    printf("\n%.2f\n", sum_y);
    
    printf("\nSUM OF X:");
    for (int i=0; i<m; i++) {
        printf("\n%.2f", sum_X[i]);
    }
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
    printf("\n\nSUM OF Y SQUARES:");
    printf("\n%.2f\n", sum_y_squared);
    
    printf("\nSUM OF X SQUARES:");
    for (int i=0; i<n; i++) {
        printf("\n%.2f", sum_X_squared[i]);
    }
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
    printf("\n\nSUM OF CROSS PRODUCTS:");
    for (int i=0; i<n; i++) {
        printf("\n%.2f", sum_cross_product[i]);
    }
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

    return r;
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

    // for checking
    
	// for (int row=0; row<n; row++){
	// 	for (int col=0; col<n; col++){
    //         mat[row*n+col] = 1;
	// 		printf("[%d]:\t%d\t", row*n+col, mat[row*n+col]);
    //         if (col==n-1) printf("\n");
	// 	}
	// }

    // creating a 10x10 matrix
    n = 10;
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
    for (int row=0; row<n; row++){
		for (int col=0; col<n; col++){
			printf("[%d]:\t%.2f\t", row*n+col, mat[row*n+col]);
            if (col==n-1) printf("\n");
		}
	}

    pearson_cor(mat, lengths, n, n);

    // free the matrix to avoid memory leaks
    free(mat);

    // ==================================================================================================
    
    /*
        creating an nxn double matrix
    */
    // double mat[n*n];

    

    return 0;
}

