/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-7
 ****
 **** MPI Exercise
 ****
 ****************************************************************/
#include <iostream>
#include <iomanip>
#include <sstream>
#include <mpi.h>
#include <unistd.h> // for hostname
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

    gethostname(hostname, HOST_NAME_MAX); // get the hostname

    #define NEXPERIMENTS 1001  // One extra iteration to discard
    #define MESSAGE_SIZE 1
//    #define MESSAGE_SIZE 8      // 8 doubles (64 bytes)
//    #define MESSAGE_SIZE 64     // 64 doubles (512 bytes)
//    #define MESSAGE_SIZE 1024   // 1024 doubles (8192 bytes)

    int processA = 0, processB = 1;
    double send[10000], recv[10000];

    send[0] = 1.1;

    //print process rank and its node info
    cout << "process " << procno << "is running on node " << hostname << endl;

    if (procno == processA) {
        double total_time = 0.0;

        for (int n = 0; n < NEXPERIMENTS; n++) {
            double t_start = MPI_Wtime();

            MPI_Send(send, MESSAGE_SIZE, MPI_DOUBLE, processB, 0, comm);
            MPI_Recv(recv, MESSAGE_SIZE, MPI_DOUBLE, processB, 0, comm, MPI_STATUS_IGNORE);

            double t_end = MPI_Wtime();

            if (n > 0)  // Discard first iteration
                total_time += (t_end - t_start);
        }

        double avg_time = total_time / (NEXPERIMENTS - 1);  // Exclude first run
        int nanosec = avg_time * 1e9;  // Convert to nanoseconds

        proctext << "Time for pingpong: " << fixed << setprecision(3)
                 << nanosec * 1.e-3 << " (microsec)" << endl;
        cerr << proctext.str();
    } else if (procno == processB) {
        for (int n = 0; n < NEXPERIMENTS; n++) {
            MPI_Recv(recv, MESSAGE_SIZE, MPI_DOUBLE, processA, 0, comm, MPI_STATUS_IGNORE);
            MPI_Send(recv, MESSAGE_SIZE, MPI_DOUBLE, processA, 0, comm);
        }
    }

    MPI_Finalize();
    return 0;
}
