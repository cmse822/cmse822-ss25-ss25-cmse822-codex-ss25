We are required to
1. give an example of a doubly-nested loop where the loops can be exchanged;
   ```python
   import numpy as np

m, n = 4 , 3

A = np.zeros((m,n))

for i in range(m):
    for j in range(n):
        A[i][j] += i*j

print(A)

#Exchanging the loops
A = np.zeros((m,n))

for j in range(n):
    for i in range(m):
        A[i][j] += i*j

print(A)
  
#In this loop, we notice that the loops are exchangeable because whether we begin with the rows or columns first, the solution remains the same.   


3. give an example where this can not be exchanged.
   ```python
   import numpy as np
  
m, n = 4, 3

A = np.zeros((m,n))
for i in range(n):
    for j in range(i):
        A[i][j] += i * j

print(A)
#This loop is non-Exchangeable because i is dependent on j. so swapping the loops affects the result.
