import numpy as np

# Generate sample data (replace this with your actual data)
m = 2000  # number of rows
n = 25000  # number of columns
matrix = np.random.rand(m, n)  # replace this with your matrix
vector = np.random.rand(n, 1)   # replace this with your vector

# Compute column-wise means of the matrix and the vector
matrix_mean = np.mean(matrix, axis=0, keepdims=True)
vector_mean = np.mean(vector)

# Center the data
matrix_centered = matrix - matrix_mean
vector_centered = vector - vector_mean

# Reshape the vector to match the matrix shape
matrix_centered = matrix_centered.T
vector_centered = vector_centered.reshape(-1, 1)

# Compute the numerator of the Pearson correlation coefficient
numerator = np.dot(matrix_centered.T, vector_centered)

# Compute the denominator
denominator = np.sqrt(np.sum(matrix_centered ** 2, axis=0, keepdims=True) * np.sum(vector_centered ** 2))

# Compute the Pearson correlation coefficient
pearson_coefficient = numerator / denominator

print("Pearson correlation coefficient:", pearson_coefficient)
