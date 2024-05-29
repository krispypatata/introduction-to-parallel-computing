import numpy as np

# ======================================================================================================
# a function for printing a square matrix that is made with 1d array
def print_matrix(mat, m, n, n_decimal):
    # total field width including decimal point and sign
    field_width = 8
    
    # number of digits after the decimal point
    precision = n_decimal
    
    # traversing through the elements of the matrix
    for row in range(m):
        for col in range(n):
            print(f"{mat[row][col]:{field_width}.{precision}f}", end=" ")
        print()

# ======================================================================================================
# a function for printing a vector
def print_vector(vector, n, n_decimal):
    # total field width including decimal point and sign
    field_width = 5
    
    # number of digits after the decimal point
    precision = n_decimal
    
    # traverse through the elements of the vector
    print("[ ", end="")
    for i in range(n):
        print(f"{vector[i]:{field_width}.{precision}f}", end=" ")
        
        # adding commas
        if i < n - 1:
            print(", ", end="")
    print("]")

# ======================================================================================================
# a function to compute the Pearson Correlation Coefficient vector of an nxn square matrix X 
# with a nx1 vector y

def pearson_cor(mat, vector):
    # for code simplicity/clarity
    X = mat.astype(float)
    y = vector.astype(float)
    m = mat.shape[0]
    n = mat.shape[1]

    # vector for the Pearson Correlation coefficients
    v = np.zeros(n)

    # ==================================================================================================
    # Step 1: Calculate the sums of x and y
    # ==================================================================================================
    sum_X = np.zeros(n)
    sum_y = 0

    # traverse through the columns of the matrix
    for j in range(n):
        sumCol = 0
        for i in range(m):
            sumCol += X[i][j]
        
        # add the sum of the column to the sum_X vector
        sum_X[j] = sumCol

    # traverse through the elements of the vector
    for i in range(m):
        sum_y += y[i]

    # ==================================================================================================
    # Step 2: Calculate x2 and y2 and their sums
    # ==================================================================================================
    sum_X_squared = np.zeros(n)
    sum_y_squared = 0

    # traverse through the columns of the matrix, square all elements and get their sum
    for j in range(n):
        sumCol = 0
        for i in range(m):
            sumCol += X[i][j] * X[i][j]
        
        # add the sum of the column to the sum_X_squared vector
        sum_X_squared[j] = sumCol

    # traverse through the elements of the vector, square all elements and get their sum
    for i in range(m):
        sum_y_squared += y[i] * y[i]

    # ==================================================================================================
    # Step 3: Calculate the cross product and its sum
    # ==================================================================================================
    sum_cross_product = np.zeros(n)

    # traverse through the columns of matrix x, compute for the cross product of each column and vector y
    for j in range(n):
        sum_col_cross_product = 0
        for i in range(m):
            sum_col_cross_product += X[i][j] * y[i]
        
        # add the sum of the column to the sum_cross_product vector
        sum_cross_product[j] = sum_col_cross_product

    # ==================================================================================================
    # Step 4: Calculate r
    # ==================================================================================================
    for i in range(n):
        r = 0
        r = (m * sum_cross_product[i] - sum_X[i] * sum_y) / np.sqrt( (m * sum_X_squared[i] - sum_X[i]*sum_X[i]) * (m * sum_y_squared - sum_y*sum_y) )
        v[i] = r

    # --------------------------------------------------------------------------------------------------
    # for checking
    print("\n\nPEARSON CORRELATION COEFFICIENTS:")
    print_vector(v, n, 4)
    # --------------------------------------------------------------------------------------------------

    return v

# ======================================================================================================
# testing the implementation of the created Pearson Correlation Coefficient algorithm
def test_algorithm():
    # test case (provided in the lab handout)
    weights = np.array([3.63, 3.02, 3.82, 3.42, 3.59, 2.87, 3.03, 3.46, 3.36, 3.3])
    lengths = np.array([53.1, 49.7, 48.4, 54.2, 54.9, 43.7, 47.2, 45.2, 54.4, 50.4])

    # creating a 10x10 matrix
    n = 25000
    # mat = np.zeros((n, n))

    # for col in range(n):
    #     mat[:, col] = weights

    # for checking (printing)
    # print_matrix(mat, 4, n, 2)

    # compute for the Pearson Correlation Coefficient of the matrix and the given vector
    y = np.random.randint(1, 256, size=(n), dtype=np.uint8)
    x = np.random.randint(1, 256, size=(n, n), dtype=np.uint8)

    # only use the first 4 rows of the matrix
    pearson_cor(x, y)    

# ======================================================================================================
# Start of the main program
if __name__ == "__main__":
    # testing the algorithm with the provided test cases
    test_algorithm()
