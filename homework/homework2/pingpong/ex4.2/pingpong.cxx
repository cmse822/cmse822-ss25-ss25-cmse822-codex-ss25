/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-7
 ****
 **** MPI Exercise 4.2: Modified Ping-Pong with Multiple Pairs
 ****
 ****************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <mpi.h>
#include <unistd.h> // for gethostname
#include <limits.h> // for HOST_NAME_MAX

using namespace std;

int main() {
    MPI_Comm comm = MPI_COMM_WORLD;
    int nprocs, procno;
    stringstream proctext;
    char hostname[HOST_NAME_MAX];

    MPI_Init(0, 0);
    MPI_Comm_size(comm, &nprocs);
    MPI_Comm_rank(comm, &procno);

    gethostname(hostname, HOST_NAME_MAX); // get the hoestname

    // Define the number of experiments and message size
    #define NEXPERIMENTS 1001  // One extra iteration to discard
   // #define MESSAGE_SIZE 1
   // #define MESSAGE_SIZE 8      // 8 doubles (64 bytes)
   // #define MESSAGE_SIZE 64     // 64 doubles (512 bytes)
    #define MESSAGE_SIZE 1024   // 1024 doubles (8192 bytes)

    // Ensure even number of processes
    if (nprocs % 2 != 0) {
        if (procno == 0) {
            cerr << "This program requires an even number of processes." << endl;
        }
        MPI_Finalize();
        return 1;
    }

    int partner;  // Store partner process number
    double send[10000], recv[10000];
    send[0] = 1.1;  // Send buffer
    
    // pring process rank and its node info
    cout << "process" << procno << "is running on node" << hostname << endl;
    
    /*
        Divide  processes into two groups:
        - First half are senders... rank < nprocs/2
        - Second half are receivers... rank >= nprocs/2
        
        Each sender `i` sends data to a corresponding receiver `i + nprocs / 2`.
    */
    if (procno < nprocs / 2) {
        // Sender processes
        partner = procno + nprocs / 2;  // Each sender pairs with receiver in second half
        double total_time = 0.0;

        for (int n = 0; n < NEXPERIMENTS; n++) {
            double t_start = MPI_Wtime();  // Start the timer

            // Send message to corresponding receiver
            MPI_Send(send, MESSAGE_SIZE, MPI_DOUBLE, partner, 0, comm);
            // Receive message back from receiver
            MPI_Recv(recv, MESSAGE_SIZE, MPI_DOUBLE, partner, 0, comm, MPI_STATUS_IGNORE);

            double t_end = MPI_Wtime();  // End timer

            // Discard first iteration... may have cold-start overhead
            if (n > 0)  
                total_time += (t_end - t_start);
        }

        // Compute the average time (excluding the first iteration)
        double avg_time = total_time / (NEXPERIMENTS - 1);
        int nanosec = avg_time * 1e9;  // nanoseconds

        // Print average time for each sender-receiver pair
        proctext << "Process " << procno << " <-> " << partner
                 << " Ping-Pong Time: " << fixed << setprecision(3)
                 << nanosec * 1.e-3 << " (microsec)" << endl;
        cerr << proctext.str();
    } else {
        // Receiver processes
        partner = procno - nprocs / 2;  // Each receiver pairs with sender in first half

        for (int n = 0; n < NEXPERIMENTS; n++) {
            // Receive message from sender
            MPI_Recv(recv, MESSAGE_SIZE, MPI_DOUBLE, partner, 0, comm, MPI_STATUS_IGNORE);
            // Send message back to sender
            MPI_Send(recv, MESSAGE_SIZE, MPI_DOUBLE, partner, 0, comm);
        }
    }
    MPI_Finalize();
    return 0;
}

