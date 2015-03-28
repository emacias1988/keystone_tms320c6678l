/**
 *   @file  edma_test.c
 *
 *   @brief
 *      This is the Example test code for the EDMA CSL Functional layer.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2009 Texas Instruments, Inc.
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
 *    Neither the name of Texas Instruments Incorporated nor the names of
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
*/


#include <stdio.h>
#include <ti/csl/csl_edma3.h>
#include <ti/csl/csl_edma3Aux.h>
#include <ti/csl/csl_cacheAux.h>
#include "EVM6678.h"

#define 	PARALLEL_TRANSFER_ENABLE 			0
// 0 - Overwrite Dst Buffer 1 - Increment Dst buffer
#define		INCREMENT_DST_BUFFER_IDX			0
#define		DEBUG_EDMA_TRANSFER					0

#define		EDMA_MAX_TRANSFER_SIZE_WORDS		(0x3FFF)
//#define		EDMA_MAX_TRANSFER_SIZE_WORDS		(0xFFFF)

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/

/* Global Variables which are used to dump the TPCC register overlay in the
 * debugger... */
CSL_TpccRegs*  gEDMACC0Regs  = (CSL_TpccRegs*)CSL_EDMA0CC_REGS;
CSL_TpccRegs*  gEDMACC1Regs  = (CSL_TpccRegs*)CSL_EDMA1CC_REGS;
CSL_TpccRegs*  gEDMACC2Regs  = (CSL_TpccRegs*)CSL_EDMA2CC_REGS;


/* Global Buffers (Source and Destination) for PING-PONG */
Uint8 srcBuff1[1];
Uint8 srcBuff2[1];
Uint8 dstBuff1[1];
Uint8 dstBuff2[1];

/**********************************************************************
 ************************ EDMA TEST FUNCTIONS *************************
 **********************************************************************/

/**
 *  @b Description
 *  @n
 *      This is utility function used by the EDMA example to verify the
 *      Data transfer.
 *
 *  @retval
 *      Success -   TRUE
 *  @retval
 *      Error -   FALSE
 */
Bool Verify_Transfer
(
    Uint16      aCnt,
    Uint16      bCnt,
    Uint16      cCnt,
    Uint16      srcBIdx,
    Uint16      dstBIdx,
    Uint16      srcCIdx,
    Uint16      dstCIdx,
    Uint8      *srcBuff,
    Uint8      *dstBuff,
    Bool        abSync
)
{
    Uint32      loopIndex1;
    Uint32      loopIndex2;
    Uint32      loopIndex3;
    Uint8*      srcArrayPtr = (Uint8*)srcBuff;
    Uint8*      dstArrayPtr = (Uint8*)dstBuff;
    Uint8*      srcFramePtr = (Uint8*)srcBuff;
    Uint8*      dstFramePtr = (Uint8*)dstBuff;
    Uint16      key;


    /* Invalidate the cache before verification */
    /* Disable Interrupts */
    key = _disable_interrupts();
    CACHE_invL1d ((void *)srcBuff, 512, CACHE_WAIT);
    CACHE_invL2 ((void *)srcBuff, 512, CACHE_WAIT);
    CACHE_invL1d ((void *)dstBuff, 512, CACHE_WAIT);
    CACHE_invL2 ((void *)dstBuff, 512, CACHE_WAIT);
    _mfence();
    /* Re-enable Interrupts. */
    _restore_interrupts(key);


    for (loopIndex1 = 0; loopIndex1 < cCnt; loopIndex1++) {
        for (loopIndex2 = 0; loopIndex2 < bCnt; loopIndex2++) {
            for (loopIndex3 = 0; loopIndex3 < aCnt; loopIndex3++)
                if (srcArrayPtr[loopIndex3] != dstArrayPtr[loopIndex3])
                    return FALSE;
            srcArrayPtr = srcArrayPtr + srcBIdx;
            dstArrayPtr = dstArrayPtr + dstBIdx;
        }

        if (abSync) {
            srcFramePtr = srcFramePtr + srcCIdx;
            srcArrayPtr = srcFramePtr;
            dstFramePtr = dstFramePtr + dstCIdx;
            dstArrayPtr = dstFramePtr;
        }
        else {
            srcFramePtr = srcArrayPtr + srcCIdx - srcBIdx;
            srcArrayPtr = srcFramePtr;
            dstFramePtr = dstArrayPtr + dstCIdx - dstBIdx;
            dstArrayPtr = dstFramePtr;
        }
    }

    return TRUE;
}

/**
 *  @b Description
 *  @n
 *      This is the example routine which perform EDMA ping pong buffer transfer
 *      using a QDMA Channel.
 *      It implements following steps:-
 *          - Initializes and Opens the EDMA module
 *          - Sets up the Module and gets the module setup values
 *          - Enables the DMA and QDMA regions
 *          - Opens the specified QDMA channel 'channelNum'
 *          - PING uses PARAM Entry 1
 *          - PONG uses PARAM Entry 2
 *          - Configures the PARAM Blocks.
 *          - Enables the channel
 *          - Initiates the QDMA Transfer by setting the Trigger Word.
 *          - Poll on the IPR bit
 *          - Clear the pending bit
 *          - Compares the transfered data
 *          - Closes EDMA module
 *
 *  @param[in]  instNum
 *      EDMA Instance Number on which the test is executed.
 *  @param[in]  channelNum
 *      EDMA Channel Number on which the test is executed
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Failure -   <0
 */
