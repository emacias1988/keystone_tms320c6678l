/******************************************************************************
* Copyright (c) 2011 Texas Instruments Incorporated - http://www.ti.com
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated emac the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

/**************************************************************************************
* FILE PURPOSE: PCIe Boot Interrupt Example
**************************************************************************************
* FILE NAME: pcieboot_interrupt.c
*
* DESCRIPTION: A simple interrupt example demonstrating PCIe boot.
*
***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform.h"
#include "mathlib_tests.h"
#include "EVM6678.h"
#include "testselector.h"

//#include <ti/mathlib/src/common/tables.c>

#include <ti/csl/src/intc/csl_intc.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/csl_cpIntcAux.h>



#define DEVICE_REG32_W(x,y)   *(volatile uint32_t *)(x)=(y)
#define DEVICE_REG32_R(x)    (*(volatile uint32_t *)(x))

#define DDR_TEST_START                 0x80000000
#define DDR_TEST_END                   0x80400000
#define BOOT_UART_BAUDRATE                 115200
#define PCIEXpress_Legacy_INTA                 50
#define PCIE_IRQ_EOI                   0x21800050
#define PCIE_EP_IRQ_SET		           0x21800064
#define PCIE_LEGACY_A_IRQ_STATUS       0x21800184

#ifdef _EVMC6678L_
#define MAGIC_ADDR     0x87fffc
#define INTC0_OUT3     63 
#endif

#ifdef _EVMC6670L_
#define MAGIC_ADDR     0x8ffffc
#define INTC0_OUT3     59 
#endif


/**********************************************************************
 ************************** Dr. Jung Defines **************************
 **********************************************************************/


#define TEST_START_MEMORY_DDR3			0x80000000


void readMemForLoop();
uint32_t deviceSpeedRange;

//#define DESTBUFFERSIZE (400*KB)
uint32_t pui32DestBuffer[DESTBUFFERSIZE];

// Timing Variables
uint64_t g_ui64StartTime;
uint64_t g_ui64StopTime;
uint64_t g_ui64ElapsedTime;

uint64_t damian;

// Platform Information Variables
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


void readMemForLoop(uint32_t ui32WordsToRead, uint32_t * pui32DDRContent)
{
		uint32_t k =0;
		uint32_t inx = 0;

		// Get Start Time
		g_ui64StartTime = (uint64_t)(TSCL) ;
		g_ui64StartTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;
	//	g_ui64StartTime = TSCL ;

	    for(k=0;k<ui32WordsToRead;k++)
	    {
	    	pui32DestBuffer[inx++] = pui32DDRContent[k];
	    	if (inx >= DESTBUFFERSIZE)
	    	{
	    		inx = 0;
	    	}
	    }

	    // Get Stop Time
		g_ui64StopTime = (uint64_t)(TSCL) ;
		g_ui64StopTime |= (uint64_t)((uint64_t)TSCH << 32 ) ;

	    g_ui64ElapsedTime = Osal_calculateElapsedTime(g_ui64StartTime,g_ui64StopTime);
//	    printf ("Transfer: Elapsed Cycles: %llu Elapsed Time: %llu (ns) BW: %llu MB/s\n\n",
//	          		g_ui64ElapsedTime, (g_ui64ElapsedTime * (1000 / g_sEvmInfo.frequency)),
//	          		(int64_t) ((int64_t)ui32WordsToRead*4000) / ((int64_t)g_ui64ElapsedTime * (1000 / (int64_t)g_sEvmInfo.frequency)) );

	    printf ("Transfer: Elapsed Cycles: %llu Elapsed Time: %llu (ns) BW: %llu MB/s\n\n",
	  	          		(uint64_t)g_ui64ElapsedTime, (uint64_t)(g_ui64ElapsedTime * (1000 / g_sEvmInfo.frequency)),
	  	          	(uint64_t)( ((int64_t)ui32WordsToRead*4000) / ((int64_t)g_ui64ElapsedTime * (1000 / (int64_t)g_sEvmInfo.frequency))));

}




