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

Answer

## Exercise 4.2

Answer

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

