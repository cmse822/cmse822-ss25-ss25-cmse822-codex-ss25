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