Int32 qdma_link_xfer_region (Int32 instNum, Uint8 channelNum, Int32 regionNum)
{
    CSL_Edma3Handle             hModule;
    CSL_Edma3Obj                edmaObj;
    CSL_Edma3ParamHandle        hParamPong,hParamPing;
    CSL_Edma3ChannelObj         chObj;
    CSL_Edma3CmdIntr            regionIntr;
    CSL_Edma3CmdDrae            regionAccess;
    CSL_Edma3ChannelHandle      hChannel;
    CSL_Edma3ParamSetup         myParamSetup;
    CSL_Edma3Context            context;
    CSL_Edma3ChannelAttr        chAttr;
    CSL_Edma3CmdQrae            qraeSetup;
    CSL_Status                  status;
    Uint32                      loopIndex;

    /* Start the EDMA PING-PONG test over the SHADOW Region. */
    printf ("Debug: Testing EDMA(%d) Ping-Pong Test (Shadow) Region %d for QDMA Channel %d...\n",
             instNum, regionNum, channelNum);

    /* Initialize data  */
    for (loopIndex = 0; loopIndex < 256; loopIndex++) {
        srcBuff1[loopIndex] = loopIndex;
        srcBuff2[loopIndex] = loopIndex;
        dstBuff1[loopIndex] = 0;
        dstBuff2[loopIndex] = 0;
    }

    /* Module initialization */
    if (CSL_edma3Init(&context) != CSL_SOK)
    {
        printf ("Error: EDMA module initialization failed\n");
        return -1;
    }

    /* Module level open */
    hModule = CSL_edma3Open(&edmaObj,instNum,NULL,&status);
    if ((hModule == NULL) || (status != CSL_SOK))
    {
        printf ("Error: EDMA module open failed\n");
        return -1;
    }

    /* Is this for GLOBAL or SHADOW Region */
    if (regionNum != CSL_EDMA3_REGION_GLOBAL)
    {
        /* Shadow Region: Enable DRAE enable(Bits 0-15) it. */
        regionAccess.region = regionNum;
        regionAccess.drae   = 0xFFFF;
        regionAccess.draeh  = 0x0000;
        if (CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_DMAREGION_ENABLE, &regionAccess) != CSL_SOK)
        {
             printf ("Error: EDMA region enable command failed\n");
             return -1;
        }

        /* Enable access for all QDMA channels in the SHADOW Region. */
        qraeSetup.region = regionNum;
        qraeSetup.qrae   = 0xFF;
        if (CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_QDMAREGION_ENABLE, &qraeSetup) != CSL_SOK)
        {
             printf ("Error: EDMA QDMA region enable command failed\n");
             return -1;
        }
    }

    /* QDMA Channel Open */
    chAttr.regionNum = regionNum;
    chAttr.chaNum    = channelNum;
    hChannel = CSL_edma3ChannelOpen(&chObj, instNum, &chAttr, &status);
    if ((hChannel == NULL) || (status != CSL_SOK))
    {
        printf ("Error: EDMA channel open failed\n");
        return -1;
    }

    /* Map QDMA Channel to the PING Param Block i.e. 1 */
    CSL_edma3HwChannelSetupParam (hChannel, 1);

    /* Setup the trigger word for the QDMA Channel. */
    CSL_edma3HwChannelSetupTriggerWord(hChannel, 7);

    /* PING Parameter Entry Handle */
    hParamPing = CSL_edma3GetParamHandle(hChannel, 1, &status);
    if (hParamPing == NULL)
    {
        printf ("Error: EDMA get handle for param entry 1 failed\n");
        return -1;
    }

    /* PONG Parameter Entry Handle */
    hParamPong = CSL_edma3GetParamHandle(hChannel, 2, &status);
    if (hParamPong == NULL)
    {
        printf ("Error: EDMA get handle for param entry 2 failed\n");
        return -1;
    }

    /* Setup param entry */
    myParamSetup.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS, \
                                             CSL_EDMA3_TCCH_DIS, \
                                             CSL_EDMA3_ITCINT_DIS, \
                                             CSL_EDMA3_TCINT_DIS,\
                                             0,CSL_EDMA3_TCC_NORMAL,\
                                             CSL_EDMA3_FIFOWIDTH_NONE, \
                                             CSL_EDMA3_STATIC_DIS, \
                                             CSL_EDMA3_SYNC_A, \
                                             CSL_EDMA3_ADDRMODE_INCR, \
                                             CSL_EDMA3_ADDRMODE_INCR);
    myParamSetup.srcAddr    = (Uint32)srcBuff1;
    myParamSetup.aCntbCnt   = CSL_EDMA3_CNT_MAKE(256,1);
    myParamSetup.dstAddr    = (Uint32)dstBuff1;
    myParamSetup.srcDstBidx = CSL_EDMA3_BIDX_MAKE(1,1);
    myParamSetup.linkBcntrld= CSL_EDMA3_LINKBCNTRLD_MAKE(hParamPong,0);
    myParamSetup.srcDstCidx = CSL_EDMA3_CIDX_MAKE(0,1);
    myParamSetup.cCnt       = 1;

    /* Setup PING to operate with this PARAM Entry. */
    if (CSL_edma3ParamSetup(hParamPing, &myParamSetup) != CSL_SOK)
    {
         printf ("Error: EDMA param setup failed\n");
         return -1;
    }

    /* Setting up the next entry */
    myParamSetup.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS, \
                                             CSL_EDMA3_TCCH_DIS, \
                                             CSL_EDMA3_ITCINT_DIS, \
                                             CSL_EDMA3_TCINT_EN,\
                                             1,CSL_EDMA3_TCC_NORMAL,\
                                             CSL_EDMA3_FIFOWIDTH_NONE, \
                                             CSL_EDMA3_STATIC_EN, \
                                             CSL_EDMA3_SYNC_A, \
                                             CSL_EDMA3_ADDRMODE_INCR, \
                                             CSL_EDMA3_ADDRMODE_INCR);
    myParamSetup.srcAddr = (Uint32)srcBuff2;
    myParamSetup.dstAddr = (Uint32)dstBuff2;

    /* Setup for the Pong buffer */
    if (CSL_edma3ParamSetup(hParamPong,&myParamSetup) != CSL_SOK)
    {
         printf ("Error: EDMA param setup failed\n");
         return -1;
    }

    /* Enable Channel */
    if (CSL_edma3HwChannelControl(hChannel,CSL_EDMA3_CMD_CHANNEL_ENABLE, NULL) != CSL_SOK)
    {
         printf ("Error: EDMA channel enable command failed\n");
         return -1;
    }

    /* Trigger the word by writing to the trigger word... */
    if (CSL_edma3ParamWriteWord(hParamPing,7,1) != CSL_SOK) {
         printf ("Error: EDMA param write word failed\n");
         return -1;
    }

    /* Poll IPR bit */
    regionIntr.region = regionNum;
    do {
        CSL_edma3GetHwStatus(hModule,CSL_EDMA3_QUERY_INTRPEND,&regionIntr);
    } while (!(regionIntr.intr & 0x2));

    /* Clear pending interrupt */
    if (CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_INTRPEND_CLEAR, &regionIntr) != CSL_SOK)
    {
         printf ("Error: EDMA clear interrupt pend command failed\n");
         return -1;
    }

    /* Check transfer by comparing the source and destination buffers */
    if(Verify_Transfer( 256, 1, 1, 0, 0, 0, 0, srcBuff1, dstBuff1,TRUE) == FALSE)
    {
        printf ("Error: Verification (Source1/Destination1) Failed\n");
        return -1;
    }
    if(Verify_Transfer( 256, 1, 1, 0, 0, 0, 0, srcBuff2, dstBuff2,TRUE) == FALSE)
    {
        printf ("Error: Verification (Source2/Destination2) Failed\n");
        return -1;
    }

    /* Disable the channel */
    CSL_edma3HwChannelControl(hChannel,CSL_EDMA3_CMD_CHANNEL_DISABLE, NULL);

    /* Close channel */
    if (CSL_edma3ChannelClose(hChannel) != CSL_SOK)
    {
        printf("Error: EDMA channel close failed\n");
        return -1;
    }

    /* Close EDMA module */
    if (CSL_edma3Close(hModule) != CSL_SOK)
    {
        printf("Error: EDMA module close failed\n");
        return -1;
    }

    /* Test Passed. */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the example routine which perform EDMA ping pong buffer transfer
 *      where channel is open in the global region.
 *      It implements following steps
 *          - Intializes and Opens the EDMA Module .
 *          - Sets up the interrupt for the EDMA global region.
 *          - Sets up the EDMA module using the API csl_edma3Hwsetup ()
 *          - Enables the EDMA global region
 *          - Opens the channel 'channelNum' get the param handle for PARAM 1, 2
 *          - PING is mapped to PARAM Entry 1
 *          - PONG is mapped to PARAM Entry 2
 *          - PING is Linked with PONG...
 *          - Enables the EDMA interrupt using CSL_EDMA3_CMD_INTR_ENABLE.
 *          - Enables Interrupt (Bit 0-2) for the global region interrupts
 *          - Manually triggers the DMA channel 'channelNum'(This should be PING)
 *          - Polls on IPR bit 0 (Since TCC for PING is 0)
 *          - Clear the pending bit
 *          - Manually triggers the channel 'channelNum' (This should be PONG)
 *          - Polls on IPR bit 1 (Since TCC for PONG is 1)
 *          - Clears the pending bit
 *          - Compares the data in the destination buffer is proper or not.
 *          - Closes the EDMA module and channel.
 *
 *  @param[in]  instNum
 *      EDMA Instance Number on which the test is executed.
 *  @param[in]  channelNum
 *      EDMA Channel Number on which the test is executed
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Failure -   <0
 */
