# Exercise 2.5

```python
for i in [1:N]:
    x[0, i] = some_function_of(i)
    x[i, 0] = some_function_of(i)

for i in [1:N]:
    for j in [1:N]:
        x[i, j] = x[i-1, j] + x[i, j-1]
```

We are required to answer the following questions about the double ```i,j``` loop:
1. Are the iterations of the inner loop independent, that is, could they be executed simultaneously?
      We notice that the inner loop of the algorithm is desgined to update ```x[i,j]``` by using the equation ```x[i, j] = x[i-1, j] + x[i, j-1]``` where;
           ```x[i-1, j]``` represents the rows and
           ```x[i, j-1]``` represents the columns.
     From this design, we notice that the iteration of the inner loop depends on the previous row and the previous column. This implies that the iterations of the innerloop
      are not independent and therefore cannot be executed simultaneously.
   
2. Are the iterations of the outer loop independent?
         We observed that outer loop iterates over ```i``` and for every iteration on the algorithm and  ```i``` depends on the previous row denoted as ```x[i-j]```.
       This implies that iterations of the outerloop are not indepedent. 

     
3. If ```x[1,1]``` is known, show that ```x[2,1]``` and ```x[1,2]``` can be computed independently.
          We know that ```x[i, j] = x[i-1, j] + x[i, j-1]```
       This implies that;
             ```x[2, 1] = x[1, 1] + x[2, o]```                (1)
        and
           ```x[1, 2] = x[0, 2] + x[1, 1]```                  (2)
       We observe that in both equations (1) and (2), there is ```x[1,1]``` on the right hand side in the the diagonal.
       We also observed that there is ```x[2,0]``` and ```x[0,2]``` in equation (1) and (2) respectively.
       This implies that in each case that is, case (1), ```x[2,0]``` is computed first before updating ```x[2,1]``` and
       similary in case (2), ```x[0,2]``` is computed first before updating ```x[1,2].```
       This therefore means that if ```x[1,1]``` is known, then both ```x[2,1]``` and ```x[1,2]``` can be computed independently.
               
       
4. Does this give you an idea for a parallelization strategy?
        Yes, this gives us an idea of a parallelization strategy. From part (3) above, there is an interesting observation of computing the diagonal elements first
       and then compute ```x[2,1]``` and ```x[1,2]``` can be computed independently in parallel. By doing this, we are solving the algorithm by using wavefronts as
        discussed in section 7.10.1 page 333 textbook of the lecture notes. We notice that the values of each wavefront are independent so they can be solved in
         parallel in the same sequential step. Starting with the diagonal elements avoids dependencies and hence allowing parallelization.
   
   
