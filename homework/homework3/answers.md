# Answers and Responses to HW3 exercises

## [High-Performance Scientific Computing](https://cmse822.github.io/assets/EijkhoutIntroToHPC2020.pdf)

### Exercise 7.38

_Fill in here._

### Exercise 7.39

_Fill in here._

## [Parallel Programming with MPI and OpenMP](https://cmse822.github.io/assets/EijkhoutParallelProgramming.pdf)

Starter code for these exercises maybe found [here](https://github.com/VictorEijkhout/TheArtOfHPC_vol2_parallelprogramming/tree/main/exercises/exercises###mpi###cxx).

### Exercise 3.2

_Fill in here._

### Exercise 3.3, 3.4

_Fill in here._

### Exercise 3.6

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

### Exercise 3.7

_Fill in here._

### Exercise 3.16

_Fill in here._

### Exercise 3.17, 3.18

_Fill in here._

### Exercise 3.19

_Fill in here._

### Exercise 3.23, 3.24

First we consider the case where the root process sends to each of the other $p-1$ processes linearly. Since the root process can only send to 1 at a time, the time to get all the data sent is $\beta n (p-1)$. Afterwards, the data will reach the last process sent to after $\alpha$ time. This term is not greater since the previous sends will already be in flight and we only need to consider the one in "last place", or the final process sent to. Overall, the total time is
$$\alpha + \beta n(p-1)$$

Now, we consider sending the data in a binary tree pattern. Specifically, the root begins by sending the data to 2 processes, those 2 processes each send to 2 processes, and so on until all processes have received it.

Communication at a single level of the tree requires some number of processes (the number does not matter since they all send concurrently) sending to at most 2 other processes. If a process has to send to 2 others it will take $\alpha + 2\beta n$ time and if it only has 1 it will simply be $\alpha + \beta n$. The number of communication levels in the tree is simply $\lfloor\log p\rfloor$. Then, in most cases the communication time will be
$$(\alpha + 2\beta n)\lfloor \log p\rfloor = \alpha\lfloor\log p\rfloor + 2\beta n\lfloor\log p\rfloor$$

It is possible that the last level of the binary communication tree does not have enough processes, such that every process only needs to send to at most 1 other. In this case, the $\beta$ term would instead be $(2\lfloor \log p\rfloor - 1)\beta n$. This can be considered a lower bound on this term, while $\alpha \lfloor\log p\rfloor$ is a lower bound on the first term.