/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
/* Intc variable declaration */
CSL_CPINTC_Handle           hnd;
CSL_IntcContext             intcContext;
CSL_IntcEventHandlerRecord  EventHandler[30];
CSL_IntcObj                 intcObj;
CSL_IntcHandle              hTest;
CSL_IntcGlobalEnableState   state;
CSL_IntcEventHandlerRecord  EventRecord;
CSL_IntcParam               vectId;

uint32_t counter = 0;

void write_uart(char* msg)
{
    uint32_t i;
    uint32_t msg_len = strlen(msg);

    /* Write the message to the UART */
    for (i = 0; i < msg_len; i++)
    {
        platform_uart_write(msg[i]);
    }
}

void start_boot(void)
{
    void (*exit)();
    uint32_t entry_addr;

    /* Clear the boot entry address */
    DEVICE_REG32_W(MAGIC_ADDR, 0);

    while(1)
    {
        entry_addr = DEVICE_REG32_R(MAGIC_ADDR);
        if (entry_addr != 0)
        {
            /* jump to the exit point, which will be the entry point for the full IBL */
            exit = (void (*)())entry_addr;
            (*exit)();
        }
        platform_delay(1);
    }
}

/**********************************************************************
 ************************ CPINTC TEST FUNCTIONS ***********************
 **********************************************************************/

/**
 *  @b Description
 *  @n
 *      This is the TEST ISR Handler which has been installed.
 *      This simply increments a global variable which counts
 *      the number of interrupts which have been received.
 *
 *  @retval
 *      Not Applicable.
 */
static void test_isr_handler(void* handle)
{
	uint32_t i;

	counter++;
	printf    ("DSP receives interrupt from host.\n");
	write_uart("DSP receives interrupt from host.\n\r");

	/* Disable host interrupt */
	CSL_CPINTC_disableHostInterrupt (hnd, 3);

    /* do a simple operation in the DDR data */
    for (i = DDR_TEST_START; i < DDR_TEST_END; i += 4)
		*(uint32_t *)i = ~(*(uint32_t *)i);
	
	/* clear PCIE interrupt */
    DEVICE_REG32_W(PCIE_LEGACY_A_IRQ_STATUS, 0x1);
    DEVICE_REG32_W(PCIE_IRQ_EOI, 0x0);
    CSL_CPINTC_clearSysInterrupt (hnd, PCIEXpress_Legacy_INTA);

    /* Enable host interrupt */
    CSL_CPINTC_enableHostInterrupt (hnd, 3);

    /* generate interrupt to host */
	*((uint32_t *)PCIE_EP_IRQ_SET) = 0x1;
	printf    ("DSP generates interrupt to host.\n");
	write_uart("DSP generates interrupt to host.\n\r");
}	

/* OSAL functions for Platform Library */
uint8_t *Osal_platformMalloc (uint32_t num_bytes, uint32_t alignment)
{
	return malloc(num_bytes);
}

void Osal_platformFree (uint8_t *dataPtr, uint32_t num_bytes)
{
    /* Free up the memory */
    if (dataPtr)
    {
        free(dataPtr);
    }
}

void Osal_platformSpiCsEnter(void)
{
    return;
}

void Osal_platformSpiCsExit (void)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      Entry point for the test code.
 *
 *  @retval
 *      Not Applicable.
 */
