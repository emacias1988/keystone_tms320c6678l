#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform.h"
#include "EVM6678.h"
//
//#include <ti/csl/src/intc/csl_intc.h>
//#include <ti/csl/tistdtypes.h>
//#include <ti/csl/csl_cpIntcAux.h>

//*****************************************************
// Timeout definitions
int _GEL_Global_Timeout1 = 0;
#define TIMEOUT_ID 10

//********************************************************
int iRESULT = 0;

/****************************************************************************
 *
 * NAME
 *      prog_pll1_values
 *
 * PURPOSE:
 *      Wait for a specified delay in number of empty loop.
 *
 * USAGE
 *      This routine can be called as:
 *
 *      Wait_Soft(nloop)
 *
 *      nloop - (i) number of empty loop to do for delay
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void prog_pll1_values_2(unsigned int pll_multiplier, unsigned int pll_divider, unsigned int odiv)
{
    int TEMP;

    /* Check the Range for the parameters */

    if (odiv > 15)
    {
    }

    if (pll_multiplier > 4095)
    {
    }

    if (pll_divider > 63)
    {
    }

    /* Set the PLL Multiplier, Divider, BWADJ                                    *
     * The PLLM[5:0] bits of the multiplier are controlled by the PLLM Register  *
     * inside the PLL Controller and the PLLM[12:6] bits are controlled by the   *
     * chip-level MAINPLLCTL0 Register.                                          *
     * PLL Control Register (PLLM)  Layout                                       *
     * |31...6   |5...0        |                                                 *
     * |Reserved |PLLM         |                                                 *
     *                                                                           *
     * Main PLL Control Register (MAINPLLCTL0)                                   *
     * |31...24   |23...19   |18...12    | 11...6   |5...0 |                     *
     * |BWADJ[7:0]| Reserved |PLLM[12:6] | Reserved | PLLD |                     */

    /* Set pll multipler (13 bit field) */
    PLL1_PLLM     = (pll_multiplier & 0x0000003F); /* bits[5:0]  */
    TEMP          = (pll_multiplier & 0x1FC0) >> 6;/* bits[12:6] */
    MAINPLLCTL0  &=~(0x0007F000);                /*Clear PLLM field */
    MAINPLLCTL0  |=((TEMP << 12) & 0x0007F000);

   /* Set the BWADJ     (12 bit field)                                          *
     * BWADJ[11:8] and BWADJ[7:0] are located in MAINPLLCTL0 and MAINPLLCTL1     *
     * registers. BWADJ[11:0] should be programmed to a value equal to half of   *
     * PLLM[12:0] value (round down if PLLM has an odd value)                    *
     * Example: If PLLM = 15, then BWADJ = 7                                     */
    TEMP = ((pll_multiplier + 1) >> 1) - 1; /* Divide the pllm by 2 */
    MAINPLLCTL0 &=~(0xFF000000);  /* Clear the BWADJ Field */
    MAINPLLCTL0 |=  ((TEMP << 24) & 0xFF000000);
    MAINPLLCTL1 &=~(0x0000000F);   /* Clear the BWADJ field */
    MAINPLLCTL1 |= ((TEMP >> 8) & 0x0000000F);

    /* Set the pll divider (6 bit field)                                         *
     * PLLD[5:0] is located in MAINPLLCTL0                                       */
    MAINPLLCTL0   &= ~(0x0000003F);    /* Clear the Field */
    MAINPLLCTL0   |= (pll_divider & 0x0000003F);

    /* Set the OUTPUT DIVIDE (4 bit field) in SECCTL */
    PLL1_SECCTL    &= ~(0x00780000);     /* Clear the field       */
    PLL1_SECCTL   |= ((odiv << 19) & 0x00780000) ;

}

void Delay_milli_seconds(uint32_t delay)
{
	platform_delay(1000*delay);
}

/****************************************************************************
 *
 * NAME
 *      Init PLL
 *
 * PURPOSE:
 *      Performs the main PLL initialization
 *
 * USAGE
 *      This function can be called as below.
 *       Init_PLL()
 *
 * RETURN VALUE
 *      iresult = 0: Success; -1=fail
 *
 * REFERENCE
 *
 * NOTE
 *     Note that this function requires pll_mult and pll_div arguments to be
 *     actual register values, i.e., the values are original values - 1
 *     (pll_* = calculated_value_of_pll_* - 1)
 *
 ****************************************************************************/
