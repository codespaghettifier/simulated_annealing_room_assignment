import numpy as numpy

num_people = 10
path = 'MatrixCost.dat'


matrix = np.triu(np.random.randint(1, 100, (num_people, num_people)), k=1)
matrix = matrix + matrix.T
np.fill_diagonal(matrix, 0)

for row in matrix:
    print(row)

with open('MatrixCost.dat', 'w') as f:
    f.write(str(num_people) + '\n')
    for row in matrix:
        f.write(' '.join(map(str, row)) + '\n')