Int32 edma_ping_pong_xfer_gbl_region
(
	Int32 instNum,
	Uint8 channelNum,
	Uint32 ui32SrcAddr,
	Uint32 ui32DstAddr,
	Uint32 ui32TransferSizeWords
)
{
    CSL_Edma3Handle                 hModule;
    CSL_Edma3Obj                    edmaObj;
    CSL_Edma3ParamHandle            hParamPing;
    CSL_Edma3ParamHandle            hParamPong;
    CSL_Edma3ChannelObj             chObj;
    CSL_Edma3ChannelObj             chObj2;
    CSL_Edma3CmdIntr                regionIntr;
    CSL_Edma3ChannelHandle          hChannelPing;
    CSL_Edma3ChannelHandle          hChannelPong;
    CSL_Edma3ParamSetup             myParamSetup;
    CSL_Edma3Context                context;
    CSL_Edma3ChannelAttr            chAttr;
    CSL_Edma3ChannelAttr            chAttr2;
    CSL_Status                      status;
    CSL_Status                      status2;
    Uint32                          loopIndex;

    Uint32							ui32WordsRemaining;
    Uint32							ui32EDMATransferSize;

    /* Start the EDMA PING-PONG test over the Global Region. */
#if	DEBUG_EDMA_TRANSFER
    printf ("Debug: EDMA(%d) configuration (Global) Region for Channel %d...\n", instNum, channelNum);
#endif

    g_ui64StartTime = TSCL;

    /* Module initialization */
    if (CSL_edma3Init(&context) != CSL_SOK)
    {
        printf ("Error: EDMA module initialization failed\n");
        return -1;
    }

    /* Open the EDMA Module using the provided instance number */
    hModule = CSL_edma3Open(&edmaObj, instNum, NULL, &status);
    if ( (hModule == NULL) || (status != CSL_SOK))
    {
        printf ("Error: EDMA module open failed\n");
        return -1;
    }

    //********************** PING CHANNEL SETUP *********************//

    /* Channel open */
    chAttr.regionNum = CSL_EDMA3_REGION_GLOBAL;
    chAttr.chaNum    = channelNum;
    hChannelPing = CSL_edma3ChannelOpen(&chObj, instNum, &chAttr, &status);
    if ((hChannelPing == NULL) || (status != CSL_SOK))
    {
        printf ("Error: Unable to open EDMA Channel:%d\n", channelNum);
        return -1;
    }

    if(!instNum)
    {
        /* For first EDMA instance there are only 2 TCs and 2 event queues
         * Modify the channel default queue setup from 0 to 1
         */
         if (CSL_edma3HwChannelSetupQue(hChannelPing,CSL_EDMA3_QUE_0) != CSL_SOK)
        {
            printf ("Error: EDMA channel setup queue failed\n");
            return -1;
        }
    }
    else
    {
        /* For EDMA instance 1 and 2 maximum of 4 TCs and 4 event queues are supported
         * Change Channel Default queue setup from 0 to 3
         */
        if (CSL_edma3HwChannelSetupQue(hChannelPing,CSL_EDMA3_QUE_3) != CSL_SOK)
        {
            printf ("Error: EDMA channel setup queue failed\n");
            return -1;
        }
    }

    /* Map the DMA Channel to PARAM Block 2. */
    CSL_edma3HwChannelSetupParam(hChannelPing,0);
//    CSL_edma3MapDMAChannelToParamBlock (hModule, channelNum, 1);

    /* Obtain a handle to parameter set 2 */
    hParamPing = CSL_edma3GetParamHandle(hChannelPing, 0, &status);
    if (hParamPing == NULL)
    {
        printf ("Error: EDMA Get Parameter Entry failed for 2.\n");
        return -1;
    }

    //********************** PING HANDLE COMPLETE*********************//

    //********************** PONG CHANNEL SETUP *********************//

#if PARALLEL_TRANSFER_ENABLE
	/* Channel open */
    chAttr2.regionNum = CSL_EDMA3_REGION_GLOBAL;
	chAttr2.chaNum    = (channelNum+1);
	hChannelPong = CSL_edma3ChannelOpen(&chObj2, instNum, &chAttr2, &status2);
	if ((hChannelPong == NULL) || (status2 != CSL_SOK))
	{
		printf ("Error: Unable to open EDMA Channel:%d\n", (channelNum+1));
		return -1;
	}

	if(!instNum)
	{
		/* For first EDMA instance there are only 2 TCs and 2 event queues
		 * Modify the channel default queue setup from 0 to 1
		 */
		 if (CSL_edma3HwChannelSetupQue(hChannelPong,CSL_EDMA3_QUE_1) != CSL_SOK)
		{
			printf ("Error: EDMA channel setup queue failed\n");
			return -1;
		}
	}
	else
	{
		/* For EDMA instance 1 and 2 maximum of 4 TCs and 4 event queues are supported
		 * Change Channel Default queue setup from 0 to 3
		 */
		if (CSL_edma3HwChannelSetupQue(hChannelPong,CSL_EDMA3_QUE_3) != CSL_SOK)
		{
			printf ("Error: EDMA channel setup queue failed\n");
			return -1;
		}
	}

	/* Map the DMA Channel to PARAM Block 2. */
	CSL_edma3HwChannelSetupParam(hChannelPong,1);
//	CSL_edma3MapDMAChannelToParamBlock (hModule, (channelNum+1), 1);

    /* Obtain a handle to parameter set 1 */
    hParamPong = CSL_edma3GetParamHandle(hChannelPong, 1, &status2);
    if (hParamPong == NULL)
    {
        printf ("Error: EDMA Get Parameter Entry failed for 1.\n");
        return -1;
    }

#endif

    //********************** PONG HANDLE COMPLETE*********************//

    //********************** PING BUFFER SETUP *********************//

    ui32WordsRemaining = ui32TransferSizeWords;

    // Check for max EDMA transfer capability (0xFFFF * 4 bytes)
    if(ui32WordsRemaining > (EDMA_MAX_TRANSFER_SIZE_WORDS) )
    {
    	ui32EDMATransferSize = EDMA_MAX_TRANSFER_SIZE_WORDS;
    }
    else
    {
    	ui32EDMATransferSize = ui32WordsRemaining;
    }

    /* Setup the parameter entry parameters (Ping buffer) */
    myParamSetup.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS, \
                                             CSL_EDMA3_TCCH_DIS, \
                                             CSL_EDMA3_ITCINT_DIS, \
                                             CSL_EDMA3_TCINT_EN, \
                                             1, CSL_EDMA3_TCC_NORMAL,\
                                             CSL_EDMA3_FIFOWIDTH_NONE, \
                                             CSL_EDMA3_STATIC_DIS, \
                                             CSL_EDMA3_SYNC_AB, \
                                             CSL_EDMA3_ADDRMODE_INCR, \
                                             CSL_EDMA3_ADDRMODE_INCR );
    myParamSetup.srcAddr    = ui32SrcAddr;
    // Divide transfer size by 2
