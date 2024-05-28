#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main()
{
    int sockfd, numReceived, bytesRead;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serverAddr;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket creation failed");
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("connect failed");
        close(sockfd);
        return 1;
    }

    // print server information
    printf("Connected to server.\n");
    printf("Server information: %s:%d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

    // print current client information
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    getsockname(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    printf("Client information: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    // ======================================================================
    // TODO: Receive and print the submatrix passed by the server right after the connection
    int nRows, nCols;  // number of rows and columns in the submatrix
    int **receivedSubmatrix;

    // Receive the number of rows and columns
    recv(sockfd, &nRows, sizeof(int), 0);
    recv(sockfd, &nCols, sizeof(int), 0);

    // Allocate memory for the received submatrix
    receivedSubmatrix = (int **)malloc(nRows * sizeof(int *));
    for (int i = 0; i < nRows; i++)
    {
        receivedSubmatrix[i] = (int *)malloc(nCols * sizeof(int));
    }

    // Receive the submatrix data
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            recv(sockfd, &receivedSubmatrix[i][j], sizeof(int), 0);
        }
    }

    // Print the received submatrix
    printf("Received Submatrix:\n");
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            printf("%d ", receivedSubmatrix[i][j]);
        }
        printf("\n");
    }

    // Free the allocated memory for the received submatrix
    for (int i = 0; i < nRows; i++)
    {
        free(receivedSubmatrix[i]);
    }
    free(receivedSubmatrix);

    // ======================================================================
    close(sockfd);
    return 0;
}