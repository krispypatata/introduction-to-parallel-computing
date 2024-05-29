#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define MAX_SUB_MATRICES 16
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <matrix size> <port> <status (0/1)>", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int port = atoi(argv[2]);
    int status = atoi(argv[3]);

    int sockfd, client_sockfd, bytes_received;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[BUFFER_SIZE];
    char config_file[100];
    FILE *fp;
    int num_slaves, i, j, k, l, m, sub_matrix_size;
    int slave_ip[MAX_SUB_MATRICES];
    int slave_port[MAX_SUB_MATRICES];
    int slave_received[MAX_SUB_MATRICES];
    int matrix[n][n], sub_matrix[n / MAX_SUB_MATRICES][n];
    struct timespec start, end;

    // Open and read configuration file
    sprintf(config_file, "config.txt");
    fp = fopen(config_file, "r");
    if (fp == NULL)
    {
        perror("Error opening configuration file");
        return 1;
    }

    fscanf(fp, "%d\n", &num_slaves);
    if (num_slaves > MAX_SUB_MATRICES)
    {
        printf("Number of slaves exceeds maximum (%d).\n", MAX_SUB_MATRICES);
        fclose(fp);
        return 1;
    }

    for (i = 0; i < num_slaves; i++)
    {
        fscanf(fp, "%d %d\n", &slave_ip[i], &slave_port[i]);
    }
    fclose(fp);

    // Get start time
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Master process
    if (status == 0)
    {
        // Create socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            perror("Error creating socket");
            return 1;
        }

        // Bind socket to port
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = INADDR_ANY;
        if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            perror("Error binding socket");
            close(sockfd);
            return 1;
        }

        // Listen for connections
        if (listen(sockfd, 5) < 0)
        {
            perror("Error listening for connections");
            close(sockfd);
            return 1;
        }

        printf("Master listening on port %d\n", port);

        // Generate matrix
        srand(time(NULL));
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                matrix[i][j] = rand() % 100 + 1;
            }
        }

        // Divide matrix into sub-matrices
        sub_matrix_size = n / num_slaves;
        for (i = 0; i < num_slaves; i++)
        {
            // Wait for slave to connect
            printf("Waiting for slave %d to connect...\n", i);
            addr_len = sizeof(client_addr);
            client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
            if (client_sockfd < 0)
            {
                perror("Error accepting connection");
                close(sockfd);
                return 1;
            }
            printf("Slave %d connected\n", i);

            // Send sub-matrix
            for (j = 0; j < sub_matrix_size; j++)
            {
                for (k = 0; k < n; k++)
                {
                    sprintf(buffer, "%d ", matrix[i * sub_matrix_size + j][k]);
                    if (send(client_sockfd, buffer, strlen(buffer), 0) < 0)
                    {
                        perror("Error sending sub-matrix");
                        close(client_sockfd);
                        close(sockfd);
                        return 1;
                    }
                }
                sprintf(buffer, "\n");
                if (send(client_sockfd, buffer, strlen(buffer), 0) < 0)
                {
                    perror("Error sending sub-matrix");
                    close(client_sockfd);
                    close(sockfd);
                    return 1;
                }
            }

            // Receive acknowledgment
            if (recv(client_sockfd, buffer, BUFFER_SIZE, 0) < 0)
            {
                perror("Error receiving acknowledgment");
                close(client_sockfd);
                close(sockfd);
                return 1;
            }

            slave_received[i] = atoi(buffer);
            printf("Master received acknowledgment from slave %d\n", i);

            close(client_sockfd);
        }

        // Get end time
        clock_gettime(CLOCK_MONOTONIC, &end);
        // Calculate elapsed time in seconds and milliseconds
        double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Elapsed time: %.3lf seconds\n", elapsed_time);
    }

        // Slave process
    else if (status == 1)
    {
        // Connect to master
        client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (client_sockfd < 0)
        {
            perror("Error creating client socket");
            return 1;
        }

        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(port);
        client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        addr_len = sizeof(client_addr);

        if (connect(client_sockfd, (struct sockaddr *)&client_addr, addr_len) < 0)
        {
            perror("Error connecting to master");
            close(client_sockfd);
            return 1;
        }

        printf("Slave connected to master\n");

        // Receive sub-matrix size
        bytes_received = recv(client_sockfd, buffer, BUFFER_SIZE, 0);
        if (bytes_received < 0)
        {
            perror("Error receiving sub-matrix size");
            close(client_sockfd);
            return 1;
        }

        int sub_rows, sub_cols;
        sscanf(buffer, "%d %d", &sub_rows, &sub_cols);

        // Receive sub-matrix
        for (i = 0; i < sub_rows; i++)
        {
            bytes_received = recv(client_sockfd, buffer, BUFFER_SIZE, 0);
            if (bytes_received < 0)
            {
                perror("Error receiving sub-matrix");
                close(client_sockfd);
                return 1;
            }

            for (j = 0; j < sub_cols; j++)
            {
                sscanf(buffer, "%d ", &sub_matrix[i][j]);
            }
        }

        // Send acknowledgment
        sprintf(buffer, "%d", 1);
        if (send(client_sockfd, buffer, strlen(buffer), 0) < 0)
        {
            perror("Error sending acknowledgment");
            close(client_sockfd);
            return 1;
        }

        // Get end time
        clock_gettime(CLOCK_MONOTONIC, &end);
        // Calculate elapsed time in seconds and milliseconds
        double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Elapsed time: %.3lf seconds\n", elapsed_time);
    }

    close(sockfd);
    return 0;
}