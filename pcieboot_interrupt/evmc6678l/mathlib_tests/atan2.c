#include "mathlib_tests.h"

void ATAN2_runTests(char * psFuncName, uint32_t ui32Size, float * pfBuffer)
{
	ATAN2_spTest(psFuncName,ui32Size,pfBuffer);

	ATAN2_dpTest(psFuncName,ui32Size,pfBuffer);
}

void ATAN2_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer)
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

		for(ui32Idx=0;ui32Idx<ui32IdxCount;ui32Idx++)
		{
			pfBuffer[ui32Idx] = atan2sp(pfBuffer[ui32Idx],pfBuffer[ui32IdxCount-1-ui32Idx]);
		}

		ui32TotalSize = ui32TotalSize - ui32IdxCount;
	}

	// Get Stop Time
	g_ui64StopTime = (uint64_t)(TSCL) ;
	g_ui64StopTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;
	g_ui64ElapsedTime = g_ui64StopTime - g_ui64StartTime;


	printf ("MATHLIB_TEST %ssp %d%s(%d) words Elapsed_Cycles: %llu Elapsed_Time: %llu (ns) Cycles/Word: %llu \n\n",
			psFuncName,
			ui32SizePrint,
			pcSizeString,
			ui32Size,
			(uint64_t)g_ui64ElapsedTime,
			(uint64_t)(g_ui64ElapsedTime * (1000 / g_sEvmInfo.frequency)),
			(uint64_t)(g_ui64ElapsedTime/ui32Size));

	//
	// Single Precision Inline Single Input
	//

	ui32TotalSize = ui32Size;

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

		for(ui32Idx=0;ui32Idx<ui32IdxCount;ui32Idx++)
		{
			pfBuffer[ui32Idx] = atan2sp_i(pfBuffer[ui32Idx],pfBuffer[ui32IdxCount-1-ui32Idx]);
		}

		ui32TotalSize = ui32TotalSize - ui32IdxCount;
	}

	// Get Stop Time
	g_ui64StopTime = (uint64_t)(TSCL) ;
	g_ui64StopTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;
	g_ui64ElapsedTime = g_ui64StopTime - g_ui64StartTime;


	printf ("MATHLIB_TEST %ssp_i %d%s(%d) words Elapsed_Cycles: %llu Elapsed_Time: %llu (ns) Cycles/Word: %llu \n\n",
			psFuncName,
			ui32SizePrint,
			pcSizeString,
			ui32Size,
			(uint64_t)g_ui64ElapsedTime,
			(uint64_t)(g_ui64ElapsedTime * (1000 / g_sEvmInfo.frequency)),
			(uint64_t)(g_ui64ElapsedTime/ui32Size));

}

void ATAN2_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer)
{
	uint32_t ui32Idx;
	uint32_t ui32IdxCount;;
	uint32_t ui32TotalSize;
	char * pcSizeString;
	uint32_t ui32SizePrint;

	volatile double * pdBuffer;

	ui32TotalSize = ui32Size;
	pdBuffer = (double *) pfBuffer;


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
	ui32TotalSize = ui32Size >> 1;

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
			pdBuffer[ui32Idx] = atan2dp(pdBuffer[ui32Idx],pdBuffer[ui32IdxCount-1-ui32Idx]);
		}

		ui32TotalSize = ui32TotalSize - ui32IdxCount;
	}

	// Get Stop Time
	g_ui64StopTime = (uint64_t)(TSCL) ;
	g_ui64StopTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;
	g_ui64ElapsedTime = g_ui64StopTime - g_ui64StartTime;

	printf ("MATHLIB_TEST %sdp %d%s(%d) words Elapsed_Cycles: %llu Elapsed_Time: %llu (ns) Cycles/Word: %llu \n\n",
			psFuncName,
			ui32SizePrint,
			pcSizeString,
			ui32Size,
			(uint64_t)g_ui64ElapsedTime,
			(uint64_t)(g_ui64ElapsedTime * (1000 / g_sEvmInfo.frequency)),
			(uint64_t)(g_ui64ElapsedTime/ui32Size));

	//
	// Single Precision Inline Single Input
	//

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
			pdBuffer[ui32Idx] = atan2dp_i(pdBuffer[ui32Idx],pdBuffer[ui32IdxCount-1-ui32Idx]);
		}

		ui32TotalSize = ui32TotalSize - ui32IdxCount;
	}

	// Get Stop Time
	g_ui64StopTime = (uint64_t)(TSCL) ;
	g_ui64StopTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;
	g_ui64ElapsedTime = g_ui64StopTime - g_ui64StartTime;

	printf ("MATHLIB_TEST %sdp_i %d%s(%d) words Elapsed_Cycles: %llu Elapsed_Time: %llu (ns) Cycles/Word: %llu \n\n",
			psFuncName,
			ui32SizePrint,
			pcSizeString,
			ui32Size,
			(uint64_t)g_ui64ElapsedTime,
			(uint64_t)(g_ui64ElapsedTime * (1000 / g_sEvmInfo.frequency)),
			(uint64_t)(g_ui64ElapsedTime/ui32Size));

}



