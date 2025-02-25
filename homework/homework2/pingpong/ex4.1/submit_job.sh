#!/bin/bash --login
#SBATCH --job-name=pingpong
#SBATCH --nodes=2                   # Number of different nodes to run on
#SBATCH --ntasks=2                  # Total number of MPI tasks
#SBATCH --ntasks-per-node=1         # Number of tasks per node
#SBATCH --time=00:10:00             # Max execution time
#SBATCH --partition=general-short   # Allows for a max job time of 4 hours

mpicxx -o pingpong pingpong.cxx     # Compile the program
srun ./pingpong          
