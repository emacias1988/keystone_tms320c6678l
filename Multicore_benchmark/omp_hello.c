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
#include "mathlib_tests.h"
#include "EVM6678.h"
#include "testselector.h"

#include "platform.h"

#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <ti/csl/csl_tsc.h> // Needed for TSC_enable()



/**********************************************************************
 ************************** Dr. Jung Defines **************************
 **********************************************************************/
#define NTHREADS  2	//Number of cores
#pragma DATA_SECTION(pui32DestBuffer, ".damian")
//#pragma DATA_ALIGN(pui32DestBuffer, 128);
uint32_t pui32DestBuffer[DESTBUFFERSIZE];

// Timing Variables
uint64_t g_ui64StartTime;
uint64_t g_ui64StopTime;
uint64_t g_ui64ElapsedTime;

//unsigned int volatile cregister TSCL;
//unsigned int volatile cregister TSCH;
platform_info	g_sEvmInfo;

uint64_t Osal_calculateElapsedTime(uint64_t ui64Start, uint64_t ui64Stop)
{
	//
	// There is a problem when register overflows more than once
	//
	uint64_t ui64ElapsedTime;

	if(ui64Start >= ui64Stop)
	{
		ui64ElapsedTime = ui64Stop + (0xFFFFFFFFFFFFFFFF - ui64Start);
	}
	else
	{
		ui64ElapsedTime = ui64Stop - ui64Start;
	}
	return ui64ElapsedTime;
}



void main()
{




int nthreads, id;// i;
volatile uint32_t ui32Size;

float * pfBuffer = (float*)pui32DestBuffer;
//float * pfBuffer;




nthreads = NTHREADS;
omp_set_num_threads(NTHREADS);	  // Set number of active cores (must set in omp_config.cfg too)
id = omp_get_thread_num(); 		  // Get master core

//
// Enable TSC registers in all cores
//

//#pragma omp parallel
//{
	CSL_tscEnable();
//}


printf("\nStarting Benchmark Tests for %d cores...\n\n",nthreads);
printf("Master core is core %d\n\n",id);
g_ui64StartTime = (uint64_t)(((uint64_t)TSCH << 32 ) | TSCL) ;
g_ui64StopTime = (uint64_t)(((uint64_t)TSCH << 32 ) | TSCL) ;

g_ui64ElapsedTime = g_ui64StopTime - g_ui64StartTime;
    printf ("Time to start and stop timer: %u cycles \n\n",
    	          		g_ui64ElapsedTime);



#if OPERATIONS

#if FOUR_K
    ui32Size = 4*4*KB;
 //   pfBuffer = (float *)malloc(DESTBUFFERSIZE*sizeof(int));
	LOG_runTests("log", ui32Size,pfBuffer);

#endif
#endif
//	g_ui64StartTime = (uint64_t)(TSCL) ;
//	g_ui64StartTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;

//#pragma omp parallel private(i,id,g_ui64ElapsedTime,g_ui64StopTime,g_ui64StartTime) shared(N,pui32DestBuffer)
//{
//	CSL_tscEnable();
//		g_ui64StartTime = (uint64_t)(TSCL) ;
//		g_ui64StartTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;
//
//
//
//#pragma omp for
//	for(i=0;i<N;i++)
//	{
//		pui32DestBuffer[i] = log10sp_i(pui32DestBuffer[i]);
//	}
//
//
//		g_ui64StopTime = (uint64_t)(TSCL) ;
//		g_ui64StopTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;
//		g_ui64ElapsedTime = g_ui64StopTime - g_ui64StartTime;
//
//		id = omp_get_thread_num();
//	printf("Core = %d, Done in %llu cycles\n", id,g_ui64ElapsedTime);
//}


id = omp_get_thread_num();
printf("Core = %d, Done with loop\n", id);


  while(1);
}





