### Exercise 2.3
*Assume that an addition takes a certain unit time, and that moving a number from one processor to another takes that same unit time. Show that the communication time equals the computation time.*

In this scenario, we have N processors, each of which store 1 number. In each step, half of the processors will send their values to a processor from the other half. That other processor will then add the received value to it's own. The processor that sent it's value is no longer useful. This means that each step, the number of active processors is halved, until there is only one processor left with the total sum. The transport of data and the addition take the same amount of time and are equally involved in every step. So, this will be true when the time taken for each step is summed up.

*Now assume that sending a number from processor $p$ to $p±k$ takes time $k$. Show that the execution time of the parallel algorithm now is of the same order as the sequential time.*

Previously, no matter which processor the data was being moved from or to, the time taken was the same. In this case, however, the time taken is proportional to the distance between the processors. Since each step halves the number of processors, it creates gaps between "adjacent" active processors, doubling the distance (and thus time taken) every step. 

The total communication time is then: 

$T_{comm}=k(1+2+4+8+...+N/2)=k(N-1)$

The number of steps taken is $log_2(N)$ and an addition happens each step. If the addition also takes $k$ time, then the total time taken for both communication and addition is: 

$T_{total}=k(N+log_2(N)-1)$ 

Code running on a single core would only need to do addition (ignoring memory overhead), so the time taken would be...

$T_{serial}=kN$

### Exercise 2.5

```
for i in [1:N]:
    x[0,i] = some_function_of(i)
    x[i,0] = some_function_of(i)

for i in [1:N]:
    for j in [1:N]:
        x[i,j] = x[i-1,j]+x[i,j-1]
```

*Answer the following questions about the double i,j loop:*
*1. Are the iterations of the inner loop independent, that is, could they be executed
simultaneously?*

No, they are not. They require the value in the previous column (j) to already be computed. So each iteration of the inner loop depends on the result of the previous iteration.

*2. Are the iterations of the outer loop independent?*

No. Similarly, the outer loop requires that the values in the previous row (i) are already computed.

*3. If x[1,1] is known, show that x[2,1] and x[1,2] can be computed independently.*

```
x[2,1] = x[1,1] + x[2,0]
x[1,2] = x[1,1] + x[0,2]
```

The first loop in the problem code initializes all values at `x[i,0]` and `x[0,i]`, so the values of `x[2,0]` and `x[0,2]` are known. Thus, neither of these computations depend on the values computed by the other. They can be computed independently.

*4. Does this give you an idea for a parallelization strategy?*

Yes. The elements that can be computed independently are diagonals. So, the outer loop could loop over diagonals and the inner loop could be parallelized to compute each element of the diagonal simultaneously.