#if PARALLEL_TRANSFER_ENABLE
    ui32TransferSizeWords =  ui32TransferSizeWords >> 1;
#endif
    // aCnt - number of 8 bits to move
    // bCnt - number of arrays of aCount size
    // The EDMA will transfer ui32TransferSizeWords (8 bits), bCnt times (in this case 4)
    myParamSetup.aCntbCnt   = CSL_EDMA3_CNT_MAKE((uint16_t) ui32EDMATransferSize,4);
    myParamSetup.dstAddr	= ui32DstAddr;
    // Increment of src and dst pointers after each transfer. Must increment based on aCnt.
    myParamSetup.srcDstBidx = CSL_EDMA3_BIDX_MAKE((uint16_t) ui32EDMATransferSize,(uint16_t) ui32EDMATransferSize);
//    myParamSetup.linkBcntrld= CSL_EDMA3_LINKBCNTRLD_MAKE(hParamPong,0);
    myParamSetup.srcDstCidx = CSL_EDMA3_CIDX_MAKE(0,1);
    myParamSetup.cCnt = 1;

    /* Ping setup */
    if (CSL_edma3ParamSetup(hParamPing,&myParamSetup) != CSL_SOK)
    {
        printf ("Error: EDMA Parameter Entry Setup failed\n");
        return -1;
    }

    //********************** PONG BUFFER SETUP *********************//

#if PARALLEL_TRANSFER_ENABLE
    /* Setup the parameter entry parameters (Pong buffer) */
    myParamSetup.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS, \
                                             CSL_EDMA3_TCCH_DIS, \
                                             CSL_EDMA3_ITCINT_DIS, \
                                             CSL_EDMA3_TCINT_EN, \
                                             0, CSL_EDMA3_TCC_NORMAL,\
                                             CSL_EDMA3_FIFOWIDTH_NONE, \
                                             CSL_EDMA3_STATIC_DIS, \
                                             CSL_EDMA3_SYNC_AB, \
                                             CSL_EDMA3_ADDRMODE_INCR, \
                                             CSL_EDMA3_ADDRMODE_INCR );

    // Base Address + 4 bytes * half of the transfer size
    myParamSetup.srcAddr    = ui32SrcAddr+(uint32_t) (ui32TransferSizeWords << 2);