int Init_PLL(int pll_mult, int pll_div )
{
    int i, TEMP;
    /* Default dividers */
    unsigned int div2=3, div5=5, div8=64;
    int dsp_freq;
    int dsp_freM,dsp_freD;

    /*Unlock Boot Config*/
    KICK0 = 0x83E70B13;
    KICK1 = 0x95A4F1E0;

    // Only core0 can set PLL

	/* 1. Wait for Stabilization time (min 100 us)                                *
	 * The below loop is good enough for the Gel file to get minimum of        *
	 * 100 micro seconds, this should be appropriately modified for port       *
	 * to a C function                                                         *
	 * Minimum delay in GEL can be 1 milli seconds, so program to 1ms=1000us,  *
	 * more than required, but should be Okay                                  */
	Delay_milli_seconds(1);

	/* 2. Check the status of BYPASS bit in SECCTL register,                   *
	 *    execute following steps if                                           *
	 *    BYPASS == 1 (if bypass enabled), if BYPASS==0 then Jump to Step 3    */
	TEMP = PLL1_SECCTL &  0x00800000; /* Check the Bit 23 value */

	if (TEMP != 0) /* PLL BYPASS is enabled, we assume if not in Bypass ENSAT = 1 */
	{

		/* 2a. Usage Note 9: For optimal PLL operation, the ENSAT bit in the PLL control *
		 * registers for the Main PLL, DDR3 PLL, and PA PLL should be set to 1.          *
		 * The PLL initialization sequence in the boot ROM sets this bit to 0 and        *
		 * could lead to non-optimal PLL operation. Software can set the bit to the      *
		 * optimal value of 1 after boot                                                 *
		 * Ref: http://www.ti.com/lit/er/sprz334b/sprz334b.pdf                           *
		 * |31...7   |6     |5 4       |3...0      |                                     *
		 * |Reserved |ENSAT |Reserved  |BWADJ[11:8]|                                     */

		MAINPLLCTL1 = MAINPLLCTL1 | 0x00000040;

		/* 2b. Clear PLLEN bit */
		PLL1_PLLCTL &= ~(1 << 0);

		/* 2c. Clear PLLENSRC bit */
		PLL1_PLLCTL &= ~(1 << 5);

		/* 2d. Wait for 4 RefClks   *
		 * Assuming slowest Ref clock of 25MHz, min: 160 ns delay */
		Delay_milli_seconds(1);

		/* 2e. Bypass needed to perform PWRDN cycle for C6670 and C6678                  *
		 * Needed on all devices when in NOBOOT, I2C or SPI boot modes                   *
		 * Ref: Figure 4-2 of http://www.ti.com/lit/ug/sprugv2a/sprugv2a.pdf             *
		 * PLL Secondary Control Register (SECCTL)  Layout                               *
		 * |31...24  |23     |22...19       |18...0   |                                  *
		 * |Reserved |BYPASS |OUTPUT DIVIDE |Reserved |                                  */
		PLL1_SECCTL |= 0x00800000; /* Set the Bit 23 */

		/* 2f. Advisory 8: Multiple PLLs May Not Lock After Power-on Reset Issue         *
		 * In order to ensure proper PLL startup, the PLL power_down pin needs to be     *
		 * toggled. This is accomplished by toggling the PLLPWRDN bit in the PLLCTL      *
		 * register. This needs to be done before the main PLL initialization            *
		 * sequence                                                                      *
		 * Ref: Figure 4-1 of http://www.ti.com/lit/ug/sprugv2a/sprugv2a.pdf             *
		 * PLL Control Register (PLLCTL)  Layout                                         *
		 * |31...4   |3      |2        |1        |0        |                             *
		 * |Reserved |PLLRST |Reserved |PLLPWRDN |Reserved |                             */

		PLL1_PLLCTL   |= 0x00000002; /*Power Down the PLL */

		/* 2g. Stay in a loop such that the bit is set for 5 µs (minimum) and           *
		 * then clear the bit.                                                          */

		Delay_milli_seconds(1); /* This is more than required delay */

		/* 2h. Power up the PLL */
		PLL1_PLLCTL   &= ~(0x00000002);

	}
	else
	{
		/* 3. Enable BYPASS in the PLL controller */

		/* 3a. Clear PLLEN bit (bypass enabled in PLL controlelr mux) */
		PLL1_PLLCTL &= ~(1 << 0);

		/* 3b. Clear PLLENSRC bit (enable PLLEN to control PLL controller mux) */
		PLL1_PLLCTL &= ~(1 << 5);

		/* 3c. Wait for 4 RefClks (to make sure the PLL controller *
		 * mux switches properly to the bypass)                    *
		 * Assuming slowest Ref clock of 25MHz, min: 160 ns delay  */
		Delay_milli_seconds(1);
	}

	/* Wait for PLL Reset assertion Time (min: 50 us)                          *
	 * Minimum delay in GEL can be 1 milli seconds, so program to 1ms=1000us,  *
	 * more than required, but should be Okay                                  */
	//Delay_milli_seconds(1);

	/* 4, 5, 6 and 7 are done here:                                              *
	 * Program the necessary multipliers/dividers and BW adjustments             */
	prog_pll1_values_2(pll_mult, pll_div, 1);

	/* 8. Set PLL dividers if needed */

	/* part of 8, go stat bit needs to be zero here                             *
	 * Read the GOSTAT bit in PLLSTAT to make sure the bit returns to 0 to      *
	 * indicate that the GO operation has completed                             */
	/* wait for the GOSTAT, but don't trap if lock is never read */
	for (i = 0; i < 1000; i++)
	{
		if ( (PLL1_STAT & 0x00000001) == 0 ) {
			break;
		}
	}
	if ( i == 1000 ) {
		return(-1);
	}

	/* part of 8, Set PLL dividers if needed */
	PLL1_DIV2 = (0x8000) | (div2 - 1);
	PLL1_DIV5 = (0x8000) | (div5 - 1);
	PLL1_DIV8 = (0x8000) | (div8 - 1);

	/* part of 8, Program ALNCTLn */
	/* Set bit 1, 4 and 7 */
	PLL1_ALNCTL |= ( (1 << 1) | (1 << 4) | (1 << 7));

	/* part of 8, Set GOSET bit in PLLCMD to initiate the GO operation to change the divide *
	 * values and align the SYSCLKs as programmed                                           */
	PLL1_CMD     |= 0x00000001;

	/* part of 8, Read the GOSTAT bit in PLLSTAT to make sure the bit returns to 0 to      *
	 * indicate that the GO operation has completed                             */

	/* wait for the GOSTAT, but don't trap if lock is never read */
	for (i = 0; i < 1000; i++)
	{

		if ( (PLL1_STAT & 0x00000001) == 0 ) {
			break;
		}

	}

	if ( i == 1000 ) {
		return(-1);
	}

	/* 9. Place PLL in Reset, In PLLCTL, write PLLRST = 1 (PLL is reset)         */
	PLL1_PLLCTL |= 0x00000008;

	/* 10. Wait for the PLL Reset duration time (min: 7us    )                */
	Delay_milli_seconds(1);

	/* 11. In PLLCTL, write PLLRST = 0 to bring PLL out of reset */
	PLL1_PLLCTL &= ~(0x00000008);

	/*
	 * 12. PLL Lock Delay needs to be 500 RefClk periods * (PLLD + 1)
	 * i.e., Wait for at least 500 * CLKIN cycles * (PLLD + 1) (PLL lock time)
	 * Using 2000 25ns RefClk periods per DM
	 * Wait for PLL to lock min 50 micro seconds
	 *
	 * */
	Delay_milli_seconds(1);

	/* 13. In SECCTL, write BYPASS = 0 to enable PLL mux to switch to PLL mode */
	PLL1_SECCTL &= ~(0x00800000); /* Release Bypass */

	/* 14. Set the PLLEN */
	PLL1_PLLCTL |= (1 << 0);

	/* 15. The PLL and PLL Controller are now initialized in PLL mode - Complete */

	// Compute the real dsp freq (*100)
	dsp_freq = (((REF_CLOCK_KHZ/10) * ((pll_mult+1)/2))/(pll_div+1));

	// Displayed frequency setup
	// dsp freq in MHz
	dsp_freM = dsp_freq / 100;

	// dsp freq first decimal if freq expressed in MHz
	dsp_freD = ((dsp_freq - dsp_freM * 100) + 5) / 10;

	// Add roundup unit to MHz displayed and reajust decimal value if necessary...
	if (dsp_freD > 9)
	{
		dsp_freD = dsp_freD - 10;
		dsp_freM = dsp_freM + 1;
	}

	// Print freq info...

	return (0);
}
