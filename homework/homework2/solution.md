# Solutions

**Team**: Thread Titans

**Team Members**: Alex, Bashar, Fizza, Maya, Ramin

## Exercise 2.3

Question: Assume that an addition takes a certain unit time, and that moving a number from one processor to another takes that same unit time. Show that the communication time equals the computation time. Now assume that sending a number from processor $p$ to $p\pm k$ takes time $k$. Show that the execution time of the parallel algorithm now is of the same order as the sequential time.

Answer:

If a processor adds its numbers then it takes $p-1$ units of time. 

To move numbers from $p$ to another processor it takes: $p-1$ units of time.

So the communication time equals the computation time.

For the total time we have to consider that in a parallel setting we can divide the $n$ numbers among the $p$ processors, so the time would take: $\frac{n}{p}-1$ time. Moving the partial results would take $p-1$ time and then combining them all together would take $p-1$.

So: $T_{par}=(\frac{n}{p}-1) + (p-1) + (p-1)=\frac{n}{p}+2(p-1)$

For the case of communication taking a time $k$ instead of $1$ we now get this for the total time: $T_{par}=(\frac{n}{p}-1) + (p-1) + k(p-1)=\frac{n}{p}+(1+k)(p-1)$.

The sequential time is: $n-1$

For these to be the same order then $\frac{n}{p}+(1+k)(p-1)$ must be $O(n)$ 

For that to be the case we must assume that $p$ (and also $n$) is large or $p\approx n$ which would give:
$T_{par}\approx\frac{n}{n}+(1+k)(n-1)=1+(1+k)(n-1)\approx(1+k)n$. This is definitly of the same order as the sequential time.

## Exercise 2.5

Question: 

```
for i in [1:N]:
	x[0, i] = some_function_of(i)
	x[i, 0] = some_function_of(i)
for i in [1:N]:
	for j in [1:N]:
		x[i,j] = x[i-1,j]+x[i,j-1]
```
Answer the following questions about the double ```i,j``` loop:
1. Are the iterations of the inner loop independent, that is, could they be executed simultaneously?
2. Are the iterations of the outer loop independent?
3. If x[1,1] is known, show that x[2,1] and x[1,2] can be computed independently.
4. Does this give you an idea for a parallelization strategy?

Answer
1. No they are not independant, this is because ```x[i,j]``` depends on the values of ```x[i-1,j]``` and ```x[i,j-1]```.
As a result, the computation of ```x[i,j]``` cannot proceed until those other values are done, meaning they cannot be done simultaniously
2. No this is not independant either. The inner loop iterates over ```j``` while depending on values from both the previous row and the previous column
3. ```x[2,1]``` depends on ```x[1,1]``` and ```x[2,0]``` which is initialized in the first loop. So yes it can be easily computed by itself. Similarly, ```x[1,2]``` depends on ```x[0,2]``` which is defined in the first loop and ```x[1,1]``` is already defined. So yes these both can run independantly
4. I think we have to use a wavefront parallelization strategy. From what we have seen, the computations along the diagonals of the matrix are diagonal.
 
## Exercise 2.6

