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

_Fill in here._
