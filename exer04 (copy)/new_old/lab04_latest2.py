import numpy as np
import socket
import sys

# =============================================================================
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
        
        n = int(sys.argv[1])  # size of the square matrix
        p = int(sys.argv[2])  # port number
        s = int(sys.argv[3])  # status (0 for master, 1 for slave)

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

    masterSocket = None
    try:
        masterSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        masterSocket.bind(('', p))
        masterSocket.listen(t)
        print("Waiting for connections...")

        for i in range(t):
            conn = None
            try:
                conn, addr = masterSocket.accept()
                print('Connected to', addr)
                data = submatrices[i].tobytes()
                # Send data in chunks
                bytes_sent = 0
                while bytes_sent < len(data):
                    chunk = data[bytes_sent:bytes_sent + 16777216]
                    conn.sendall(chunk)
                    bytes_sent += len(chunk)
                    print(bytes_sent, 'bytes sent out of', len(data))

                print('Sent submatrix to', addr)

            except Exception as e:
                print(f"An error occurred while communicating with {addr}: {e}")

            finally:
                if conn:
                    conn.close()
    except Exception as e:
        print("An error occurred in master logic:", e)
    finally:
        if masterSocket:
            masterSocket.close()

# function to handle logic for the slave node
def handleSlaveLogic(n, t, masterIP, masterPort, port):
    slaveSocket = None
    try:
        slaveSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        slaveSocket.bind(('', port))
        slaveSocket.connect((masterIP, masterPort))
        print('Connected to master')

        data = b''
        while True:
            chunk = slaveSocket.recv(16777216)
            if not chunk:
                break
            data += chunk

        submatrix = np.frombuffer(data, dtype=np.uint8).reshape((-1, n))
        
        print("Received submatrix:")
        printMatrixTruncated(submatrix)

    except Exception as e:
        print("An error occurred in slave logic:", e)
    finally:
        if slaveSocket:
            slaveSocket.close()

# =============================================================================
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
