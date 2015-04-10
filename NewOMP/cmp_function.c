#include "EVM6678.h"
#include "cmp_function.h"

void CMP_runTests(char * psFuncName, uint32_t ui32Size, char * pcBuffer)
{
	CMP_spTest(psFuncName,ui32Size,pcBuffer);
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
	g_ui64StartTime = (uint64_t)(TSCL) ;
	g_ui64StartTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;

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

#pragma omp parallel private(ui32Idx) shared(ui32IdxCount,pcBuffer)
{

#pragma omp for
		for(ui32Idx=0;ui32Idx<ui32IdxCount;ui32Idx++)
		{
			pcBuffer[ui32Idx] = cmpsp_i(pcBuffer[ui32Idx],(char)0x45);
		}
}
		ui32TotalSize = ui32TotalSize - ui32IdxCount;
	}

	// Get Stop Time
	g_ui64StopTime = (uint64_t)(TSCL) ;
	g_ui64StopTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;
	g_ui64ElapsedTime = g_ui64StopTime - g_ui64StartTime;


	printf ("MATHLIB_TEST %s %d%s(%d) words Elapsed_Cycles: %llu Elapsed_Time: %llu (ns) Cycles/Word: %llu \n\n",
			psFuncName,
			ui32SizePrint,
			pcSizeString,
			ui32Size,
			(uint64_t)g_ui64ElapsedTime,
			(uint64_t)(g_ui64ElapsedTime),// * (1000 / g_sEvmInfo.frequency)),
			(uint64_t)(g_ui64ElapsedTime/ui32Size));
}
