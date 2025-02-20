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

using namespace std;

int main() {
    // Initialize MPI environment
    MPI_Comm comm = MPI_COMM_WORLD;
    int nprocs, procno;
    stringstream proctext;
    MPI_Init(0, 0);
    MPI_Comm_size(comm, &nprocs);
    MPI_Comm_rank(comm, &procno);

    // Define the number of experiments and message size
    #define NEXPERIMENTS 1001  // One extra iteration to discard
    #define MESSAGE_SIZE 1
    // #define MESSAGE_SIZE 8      // 8 doubles (64 bytes)
    // #define MESSAGE_SIZE 64     // 64 doubles (512 bytes)
    // #define MESSAGE_SIZE 1024   // 1024 doubles (8192 bytes)

    // Ensure we have an even number of processes
    if (nprocs % 2 != 0) {
        if (procno == 0) {
            cerr << "This program requires an even number of processes." << endl;
        }
        MPI_Finalize();
        return 1;
    }

    int partner;  // Variable to store the partner process number
    double send[10000], recv[10000];
    send[0] = 1.1;  // Initialize the send buffer with some value

    /*
        Divide the processes into two groups:
        - First half (rank < nprocs / 2) acts as **senders**
        - Second half (rank >= nprocs / 2) acts as **receivers**
        
        Each sender `i` sends data to a corresponding receiver `i + nprocs / 2`.
    */
    if (procno < nprocs / 2) {
        // Sender processes
        partner = procno + nprocs / 2;  // Each sender pairs with a receiver in the second half
        double total_time = 0.0;

        for (int n = 0; n < NEXPERIMENTS; n++) {
            double t_start = MPI_Wtime();  // Start timing the communication

            // Send a message to the corresponding receiver
            MPI_Send(send, MESSAGE_SIZE, MPI_DOUBLE, partner, 0, comm);
            // Receive the message back from the receiver
            MPI_Recv(recv, MESSAGE_SIZE, MPI_DOUBLE, partner, 0, comm, MPI_STATUS_IGNORE);

            double t_end = MPI_Wtime();  // End timing

            // Discard the first iteration as it may have cold-start overhead
            if (n > 0)  
                total_time += (t_end - t_start);
        }

        // Compute the average round-trip time (excluding the first iteration)
        double avg_time = total_time / (NEXPERIMENTS - 1);
        int nanosec = avg_time * 1e9;  // Convert seconds to nanoseconds

        // Print the average time for each sender-receiver pair
        proctext << "Process " << procno << " <-> " << partner
                 << " Ping-Pong Time: " << fixed << setprecision(3)
                 << nanosec * 1.e-3 << " (microsec)" << endl;
        cerr << proctext.str();
    } else {
        // Receiver processes
        partner = procno - nprocs / 2;  // Each receiver pairs with a sender in the first half

        for (int n = 0; n < NEXPERIMENTS; n++) {
            // Receive a message from the sender
            MPI_Recv(recv, MESSAGE_SIZE, MPI_DOUBLE, partner, 0, comm, MPI_STATUS_IGNORE);
            // Send the message back to the sender
            MPI_Send(recv, MESSAGE_SIZE, MPI_DOUBLE, partner, 0, comm);
        }
    }

    // Finalize MPI environment
    MPI_Finalize();
    return 0;
}