Prove that $E = 1$ implies that all processors are active all the time. (Hint: suppose all processors finish their work in time $T$ , except for one processor in $T'$ < $T$ . What is $T_p$ in this case? Explore the above relations.)

Answer: Assume, for contradiction, that $E = 1$ but not all processors are active at all times. Efficiency is defined as  

$$ E_p = \frac{S_p}{p} = \frac{T_1 / T_p}{p} $$  

If $E_p = 1$, then  

$$ \frac{T_1 / T_p}{p} = 1 \implies T_p = \frac{T_1}{p} $$  

which represents perfect parallelization where all processors finish simultaneously. Suppose one processor finishes early at $T' < T_p$, while the slowest processor takes $T_p$. Then, the system's execution time remains $T_p$, but at least one processor is idle for  

$$ \Delta T = T_p - T' $$  

Since efficiency is based on useful work, the presence of idle processors means the effective work done is strictly less than $T_p \cdot p$, leading to  

$$ E_p = \frac{T_1}{T_p \cdot p} < 1 $$  

which contradicts the assumption that $E_p = 1$. Therefore, all processors must be active for the entire duration.  

## Exercise 2.10

Show that, with the scheme for parallel addition just outlined, you can multiply two matrices in $\log_2 N$ time with $N^3/2$ processors. What is the resulting efficiency?

Answer: The standard matrix multiplication algorithm for two $N \times N$ matrices requires $O(N^3)$ operations. Each element $C_{ij}$ in the result matrix is computed as:

$$C_{ij} = \sum_{k=1}^{N} A_{ik} B_{kj}$$

which involves summing $N$ terms. A naive approach assigns $N^3$ processors, each computing a single multiplication, resulting in an initial step of $O(1)$ time. However, summation, the bottleneck still takes $O(n)$ time.

Using the parallel sum strategy, each sum can be computed in $O(\log_2 N)$ time using $N/2$ processors, as each step halves the number of active processors. Since there are $N^2$ independent summations (one per entry in $C$), the total number of processors required for summation is:

$$\frac{N^2 \cdot N}{2} = \frac{N^3}{2}$$

Thus, with $\frac{N^3}{2}$ processors, the total time for matrix multiplication is determined by the longest step, which is the summation step, giving a parallel time of:

$$T_P = O(\log_2 N)$$

Efficiency is given by:

$$E = \frac{\text{sequential time}}{\text{parallel time} \times \text{number of processors}}$$

Since the sequential execution time is $O(N^3)$, and the parallel time is $O(\log_2 N)$ with $O(N^3/2)$ processors, we obtain:

$$E = \frac{O(N^3)}{O(\log_2 N) \times O(N^3/2)}$$

$$E = O\left(\frac{N^3}{(N^3/2) \log_2 N}\right) = O\left(\frac{2}{\log_2 N}\right)$$

The efficiency decreases as $N$ grows, converging to zero for large $N$, showing weak scaling.


## Exercise 2.11

Let's do a specific example. Assume that a code has a setup that takes 1 second and a parallelizable section that takes 1000 seconds on one processor. What are the speedup and efficiency if the code is executed with 100 processors? What are they for 500 processors? Express your answer to at most two significant digits.

Answer: Let the total execution time on one processor be $T_1 = 1 + 1000 = 1001$ seconds. The sequential fraction of the code is $F_s = \frac{1}{1001}$, and the parallelizable fraction is $F_p = \frac{1000}{1001}$. To compute the speedup and efficiency for 100 processors, we use Amdahl’s Law: 

$$ T_P = T_1(F_s + F_p/P)$$
$$ T_{100} = 1001 \left( \frac{1}{1001} + \frac{1000}{1001 \times 100} \right) = 11$$

The speedup is:

$$ S_{100} = \frac{T_1}{T_{100}} = \frac{1001}{11} = 91$$

and the efficiency is:

$$ E_{100} = \frac{S_{100}}{p} = \frac{91}{100} = 0.91$$

For 500 processors, we similarly compute:

$$ T_{500} = 1001 \left( \frac{1}{1001} + \frac{1000}{1001 \times 500} \right) = 3$$

The speedup is:

$$ S_{500} = \frac{1001}{3} = 333.6667 $$

and the efficiency is:

$$ E_{500} = \frac{333.6667}{500} = 0.6673334$$

Using the above formulas, we find that for 100 processors, the speedup is approximately 91 and the efficiency is 0.91, while for 500 processors, the speedup is approximately 334 and the efficiency is 0.67.

## Exercise 2.12

Investigate the implications of Amdahl's law: if the number of processors $P$ increases, how does the parallel fraction of a code have to increase to maintain a fixed efficiency?

Answer: Consider the speedup and efficiency given by Amdahl’s law:  

$$ S_P = \frac{1}{F_s + \frac{F_p}{P}} $$  

$$ E_P = \frac{S_P}{P} = \frac{1}{P \left( F_s + \frac{F_p}{P} \right)} $$  

To maintain a fixed efficiency as $P$ increases, the denominator must remain constant. This requires either increasing $F_p$ or reducing $F_s$ by converting some of the sequential work into parallel work.  

Since $F_s$ may represent setup or initialization costs, minimizing it could help. For large $P$, $\frac{F_p}{P}$ becomes negligible, making $F_s$ the dominant term. Thus, to sustain efficiency, $F_p$ must scale with $P$ to counterbalance the effect of $F_s$.  

In short, as $P$ grows, efficiency declines unless $F_p$ increases. If $F_s$ stays constant, speedup is limited, and efficiency drops. To maintain efficiency, the parallel fraction must grow proportionally with $P$.  

## Exercise 2.17

For the strong scaling, run-time is given as

$$t = \frac{c}{p} \to \log{t} = \log{c} - \log{p}$$

Now if we consider $\log{p}\to x$ and $\log{t} \to y$ as the variables :

$$y = \log{c} - x$$

which is a line with the slop of $-1$.

Next, if run-time has non-parallelizable part :

```math
t = c_s + \frac{c_p}{p} \; \to \; \log{t} = \log{c_s}+\log{\big(1+\frac{c_p}{p\times c_s}\big)} \; \to \;   \log{t} = \log{c_p}-\log{c^*}+\log{\big(1+\frac{c^*}{p}\big)}
```

where $c_s$ and $c_p$ denote the non-parallelizable and parallelizable portion respectively. $c^*$ is the ratio of parallelizable to non-parallelizable part as $c^\* = c_p/c_s$.  Here, our goal is to derive a similar expression as above under a certain assumption. Let us compare  $\log{\big(1+x\big)}$ (blue) and $\log{x}$ (red) :

<div style="text-align: center;">
    <img src="graph.png" alt="My Image" width="300">
</div>

We observe, if $x$ is large enough we almost have $\log{\big(1+x\big)} \approx \log{x}$. So we can assume if $c^*$ is large enough :

```math
\log{t}\approx\log{c_p}-\log{c^*}-\log{p}+\log{c^*}\approx\log{c_p}-\log{p}
```

We can conclude that if the computational cost of the serial part of the problem is significantly smaller compared to the parallelizable part, we can almost expect the same scaling behavior. This can be achieved by optimizing the code to reduce the computational cost of the serial part or by increasing the problem size to increase the computational cost of the parallelizable part. Additionally, we can explore different parallel schemes that can further reduce the computational cost of the serial part.

## Exercise 2.19

For weak scaling if given that $n=p$ :

$$\textrm{speed-up} = \frac{p\log{p}}{(\log{p})^2} = \frac{p}{\log{p}}$$

For strong scaling the size of problem is fixed and number of processor is $p$. For bitonic sort we know $\Theta(n,p)=\frac{n(\log{n})^2}{p}$ so:

$$\textrm{speed-up} = \frac{n\log{n}}{n\log{n}^2/p} = \frac{p}{\log{n}}$$

Which is a decreasing function of $n$.


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

(There is a skeleton for this exercise under the name isendirecv.) Now use
nonblocking send/receive routines to implement the three-point averaging operation
$$
y_i = (x_{i-1} + x_{i} + x_{i+1})/3 : i=1,...,N-1
$$
on a distributed array. There are two approaches to the first and last process:

1. you can use `MPI_PROC_NULL` for the 'missing' communications;
2. you can skip these communications altogether, but now you have to count the
requests carefully

### Answer

This implementation uses non-blocking MPI communication to perform a three-point averaging operation. We'll use the `MPI_PROC_NULL` approach for boundary processes as it provides a cleaner solution. Here's the complete implementation with explanations:

```cxx
/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-9
 ****
 **** MPI Exercise for Isend/Irecv
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
using namespace std;
#include <mpi.h>

#include "tools.h"

int main() {

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  stringstream proctext;

  MPI_Init(0,0);

  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  double mydata=procno,leftdata=0.,rightdata=0.;
  int sendto,recvfrom;
  MPI_Request requests[4];

  // For left communication
  // If process 0, use MPI_PROC_NULL for left neighbor
  recvfrom = (procno == 0) ? MPI_PROC_NULL : procno-1;
  sendto = (procno == 0) ? MPI_PROC_NULL : procno-1;
  
  MPI_Isend(&mydata,1,MPI_DOUBLE, sendto,0, comm,&requests[0]);
  MPI_Irecv(&leftdata,1,MPI_DOUBLE, recvfrom,0, comm,&requests[1]);

  // For right communication
  // If last process, use MPI_PROC_NULL for right neighbor
  recvfrom = (procno == nprocs-1) ? MPI_PROC_NULL : procno+1;
  sendto = (procno == nprocs-1) ? MPI_PROC_NULL : procno+1;
  
  MPI_Isend(&mydata,1,MPI_DOUBLE, sendto,0, comm,&requests[2]);
  MPI_Irecv(&rightdata,1,MPI_DOUBLE, recvfrom,0, comm,&requests[3]);

  // Wait for all communications to complete
  MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);
  
  /*
   * Correctness check:
   * `error' will be:
   * - the lowest process number where an error occured, or
   * - `nprocs' if no error.
   */
  // check correctness
  mydata = mydata+leftdata+rightdata;

  double res;
  if (procno==0) {
    res = 2*procno+1;
  } else if (procno==nprocs-1) {
    res = 2*procno-1;
  } else {
    res = 3*procno;
  }

  int error_test = !ISAPPROX(mydata,res);
  if (error_test) {
    stringstream proctext;
    proctext << "Data on proc " << procno << " should be " << res << ", found " << mydata;
    cout << proctext.str() << "\n";
  }
  print_final_result(error_test,comm);

  MPI_Finalize();
  return 0;
}
```

Key points about the implementation:

1. **Non-blocking Communication**: We use `MPI_Isend` and `MPI_Irecv` for asynchronous communication, which allows for potential overlap of communication and computation.

2. **Boundary Handling**: For processes at the boundaries (rank 0 and rank n-1), we use `MPI_PROC_NULL` as the communication partner. This elegantly handles the boundary conditions:
   - Process 0 has no left neighbor
   - Process (nprocs-1) has no right neighbor

3. **Communication Pattern**:
   - Each process sends its value to both neighbors (except boundary processes)
   - Each process receives values from both neighbors (except boundary processes)
   - Four non-blocking operations per process (2 sends + 2 receives)

4. **Synchronization**: `MPI_Waitall` ensures all communications are complete before proceeding with the averaging calculation.

5. **Result Verification**: The code includes a verification step that checks if the results match the expected values:
   - For process 0: result = 2*procno + 1
   - For last process: result = 2*procno - 1
   - For middle processes: result = 3*procno

This implementation demonstrates proper use of non-blocking MPI communication while handling boundary conditions efficiently using `MPI_PROC_NULL`.

## Exercise 4.14


(There is a skeleton for this exercise under the name isendirecvarray.) Take your
code of exercise 4.13 and modify it to use latency hiding. Operations that can be
performed without needing data from neighbors should be performed in between
the MPI_Isend / MPI_Irecv calls and the corresponding MPI_Wait calls.

### Answer

The key modification for latency hiding is to reorganize the code to perform independent computations while waiting for communication. Here are the essential modified blocks:

```cxx
// 1. Initialize data structures
const int N = 10;  // Size of local array
vector<double> x(N), y(N);
vector<double> left_data(1), right_data(1);

// 2. Start non-blocking communication early
MPI_Request requests[4];
int left = (procno == 0) ? MPI_PROC_NULL : procno-1;
int right = (procno == nprocs-1) ? MPI_PROC_NULL : procno+1;

// Post receives first
MPI_Irecv(&left_data[0], 1, MPI_DOUBLE, left, 0, comm, &requests[0]);
MPI_Irecv(&right_data[0], 1, MPI_DOUBLE, right, 0, comm, &requests[1]);

// Post sends
MPI_Isend(&x[0], 1, MPI_DOUBLE, left, 0, comm, &requests[2]);
MPI_Isend(&x[N-1], 1, MPI_DOUBLE, right, 0, comm, &requests[3]);

// 3. While communication happens, compute interior points
for (int i = 1; i < N-1; i++) {
    y[i] = (x[i-1] + x[i] + x[i+1]) / 3.0;
}

// 4. Wait for communication to complete
MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);

// 5. Now compute boundary points that required neighbor data
if (procno == 0) {
    y[N-1] = (x[N-2] + x[N-1] + right_data[0]) / 3.0;
    y[0] = (x[0] + x[1]) / 2.0;  // No left neighbor
} else if (procno == nprocs-1) {
    y[0] = (left_data[0] + x[0] + x[1]) / 3.0;
    y[N-1] = (x[N-2] + x[N-1]) / 2.0;  // No right neighbor
} else {
    y[0] = (left_data[0] + x[0] + x[1]) / 3.0;
    y[N-1] = (x[N-2] + x[N-1] + right_data[0]) / 3.0;
}
```

Key improvements for latency hiding:

1. Only communicate necessary boundary values (single values instead of arrays)
2. Start non-blocking communication as early as possible
3. Compute interior points while waiting for communication
4. Handle boundary points only after communication completes
5. Use proper boundary conditions:
   - Two-point average at domain edges
   - Three-point average everywhere else

This implementation maximizes the overlap between computation and communication, improving overall performance.

