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

# def pearson_cor(mat, vector):
#     m, n = mat.shape

#     # Calculate means
#     mat_mean = mat.mean(axis=1, keepdims=True)
#     vector_mean = vector.mean()

#     # Subtract means
#     mat_centered = mat - mat_mean
#     vector_centered = vector - vector_mean

#     # Calculate Pearson correlation coefficient
#     numerator = (mat_centered * vector_centered).sum(axis=1)
#     denominator = np.sqrt((mat_centered ** 2).sum(axis=1)) * np.sqrt((vector_centered ** 2).sum())

#     cor = numerator / denominator

#     # return the Pearson correlation coefficients
#     return cor


# ======================================================================================================
# testing the implementation of the created Pearson Correlation Coefficient algorithm
def test_algorithm():
    # test case (provided in the lab handout)
    weights = np.array([3.63, 3.02, 3.82, 3.42, 3.59, 2.87, 3.03, 3.46, 3.36, 3.3])
    lengths = np.array([53.1, 49.7, 48.4, 54.2, 54.9, 43.7, 47.2, 45.2, 54.4, 50.4])

    # creating a 10x10 matrix
    n = 10
    mat = np.zeros((n, n))

    for row in range(n):
        mat[row, :] = weights

    # for checking (printing)
    print_matrix(mat, 4, n, 2)
    pearson_cor(mat[:4], lengths)

    # compute for the Pearson Correlation Coefficient of the matrix and the given vector
    x = np.random.randint(1, 256, size=(4, n), dtype=np.uint8)
    y = np.random.randint(1, 256, size=(n), dtype=np.uint8)
    print(x)
    print(y)
    print(pearson_cor(x, y))

    # z = np.random.random([12500, 25000])
    # w = np.random.random(25000)
    # print(z)
    # print(w)
    # pearson_cor2(z, w)
    

    
    # print()
    # print(y)

    # only use the first 4 rows of the matrix
    # print(pearson_cor(mat[:4], lengths))  
    # pearson_cor(mat[:4], lengths) 
    
    m = 12500
    n = 25000
    a = np.random.randint(1, 256, size=(m, n), dtype=np.uint8)
    b = np.random.randint(1, 256, size=(n), dtype=np.uint8)
    print(a.shape)
    print(b.shape)
    print(pearson_cor(a, b))



# ======================================================================================================
# Start of the main program
if __name__ == "__main__":
    # testing the algorithm with the provided test cases
    test_algorithm()
