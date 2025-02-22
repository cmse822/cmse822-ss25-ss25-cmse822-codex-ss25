#!/bin/bash --login
#SBATCH --job-name=pingpong
#SBATCH --nodes=1                   # Number of different nodes to run on
#SBATCH --ntasks=12                 # Total number of MPI tasks
#SBATCH --ntasks-per-node=12        # Number of tasks per node
#SBATCH --time=00:10:00             # Max execution time
#SBATCH --partition=general-short   # Allows for a max job time of 4 hours

mpicxx -o pingpong pingpong.cxx      # Compile the program
srun ./pingpong                      
