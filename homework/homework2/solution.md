# Solutions

**Team**: Thread Titans

**Team Members**: Alex, Bashar, Fizza, Maya, Ramin

## Exercise 2.3

Question: Assume that an addition takes a certain unit time, and that moving a number from one processor to another takes that same unit time. Show that the communication time equals the computation time. Now assume that sending a number from processor $p$ to $p\pm k$ takes time $k$. Show that the execution time of the parallel algorithm now is of the same order as the sequential time.

Answer:

If a processor adds its numbers then it takes $p-1$ units of time. 

To move numbers from $p$ to another processor it takes: $p-1$ units of time.

So the communication time equals the computation time.

For the total time we have to consider that in a parallel setting we can divide the $n$ numbers among the $p$ processors, so the time would take: $\frac{n}{p}-1$ time. Moving the partial results would take $p-1$ time and then combining them all together would take $p-1$.

So: $T_{par}=(\frac{n}{p}-1) + (p-1) + (p-1)=\frac{n}{p}+2(p-1)$

For the case of communication taking a time $k$ instead of $1$ we now get this for the total time: $T_{par}=(\frac{n}{p}-1) + (p-1) + k(p-1)=\frac{n}{p}+(1+k)(p-1)$.

The sequential time is: $n-1$

For these to be the same order then $\frac{n}{p}+(1+k)(p-1)$ must be $O(n)$ 

For that to be the case we must assume that $p$ (and also $n$) is large or $p\approx n$ which would give:
$T_{par}\approx\frac{n}{n}+(1+k)(n-1)=1+(1+k)(n-1)\approx(1+k)n$. This is definitly of the same order as the sequential time.

## Exercise 2.5

Question: 

```
for i in [1:N]:
	x[0, i] = some_function_of(i)
	x[i, 0] = some_function_of(i)
for i in [1:N]:
	for j in [1:N]:
		x[i,j] = x[i-1,j]+x[i,j-1]
```
Answer the following questions about the double ```i,j``` loop:
1. Are the iterations of the inner loop independent, that is, could they be executed simultaneously?
2. Are the iterations of the outer loop independent?
3. If x[1,1] is known, show that x[2,1] and x[1,2] can be computed independently.
4. Does this give you an idea for a parallelization strategy?

Answer
1. No they are not independant, this is because ```x[i,j]``` depends on the values of ```x[i-1,j]``` and ```x[i,j-1]```.
As a result, the computation of ```x[i,j]``` cannot proceed until those other values are done, meaning they cannot be done simultaniously
2. No this is not independant either. The inner loop iterates over ```j``` while depending on values from both the previous row and the previous column
3. ```x[2,1]``` depends on ```x[1,1]``` and ```x[2,0]``` which is initialized in the first loop. So yes it can be easily computed by itself. Similarly, ```x[1,2]``` depends on ```x[0,2]``` which is defined in the first loop and ```x[1,1]``` is already defined. So yes these both can run independantly
4. I think we have to use a wavefront parallelization strategy. From what we have seen, the computations along the diagonals of the matrix are diagonal.
 
## Exercise 2.6

Answer

## Exercise 2.10

Answer

## Exercise 2.11

Answer

## Exercise 2.12

Answer

## Exercise 2.17

Answer

## Exercise 2.19

Answer

## Exercise 4.1

Answer

## Exercise 4.2

Answer

## Exercise 4.13

Answer

## Exercise 4.14

Answer
