#!/bin/bash --login
#SBATCH --job-name=scaling_study
#SBATCH --nodes=1
#SBATCH --ntasks=128
#SBATCH --mem=493G
#SBATCH --time=4:00:00
#SBATCH --constraint=amd20

# Load necessary modules
module purge
module load HDF5

# Change to the directory containing the scripts
cd /mnt/home/scouch/cmse822/cmse822-codex-private/agoge

# Run the weak scaling study
bash ../utils/weak_scaling.sh

# Run the strong scaling study
bash ../utils/strong_scaling.sh
