import numpy as np
import socket
import sys

# =============================================================================
# function to verify the size of a numpy matrix
def printMatrixSize(mat):
    print("Matrix size: ", mat.shape)

# function to print the elements of an mxn matrix
def printMatrixAllElements(mat, nRow, nCol):
    # print the size of the matrix
    for row in range(nRow):
        for col in range(nCol):
            print(mat[row][col], end = " ")
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

    return submatrices

# function to generate an nxn square matrix with random positive integers
def generateNxNMatrix(n):
    # define the maximum integer value for np.int32
    maxInt = np.iinfo(np.int32).max
    
    # generate the nxn square matrix with random positive integer values
    matrix = np.random.randint(1, maxInt, size=(n, n), dtype=np.int32)
    
    # return the generated matrix
    return matrix
# =============================================================================
# function to read the configuration file (config.in)
# The configuration file should have the following format:
# <master_ip>
# <number_of_slaves>
# <slave1_ip> <slave1_port>
# <slave2_ip> <slave2_port>
# ...
def readConfig(filename):
    try:
        with open(filename, "r") as file:
            lines = file.readlines()
            masterIP = lines[0].strip()
            numSlaves = int(lines[1])
            slavesInfo = [(line.split()[0], int(line.split()[1])) for line in lines[2:]] # list of tuples (ip, port)
        return masterIP, numSlaves, slavesInfo
    except Exception as e:
        print("An error occurred while reading the configuration file:", e)
        sys.exit(1)

# function to read arguments from the command line
def readArguments():
    try:
        if len(sys.argv) != 4:
            raise ValueError("Usage: python lab04.py <square_matrix_size> <port> <status>")
        
        n = sys.argv[1]         # size of the square matrix
        p = int(sys.argv[2])    # port number
        s = int(sys.argv[3])    # status (0 for master, 1 for slave)

        return n, p, s
    except Exception as e:
        print("An error occurred while parsing command-line arguments:", e)
        sys.exit(1)

# function to handle logic for the master node
def handleMasterLogic(n, p, masterIP, t, slavesInfo):
    # generate an nxn square matrix populated with random positive integers
    M = generateNxNMatrix(n)

    # divide the matrix into t submatrices
    submatrices = divideMatrixIntoSubmatrices(M, t)

    # Connect to each slave and send the submatrix
    for slaveIP, slavePort in slavesInfo:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((slaveIP, slavePort))
        s.sendall(submatrices.pop(0).tobytes())
        print("Sent submatrix to", slaveIP, ":", slavePort)
        ack = s.recv(1024)
        if ack.decode() == "ack":
            print("Received acknowledgment from", slaveIP, ":", slavePort)
        else:
            print("Failed to receive acknowledgment from", slaveIP, ":", slavePort)
        s.close()

        # Calculate the elapsed time
        # time_elapsed = time.time() - time_before
        # print(f"Elapsed time: {time_elapsed:.2f} seconds")

# function to handle logic for the slave node
def handleSlaveLogic(n, t, masterIP, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((masterIP, port))  # Bind to the specified port
    s.listen()
    conn, addr = s.accept()
    print('Connected by', addr)
    data = conn.recv(1024)
    conn.sendall(b'ack')
    submatrix = np.frombuffer(data, dtype=np.int32).reshape((n // t, n))
    print("Received submatrix:")
    printMatrixTruncated(submatrix)
    s.close()

# =============================================================================
# start of the main program
def main():
    # read the arguments from the command line
    n, p, s = readArguments()

    # read the configuration file
    configFile = "config.in"
    masterIP, numSlaves, slavesInfo = readConfig(configFile)
    t = numSlaves
    print(masterIP, numSlaves, slavesInfo)

    if s == 0:
        # master
        handleMasterLogic(n, p, masterIP, t, slavesInfo)
        print("Master")
    elif s == 1:
        # slave
        handleSlaveLogic()
        print("Slave")

    else:
        print("Invalid status. Please enter 0 for master or 1 for slave.")
        sys.exit(1)

if __name__ == "__main__":
    main()