//    ui16TransferSizeBytes =  ui16TransferSizeBytes >> 1;
    // aCnt - number of 8 bits to move
    // bCnt - number of arrays of aCount size
    // The EDMA will transfer ui16TransferSizeBytes (8 bits), bCnt times (in this case 4)
    myParamSetup.aCntbCnt   = CSL_EDMA3_CNT_MAKE((uint16_t) ui32TransferSizeWords,4);
    myParamSetup.dstAddr	= ui32DstAddr+(uint32_t) (ui32TransferSizeWords << 2);
    // Increment of src and dst pointers after each transfer. Must increment based on aCnt.
    myParamSetup.srcDstBidx = CSL_EDMA3_BIDX_MAKE((uint16_t) ui32TransferSizeWords,(uint16_t) ui32TransferSizeWords);
//    myParamSetup.linkBcntrld= CSL_EDMA3_LINKBCNTRLD_MAKE(hParamPong,0);
    myParamSetup.srcDstCidx = CSL_EDMA3_CIDX_MAKE(0,1);
    myParamSetup.cCnt = 1;

    /* Pong setup */
    if (CSL_edma3ParamSetup(hParamPong,&myParamSetup) != CSL_SOK)
    {
        printf ("Error: EDMA Parameter Entry Setup failed\n");
        return -1;
    }

#endif

    /* Interrupt enable (Bits 0-1)  for the global region interrupts */
    regionIntr.region = CSL_EDMA3_REGION_GLOBAL;
    regionIntr.intr   = 0x3;
    regionIntr.intrh  = 0x0000;
    CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_INTR_ENABLE,&regionIntr);

    g_ui64StopTime = TSCL;

    g_ui64ElapsedTime = Osal_calculateElapsedTime(g_ui64StartTime,g_ui64StopTime);

    printf ("Config: Elapsed Cycles %llu Elapsed Time %llu (ns)\n",g_ui64ElapsedTime, (uint64_t)(g_ui64ElapsedTime * (1000 / g_sEvmInfo.frequency)) );

    //********************** TRIGGER PING AND PONG *********************//

    g_ui64StartTime = TSCL;

    do
    {
    	/* Trigger channel */
    	    CSL_edma3HwChannelControl(hChannelPing,CSL_EDMA3_CMD_CHANNEL_SET,NULL);
    	#if PARALLEL_TRANSFER_ENABLE
    	    CSL_edma3HwChannelControl(hChannelPong,CSL_EDMA3_CMD_CHANNEL_SET,NULL);
    	#endif

		regionIntr.region = CSL_EDMA3_REGION_GLOBAL;
		regionIntr.intr   = 0;
		regionIntr.intrh  = 0;

#if PARALLEL_TRANSFER_ENABLE
    	    /* Poll on IPR bit 0 */
    	    do {
    	        CSL_edma3GetHwStatus(hModule,CSL_EDMA3_QUERY_INTRPEND,&regionIntr);
    	    } while (!(regionIntr.intr & 0x1));
#endif

    	    /* Poll on IPR bit 1 */
    	    do {
    	        CSL_edma3GetHwStatus(hModule,CSL_EDMA3_QUERY_INTRPEND,&regionIntr);
    	    } while (!(regionIntr.intr & 0x2));

    	    /* Clear the pending bit */
    	    CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_INTRPEND_CLEAR,&regionIntr);

    	    ui32SrcAddr += (ui32EDMATransferSize << 2);

    	    // Overwrite the data to the destination buffer
#if OVERWRITE_DEST_BUFFER_DISABLE
    	    ui32DstAddr += (ui32EDMATransferSize << 2);
#endif
			ui32WordsRemaining = ui32WordsRemaining - ui32EDMATransferSize;

#if DEBUG_EDMA_TRANSFER
    	    printf("Debug: Transferred %d words \t Remaining %d words\n",ui32EDMATransferSize,ui32WordsRemaining);
#endif
    	    if(ui32WordsRemaining)
    	    {
            	// If there are words remaining configure the hParamPing Transfer Setup

    	        // Check for max EDMA transfer capability
    	        if(ui32WordsRemaining > (EDMA_MAX_TRANSFER_SIZE_WORDS) )
    	        {
    	        	ui32EDMATransferSize = EDMA_MAX_TRANSFER_SIZE_WORDS;
    	        }
    	        else
    	        {
    	        	ui32EDMATransferSize = ui32WordsRemaining;
    	        }

        	    myParamSetup.srcAddr = ui32SrcAddr;
        	    myParamSetup.dstAddr = ui32DstAddr;

				// The EDMA will transfer ui32TransferSizeWords (8 bits), bCnt times (in this case 4)
				myParamSetup.aCntbCnt   = CSL_EDMA3_CNT_MAKE((uint16_t) ui32EDMATransferSize,4);

				// Increment of src and dst pointers after each transfer. Must increment based on aCnt.
				myParamSetup.srcDstBidx = CSL_EDMA3_BIDX_MAKE((uint16_t) ui32EDMATransferSize,(uint16_t) ui32EDMATransferSize);

				/* Ping setup */
				if (CSL_edma3ParamSetup(hParamPing,&myParamSetup) != CSL_SOK)
				{
					printf ("Error: EDMA Parameter Entry Setup failed\n");
					return -1;
				}
    	    }
    }
    while(ui32WordsRemaining > 0);

    g_ui64StopTime = TSCL;

    g_ui64ElapsedTime = Osal_calculateElapsedTime(g_ui64StartTime,g_ui64StopTime);

