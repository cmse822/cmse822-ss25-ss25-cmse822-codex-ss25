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
Please see the attatched code, pingpong.cxx, which is part of this solution. 

To have the source and target processors close together (ie. on the same node), I set:
```int processA = 0, processB=1;```
Running the attatched code with `NEXPERIMENTS 10000`, we see that...  

Now, to have the source and target processors far from each other (ie. on different nodes), I set:
```int processA = , processB = ;```
Running the attatched code with `NEXPERIMENTS 10000` again, we see that...

Comment on differences here. 

Now, I redo the two steps above, but I add the line `#define MESSAGE_SIZE 10000`. For two processes on the same node with increased message size, we see that ..... For two processes on different nodes, we see that.... . Thus, we conclude that .... 

## Exercise 4.2
Take your ping-pong program and modify it to let half the processors be source andd the other half the targets. Does the ping pong time increase? Does the observed behavior depend on how you choose the two sets?

## Answer

## Exercise 4.13

Answer

## Exercise 4.14

Answer