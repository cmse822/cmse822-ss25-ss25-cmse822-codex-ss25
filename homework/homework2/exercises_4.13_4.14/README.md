# Exercise 4.13

The starter code `isendirecv.cxx` was modified to implement an MPI code which uses non-blocking communication at process $i$ to send and receive the process number to and from processes $i-1$ and $i+1$.
For the boundary processes (processes $0$ and $procno-1$), `MPI_PROC_NULL` was used to indicate sending and receiving need not occur.

The variable `mydata` contains the data we wish to send, and the following code posts a send and receive to the "left side" process:
```cpp
MPI_Isend(&mydata,1,MPI_DOUBLE, sendto,0, comm, requests);
MPI_Irecv(&leftdata,1,MPI_DOUBLE, recvfrom,0, comm, requests + 1);
```

The communication with the right side is symmetric to this.

Then, we must use the following code to wait on the 4 requests to finish:
```cpp
MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);
```

Since the communication is non-blocking, until this wait is done we cannot be sure that `leftdata` and `rightdata` actually contain the data.

# Exercise 4.14

For this exercise the starter code `isendirecvarray.cxx` was modified to implement an MPI code which uses non-blocking communication in a similar fashion to the previous exercise.

The key difference is that now each process has an array of 100 elements to compute the 3-point average of, only 2 of which (first and last) require neighboring processes' data to compute. Thus, we have some work which can be done while waiting on the requests.

First, we post the sends and receives for each process to send their first and last element and receive the neighboring element they need. While waiting on those requests, we can finish constructing the rest of the array and compute the 3 point average for the inner elements:

```cpp
for (int i=0; i<mylocal; i++)
    indata[i] = myfirst+i;

for (int i=1; i<mylocal-1; i++)
    outdata[i] = indata[i-1] + indata[i] + indata[i+1];
```

Afterwards, we can call `MPI_Waitall` similarly to the previous exercise.

This is called latency hiding, since we perform necessary work while the non-blocking communication requests complete. Ideally, we will spend less time blocking on the `MPI_Waitall` call than if all those computations were done before or after posting the requests.
