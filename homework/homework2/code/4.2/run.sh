#!/bin/bash

# Compile each C++ file
mpicc --std=c++17 opt1.cpp -lstdc++ -O3 -o opt1.out
mpicc --std=c++17 opt2.cpp -lstdc++ -O3 -o opt2.out

# Clear previous output files
echo "" > opt1_output.txt
echo "" > opt2_output.txt

X=40

echo "Running opt1 with X=$X" >> opt1_output.txt
mpirun -n $X ./opt1.out >> opt1_output.txt 2>&1
  
echo "Running opt2 with X=$X" >> opt2_output.txt
mpirun -n $X ./opt2.out >> opt2_output.txt 2>&1
