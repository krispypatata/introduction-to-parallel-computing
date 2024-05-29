import numpy as np
import socket
import struct
import sys
import time


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
# function to handle logic for the master node
def handleMasterLogic(n, p, t, slavesInfo):
    # generate an nxn square matrix populated with random positive integers
    M = generateNxNMatrix(n)
    print("Generated matrix:")
    printMatrixTruncated(M)

    # divide the matrix into t submatrices
    submatrices = divideMatrixIntoSubmatrices(M, t)

    totalTime = 0
    try:
        for i, slaveInfo in enumerate(slavesInfo):
            conn = None
            try:
                conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                conn.connect(slaveInfo)
                print()
                print("="*80)
                print('Connected to', slaveInfo)
                startTime = time.time()  # Record the start time after establishing connection
                data = submatrices[i].tobytes()

                sendMessage(conn, data)
                print(f'Sent {len(data)} bytes to', slaveInfo)

                # Receive "ack" from slave
                received = receiveMessage(conn)
                print(f"Received '{received.decode()}' from {slaveInfo}")

                endTime = time.time()
                elapsedTime = endTime - startTime
                totalTime += elapsedTime
                print(f"Time taken to send submatrix to {slaveInfo}: {elapsedTime} seconds")

            except Exception as e:
                print(f"An error occurred while communicating with {slaveInfo}: {e}")

            finally:
                if conn:
                    print("Closing connection with", slaveInfo)
                    conn.close()



    except Exception as e:
        print("An error occurred in master logic:", e)
    
    print()
    print("="*80)
    print("Total elapsed taken:", totalTime, "seconds")

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
    # configFile = "config2.in"        # 2 slaves
    configFile = "config16.in"     # 16 slaves
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
