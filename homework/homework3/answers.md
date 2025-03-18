# Answers and Responses to HW3 exercises

## [High-Performance Scientific Computing](https://cmse822.github.io/assets/EijkhoutIntroToHPC2020.pdf)

### Exercise 7.38

_Fill in here._

### Exercise 7.39

_Fill in here._

## [Parallel Programming with MPI and OpenMP](https://cmse822.github.io/assets/EijkhoutParallelProgramming.pdf)

Starter code for these exercises maybe found [here](https://github.com/VictorEijkhout/TheArtOfHPC_vol2_parallelprogramming/tree/main/exercises/exercises###mpi###cxx).

### Exercise 3.2

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

### Exercise 3.3, 3.4

_Fill in here._

### Exercise 3.6

_Fill in here._

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