//    printf ("Transfer: Elapsed Cycles: %d Elapsed Time: %d (ns) BW: %d MB/s\n\n",
//    		g_ui64ElapsedTime, (g_ui64ElapsedTime * (1000 / g_sEvmInfo.frequency)),
//    		(int64_t) ((int64_t)ui32TransferSizeWords*4000) / ((int64_t)g_ui64ElapsedTime * (1000 / (int64_t)g_sEvmInfo.frequency)) );

    printf ("Transfer: Elapsed Cycles: %llu Elapsed Time: %llu (ns) BW: %llu MB/s\n\n",
    	  	          		(uint64_t)g_ui64ElapsedTime, (uint64_t)(g_ui64ElapsedTime * (1000 / g_sEvmInfo.frequency)),
    	  	          	(uint64_t)( ((int64_t)ui32TransferSizeWords*4000) / ((int64_t)g_ui64ElapsedTime * (1000 / (int64_t)g_sEvmInfo.frequency))));

    /* Close Ping channel */
    if (CSL_edma3ChannelClose(hChannelPing) != CSL_SOK)
    {
        printf("Error: EDMA Channel Ping Close failed\n");
        return -1;
    }
#if PARALLEL_TRANSFER_ENABLE
    /* Close Pong channel */
    if (CSL_edma3ChannelClose(hChannelPong) != CSL_SOK)
    {
        printf("Error: EDMA Channel Pong Close failed\n");
        return -1;
    }
#endif

    /* Close EDMA module */
    if (CSL_edma3Close(hModule) != CSL_SOK)
    {
        printf("Error: EDMA Module Close failed\n");
        return -1;
    }

    /* The test passed. */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the example routine which performs the EDMA ping pong buffer transfer in
 *      the specified SHADOW Region.
 *
 *      It implements following steps
 *          - Intializes and Opens the EDMA Module .
 *          - Sets up the interrupt for the EDMA SHADOW region.
 *          - Opens the channel 'channelNum' get the param handle for PARAM 1, 2
 *          - PING is mapped to PARAM Entry 1
 *          - PONG is mapped to PARAM Entry 2
 *          - PING is Linked with PONG...
 *          - Enables the EDMA interrupts for the specific SHADOW Region.
 *          - Manually triggers the DMA channel 'channelNum'(This should be PING)
 *          - Polls on IPR bit 0 (Since TCC for PING is 0)
 *          - Clear the pending bit
 *          - Manually triggers the channel 'channelNum' (This should be PONG)
 *          - Polls on IPR bit 1 (Since TCC for PONG is 1)
 *          - Clears the pending bit
 *          - Compares the data in the destination buffer is proper or not.
 *          - Closes the EDMA module and channel.
 *
 *  @param[in]  instNum
 *      EDMA Instance Number on which the test is executed.
 *  @param[in]  channelNum
 *      EDMA Channel Number on which the test is executed
 *  @param[in]  regionNum
 *      Shadown Region Number on which the test is executed.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Failure -   <0
 */
