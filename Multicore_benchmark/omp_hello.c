/******************************************************************************
* FILE: omp_hello.c
* DESCRIPTION:
*   OpenMP Example - Hello World - C/C++ Version
*   In this simple example, the master thread forks a parallel region.
*   All threads in the team obtain their unique thread number and print it.
*   The master thread only prints the total number of threads.  Two OpenMP
*   library routines are used to obtain the number of threads and each
*   thread's number.
* AUTHOR: Blaise Barney  5/99
* LAST REVISED: 04/06/05
******************************************************************************/
#include <ti/omp/omp.h>

#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <ti/csl/csl_tsc.h> // Needed for TSC_read()
#include "mathlib_tests.h"

#define NTHREADS  4

#define SIZE 150*1024
//unsigned int volatile cregister TSCL;
//unsigned int volatile cregister TSCH;

#pragma DATA_SECTION(A, ".damian")
int A[SIZE];

void main()
{

int N = SIZE;

//int B[20][20];
//int C[20];
int nthreads, id, i;
//int sum;

// Timing variables
uint64_t g_ui64StartTime;
uint64_t g_ui64StopTime;
uint64_t g_ui64ElapsedTime;


nthreads = NTHREADS;
omp_set_num_threads(NTHREADS);

id = omp_get_thread_num();
printf("Core = %d, This is serial\n", id);
CSL_tscEnable();
//	g_ui64StartTime = (uint64_t)(TSCL) ;
//	g_ui64StartTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;

#pragma omp parallel private(i,id,g_ui64ElapsedTime,g_ui64StopTime,g_ui64StartTime) shared(N,A)
{
	CSL_tscEnable();
		g_ui64StartTime = (uint64_t)(TSCL) ;
		g_ui64StartTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;



#pragma omp for
	for(i=0;i<N;i++)
	{
		A[i] = log10sp_i(A[i]);
	}


		g_ui64StopTime = (uint64_t)(TSCL) ;
		g_ui64StopTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;
		g_ui64ElapsedTime = g_ui64StopTime - g_ui64StartTime;

		id = omp_get_thread_num();
	printf("Core = %d, Done in %llu cycles\n", id,g_ui64ElapsedTime);
}

//g_ui64StopTime = (uint64_t)(TSCL) ;
//g_ui64StopTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;
//g_ui64ElapsedTime = g_ui64StopTime - g_ui64StartTime;
//
//id = omp_get_thread_num();
//printf("Core = %d, Done in %llu cycles\n", id,g_ui64ElapsedTime);

//#pragma omp parallel private(i) shared(A)
//{
//	int id,istart, iend;
//	id = omp_get_thread_num();
//	nthreads = omp_get_num_threads();
//	istart = id*N/nthreads;
//	iend = (id+1)*N/nthreads;
//	for(i=istart;i<iend;i++)
//	{
//		A[i]=id+i;
//		printf("Core = %d, result[%d]= %d CLK cycles\n", id,i,A[i]);
//	}
//	printf("Core = %d is done!", id);
//}

id = omp_get_thread_num();
printf("Core = %d, Done with loop\n", id);

//#pragma omp parallel private(i,j,sum,g_ui64ElapsedTime,g_ui64StopTime,g_ui64StartTime) shared(A,B,C)
//  {
//	CSL_tscEnable();
//
//	sum = 0;
//	for(i=0;i<5;i++)
//	{
//		//sum = 0;
////		g_ui64StartTime = CSL_tscRead();
//
//		g_ui64StartTime = (uint64_t)(TSCL) ;
//				g_ui64StartTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;
//		A[i] = (sum++)*i;
//
//	    // Get Stop Time
////		g_ui64StopTime =  CSL_tscRead();
//
//				g_ui64StopTime = (uint64_t)(TSCL) ;
//				g_ui64StopTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;
//
//
//
//	    g_ui64ElapsedTime = g_ui64StopTime - g_ui64StartTime;
//
//		tid = omp_get_thread_num();
//		printf("Core = %d, result[%d]= %d, Elapsed time for sum++: %llu\n", tid,i,A[i],g_ui64ElapsedTime);
//	//
//	}
//
//
//  }
  while(1);
}

///* Fork a team of threads giving them their own copies of variables */
//#pragma omp parallel private(nthreads, tid)
//  {
//
//  /* Obtain thread number */
//  tid = omp_get_thread_num();
//  printf("Hello World from thread = %d\n", tid);
//
//  /* Only master thread does this */
//  if (tid == 0)
//    {
//    nthreads = omp_get_num_threads();
//    printf("Number of threads = %d\n", nthreads);
//    }
//
//  }  /* All threads join master thread and disband */
//
//}




