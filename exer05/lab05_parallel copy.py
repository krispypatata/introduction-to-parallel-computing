import numpy as np
import socket
import struct
import sys
import threading
import time


# ======================================================================================================
# a function for calculating the Pearson Correlation Coefficient vector of an mxn square matrix X and an nx1 vector y
def pearson_cor(mat, vector):
    # for code simplicity/clarity
    m = mat.shape[0]
    n = mat.shape[1]

    # get the Pearson correlation coefficient of the matrix and the vector, use numpy's built-in function (corrcoef)
    cor = np.zeros(m)

    for i in range(m):
        # get the correlation coefficient matrix of the row and the vector
        corr_matrix = np.corrcoef(mat[i], vector)[0,1]
        # extract the correlation coefficient
        cor[i] = corr_matrix

    # return the Pearson correlation coefficients
    return cor


# ==================================================================================================
# function to verify the size of a numpy matrix
def printMatrixSize(mat):
    print("Matrix size:", mat.shape)

# function to print the elements of an mxn matrix
def printMatrixAllElements(mat, nRow, nCol):
    for row in range(nRow):
        for col in range(nCol):
            print(mat[row][col], end=" ")
        print()

# function to print a matrix (truncated) with its size
def printMatrixTruncated(mat):
    printMatrixSize(mat)
    print(mat)

# function to divide an nxn matrix into t submatrices
def divideMatrixIntoSubmatrices(matrix, t):
    n = matrix.shape[0]
    submatrices = []
    rowsPerSubmatrix = n // t
    extraRows = n % t

    # Divide the matrix into submatrices
    startRow = 0
    for i in range(t):
        # Calculate the number of rows for this submatrix
        numRows = rowsPerSubmatrix
        if i == t - 1:
            numRows += extraRows

        # Extract the submatrix
        submatrix = matrix[startRow:startRow+numRows, :]
        submatrices.append(submatrix)
        
        # Update the starting row for the next submatrix
        startRow += numRows

    print("Submatrices generated")
    return submatrices

# function to generate an nxn square matrix with random positive integers between 1 and 255
def generateNxNMatrix(n):
    # generate the nxn square matrix with random positive integer values between 1 and 255
    matrix = np.random.randint(1, 256, size=(n, n), dtype=np.uint8)
    return matrix


# ==================================================================================================
# function to read the configuration file (config.in)
# The configuration file should have the following format:
# <master_ip> <master_port>
# <number_of_slaves>
# <slave1_ip> <slave1_port>
# <slave2_ip> <slave2_port>
# ...
def readConfig(filename):
    try:
        with open(filename, "r") as file:
            lines = file.readlines()
            masterIP = lines[0].strip().split()[0]
            masterPort = int(lines[0].strip().split()[1])
            numSlaves = int(lines[1])
            slavesInfo = [(line.split()[0], int(line.split()[1])) for line in lines[2:]] # list of tuples (ip, port)
        return masterIP, masterPort, numSlaves, slavesInfo
    except Exception as e:
        print("An error occurred while reading the configuration file:", e)
        sys.exit(1)

# function to read arguments from the command line
# The command line should have the following format:
# python lab04.py <square_matrix_size> <port> <status>
def readArguments():
    try:
        if len(sys.argv) != 4:
            raise ValueError("Usage: python lab04.py <square_matrix_size> <port> <status>")
        
        n = int(sys.argv[1])  # size of the square matrix
        p = int(sys.argv[2])  # port number
        s = int(sys.argv[3])  # status (0 for master, 1 for slave)

        return n, p, s
    except Exception as e:
        print("An error occurred while parsing command-line arguments:", e)
        sys.exit(1)


# ==================================================================================================
# ref: https://stackoverflow.com/questions/17667903/python-socket-receive-large-amount-of-data
def sendMessage(sock, msg):
    # Prefix each message with a 4-byte length (network byte order)
    msg = struct.pack('>I', len(msg)) + msg
    sock.sendall(msg)

def receiveMessage(sock):
    # Read message length and unpack it into an integer
    raw_msglen = receiveAllMessage(sock, 4)
    if not raw_msglen:
        return None
    msglen = struct.unpack('>I', raw_msglen)[0]
    # Read the message data
    return receiveAllMessage(sock, msglen)

def receiveAllMessage(sock, n):
    # Helper function to recv n bytes or return None if EOF is hit
    data = bytearray()
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            return None
        data.extend(packet)
    return data


