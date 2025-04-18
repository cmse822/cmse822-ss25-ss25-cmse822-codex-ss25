/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-2025
 ****
 **** MPI Exercise for Isend/Irecv, sending an array
 ****
 ****************************************************************/

 #include <iostream>
 #include <sstream>
 #include <vector>
 using namespace std;
 #include <mpi.h>
 
 #include "tools.h"
 
 int main(int argc,char **argv) {
 
   MPI_Comm comm = MPI_COMM_WORLD;
   int nprocs, procno;
   stringstream proctext;
 
   MPI_Init(0,0);
 
   MPI_Comm_size(comm,&nprocs);
   MPI_Comm_rank(comm,&procno);
 
 #define N 100
   int mylocal = N;
   vector<double> indata(mylocal),outdata(mylocal);
   int myfirst=0;
   MPI_Exscan( &mylocal,&myfirst,1,MPI_INT,MPI_SUM,comm);
 
   double leftdata=0.,rightdata=0.;
   int sendto,recvfrom;
   MPI_Request requests[4];
 
   // Exercise:
   // -- set `sendto' and `recvfrom' twice
   //    once to get data from the left, once from the right
   // -- for first/last process use MPI_PROC_NULL
 
   // Data to be sent
   double first = myfirst;
   double last = myfirst + mylocal - 1;

   // first specify left neighbor data
   if (procno == 0) {
    sendto = MPI_PROC_NULL;
   } else {
    sendto = procno - 1;
   }
   recvfrom = sendto;
  
   MPI_Isend(&first, 1,MPI_DOUBLE, sendto,0, comm,requests);
   MPI_Irecv(&leftdata,1,MPI_DOUBLE, recvfrom,0, comm,requests + 1);
 
   // then the right neighbor data
   if (procno == nprocs - 1) {
    sendto = MPI_PROC_NULL;
   } else {
    sendto = procno + 1;
   }
   recvfrom = sendto;

   MPI_Isend(&last, 1,MPI_DOUBLE, sendto,0, comm,requests + 2);
   MPI_Irecv(&rightdata,1,MPI_DOUBLE, recvfrom,0, comm,requests + 3);
 
   // Modifying local array can be done while waiting
   // (Latency hiding)
   for (int i=0; i<mylocal; i++)
     indata[i] = myfirst+i;

   // Compute outdata for all inner elements
   // Neighbor data is not required
   for (int i=1; i<mylocal-1; i++)
     outdata[i] = indata[i-1] + indata[i] + indata[i+1];

   // make sure all irecv/isend operations are concluded
   MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);
 
   // Compute outdata for boundaries
   outdata[0] = leftdata + indata[0] + indata[1];
   outdata[mylocal-1] = indata[mylocal-2] + indata[mylocal-1] + rightdata;
   
   /*
    * Check correctness of the result:
    * value should be 3 times the invalue, except at the end points
    */
   vector<double> answer(mylocal);
   for (int i=0; i<mylocal; i++) {
     answer[i] = 3*(myfirst+i)
       - ( procno==0 && i==0 ) * (-1) // fictitious leftleft elements
       - ( procno==nprocs-1 && i==mylocal-1) * (myfirst+mylocal) // rightright
       ;
     //printf("%e ",answer[i]);
   }
   double error_test = array_error(answer,outdata);
   print_final_result( error_test>nprocs*1.e-14,comm);
 
   MPI_Finalize();
   return 0;
 }