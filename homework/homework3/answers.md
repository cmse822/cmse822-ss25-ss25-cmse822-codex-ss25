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

Let each process compute a random number, and compute the sum of these
numbers using the MPI_Allreduce routine.

$𝜉 = ∑_𝑖 𝑥_i$

Each process then scales its value by this sum.

$𝑥′_𝑖 ← 𝑥_𝑖/𝜉$

Compute the sum of the scaled numbers

$𝜉′ = ∑_𝑖 𝑥′_𝑖$

and check that it is 1. (There is a skeleton for this exercise under the name randommax.)

Below are the altered code snippets from the skeleton code *"randommax.cpp"*. ([MPI Tutorial for MPI Reduce & Allreduce](https://mpitutorial.com/tutorials/mpi-reduce-and-allreduce/) used to figure out inputs). The completed code file can be found in the `code` folder. 

```C++
/*
* Send the reference of the computed random number
* Recieve a reference to the sum
* Size of 1 float
* Perform the summation
*/
MPI_Allreduce(&myrandom, &sum_random, 1, MPI_FLOAT, MPI_SUM, comm);

............................................................................

/*
* Send the reference of the *scaled* random number
* Recieve a reference to the *scaled* sum
* Size of 1 float
* Perform the summation
*/
MPI_Allreduce(&scaled_random, &sum_scaled_random, 1, MPI_FLOAT, MPI_SUM, comm);
```

Code was run on 2, 4, 8, 16, and 32 processors each time getting the expected value of $1$, and the output "Part 1 finished; all results correct."

Commands used:

```bash
mpic++ -o randommax ss25-parallel-peeps/homework/homework3/code/randommax.cpp
mpirun -np 32 ./randommax
```

## Exercise 3.3, 3.4

### 3.3
> Code on code/3_3/main.cpp

### 3.4

In case we gather the whole distributed array on the single processor is not good for mostly 2 reasons:

1. You need all other processes to communicate with that processor. This could generate a significant wait time.
2. Loss of parallel efficiency. If all processors communicate into a single processor, then part of the computation needs to be done sequentially, leading to a worse parallel efficiency. Depending on how big the array is it could also have a big load imbalance on a single processor.
## Exercise 3.6

_Fill in here._

## Exercise 3.7

Below is a code snippet containing the implementation of the reduce to a root operation.

A fully functional version and source code is found under the code/3.6/ folder.
```C++
#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include <random>


int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank = 0; 
    int size = 1;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //preparing a random number generator
    //thanks, stack overflow!
    //(the normal method of ctime and srand() gives the same number on every rank.)
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist100(1,100);

    //generate a random integer;
    int number {static_cast<int>(dist100(rng))};
    std::cout << "rank " << rank << ": " << number << '\n';

    int max {0};
    //using MPI_Reduce to find and print the maximum;
    MPI_Reduce(&number, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if(rank == 0)
        {std::cout << "MAXIMUM IS " << max << '\n';}

    MPI_Finalize();
    return 0;
}

```

## Exercise 3.16

_Fill in here._

## Exercise 3.17, 3.18

_Fill in here._

## Exercise 3.19

_Fill in here._

## Exercise 3.23, 3.24

First we consider the case where the root process sends to each of the other $p-1$ processes linearly. Since the root process can only send to 1 at a time, the time to get all the data sent is $\beta n (p-1)$. Afterwards, the data will reach the last process sent to after $\alpha$ time. This term is not greater since the previous sends will already be in flight and we only need to consider the one in "last place", or the final process sent to. Overall, the total time is
$$\alpha + \beta n(p-1)$$

Now, we consider sending the data in a binary tree pattern. Specifically, the root begins by sending the data to 2 processes, those 2 processes each send to 2 processes, and so on until all processes have received it.

Communication at a single level of the tree requires some number of processes (the number does not matter since they all send concurrently) sending to at most 2 other processes. If a process has to send to 2 others it will take $\alpha + 2\beta n$ time and if it only has 1 it will simply be $\alpha + \beta n$. The number of communication levels in the tree is simply $\lfloor\log p\rfloor$. Then, in most cases the communication time will be
$$(\alpha + 2\beta n)\lfloor \log p\rfloor = \alpha\lfloor\log p\rfloor + 2\beta n\lfloor\log p\rfloor$$

It is possible that the last level of the binary communication tree does not have enough processes, such that every process only needs to send to at most 1 other. In this case, the $\beta$ term would instead be $(2\lfloor \log p\rfloor - 1)\beta n$. This can be considered a lower bound on this term, while $\alpha \lfloor\log p\rfloor$ is a lower bound on the first term.
