/*
	Author: Gabinete, Keith Ginoel S.
	Section: CMSC 180 - T-5L

    Lab Exercise 01
    Date Created: February 18, 2024

	Program Description: 
		This is a simple program that computes the Pearson Correlation Coefficient of a matrix and a vector.
*/

#include <stdio.h>
#include <stdbool.h>

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
    double mat[n*n];

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

    // ==================================================================================================
    
    /*
        creating an nxn double matrix
    */
    // double mat[n*n];



    





    

    return 0;
}