void main (void)
{
	platform_init_flags     init_flags;
    platform_init_config    init_config;

    uint32_t ui32Status;

    // Read Test Variables
    uint32_t ui32WordsToRead;
    uint32_t ui32DDRContent;
    uint32_t * pui32DDRContent;

    uint8_t * pui8DDRContent;

    // For Loop Variables
    uint32_t i,j,k;

	volatile uint32_t ui32Size;

	float * pfBuffer = (float*)pui32DestBuffer;

    /* Initialize main Platform lib */
    memset(&init_config, 0, sizeof(platform_init_config));
    memset(&init_flags, 1, sizeof(platform_init_flags));

#if FLASHREAD
	init_flags.pll = 0;
    init_flags.ddr = 0;
#endif	
	
	
    init_config.pllm = 20;
//    init_config.plld = 2;
//    init_config.postdiv = 1;
//    init_config.prediv = 2;

    platform_init(&init_flags, &init_config);

    platform_get_info(&g_sEvmInfo);

#if FLASHREAD
		PLATFORM_DEVICE_info    *p_device;

		p_device = platform_device_open(PLATFORM_DEVID_NAND512R3A2D, 0);
			if (p_device == NULL) 
			{
				printf ("NAND device open failed!\n");
		  //      print_platform_errno();
				return;
			}
	
	/* Read a block of data */
	
	uint8_t pui8buff[100];
	
        if(platform_device_read(p_device->handle, 
                                0, 
                                pui8buff,
                                1) != Platform_EOK)
        {
            printf ("Failure in reading block %d\n", 0);
//            print_platform_errno();
//            if (platform_errno == PLATFORM_ERRNO_ECC_FAIL)
//            {
//                printf ("marking block %d as bad, re-flash attempted\n", block);
//                markBlockBad (p_device, block);
//            }
//            if (swap_byte) free (scrach_block);
//            return (FALSE);
        }

	
#endif

    printf("\nStarting Benchmark Tests ...\n\n");
    g_ui64StartTime = (uint64_t)(((uint64_t)TSCH << 32 ) | TSCL) ;
    g_ui64StopTime = (uint64_t)(((uint64_t)TSCH << 32 ) | TSCL) ;

    g_ui64ElapsedTime = g_ui64StopTime - g_ui64StartTime;
        printf ("Time to start and stop timer: %u cycles \n\n",
        	          		g_ui64ElapsedTime);

#if FOUR_K
#if DDR3_READ_EDMA
    //
    // 4kB Test - EDMA
    //
	ui32WordsToRead = 4*KB;
	pui32DDRContent = (uint32_t *) TEST_START_MEMORY_DDR3;
    printf ("Read Test 4KB using EDMA (DDR3->l2)@ DSP Freq %d (Mhz)\n", g_sEvmInfo.frequency);



    ui32Status = edma_ping_pong_xfer_gbl_region(0, 0,(uint32_t)pui32DDRContent, (uint32_t) pui32DestBuffer, ui32WordsToRead);
#endif

#if DDR3_READ_LOOP
    //
    // 4kB Test - For Loop
    //

    printf ("Read Test 4KB using for-loop (DDR3->l2)@ DSP Freq %d (Mhz)\n", g_sEvmInfo.frequency);

    ui32WordsToRead = 4*KB;
    pui32DDRContent = (uint32_t *) TEST_START_MEMORY_DDR3;

    readMemForLoop(ui32WordsToRead, pui32DDRContent );
#endif
#endif

#if THIRTY_TWO_K
#if DDR3_READ_EDMA
    //
    // 32kB Test - EDMA
    //
    printf ("Read Test 32KB using EDMA (DDR3->l2)@ DSP Freq %d (Mhz)\n", g_sEvmInfo.frequency);

    ui32WordsToRead = (32*KB);

    ui32Status = edma_ping_pong_xfer_gbl_region(0, 0,(uint32_t)pui32DDRContent, (uint32_t) pui32DestBuffer, ui32WordsToRead);
#endif
#if DDR3_READ_LOOP
      //
      // 32kB Test - For Loop
      //

    printf ("Read Test 32KB using for-loop (DDR3->l2)@ DSP Freq %d (Mhz)\n", g_sEvmInfo.frequency);

    ui32WordsToRead = (32*KB);
    pui32DDRContent = (uint32_t *) TEST_START_MEMORY_DDR3;

    readMemForLoop(ui32WordsToRead, pui32DDRContent );
#endif
#endif

#if ONE_M
#if DDR3_READ_EDMA
    //
    // 1MB Test - EDMA
    //
    printf ("Read Test 1MB using EDMA (DDR3->l2)@ DSP Freq %d (Mhz)\n", g_sEvmInfo.frequency);

    ui32WordsToRead = (1*MB);

    ui32Status = edma_ping_pong_xfer_gbl_region(0, 0,(uint32_t)pui32DDRContent, (uint32_t) pui32DestBuffer, ui32WordsToRead);
#endif
#if DDR3_READ_LOOP
    //
    // 1MB Test - For Loop
    //

    printf ("Read Test 1MB using for-loop (DDR3->l2)@ DSP Freq %d (Mhz)\n", g_sEvmInfo.frequency);

    ui32WordsToRead = 1*MB;
    pui32DDRContent = (uint32_t *) TEST_START_MEMORY_DDR3;

    readMemForLoop(ui32WordsToRead, pui32DDRContent );
#endif
#endif

#if ONE_G
#if DDR3_READ_EDMA
    //
    // 1GB Test - EDMA
    //
    printf ("Read Test 1GB using EDMA (DDR3->l2)@ DSP Freq %d (Mhz)\n", g_sEvmInfo.frequency);

    ui32WordsToRead = (1*GB);

    ui32Status = edma_ping_pong_xfer_gbl_region(0, 0,(uint32_t)pui32DDRContent, (uint32_t) pui32DestBuffer, ui32WordsToRead);
#endif
#if DDR3_READ_LOOP
    //
    // 1GB Test - For Loop
    //

    printf ("Read Test 1GB using for-loop (DDR3->l2)@ DSP Freq %d (Mhz)\n", g_sEvmInfo.frequency);

    ui32WordsToRead = 1*GB;
    pui32DDRContent = (uint32_t *) TEST_START_MEMORY_DDR3;

    readMemForLoop(ui32WordsToRead, pui32DDRContent );
#endif
#endif

#if TWO_G
#if DDR3_READ_EDMA
    //
    // 2GB Test - EDMA
    //
    printf ("Read Test 2GB using EDMA (DDR3->l2)@ DSP Freq %d (Mhz)\n", g_sEvmInfo.frequency);

    ui32WordsToRead = (2*GB);

    ui32Status = edma_ping_pong_xfer_gbl_region(0, 0,(uint32_t)pui32DDRContent, (uint32_t) pui32DestBuffer, ui32WordsToRead);
#endif
#if DDR3_READ_LOOP
    //
    // 2GB Test - For Loop
    //

    printf ("Read Test 2GB using for-loop (DDR3->l2)@ DSP Freq %d (Mhz)\n", g_sEvmInfo.frequency);
    ui32WordsToRead = 2*GB;
    pui32DDRContent = (uint32_t *) TEST_START_MEMORY_DDR3;

    readMemForLoop(ui32WordsToRead, pui32DDRContent );
#endif
#endif

    //
    // Testing Operations
    //

#define PRINTRESULT 0

#if OPERATIONS

#if FOUR_K
	ui32Size = 4*4*KB;
	ATAN_runTests("atan", ui32Size,pfBuffer);
	ATAN2_runTests("atan2", ui32Size,pfBuffer);
	COS_runTests("cos", ui32Size,pfBuffer);
	DIV_runTests("div", ui32Size,pfBuffer);
	EXP_runTests("exp", ui32Size,pfBuffer);
	EXP10_runTests("exp10", ui32Size,pfBuffer);
	EXP2_runTests("exp2", ui32Size,pfBuffer);
	LOG10_runTests("log10", ui32Size,pfBuffer);
	LOG2_runTests("log2", ui32Size,pfBuffer);
	LOG_runTests("log", ui32Size,pfBuffer);
	POW_runTests("pow", ui32Size,pfBuffer);
	RECIP_runTests("recip", ui32Size,pfBuffer);
	RSQRT_runTests("rsqrt", ui32Size,pfBuffer);
	SIN_runTests("sin", ui32Size,pfBuffer);
	SQRT_runTests("sqrt", ui32Size,pfBuffer);
	CMP_runTests("cmp",ui32Size,(char *) pfBuffer);
#endif
#if THIRTY_TWO_K
	ui32Size = 32*4*KB;
	ATAN_runTests("atan", ui32Size,pfBuffer);
	ATAN2_runTests("atan2", ui32Size,pfBuffer);
	COS_runTests("cos", ui32Size,pfBuffer);
	DIV_runTests("div", ui32Size,pfBuffer);
	EXP_runTests("exp", ui32Size,pfBuffer);
	EXP10_runTests("exp10", ui32Size,pfBuffer);
	EXP2_runTests("exp2", ui32Size,pfBuffer);
	LOG10_runTests("log10", ui32Size,pfBuffer);
	LOG2_runTests("log2", ui32Size,pfBuffer);
	LOG_runTests("log", ui32Size,pfBuffer);
	POW_runTests("pow", ui32Size,pfBuffer);
	RECIP_runTests("recip", ui32Size,pfBuffer);
	RSQRT_runTests("rsqrt", ui32Size,pfBuffer);
	SIN_runTests("sin", ui32Size,pfBuffer);
	SQRT_runTests("sqrt", ui32Size,pfBuffer);
	CMP_runTests("cmp",ui32Size,(char *) pfBuffer);
#endif

#if ONE_M
	ui32Size = 1*4*MB;
	ATAN_runTests("atan", ui32Size,pfBuffer);
	ATAN2_runTests("atan2", ui32Size,pfBuffer);
	COS_runTests("cos", ui32Size,pfBuffer);
	DIV_runTests("div", ui32Size,pfBuffer);
	EXP_runTests("exp", ui32Size,pfBuffer);
	EXP10_runTests("exp10", ui32Size,pfBuffer);
	EXP2_runTests("exp2", ui32Size,pfBuffer);
	LOG10_runTests("log10", ui32Size,pfBuffer);
	LOG2_runTests("log2", ui32Size,pfBuffer);
	LOG_runTests("log", ui32Size,pfBuffer);
	POW_runTests("pow", ui32Size,pfBuffer);
	RECIP_runTests("recip", ui32Size,pfBuffer);
	RSQRT_runTests("rsqrt", ui32Size,pfBuffer);
	SIN_runTests("sin", ui32Size,pfBuffer);
	SQRT_runTests("sqrt", ui32Size,pfBuffer);
	CMP_runTests("cmp",ui32Size,(char *) pfBuffer);
#endif
#if ONE_G
	ui32Size = 1*4*GB;
	ATAN_runTests("atan", ui32Size,pfBuffer);
	ATAN2_runTests("atan2", ui32Size,pfBuffer);
	COS_runTests("cos", ui32Size,pfBuffer);
	DIV_runTests("div", ui32Size,pfBuffer);
	EXP_runTests("exp", ui32Size,pfBuffer);
	EXP10_runTests("exp10", ui32Size,pfBuffer);
	EXP2_runTests("exp2", ui32Size,pfBuffer);
	LOG10_runTests("log10", ui32Size,pfBuffer);
	LOG2_runTests("log2", ui32Size,pfBuffer);
	LOG_runTests("log", ui32Size,pfBuffer);
	POW_runTests("pow", ui32Size,pfBuffer);
	RECIP_runTests("recip", ui32Size,pfBuffer);
	RSQRT_runTests("rsqrt", ui32Size,pfBuffer);
	SIN_runTests("sin", ui32Size,pfBuffer);
	SQRT_runTests("sqrt", ui32Size,pfBuffer);
	CMP_runTests("cmp",ui32Size,(char *) pfBuffer);
#endif
#endif

	printf ("Test Complete!\n");

    while(1);

    printf ("**************************************************\n");
    printf ("******************* EDMA Testing *****************\n");
    printf ("**************************************************\n");

    Init_PLL(19,1);

    //LED
    while(1)
    {
    	for (j = PLATFORM_USER_LED_CLASS; j < PLATFORM_END_LED_CLASS; j++)
    	{
    		for (i = 0; i < 4; i++) {
    			platform_write("LED %d ON\n", i);
    			platform_led(i, PLATFORM_LED_ON, (LED_CLASS_E)j);
    			for(k = 0; k < 10000000; k++)
    			{

    			}
//    			platform_delay(100000);
    			platform_write("LED %d OFF\n", i);
    			platform_led(i, PLATFORM_LED_OFF, (LED_CLASS_E) j);
//    			platform_delay(100000);
    			for(k = 0; k < 10000000; k++)
    			{

    			}
    			platform_write("LED %d ON\n", i);
    			platform_led(i, PLATFORM_LED_ON, (LED_CLASS_E)j);
    		}
    	}
    }

	/* Initialize UART */
	platform_uart_init();
    platform_uart_set_baudrate(BOOT_UART_BAUDRATE);

    DEVICE_REG32_W(MAGIC_ADDR, 0);
    DEVICE_REG32_W(PCIE_LEGACY_A_IRQ_STATUS, 0x1);

    /************************************************
     *************** INTC Configuration *************
     ************************************************/
    write_uart("Debug: GEM-INTC Configuration...\n\r");

    /* INTC module initialization */
    intcContext.eventhandlerRecord = EventHandler;
    intcContext.numEvtEntries      = 10;
    if (CSL_intcInit(&intcContext) != CSL_SOK)
    {
        write_uart("Error: GEM-INTC initialization failed n\r");
        return;
    }

    /* Enable NMIs */
    if (CSL_intcGlobalNmiEnable() != CSL_SOK)
    {
    	write_uart("Error: GEM-INTC global NMI enable failed n\r");
        return;
    }

    /* Enable global interrupts */
    if (CSL_intcGlobalEnable(&state) != CSL_SOK)
    {
    	write_uart("Error: GEM-INTC global enable failed \n\r");
        return;
    }

    /* Open the INTC Module for Vector ID: 4 and Event ID: 63 (C6678) 59 (C6670)
     * 	Refer to the interrupt architecture and mapping document for the Event ID  (INTC0_OUT3)*/
    vectId = CSL_INTC_VECTID_4;
    hTest = CSL_intcOpen (&intcObj, INTC0_OUT3, &vectId , NULL);
    if (hTest == NULL)
    {
    	write_uart("Error: GEM-INTC Open failed\n\r");
        return;
    }

    /* Register an call-back handler which is invoked when the event occurs. */
    EventRecord.handler = &test_isr_handler;
    EventRecord.arg = 0;
    if (CSL_intcPlugEventHandler(hTest,&EventRecord) != CSL_SOK)
    {
    	write_uart("Error: GEM-INTC Plug event handler failed\n\r");
        return;
    }

    /* Enabling the events. */
    if (CSL_intcHwControl(hTest,CSL_INTC_CMD_EVTENABLE, NULL) != CSL_SOK)
    {
    	write_uart("Error: GEM-INTC CSL_INTC_CMD_EVTENABLE command failed\n\r");
        return;
    }

    write_uart("Debug: GEM-INTC Configuration Completed \n\r");

    /**************************************************
     ************* CPINTC-0 Configuration *************
     **************************************************/

    write_uart("Debug: CPINTC-0 Configuration...\n\r");

    /* Open the handle to the CPINT Instance */
    hnd = CSL_CPINTC_open(0);
    if (hnd == 0)
    {
    	write_uart("Error: Unable to open CPINTC-0\n\r");
        return;
    }

    /* Disable all host interrupts. */
    CSL_CPINTC_disableAllHostInterrupt(hnd);

    /* Configure no nesting support in the CPINTC Module. */
    CSL_CPINTC_setNestingMode (hnd, CPINTC_NO_NESTING);

   	/* We now map System Interrupt 0 - 3 to channel 3 */
  	CSL_CPINTC_mapSystemIntrToChannel (hnd, PCIEXpress_Legacy_INTA, 3);

   	/* We now enable system interrupt 0 - 3 */
   	CSL_CPINTC_enableSysInterrupt (hnd, PCIEXpress_Legacy_INTA);

   	/* We enable host interrupts. */
   	CSL_CPINTC_enableHostInterrupt (hnd, 3);

   	/* Enable all host interrupts also. */
   	CSL_CPINTC_enableAllHostInterrupt(hnd);

   	write_uart("Debug: CPINTC-0 Configuration Completed\n\r");

    start_boot();
}