Int32 edma_ping_pong_xfer_region (Int32 instNum, Uint8 channelNum, Int32 regionNum)
{
    CSL_Edma3Handle             hModule;
    CSL_Edma3Obj                edmaObj;
    CSL_Edma3ParamHandle        hParamPing;
    CSL_Edma3ParamHandle        hParamPong;
    CSL_Edma3ChannelObj         chObj;
    CSL_Edma3CmdIntr            regionIntr;
    CSL_Edma3CmdDrae            regionAccess;
    CSL_Edma3ChannelHandle      hChannel;
    CSL_Edma3ParamSetup         myParamSetup;
    CSL_Edma3Context            context;
    CSL_Edma3ChannelAttr        chAttr;
    CSL_Status                  status;
    Uint32                      loopIndex;

    /* Start the EDMA PING-PONG test over the Shadow Region. */
    printf ("Debug: Testing EDMA(%d) Ping-Pong Test Shadow-Region %d for Channel %d...\n", instNum, regionNum, channelNum);

    /* Module Initialization */
    if (CSL_edma3Init(&context) != CSL_SOK)
    {
        printf ("Error: EDMA module initialization failed\n");
        return -1;
    }

    /* Module level open */
    hModule = CSL_edma3Open(&edmaObj, instNum, NULL, &status);
    if ((hModule == NULL) || (status != CSL_SOK))
    {
        printf ("Error: EDMA module open failed\n");
        return -1;
    }

    /* Determine the number of channels we need to enable. Is this Instance 0? */
    if (instNum == 0)
    {
        /* YES. DRAE enable(Bits 0-15) for the shadow region; since there are 16 channels. */
        regionAccess.region = regionNum;
        regionAccess.drae   = 0xFFFF;
        regionAccess.draeh  = 0x0;
        if (CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_DMAREGION_ENABLE, &regionAccess) != CSL_SOK)
        {
            printf ("Error: EDMA region enable command failed\n");
            return -1;
        }
    }
    else
    {
        /* YES. DRAE enable(Bits 0-63) for the shadow region; since there are 64 channels. */
        regionAccess.region = regionNum;
        regionAccess.drae   = 0xFFFFFFFF;
        regionAccess.draeh  = 0xFFFFFFFF;
        if (CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_DMAREGION_ENABLE, &regionAccess) != CSL_SOK)
        {
            printf ("Error: EDMA region enable command failed\n");
            return -1;
        }
    }

    /* Interrupt enable (Bits 0-11)  for the shadow region 5 */
    regionIntr.region = regionNum;
    regionIntr.intr   = 0x0FFF;
    regionIntr.intrh  = 0x0000;
    if (CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_INTR_ENABLE, &regionIntr) != CSL_SOK)
    {
        printf ("Error: EDMA interrupts enable command failed\n");
        return -1;
    }

    /* Open the channel in context of the specified region number. */
    chAttr.regionNum = regionNum;
    chAttr.chaNum    = channelNum;
    hChannel = CSL_edma3ChannelOpen(&chObj, instNum, &chAttr, &status);
    if ((hChannel == NULL) || (status != CSL_SOK))
    {
        printf ("Error: EDMA channel open failed\n");
        return -1;
    }

    /* Map the DMA Channel to the appropriate PARAM Block. We start with PING
     * which is located at PARAM Block 2. */
    if (CSL_edma3HwChannelSetupParam(hChannel, 2) != CSL_SOK)
    {
        printf ("Error: EDMA channel setup param failed\n");
        return -1;
    }

    /* Obtain a handle to parameter set 2 */
    hParamPing = CSL_edma3GetParamHandle(hChannel, 2, &status);
    if (hParamPing == NULL) {
        printf ("Error: EDMA get param handle for param entry 2 failed\n");
        return -1;
    }

    /* Obtain a handle to parameter set 1 */
    hParamPong = CSL_edma3GetParamHandle(hChannel, 1, &status);
    if (hParamPong == NULL) {
        printf ("Error: EDMA get param handle for param entry 1 failed\n");
        return -1;
    }

    /* Setup the first param set (Ping buffer) */
    myParamSetup.option = CSL_EDMA3_OPT_MAKE( CSL_EDMA3_ITCCH_DIS, \
                                              CSL_EDMA3_TCCH_DIS, \
                                              CSL_EDMA3_ITCINT_DIS, \
                                              CSL_EDMA3_TCINT_EN,\
                                              0, CSL_EDMA3_TCC_NORMAL,\
                                              CSL_EDMA3_FIFOWIDTH_NONE, \
                                              CSL_EDMA3_STATIC_DIS, \
                                              CSL_EDMA3_SYNC_A, \
                                              CSL_EDMA3_ADDRMODE_INCR, \
                                              CSL_EDMA3_ADDRMODE_INCR
                                             );
    myParamSetup.srcAddr    = (Uint32)srcBuff1;
    myParamSetup.aCntbCnt   = CSL_EDMA3_CNT_MAKE(512,1);
    myParamSetup.dstAddr    = (Uint32)dstBuff1;
    myParamSetup.srcDstBidx = CSL_EDMA3_BIDX_MAKE(1,1);
    myParamSetup.linkBcntrld= CSL_EDMA3_LINKBCNTRLD_MAKE(hParamPong,0);
    myParamSetup.srcDstCidx = CSL_EDMA3_CIDX_MAKE(0,1);
    myParamSetup.cCnt = 1;

    /* Setup the Ping Entry which loaded after the Pong entry gets exhausted */
    if (CSL_edma3ParamSetup(hParamPing,&myParamSetup) != CSL_SOK)
    {
        printf("Error: EDMA ping parameter entry setup is failed\n");
        return -1;
    }

    /* Setup the Pong Entry which loaded after the Ping entry gets exhausted */
    myParamSetup.option = CSL_EDMA3_OPT_MAKE( CSL_EDMA3_ITCCH_DIS, \
                                              CSL_EDMA3_TCCH_DIS, \
                                              CSL_EDMA3_ITCINT_DIS, \
                                              CSL_EDMA3_TCINT_EN, \
                                              1,CSL_EDMA3_TCC_NORMAL, \
                                              CSL_EDMA3_FIFOWIDTH_NONE, \
                                              TRUE,CSL_EDMA3_SYNC_A, \
                                              CSL_EDMA3_ADDRMODE_INCR, \
                                              CSL_EDMA3_ADDRMODE_INCR
                                             );
    myParamSetup.srcAddr = (Uint32)srcBuff2;
    myParamSetup.dstAddr = (Uint32)dstBuff2;
    myParamSetup.linkBcntrld = CSL_EDMA3_LINKBCNTRLD_MAKE(hParamPing,0);
    if (CSL_edma3ParamSetup(hParamPong,&myParamSetup) != CSL_SOK)
    {
        printf("Error: EDMA pong parameter entry setup is failed\n");
        return -1;
    }

    /* Enable channel */
    if (CSL_edma3HwChannelControl(hChannel,CSL_EDMA3_CMD_CHANNEL_ENABLE, NULL) != CSL_SOK)
    {
        printf("Error: EDMA channel enable command is failed\n");
        return -1;
    }

    /* Initialize data */
    for (loopIndex = 0; loopIndex < 512; loopIndex++) {
        srcBuff1[loopIndex] = loopIndex;
        srcBuff2[loopIndex] = loopIndex;
        dstBuff1[loopIndex] = 0;
        dstBuff2[loopIndex] = 0;
    }

    /* Manually trigger the channel */
    if (CSL_edma3HwChannelControl(hChannel,CSL_EDMA3_CMD_CHANNEL_SET,NULL) != CSL_SOK)
    {
        printf("Error: EDMA channel set command is failed\n");
        return -1;
    }

    regionIntr.region = regionNum;
    regionIntr.intr = 0;
    regionIntr.intrh = 0;

    do {
        /* Poll on interrupt bit 0 */
        CSL_edma3GetHwStatus(hModule,CSL_EDMA3_QUERY_INTRPEND,&regionIntr);
    } while (!(regionIntr.intr & 0x1));

    /* Clear interrupt bit 0 */
    if (CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_INTRPEND_CLEAR, &regionIntr) != CSL_SOK)
    {
        printf("Error: EDMA clear interrupt bit 0 command is failed\n");
        return -1;
    }

    /* Mapping channel 0 to event queue 1 */
    if (CSL_edma3HwChannelSetupQue(hChannel,CSL_EDMA3_QUE_1) != CSL_SOK)
    {
        printf("Error: EDMA channel setup queue is failed\n");
        return -1;
    }

    /* Manually trigger the channel */
    if (CSL_edma3HwChannelControl(hChannel,CSL_EDMA3_CMD_CHANNEL_SET,NULL) != CSL_SOK)
    {
        printf("Error: EDMA channel set command is failed\n");
        return -1;
    }

    regionIntr.region = regionNum;
    regionIntr.intr = 0;
    regionIntr.intrh = 0;

    /* Poll on interrupt pend bit 1 */
    do {
        CSL_edma3GetHwStatus(hModule,CSL_EDMA3_QUERY_INTRPEND,&regionIntr);
    } while (!(regionIntr.intr & 0x2));

    /* Clear interrupt bit 1 */
    if (CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_INTRPEND_CLEAR, &regionIntr) != CSL_SOK)
    {
        printf("Error: EDMA clear interrupt bit 1 command is failed\n");
        return -1;
    }

    /* Verify the data has been transferred correctly. */
    if(Verify_Transfer(512, 1, 1, 0, 0, 0, 0, srcBuff1, dstBuff1,TRUE) == FALSE)
    {
        printf ("Error: Verification (Source1/Destination1) Failed\n");
        return -1;
    }
    if(Verify_Transfer(512, 1, 1, 0, 0, 0, 0, srcBuff2, dstBuff2,TRUE) == FALSE)
    {
        printf ("Error: Verification (Source2/Destination2) Failed\n");
        return -1;
    }

    /* Close channel */
    if (CSL_edma3ChannelClose(hChannel) != CSL_SOK)
    {
        printf("Error: EDMA channel close failed\n");
        return -1;
    }

    /* Close EDMA module */
    if (CSL_edma3Close(hModule) != CSL_SOK)
    {
        printf("Error: EDMA module close failed\n");
        return -1;
    }

    /* Test Passed. */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Entry point for the test code.
 *
 *  @retval
 *      Not Applicable.
 */