# ==================================================================================================
# ==================================================================================================
# function to handle logic for the master node
def handleSlaveConnection(slaveInfo, submatrix, vector, results):
    conn = None
    try:
        conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        conn.connect(slaveInfo)
        print(f'Connected to {slaveInfo}')

        # Send the submatrix to the slave
        data = submatrix.tobytes()
        sendMessage(conn, data)
        print(f'Sent {len(data)} bytes to {slaveInfo}')

        # Send the vector to the slave
        data = vector.tobytes()
        sendMessage(conn, data)
        print(f'Sent vector Y to {slaveInfo}')

        # Receive computed Pearson Correlation Coefficient vector from slave
        data = receiveMessage(conn)
        correlationVector = np.frombuffer(data, dtype=np.float64)
        print(f"Received Pearson Correlation Coefficient vector from {slaveInfo}")

        # Receive "ack" from slave
        received = receiveMessage(conn)
        print(f"Received '{received.decode()}' from {slaveInfo}")

        results[slaveInfo] = "Success"
    except Exception as e:
        print(f"An error occurred while communicating with {slaveInfo}: {e}")
        results[slaveInfo] = f"Failed: {e}"
    finally:
        if conn:
            print(f"Closing connection with {slaveInfo}")
            conn.close()

def handleMasterLogic(n, p, t, slavesInfo):
    # generate an nxn square matrix populated with random positive integers
    M = generateNxNMatrix(n)
    print("Generated matrix:")
    printMatrixTruncated(M)

    # divide the matrix into t submatrices
    submatrices = divideMatrixIntoSubmatrices(M, t)

    # generate a random vector of size n
    vectorY = np.random.randint(1, 256, size=(n), dtype=np.uint8)

    

    threads = []
    results = {}

    for i, slaveInfo in enumerate(slavesInfo):
        submatrix = submatrices[i]
        thread = threading.Thread(target=handleSlaveConnection, args=(slaveInfo, submatrix, vectorY, results))
        threads.append(thread)

    startTime = time.time()  # Record the start time

    for thread in threads:
        thread.start()

    for thread in threads:
        thread.join()

    endTime = time.time()
    elapsedTime = endTime - startTime

    print()
    print("="*80)
    print("Elapsed Time:", elapsedTime, "seconds")
    print("Results:", results)

# function to handle logic for the slave node
def handleSlaveLogic(n, t, masterIP, masterPort, port):
    slaveSocket = None
    try:
        slaveSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        slaveSocket.bind(('', port))
        slaveSocket.listen(1)  # Listen for connections from master
        print("Waiting for connection from master...")

        conn, addr = slaveSocket.accept()
        print('Connected to master')

        # measure the elapsed time of the slave process
        startTime = time.time()

        # receive the submatrix from the master
        data = receiveMessage(conn)

        # process the received data
        submatrix = np.frombuffer(data, dtype=np.uint8).reshape((-1, n))
        
        # print the processed submatrix
        print("Received submatrix:")
        printMatrixTruncated(submatrix)

        # receive the vector from the master
        data = receiveMessage(conn)
        vectorY = np.frombuffer(data, dtype=np.uint8)
        print("Received vector Y:")
        print(vectorY)

        # compute the Pearson correlation coefficient of the submatrix and the vector
        correlationVector = pearson_cor(submatrix, vectorY)
        print("Pearson correlation coefficient vector:")
        print(correlationVector)

        # send the correlation vector to the master
        data = correlationVector.tobytes()
        sendMessage(conn, data)
        print(f"Sent computed Pearson Correlation Coefficient vector to master")

        # Send "ack" to master
        print("Sending 'ack' to master")
        sendMessage(conn, b'ack')

        endTime = time.time()
        elapsedTime = endTime - startTime
        print(f"Elapsed time: {elapsedTime} seconds")

        # # Handle subsequent communication with master
        # while True:
        #     # For example, if you need to send more data to the master, do it here
        #     pass

    except Exception as e:
        print("An error occurred in slave logic:", e)
    finally:
        if slaveSocket:
            slaveSocket.close()

# ==================================================================================================
# start of the main program
def main():
    # read the arguments from the command line
    matrixSize, port, status = readArguments()

    # read the configuration file
    configFile = "config2.in"        # 2 slaves
    # configFile = "config16.in"     # 16 slaves
    masterIP, masterPort, numSlaves, slavesInfo = readConfig(configFile)
    # print(masterIP, masterPort, numSlaves, slavesInfo)

    if status == 0:
        # master
        handleMasterLogic(matrixSize, port, numSlaves, slavesInfo)
        print("Master")
    elif status == 1:
        # slave
        handleSlaveLogic(matrixSize, numSlaves, masterIP, masterPort, port)
        print("Slave")
    else:
        print("Invalid status. Please enter 0 for master or 1 for slave.")
        sys.exit(1)

if __name__ == "__main__":
    main()
