# Answers and Responses to HW3 exercises

## [High-Performance Scientific Computing](https://cmse822.github.io/assets/EijkhoutIntroToHPC2020.pdf)

## Exercise 7.38

### Equation A:

$$
(\alpha I + \frac{d^2}{dx^2} + \frac{d^2}{dy^2}) u^{(t+1)} = u^{(t)}
$$

### Operation Count:

Considering we solve it using an iterative approach as Conjugate Gradient, the operations count is roughly $O(N^2)$ per step. The number of steps can vary depending on the preconditioning before the operation but can vary from a constant k to a worst case of $N$. In this case the operation count for the iterative method ranges from $O(N^2)$ to $O(N^3)$
### Parallel Performance:

While the matrix multiplication can be made in parallel, each iteration still depends on the previous iteration to be calculating, leaving less space to be made parallel.

### Equation B:

$$
(\beta I + \frac{d^2}{dx^2})(\beta I + \frac{d^2}{dy^2}) u^{(t+1)} = u^{(t)}
$$

### Operation Count

The operator splitting approach rewrites the equation in a way that allows you to solve two simpler 1D problems instead of one big 2D problem.

Each of the directions requires a 1D system to be solved, which requires $O(N^2)$ operations ($N$ rows and $O(N)$ for each row), as there are two directions the total time complexity is $2*O(N^2)$ which reduces to $O(N^2)$.
### Parallel Performance

Even though is is possible that both equations have the same time complexity, the **operation splitting** approach has a higher parallelization possibility.

For each directions we have to solve $N$ systems, one for each row. These systems are independent of the other rows so they can all be done concurrently. Later can nbe synchronized into a single processor.

Using this strategy, and disregarding some other costs, could lead to an efficiency close to 1 and a total execution time of $O(N)$, which is better than the _Equation A_ performance.

### $\alpha$ dependence on $t$

If $\alpha$ is independent of $t$ on operation A, then the matrix remains unchanged and can mostly be reused across all the calculations.

When $\alpha$ is dependent on $t$, then for each step on the iterative approach it is necessary to recalculate the matrix $\alpha I$, making it less efficient and pushing the total cost to be $O(N^3)$

For operation B and $\alpha$ dependent on $t$ has a smaller impact. As we are only solving smaller steps on each row, we don't need to compute the whole $\alpha I$ matrix in order to complete the matrix multiplication, leading to a higher speed up when compared to a dependent $\alpha$ with operation A.

## Exercise 7.39

_Fill in here._

## [Parallel Programming with MPI and OpenMP](https://cmse822.github.io/assets/EijkhoutParallelProgramming.pdf)

Starter code for these exercises maybe found [here](https://github.com/VictorEijkhout/TheArtOfHPC_vol2_parallelprogramming/tree/main/exercises/exercises##mpi##cxx).

## Exercise 3.2

_Fill in here._

## Exercise 3.3, 3.4

### 3.3
> Code on code/3_3/main.cpp

### 3.4

In case we gather the whole distributed array on the sindle processor is not good formostly 2 reasons:

1. You need all other processes to communicate with that processor. This could generate a significant wait time.
2. Loss of parallel efficiency. If all processors communicate into a single processor, then part of the computation needs to be done sequencially, leading to a worse parallel efficiency. Depending on how big the array is it could also have a big load impalance on a single processor.

## Exercise 3.6

_Fill in here._

## Exercise 3.7

_Fill in here._

## Exercise 3.16

_Fill in here._

## Exercise 3.17, 3.18

_Fill in here._

## Exercise 3.19

_Fill in here._

## Exercise 3.23, 3.24

_Fill in here._
