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

Answer
This solution references the attatched code, pingpong.cxx... 

Running that attatched code with two communicating processes on the same node shows... 

Running the attatched code with two communicating processes on different nodes shows... 

If we were to modify the program to use longer messages (ie. change `MPI_Send(send, 1, MPI_DOUBLE, processB, 0, comm);` to `MPI_Send(send, 1000, MPI_DOUBLE, processB, 0, comm);`), then we see a timing increase of... 

## Exercise 4.2
Take your ping-pong program and modify it to let half the processors be source andd the other half the targets. Does the ping pong time increase? Does the observed behavior depend on how you choose the two sets?
Answer

## Exercise 4.13

Answer

## Exercise 4.14

Answer