//void main_2 (void)
//{
//    Uint8  channelNum;
//    Int32  regionNum;
//    Uint8  instNum = 0;
//
//    printf ("**************************************************\n");
//    printf ("******************* EDMA Testing *****************\n");
//    printf ("**************************************************\n");
//
//    /* EDMA Instance 0: Supports only 16 DMA Channels. */
//    for (channelNum = 0; channelNum < 16; channelNum++)
//    {
//        if (edma_ping_pong_xfer_gbl_region(instNum, channelNum) < 0)
//        {
//            printf ("Error: Testing EDMA(%d) Ping-Pong Test (Global) Region for Channel %d FAILED\n", instNum, channelNum);
//            return;
//        }
//        printf ("Debug: Testing EDMA(%d) Ping-Pong Test (Global) Region for Channel %d Passed\n", instNum, channelNum);
//    }
//
//    /* EDMA Shadow Testing: This is run for all the SHADOW Regions from 0-7
//     * and on all supported DMA Channels. */
//    for (regionNum = CSL_EDMA3_REGION_0; regionNum <= CSL_EDMA3_REGION_7; regionNum++)
//    {
//        for (channelNum = 0; channelNum < 16; channelNum++)
//        {
//            if (edma_ping_pong_xfer_region(instNum, channelNum, regionNum) < 0)
//            {
//                printf ("Error: Testing EDMA(%d) Ping-Pong Test Shadow-Region %d for Channel %d FAILED\n",
//                        instNum, regionNum, channelNum);
//                return;
//            }
//            printf ("Debug: Testing EDMA(%d) Ping-Pong Test Shadow-Region %d for Channel %d Passed\n",
//                    instNum, regionNum, channelNum);
//        }
//    }
//
//    /* QDMA Testing: This is run for all SHADOW Regions from 0-7 and on
//     * all supported QDMA Channels; there are 8 supported. Remember QDMA Channels start after the
//     * DMA Channels which are 16 for Instance 0. */
//    for (regionNum = CSL_EDMA3_REGION_GLOBAL; regionNum <= CSL_EDMA3_REGION_7; regionNum++)
//    {
//        for (channelNum = 16; channelNum < 24; channelNum++)
//        {
//            if (qdma_link_xfer_region(instNum, channelNum, regionNum) < 0)
//            {
//                printf ("Error: Testing EDMA(%d) Ping-Pong Test (Shadow) Region %d for QDMA Channel %d FAILED\n",
//                        instNum, regionNum, channelNum);
//                return;
//            }
//            printf ("Debug: Testing EDMA(%d) Ping-Pong Test (Shadow) Region %d for QDMA Channel %d Passed\n",
//                    instNum, regionNum, channelNum);
//        }
//    }
//
//    /* Run the test for EDMA Instance 1 and 2; both have the same limits. */
//    for (instNum = 1; instNum < 3; instNum++)
//    {
//        /* EDMA Instance 1 and 2: Supports 64 DMA Channels. */
//        for (channelNum = 0; channelNum < 64; channelNum++)
//        {
//            if (edma_ping_pong_xfer_gbl_region(instNum, channelNum) < 0)
//            {
//                printf ("Error: Testing EDMA(%d) Ping-Pong Test (Global) Region for Channel %d FAILED\n",
//                        instNum, channelNum);
//                return;
//            }
//            printf ("Debug: Testing EDMA(%d) Ping-Pong Test (Global) Region for Channel %d Passed\n",
//                    instNum, channelNum);
//        }
//
//        /* EDMA Shadow Testing: This is run for all the SHADOW Regions from 0-7
//         * and on all supported DMA Channels. */
//        for (regionNum = CSL_EDMA3_REGION_0; regionNum <= CSL_EDMA3_REGION_7; regionNum++)
//        {
//            for (channelNum = 0; channelNum < 64; channelNum++)
//            {
//                if (edma_ping_pong_xfer_region(instNum, channelNum, regionNum) < 0)
//                {
//                    printf ("Error: Testing EDMA(%d) Ping-Pong Test Shadow-Region %d for Channel %d FAILED\n",
//                            instNum, regionNum, channelNum);
//                    return;
//                }
//                printf ("Debug: Testing EDMA(%d) Ping-Pong Test Shadow-Region %d for Channel %d Passed\n",
//                        instNum, regionNum, channelNum);
//            }
//        }
//
//        /* QDMA Testing: This is run for all SHADOW Regions from 0-7 and on
//         * all supported QDMA Channels; there are 8 supported. Remember QDMA Channels start after the
//         * DMA Channels which are 64 for Instance 1 and 2 */
//        for (regionNum = CSL_EDMA3_REGION_GLOBAL; regionNum <= CSL_EDMA3_REGION_7; regionNum++)
//        {
//            for (channelNum = 64; channelNum < 72; channelNum++)
//            {
//                if (qdma_link_xfer_region(instNum, channelNum, regionNum) < 0)
//                {
//                    printf ("Error: Testing EDMA(%d) Ping-Pong Test (Shadow) Region %d for QDMA Channel %d FAILED\n",
//                            instNum, regionNum, channelNum);
//                    return;
//                }
//                printf ("Debug: Testing EDMA(%d) Ping-Pong Test (Shadow) Region %d for QDMA Channel %d Passed\n",
//                        instNum, regionNum, channelNum);
//            }
//        }
//    }
//
//    printf ("**************************************************\n");
//    printf ("************* EDMA Testing Successful ************\n");
//    printf ("**************************************************\n");
//
//    return;
//}
