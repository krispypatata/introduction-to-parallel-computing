#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define MAX_SUB_MATRICES 16
#define BUFFER_SIZE 1024

int num_slaves;
int slave_ip[MAX_SUB_MATRICES];
int slave_port[MAX_SUB_MATRICES];
int slave_connected[MAX_SUB_MATRICES];
pthread_mutex_t mutex;

int n, port;
int** matrix;
int** sub_matrices;
int sub_matrix_size;

void* handle_slave_connection(void* arg)
{
    int slave_index = *((int*)arg);
    int sockfd, bytes_received;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
    char buffer[BUFFER_SIZE];

    // Connect to slave
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        pthread_exit(NULL);
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(slave_port[slave_index]);
    client_addr.sin_addr.s_addr = inet_addr(slave_ip);
    addr_len = sizeof(client_addr);

    if (connect(sockfd, (struct sockaddr *)&client_addr, addr_len) < 0)
    {
        perror("Error connecting to slave");
        close(sockfd);
        pthread_exit(NULL);
    }

    printf("Connected to slave %d\n", slave_index);

    // Send sub-matrix
    for (int i = 0; i < sub_matrix_size; i++)
    {
        for (int j = 0; j < n; j++)
        {
            sprintf(buffer, "%d ", matrix[slave_index * sub_matrix_size + i][j]);
            if (send(sockfd, buffer, strlen(buffer), 0) < 0)
            {
                perror("Error sending sub-matrix");
                close(sockfd);
                pthread_exit(NULL);
            }
        }
        sprintf(buffer, "\n");
        if (send(sockfd, buffer, strlen(buffer), 0) < 0)
        {
            perror("Error sending sub-matrix");
            close(sockfd);
            pthread_exit(NULL);
        }
    }

    // Receive acknowledgment
    if (recv(sockfd, buffer, BUFFER_SIZE, 0) < 0)
    {
        perror("Error receiving acknowledgment");
        close(sockfd);
        pthread_exit(NULL);
    }

    int ack = atoi(buffer);
    printf("Received acknowledgment from slave %d\n", slave_index);

    close(sockfd);

    // Update slave connection status
    pthread_mutex_lock(&mutex);
    slave_connected[slave_index] = 1;
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <matrix size> <port> <status (0/1)>", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    port = atoi(argv[2]);
    int status = atoi(argv[3]);

    int sockfd, client_sockfd, bytes_received;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[BUFFER_SIZE];
    char config_file[100];
    FILE *fp;
    int i, j, k, l, m;

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

    // Initialize matrix and sub-matrices
    matrix = malloc(n * n * sizeof(int*));
    for (i = 0; i < n * n; i++)
    {
        matrix[i] = malloc(sizeof(int));
    }

    sub_matrices = malloc(num_slaves * sub_matrix_size * sizeof(int*));
    for (i = 0; i < num_slaves * sub_matrix_size; i++)
    {
        sub_matrices[i] = malloc(n * sizeof(int));
    }

    sub_matrix_size = n / num_slaves;

    // Initialize slave connection status
    for (i = 0; i < num_slaves; i++)
    {
        slave_connected[i] = 0;
    }

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Get start time
    struct timespec start, end;
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
                matrix[i * n + j] = rand() % 100 + 1;
            }
        }

        // Divide matrix into sub-matrices
        for (i = 0; i < num_slaves; i++)
        {
            for (j = 0; j < sub_matrix_size; j++)
            {
                for (k = 0; k < n; k++)
                {
                    sub_matrices[i * sub_matrix_size + j][k] = matrix[i * sub_matrix_size + j][k];
                }
            }
        }

        // Connect to slaves
        pthread_t slave_threads[num_slaves];
        int slave_index;
        for (i = 0; i < num_slaves; i++)
        {
            slave_index = i;
            pthread_create(&slave_threads[i], NULL, handle_slave_connection, &slave_index);
        }

        // Wait for all slaves to connect
        while (1)
        {
            int all_connected = 1;
            for (i = 0; i < num_slaves; i++)
            {
                pthread_mutex_lock(&mutex);
                if (!slave_connected[i])
                {
                    all_connected = 0;
                    break;
                }
                pthread_mutex_unlock(&mutex);
            }

            if (all_connected)
            {
                break;
            }
        }

        // Get end time
        clock_gettime(CLOCK_MONOTONIC, &end);
        // Calculate elapsed time in seconds and milliseconds
        double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("All slaves connected. Elapsed time: %.3lf seconds\n", elapsed_time);

        // Free resources
        for (i = 0; i < n * n; i++)
        {
            free(matrix[i]);
        }
        free(matrix);

        for (i = 0; i < num_slaves * sub_matrix_size; i++)
        {
            free(sub_matrices[i]);
        }
        free(sub_matrices);

        pthread_mutex_destroy(&mutex);

        printf("Master process ended.\n");
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
        int* sub_matrix = malloc(sub_rows * sub_cols * sizeof(int));
        for (i = 0; i < sub_rows; i++)
        {
            bytes_received = recv(client_sockfd, buffer, BUFFER_SIZE, 0);
            if (bytes_received < 0)
            {
                perror("Error receiving sub-matrix");
                close(client_sockfd);
                free(sub_matrix);
                return 1;
            }

            for (j = 0; j < sub_cols; j++)
            {
                sscanf(buffer, "%d ", &sub_matrix[i * sub_cols + j]);
            }
        }

        // Send acknowledgment
        sprintf(buffer, "%d", 1);
        if (send(client_sockfd, buffer, strlen(buffer), 0) < 0)
        {
            perror("Error sending acknowledgment");
            close(client_sockfd);
            free(sub_matrix);
            return 1;
        }

        // Get end time
        clock_gettime(CLOCK_MONOTONIC, &end);
        // Calculate elapsed time in seconds and milliseconds
        double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Slave process ended. Elapsed time: %.3lf seconds\n", elapsed_time);

        free(sub_matrix);
    }

    close(sockfd);
    return 0;
}