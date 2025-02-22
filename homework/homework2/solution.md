# Solutions

**Team**: Thread Titans

**Team Members**: Alex, Bashar, Fizza, Maya, Ramin

## Exercise 2.3

Assume that an addition takes a certain unit time, and that moving a number from one processor to another takes that same unit time. Show that the communication time equals the computation time. 

Now assume that sending a number from processor $p$ to $p \pm k$ takes time $k$. Show that the execution time of the parallel algorithm now is of the same order as the sequential time.

Answer:

## Exercise 2.5

Answer

## Exercise 2.6

Answer

## Exercise 2.10

Answer

## Exercise 2.11

Answer

## Exercise 2.12

Answer

## Exercise 2.17

Answer

## Exercise 2.19

Answer

## Exercise 4.1
Implement the ping-pong program. Add a timer using `MPI_Wtime`. For the state argument of the receive call, use `MPI_ST` `TUS_IGNORE`. Run multiple ping-ons (say a thousand) and put the timer around the loop. The first run may take longer, try to discard it. Run your code with the two communicating processes first on the same node, then on different nodes. Do you see a difference? Then modify the program to use longer messages. How does the timing increase with message size?

## Answer
Please see `/homework2/pingpong/ex4.1/...` for the code associated with this exercise. 

The program measures the communication time between two processes using `MPI_Wtime()`. To account for delays while the code is initializing, the first iteration is discarded from the timing calculation. The code is ran with differing message sizes, with the processes on the same node and on different nodes. 
* `processA = 0` means rank 0 is the sender (initially).
* `processB = 1` means rank 1 is the receiver (initially). 

#### Running on both processes on a single node:
```c
#SBATCH --nodes=1            # Run on 1 node
#SBATCH --ntasks=2           # Total number of MPI tasks
#SBATCH --ntasks-per-node=2  # Set 2 task per node
```
| message size | pingpong time (us) [node]|
|--------------|---------------|
|  1B  | 0.385 [lac-021] |
|  8B  | 0.474 [lac-010] |
| 64B  | 1.098 [lac-010] |
| 1024B| 4.874 [lac-021] |

#### Running processes on two different nodes: 
```c
#SBATCH --nodes=2            # Run on 2 nodes
#SBATCH --ntasks=2           # Total number of MPI tasks
#SBATCH --ntasks-per-node=1  # Set 1 task per node
```
| message size | pingpong time (us) [nodes] |
|--------------|---------------|
|  1B  | 2.146 [lac-023 <-> lac-021] |
|  8B  | 3.057 [lac-023 <-> lac-021] |
| 64B  | 5.058 [lac-023 <-> lac-021] |
| 1024B| 12.384 [lac-023 <-> lac-021] |

In conclusion, the pingoing test shows that communication time unsurprisingly increases with message size and that inter-node communication introduces significant delay compared to intra-node communication.

## Exercise 4.2
Take your ping-pong program and modify it to let half the processors be source and the other half the targets. Does the ping pong time increase? Does the observed behavior depend on how you choose the two sets?

## Answer
Please see `/homework2/pingpong/ex4.2/...` for the code associated with this exercise.

This version of the pingpong code splits the processes into two groups:
* The first half are the senders.
* The second half are the receivers.

#### Running processes on two different nodes:
```c
#SBATCH --nodes=2             # Run on 2 nodes
#SBATCH --ntasks=12           # Total number of MPI tasks
#SBATCH --ntasks-per-node=6   # Set 6 tasks per node
```

| process pair [lac-255 <-> lac-254] | 1B (us) | 8B (us) | 64B (us) | 1024B (us) |
|--------------|---------|---------|----------|------------|  
| 1 <-> 7 | 2.060 | 2.695 | 4.096 | 10.058 |
| 2 <-> 8 | 2.097 | 2.734 | 4.070 | 10.004 |
| 3 <-> 9 | 2.054 | 2.422 | 4.090 | 9.995 |
| 0 <-> 6 | 1.955 | 2.434 | 3.821 | 9.924 |
| 4 <-> 10| 1.921 | 2.736 | 3.711 | 9.982 |
| 5 <-> 11| 1.915 | 2.452 | 3.740 | 9.960 |

#### Running processes on the same node:
```c
#SBATCH --nodes=1             # Run on 1 node
#SBATCH --ntasks=12           # Total number of MPI tasks
#SBATCH --ntasks-per-node=12  # Set 12 tasks per node
```

| Process Pair | 1B (µs) [lac-010] | 8B (µs) [lac-021] | 64B (µs) [lac-361] | 1024B (µs) [lac-361] |
|-------------|---------|---------|----------|-----------|
| 1 <-> 7    | 0.396   | 0.486   | 1.190    | 4.661     |
| 2 <-> 8    | 0.408   | 0.493   | 1.176    | 4.666     |
| 3 <-> 9    | 0.409   | 0.504   | 1.175    | 4.645     |
| 4 <-> 10   | 0.803   | 1.190   | 1.165    | 4.710     |
| 0 <-> 6    | 0.375   | 0.485   | 1.134    | 4.663     |
| 5 <-> 11   | 0.811   | 1.253   | 2.444    | 8.724     |

Thus, when running across two nodes, the pingpong time increases significantly due to network communication overhead. There is roughly a x5 slowdown in the 1B message as it goes from roughly ~0.4us to ~2.0us, with greater message sizes increasing even more. Thus, **latency** is significantly higher  for inter-node communication and **bandwidth** is lower since larger messages experience a higher cost across nodes as the network link becomes a bottleneck. 

## Exercise 4.13

Answer

## Exercise 4.14

Answer
