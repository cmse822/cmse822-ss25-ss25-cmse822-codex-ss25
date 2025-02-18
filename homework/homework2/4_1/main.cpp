/**
**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
****
**** copyright Victor Eijkhout 2012-7
****
**** MPI Exercise
****
****************************************************************/

// mpicc -std=c++17 main.cpp -lstdc++ -O3
// mpirun -n 2 ./a.out 10000

#include <iomanip>
#include <iostream>
#include <mpi.h>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
  int NEXPERIMENTS = 1'000'000;
  if(argc > 1) {
    NEXPERIMENTS = std::stoi(argv[1]);
  }
  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  std::stringstream proctext;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(comm, &nprocs);
  MPI_Comm_rank(comm, &procno);

  // Exercise:
  // -- set source and target processors two ways:
  //    close together and far apart
  // -- run the experiment both ways.

  int processA = 0;
  int processB = nprocs - 1;
  double t, send[10000], recv[10000];
  send[0] = 1.1;
  if (procno == processA) {
    t = MPI_Wtime();
    for (int n = 0; n < NEXPERIMENTS; n++) {

      MPI_Send(send,       // Buffer
               1,          // Number of elements to send in buffer
               MPI_DOUBLE, // Type sending in buffer
               processB,   // Destination rank
               0,          // Tag, usually 0
               comm);
      MPI_Recv(recv,       // Receive buffer
               1,          // Size of receiving buffer
               MPI_DOUBLE, // Type of receiving buffer
               processB,   // Receive from processB
               0,          // Tag 0
               comm, MPI_STATUS_IGNORE);
    }
    t = MPI_Wtime() - t;
    t /= NEXPERIMENTS;
    {
      int nanosec = t * 1000000000;
      proctext << nprocs << " - Time for pingpong: " << std::fixed << std::setprecision(3)
               << nanosec * 1.e-3 << " (microsec)" << std::endl;
    }
    std::cerr << proctext.str();
    proctext.clear();
  } else if (procno == processB) {
    for (int n = 0; n < NEXPERIMENTS; n++) {
      MPI_Recv(recv,       // Receive buffer
               1,          // Size of receiving buffer
               MPI_DOUBLE, // Type of receiving buffer
               processA,   // Receive from A
               0,          // Tag 0
               comm, MPI_STATUS_IGNORE);

      MPI_Send(send,       // Buffer
               1,          // Number of elements to send in buffer
               MPI_DOUBLE, // Type sending in buffer
               processA,   // Destination rank
               0,          // Tag, usually 0
               comm);
    }
  }

  MPI_Finalize();
  return 0;
}
