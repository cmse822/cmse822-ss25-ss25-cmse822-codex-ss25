/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-9
 ****
 **** MPI Exercise for Isend/Irecv
 **** Exercise 4.13
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
 
   // Exercise:
   // -- set `sendto' and `recvfrom' twice
   //    once to get data from the left, once from the right
   // -- for first/last process use MPI_PROC_NULL
 
   // get data from the left: who are you communicating with?
   if (procno == 0) {
    sendto = MPI_PROC_NULL;
   } else {
    sendto = procno - 1;
   }
   recvfrom = sendto;

   MPI_Isend(&mydata,1,MPI_DOUBLE, sendto,0, comm, requests);
   MPI_Irecv(&leftdata,1,MPI_DOUBLE, recvfrom,0, comm, requests + 1);
 
   // get data from the right: who are you communicating with?
   if (procno == nprocs - 1) {
    sendto = MPI_PROC_NULL;
   } else {
    sendto = procno + 1;
   }
   recvfrom = sendto;

   MPI_Isend(&mydata,1,MPI_DOUBLE, sendto,0, comm, requests + 2);
   MPI_Irecv(&rightdata,1,MPI_DOUBLE, recvfrom,0, comm, requests + 3);
 
   //
   // Now make sure all Isend/Irecv operations are completed
   //
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