#!/bin/bash
mpicc --std=c++17 main.cpp -lstdc++ -O3

# Loop from 1 to 40
for X in {1..40}; do
  mpirun -n $X ./a.out
done

