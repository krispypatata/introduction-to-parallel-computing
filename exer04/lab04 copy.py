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
        
        n = int(sys.argv[1])         # size of the square matrix
        p = int(sys.argv[2])    # port number
        s = int(sys.argv[3])    # status (0 for master, 1 for slave)

        return n, p, s
    except Exception as e:
        print("An error occurred while parsing command-line arguments:", e)
        sys.exit(1)

# function to handle logic for the master node
def handleMasterLogic(n, p, t, slavesInfo):
    # generate an nxn square matrix populated with random positive integers
    M = generateNxNMatrix(n)
    print("Generated matrix:")
    printMatrixTruncated(M)

    # divide the matrix into t submatrices
    submatrices = divideMatrixIntoSubmatrices(M, t)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(('', p))
        s.listen(t)
        print("Waiting for connections...")

        for i in range(t):
            conn, addr = s.accept()
            with conn:
                print('Connected to', addr)
                data = submatrices[i].tobytes()
                conn.sendall(data)
                print('Sent submatrix to', addr)



# function to handle logic for the slave node
def handleSlaveLogic(n, t, masterIP, masterPort, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(('', port))
        s.connect((masterIP, masterPort))
        print('Connected to master')

        data = b''
        while True:
            chunk = s.recv(4096)
            if not chunk:
                break
            data += chunk

        submatrix = np.frombuffer(data, dtype=np.int32).reshape((n, -1))
        print("Received submatrix:")
        printMatrixTruncated(submatrix)




# =============================================================================
# start of the main program
def main():
    # read the arguments from the command line
    matrixSize, port, status = readArguments()

    # read the configuration file
    configFile = "config.in"
    masterIP, masterPort, numSlaves, slavesInfo = readConfig(configFile)
    print(masterIP, masterPort, numSlaves, slavesInfo)

    # start a socket
    # networkSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # networkSocket.bind(('', port))
    # print("Socket bound to port", port)
    # # get host ip address
    # print(socket.gethostbyname(socket.gethostname()))

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

