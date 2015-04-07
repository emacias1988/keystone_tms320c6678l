#include "mathlib_tests.h"

struct timespec t2,t3;
double dElapsedTime;

char * g_pcDestBuffer;

bool cmpsp (char a, char b)
{
	if(a>b)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CMP_runTests(char * psFuncName, uint32_t ui32Size, char * pcSrcBuffer, char * pcDestBuffer)
{
	g_pcDestBuffer = pcDestBuffer;

	CMP_spTest(psFuncName,ui32Size,pcSrcBuffer);
}

void CMP_spTest(char * psFuncName, uint32_t ui32Size, char * pcBuffer)
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
		pcBuffer[ui32Idx] = rand()%0xFF+1;
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
			g_pcDestBuffer[ui32Idx] = cmpsp(pcBuffer[ui32Idx],pcBuffer[ui32IdxCount-1-ui32Idx]);
		}

		ui32TotalSize = ui32TotalSize - ui32IdxCount;
	}

	// Get Stop Time
	clock_gettime(CLOCK_MONOTONIC, &t3);
	dElapsedTime = ((t3.tv_sec - t2.tv_sec)*1e9+(double)(t3.tv_nsec-t2.tv_nsec));

	printf ("MATHLIB_TEST %sdp_i %u%s(%u) words Elapsed_Cycles: %f Elapsed_Time: %f (ns) Cycles/Word: %f \n\n",
			psFuncName,
			ui32SizePrint,
			pcSizeString,
			ui32Size,
			(dElapsedTime * ((double)g_ui16CPUFrequency / 1000)),
			dElapsedTime,
			(dElapsedTime*(double)(g_ui16CPUFrequency/1000)/ui32Size));
}
