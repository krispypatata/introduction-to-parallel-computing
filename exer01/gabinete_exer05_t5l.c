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

    
    /*
        creating an nxn matrix
    */

    return 0;
}

