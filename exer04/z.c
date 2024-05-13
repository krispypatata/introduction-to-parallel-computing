// Example code: A simple server side code, which echos back the received message.
// Handle multiple socket connections with select and fd_set on Linux
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>	   //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <limits.h>
#include <time.h>

#define EXIT_MESSAGE "exit"
#define TRUE 1
#define FALSE 0
#define PORT 1234
#define MAX 80

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
// endregion

int main(int argc, char *argv[])
{
	int opt = TRUE;
	int master_socket, addrlen, new_socket, client_socket[30],
		max_clients = 30, activity, i, valread, sd;
	int max_sd;
	struct sockaddr_in address;

	char buffer[1025]; // data buffer of 1K

	// set of socket descriptors
	fd_set readfds;

	// a message
	char *message = "ECHO Daemon v1.0 \r\n";

	// initialise all client_socket[] to 0 so not checked
	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}

	// create a master socket
	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// set master socket to allow multiple connections ,
	// this is just a good habit, it will work without this
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
				   sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	// type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// bind the socket to localhost port 8888
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", PORT);

	// try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// accept the incoming connection
	addrlen = sizeof(address);
	puts("Waiting for connections ...");

	int n = 3; // Size of the square matrix
	// Create a non-zero nxn square matrix M with random non-zero positive integers
	int **M = createNonZeroSquareMatrix(n);
	// print the matrix
	printMatrix(M, n, n);

	// Divide the matrix into three submatrices
	int t = 3; // number of clients
	int ***submatrices = divideSquareMatrix(n, t, M);

	while (TRUE)
	{
		// clear the socket set
		FD_ZERO(&readfds);

		// add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		// add child sockets to set
		for (i = 0; i < max_clients; i++)
		{
			// socket descriptor
			sd = client_socket[i];

			// if valid socket descriptor then add to read list
			if (sd > 0)
				FD_SET(sd, &readfds);

			// highest file descriptor number, need it for the select function
			if (sd > max_sd)
				max_sd = sd;
		}

		// wait for an activity on one of the sockets , timeout is NULL ,
		// so wait indefinitely
		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		if ((activity < 0) && (errno != EINTR))
		{
			printf("select error");
		}

		// If something happened on the master socket ,
		// then its an incoming connection
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket,
									 (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}

			// inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

			// send new connection greeting message
			if (send(new_socket, message, strlen(message), 0) != strlen(message))
			{
				perror("send");
			}

			puts("Welcome message sent successfully");

			if ((new_socket = accept(master_socket,
									 (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}

			// Get the client index
			int client_index = -1;
			for (int i = 0; i < t; i++)
			{
				if (client_socket[i] == 0)
				{
					client_index = i;
					break;
				}
			}

			if (client_index != -1)
			{
				// TODO: Send a submatrix to the client, also pass the number of rows and columns in the submatrix
				// Send a submatrix to the client
				int submatrixIndex = client_index;
				int nRows = (submatrixIndex == t - 1) ? (n / t + n % t) : (n / t);
				int nCols = n;

				// Send the number of rows and columns
				send(new_socket, &nRows, sizeof(int), 0);
				send(new_socket, &nCols, sizeof(int), 0);

				// Send the submatrix data
				for (int i = 0; i < nRows; i++)
				{
					for (int j = 0; j < nCols; j++)
					{
						send(new_socket, &submatrices[submatrixIndex][i][j], sizeof(int), 0);
					}
				}

				puts("Submatrix sent successfully to the client");

				// Add new socket to the list of sockets
				client_socket[client_index] = new_socket;
				printf("Adding to list of sockets as %d\n", client_index);
			}
			else
			{
				printf("All client slots are occupied. Rejecting new connection.\n");
				close(new_socket);
			}
		}

		// else its some IO operation on some other socket
		for (i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];

			if (FD_ISSET(sd, &readfds))
			{
				// Check if it was for closing , and also read the
				// incoming message
				//  if ((valread = read( sd , buffer, 1024)) == 0 || strncmp(buffer, EXIT_MESSAGE, strlen(EXIT_MESSAGE)) == 0)
				if ((valread = read(sd, buffer, 1024)) == 0)
				{
					// Somebody disconnected , get his details and print
					getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
					printf("Host disconnected , ip %s , port %d \n",
						   inet_ntoa(address.sin_addr), ntohs(address.sin_port));

					// Close the socket and mark as 0 in list for reuse
					close(sd);
					client_socket[i] = 0;
				}

				// Echo back the message that came in
				else
				{
					// set the string terminating NULL byte on the end
					// of the data read
					buffer[valread] = '\0';
					send(sd, buffer, strlen(buffer), 0);
				}
			}
		}
	}

	// Free matrix memory
	freeSquareMatrix(n, M);
	// Free submatrices memory
	freeSubMatrices(t, n, submatrices);

	return 0;
}
