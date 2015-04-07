#include "mathlib_tests.h"

struct timespec t2,t3;
double dElapsedTime;

float * g_pfDestBuffer;
double * g_pdDestBuffer;

void LOG_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer)
{
	g_pfDestBuffer = pfDestBuffer;
	g_pdDestBuffer = (double *) pfDestBuffer;

	LOG_spTest(psFuncName,ui32Size,pfSrcBuffer);

	LOG_dpTest(psFuncName,ui32Size,pfSrcBuffer);
}

void LOG_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer)
{
	uint32_t ui32Idx;
	uint32_t ui32IdxCount;;
	uint32_t ui32TotalSize;
	char * pcSizeString;
	uint32_t ui32SizePrint;

	ui32TotalSize = ui32Size;


	// The size will be displayed in K
	if(ui32TotalSize < (1024*1024))
	{
		ui32SizePrint = ui32Size / (1024);
		pcSizeString = "K";
	}
	// The size will be displayed in WORDS
	else if(ui32TotalSize < (1024*1024*1024))
	{
		ui32SizePrint = ui32Size / (1024*1024);
		pcSizeString = "M";
	}
	// The size will be displayed
	else
	{
		ui32SizePrint = ui32Size / (1024*1024*1024);
		pcSizeString = "G";
	}

	//
	// Initialize Buffer with known values
	//
	if(ui32TotalSize < DESTBUFFERSIZE)
	{
		ui32IdxCount = ui32TotalSize;
	}
	else
	{
		ui32IdxCount = DESTBUFFERSIZE;
	}


	for(ui32Idx=0;ui32Idx<ui32IdxCount;ui32Idx++)
	{
		pfBuffer[ui32Idx] = ui32Idx+0.001;
	}

	//
	//   Single Precision Single Input Test
	//

	//
	// Initialize Complete
	//

	// Get Start Time
	clock_gettime(CLOCK_MONOTONIC, &t2);

	while(ui32TotalSize)
	{
		if(ui32TotalSize < DESTBUFFERSIZE)
		{
			ui32IdxCount = ui32TotalSize;

		}
		else
		{
			ui32IdxCount = DESTBUFFERSIZE;
		}

		for(ui32Idx=0;ui32Idx<ui32IdxCount;ui32Idx++)
		{
			g_pfDestBuffer[ui32Idx] = logf(pfBuffer[ui32Idx]);
		}

		ui32TotalSize = ui32TotalSize - ui32IdxCount;
	}

	// Get Stop Time
	clock_gettime(CLOCK_MONOTONIC, &t3);
	dElapsedTime = ((t3.tv_sec - t2.tv_sec)*1e9+(double)(t3.tv_nsec-t2.tv_nsec));

	printf ("MATHLIB_TEST %ssp_i %u%s(%u) words Elapsed_Cycles: %f Elapsed_Time: %f (ns) Cycles/Word: %f \n\n",
			psFuncName,
			ui32SizePrint,
			pcSizeString,
			ui32Size,
			(dElapsedTime * ((double)g_ui16CPUFrequency / 1000)),
			dElapsedTime,
			(dElapsedTime*(double)(g_ui16CPUFrequency/1000)/ui32Size));
}

void LOG_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer)
{
	uint32_t ui32Idx;
	uint32_t ui32IdxCount;;
	uint32_t ui32TotalSize;
	char * pcSizeString;
	uint32_t ui32SizePrint;

	volatile double * pdBuffer;

	pdBuffer = (double *) pfBuffer;

	ui32TotalSize = ui32Size;

	// The size will be displayed in K
	if(ui32TotalSize < (1024*1024))
	{
		ui32SizePrint = ui32Size / (1024);
		pcSizeString = "K";
	}
	// The size will be displayed in WORDS
	else if(ui32TotalSize < (1024*1024*1024))
	{
		ui32SizePrint = ui32Size / (1024*1024);
		pcSizeString = "M";
	}
	// The size will be displayed
	else
	{
		ui32SizePrint = ui32Size / (1024*1024*1024);
		pcSizeString = "G";
	}

	ui32TotalSize = ui32Size >> 1;

	//
	// Initialize Buffer with known values
	//
	if(ui32TotalSize < (DESTBUFFERSIZE >> 1) )
	{
		ui32IdxCount = ui32TotalSize;
	}
	else
	{
		ui32IdxCount = (DESTBUFFERSIZE >> 1);
	}


	for(ui32Idx=0;ui32Idx<ui32IdxCount;ui32Idx++)
	{
		pdBuffer[ui32Idx] = ui32Idx+0.001;
	}

	//
	//   Double Precision Single Input Test
	//

	//
	// Initialize Complete
	//

	// Get Start Time
	clock_gettime(CLOCK_MONOTONIC, &t2);

	while(ui32TotalSize)
	{
		if(ui32TotalSize < (DESTBUFFERSIZE >> 1))
		{
			ui32IdxCount = ui32TotalSize;

		}
		else
		{
			ui32IdxCount = (DESTBUFFERSIZE >> 1);
		}

		for(ui32Idx=0;ui32Idx<ui32IdxCount;ui32Idx++)
		{
			g_pdDestBuffer[ui32Idx] = log(pdBuffer[ui32Idx]);
		}

		ui32TotalSize = ui32TotalSize - ui32IdxCount;
	}

	// Get Stop Time
	clock_gettime(CLOCK_MONOTONIC, &t3);
	dElapsedTime = ((t3.tv_sec - t2.tv_sec)*1e9+(double)(t3.tv_nsec-t2.tv_nsec));

	ui32TotalSize = ui32Size >> 1;

	//
	// Initialize Buffer with known values
	//
	if(ui32TotalSize < (DESTBUFFERSIZE >> 1) )
	{
		ui32IdxCount = ui32TotalSize;
	}
	else
	{
		ui32IdxCount = (DESTBUFFERSIZE >> 1);
	}

	for(ui32Idx=0;ui32Idx<ui32IdxCount;ui32Idx++)
	{
		pdBuffer[ui32Idx] = ui32Idx+0.002;
	}

	// Get Start Time
	clock_gettime(CLOCK_MONOTONIC, &t2);

	while(ui32TotalSize)
	{
		if(ui32TotalSize < (DESTBUFFERSIZE >> 1))
		{
			ui32IdxCount = ui32TotalSize;

		}
		else
		{
			ui32IdxCount = (DESTBUFFERSIZE >> 1);
		}

		for(ui32Idx=0;ui32Idx<ui32IdxCount;ui32Idx++)
		{
			g_pdDestBuffer[ui32Idx] = log(pdBuffer[ui32Idx]);
		}

		ui32TotalSize = ui32TotalSize - ui32IdxCount;
	}

	// Get Stop Time
	clock_gettime(CLOCK_MONOTONIC, &t3);
	dElapsedTime = dElapsedTime + ((t3.tv_sec - t2.tv_sec)*1e9+(double)(t3.tv_nsec-t2.tv_nsec));

	printf ("MATHLIB_TEST %sdp_i %u%s(%u) words Elapsed_Cycles: %f Elapsed_Time: %f (ns) Cycles/Word: %f \n\n",
			psFuncName,
			ui32SizePrint,
			pcSizeString,
			ui32Size,
			(dElapsedTime * ((double)g_ui16CPUFrequency / 1000)),
			dElapsedTime,
			(dElapsedTime*(double)(g_ui16CPUFrequency/1000)/ui32Size));
}
