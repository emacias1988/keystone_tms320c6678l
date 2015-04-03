#ifndef	_EVM6678_H_
#define _EVM6678_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
//#include "platform.h"
#include <ti/csl/csl_types.h>



#define KB								(256)
#define MB								(256*1024)
#define GB								(256*1024*1024)

#define bool int
#define true 1
#define false 0

//#define DESTBUFFERSIZE (300*KB)
//
//#define DDR3_BASE_ADDRESS            0x80000000
//
//// Global Register and constant definitions
//#define REF_CLOCK_KHZ 100000
//
//// Global timeout value
//#define GTIMEOUT 1000
//
////*****************************************************
//// Power definitions
//#define PSC_BASE            0x02350000
//#define PSC_PTCMD           *( unsigned int* )( PSC_BASE+0x120 )
//#define PSC_PTSTAT          *( unsigned int* )( PSC_BASE+0x128 )
//#define PSC_PDCTL_BASE      ( PSC_BASE+0x300 )
//#define PSC_MDSTAT_BASE     ( PSC_BASE+0x800 )
//#define PSC_MDCTL_BASE      ( PSC_BASE+0xA00 )
//
//#define LPSC_EMIF25_SPI 3 // EMIF16
//#define LPSC_TSIP 4
//#define LPSC_DEBUGSS_TRC 5
//// PD 1
//#define LPSC_TETB_TRC 6
//// PD 2
//#define LPSC_PKTPROC 7  // PA
//#define LPSC_CPGMAC 8   // SGMII
//#define LPSC_Crypto  9  // SA
//// PD 3
//#define LPSC_PCIEX 10
//// PD 4
//#define LPSC_SRIO 11
//// PD 5
//#define LPSC_Hyperbridge 12
//// PD 7
//#define LPSC_MSMCSRAM 14
//
//// Power domains definitions
//#define PD0         (0)     // Power Domain-0
//#define PD1         (1)     // Power Domain-1
//#define PD2         (2)     // Power Domain-2
//#define PD3         (3)     // Power Domain-3
//#define PD4         (4)     // Power Domain-4
//#define PD5         (5)     // Power Domain-5
//#define PD6         (6)     // Power Domain-6
//#define PD7         (7)     // Power Domain-7
//#define PD8         (8)     // Power Domain-8
//#define PD9         (9)     // Power Domain-9
//#define PD10        (10)    // Power Domain-10
//#define PD11        (11)    // Power Domain-11
//#define PD12        (12)    // Power Domain-12
//#define PD13        (13)    // Power Domain-13
//#define PD14        (14)    // Power Domain-14
//#define PD15        (15)    // Power Domain-15
//#define PD16        (16)    // Power Domain-16
//#define PD17        (17)    // Power Domain-17
//
//#define PSC_SYNCRESET (0x1)
//#define PSC_DISABLE   (0x2)
//#define PSC_ENABLE    (0x3)
//
////*****************************************************
//// BOOT and CONFIG dsp system modules Definitions
//#define CHIP_LEVEL_REG  0x02620000
//#define DEVSTAT                     (*(unsigned int*)(CHIP_LEVEL_REG + 0x0020))
//#define KICK0                       (*(unsigned int*)(CHIP_LEVEL_REG + 0x0038))
//#define KICK1                       (*(unsigned int*)(CHIP_LEVEL_REG + 0x003C))
//#define TINPSEL                     (*(unsigned int*)(CHIP_LEVEL_REG + 0x0300))
//#define TOUTPSEL                    (*(unsigned int*)(CHIP_LEVEL_REG + 0x0304))
//#define MAINPLLCTL0                 (*(unsigned int*)(CHIP_LEVEL_REG + 0x0328))
//#define MAINPLLCTL1                 (*(unsigned int*)(CHIP_LEVEL_REG + 0x032C))
//#define DDR3PLLCTL0                 (*(unsigned int*)(CHIP_LEVEL_REG + 0x0330))
//#define DDR3PLLCTL1                 (*(unsigned int*)(CHIP_LEVEL_REG + 0x0334))
//#define PAPLLCTL0                   (*(unsigned int*)(CHIP_LEVEL_REG + 0x0338))
//#define PAPLLCTL1                   (*(unsigned int*)(CHIP_LEVEL_REG + 0x033C))
//#define OBSCLKCTL                   (*(unsigned int*)(CHIP_LEVEL_REG + 0x03AC))
//
//// DDR3 tuning registers
//#define DATA0_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x043C))
//#define DATA1_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0440))
//#define DATA2_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0444))
//#define DATA3_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0448))
//#define DATA4_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x044C))
//#define DATA5_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0450))
//#define DATA6_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0454))
//#define DATA7_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0458))
//#define DATA8_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x045C))
//
//#define DATA0_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x040C))
//#define DATA1_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0410))
//#define DATA2_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0414))
//#define DATA3_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0418))
//#define DATA4_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x041C))
//#define DATA5_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0420))
//#define DATA6_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0424))
//#define DATA7_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0428))
//#define DATA8_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x042C))
//
//#define DDR3_CONFIG_REG_0           (*(unsigned int*)(CHIP_LEVEL_REG + 0x0404))
//#define DDR3_CONFIG_REG_12          (*(unsigned int*)(CHIP_LEVEL_REG + 0x0434))
//#define DDR3_CONFIG_REG_23          (*(unsigned int*)(CHIP_LEVEL_REG + 0x0460))
//#define DDR3_CONFIG_REG_24          (*(unsigned int*)(CHIP_LEVEL_REG + 0x0464))
//
//#define SGMII_SERDES_CFGPLL         (*(unsigned int*)(CHIP_LEVEL_REG + 0x340))
//#define SGMII_SERDES_CFGRX0         (*(unsigned int*)(CHIP_LEVEL_REG + 0x344))
//#define SGMII_SERDES_CFGTX0         (*(unsigned int*)(CHIP_LEVEL_REG + 0x348))
//#define SGMII_SERDES_CFGRX1         (*(unsigned int*)(CHIP_LEVEL_REG + 0x34C))
//#define SGMII_SERDES_CFGTX1         (*(unsigned int*)(CHIP_LEVEL_REG + 0x350))
//
//#define KICK0_UNLOCK (0x83E70B13)
//#define KICK1_UNLOCK (0x95A4F1E0)
//#define KICK_LOCK    0
//
///* PA PLL Observation Clock Control Register */
////#define OBSCLKCTL (*((unsigned int *) 0x026203AC))
//#define PA_PLL_OBS_CLK_SEL_MASK (1 << 4) /* OBSCLKCTL Register Bit 4 - set to 0 to see PA PLL reference (input) clock, set to 1 to see PA PLL output*/
//#define PA_PLL_OBS_CLK_EN_MASK  (1 << 5) /* OBSCLKCTL Register Bit 5 - set to 1 to enable power to PA PLL observation clock*/
//
///* PA PLL Registers */
//#define BYPASS_BIT_SHIFT 23
//#define CLKF_BIT_SHIFT   6
//#define CLKR_BIT_SHIFT   0
////#define DEVSTAT    (*((unsigned int *) 0x02620020))
////#define PAPLLCTL0  (*((unsigned int *) 0x02620338))
////#define PAPLLCTL1  (*((unsigned int *) 0x0262033C))
//#define PASSCLKSEL_MASK    (1 << 17)    /* Tells the configuration of the PASSCLKSEL pin */
//#define PA_PLL_BYPASS_MASK (1 << BYPASS_BIT_SHIFT)    /* Tells whether the PA PLL is in BYPASS mode or not */
//#define PA_PLL_CLKOD_MASK  (0x00780000) /* Tells the output divider value for the PA PLL */
//#define PA_PLL_CLKF_MASK   (0x0007FFC0) /* Tells the multiplier value for the PA PLL */
//#define PA_PLL_CLKR_MASK   (0x0000003F) /* Tells the divider value for the PA PLL */
//
////*****************************************************
//// Extended Memory Controller (XMC) Configuration
//#define XMC_BASE_ADDR (0x08000000)
//#define XMPAX2_L     (*(unsigned int*)(XMC_BASE_ADDR + 0x00000010))
//#define XMPAX2_H     (*(unsigned int*)(XMC_BASE_ADDR + 0x00000014))
//
//// DDR3 definitions
//#define DDR_BASE_ADDR          0x21000000
//
//#define DDR_MIDR                    (*(unsigned int*)(DDR_BASE_ADDR + 0x00000000))
//#define DDR_SDCFG                   (*(unsigned int*)(DDR_BASE_ADDR + 0x00000008))
//#define DDR_SDRFC                   (*(unsigned int*)(DDR_BASE_ADDR + 0x00000010))
//#define DDR_SDTIM1                  (*(unsigned int*)(DDR_BASE_ADDR + 0x00000018))
//#define DDR_SDTIM2                  (*(unsigned int*)(DDR_BASE_ADDR + 0x00000020))
//#define DDR_SDTIM3                  (*(unsigned int*)(DDR_BASE_ADDR + 0x00000028))
//#define DDR_PMCTL                   (*(unsigned int*)(DDR_BASE_ADDR + 0x00000038))
//#define DDR_ZQCFG                   (*(unsigned int*)(DDR_BASE_ADDR + 0x000000C8))
//#define DDR_RDWR_LVL_RMP_CTRL       (*(unsigned int*)(DDR_BASE_ADDR + 0x000000D8))
//#define DDR_TMPALRT                 (*(unsigned int*)(DDR_BASE_ADDR + 0x000000CC))
//#define DDR_RDWR_LVL_CTRL           (*(unsigned int*)(DDR_BASE_ADDR + 0x000000DC))
//#define DDR_DDRPHYC                 (*(unsigned int*)(DDR_BASE_ADDR + 0x000000E4))
//
//#define RD_DQS_SLAVE_RATIO_1333 0x34
//#define WR_DQS_SLAVE_RATIO_1333 0x45
//#define WR_DATA_SLAVE_RATIO_1333 0x85
//#define FIFO_WE_SLAVE_RATIO_1333 0xBC
//
//#define RD_DQS_SLAVE_RATIO_1066 0x34
//#define WR_DQS_SLAVE_RATIO_1066 0x37
//#define WR_DATA_SLAVE_RATIO_1066 0x77
//#define FIFO_WE_SLAVE_RATIO_1066 0xA0
//
////*****************************************************
//// PLL 1 definitions (DSP clk and subsystems)
//#define PLL1_BASE           0x02310000
//#define PLL1_PLLCTL              (*(unsigned int*)(PLL1_BASE + 0x100))   // PLL1 Control
//#define PLL1_SECCTL              (*(unsigned int*)(PLL1_BASE + 0x108))   // PLL1 Sec Control
//#define PLL1_PLLM                (*(unsigned int*)(PLL1_BASE + 0x110))   // PLL1 Multiplier
//#define PLL1_DIV1                (*(unsigned int*)(PLL1_BASE + 0x118))   // DIV1 divider
//#define PLL1_DIV2                (*(unsigned int*)(PLL1_BASE + 0x11C))   // DIV2 divider
//#define PLL1_DIV3                (*(unsigned int*)(PLL1_BASE + 0x120))   // DIV3 divider
//#define PLL1_CMD                 (*(unsigned int*)(PLL1_BASE + 0x138))   // CMD control
//#define PLL1_STAT                (*(unsigned int*)(PLL1_BASE + 0x13C))   // STAT control
//#define PLL1_ALNCTL              (*(unsigned int*)(PLL1_BASE + 0x140))   // ALNCTL control
//#define PLL1_DCHANGE             (*(unsigned int*)(PLL1_BASE + 0x144))   // DCHANGE status
//#define PLL1_CKEN                (*(unsigned int*)(PLL1_BASE + 0x148))   // CKEN control
//#define PLL1_CKSTAT              (*(unsigned int*)(PLL1_BASE + 0x14C))   // CKSTAT status
//#define PLL1_SYSTAT              (*(unsigned int*)(PLL1_BASE + 0x150))   // SYSTAT status
//#define PLL1_DIV4                (*(unsigned int*)(PLL1_BASE + 0x160))   // DIV4 divider
//#define PLL1_DIV5                (*(unsigned int*)(PLL1_BASE + 0x164))   // DIV5 divider
//#define PLL1_DIV6                (*(unsigned int*)(PLL1_BASE + 0x168))   // DIV6 divider
//#define PLL1_DIV7                (*(unsigned int*)(PLL1_BASE + 0x16C))   // DIV7 divider
//#define PLL1_DIV8                (*(unsigned int*)(PLL1_BASE + 0x170))   // DIV8 divider
//#define PLL1_DIV9                (*(unsigned int*)(PLL1_BASE + 0x174))   // DIV9 divider
//#define PLL1_DIV10               (*(unsigned int*)(PLL1_BASE + 0x178))   // DIV10 divider
//#define PLL1_DIV11               (*(unsigned int*)(PLL1_BASE + 0x17C))   // DIV11 divider
//#define PLL1_DIV12               (*(unsigned int*)(PLL1_BASE + 0x180))   // DIV12 divider
//#define PLL1_DIV13               (*(unsigned int*)(PLL1_BASE + 0x184))   // DIV13 divider
//#define PLL1_DIV14               (*(unsigned int*)(PLL1_BASE + 0x188))   // DIV14 divider
//#define PLL1_DIV15               (*(unsigned int*)(PLL1_BASE + 0x18C))   // DIV15 divider
//#define PLL1_DIV16               (*(unsigned int*)(PLL1_BASE + 0x190))   // DIV16 divider
//
//#define PLL_REG_RSCTL_VALUE_KEY                  (0x5A69)
//#define PLL_REG_RSCFG_FIELD_POWER_ON_RESET       (1<<29)
//
//
////*****************************************************
//// CACHE definitions
//#define CACHE_BASE          0x01840000
//#define CACHE_L2CFG             (*( unsigned int* )( CACHE_BASE ))
//#define CACHE_L1PCFG            (*( unsigned int* )( CACHE_BASE + 0x0020 ))
//#define CACHE_L1DCFG            (*( unsigned int* )( CACHE_BASE + 0x0040 ))
//#define L2WBINV                 (CACHE_BASE + 0x5004) // L2WBINV Control
//#define L2INV                   (CACHE_BASE + 0x5008) // L2INV Control
//#define L1PINV                  (CACHE_BASE + 0x5028) // L1PINV Control
//#define L1DWBINV                (CACHE_BASE + 0x5044) // L1DWBINV Control
//#define L1DINV                  (CACHE_BASE + 0x5048) // L1DINV Control
//
////*****************************************************
//// EDMA3 definitions
//#define EDMA3_TPCC0_BASE    0x02700000
//#define IERH_0                (EDMA3_TPCC0_BASE + 0x1054) // IERH Control
//#define EERH_0                (EDMA3_TPCC0_BASE + 0x1024) // EERH Control
//#define ICRH_0                (EDMA3_TPCC0_BASE + 0x1074) // ICRH Control
//#define ECRH_0                (EDMA3_TPCC0_BASE + 0x100C) // ECRH Control
//#define IER_0                 (EDMA3_TPCC0_BASE + 0x1050) // IER Control
//#define EER_0                 (EDMA3_TPCC0_BASE + 0x1020) // EER Control
//#define ICR_0                 (EDMA3_TPCC0_BASE + 0x1070) // ICR Control
//#define ECR_0                 (EDMA3_TPCC0_BASE + 0x1008) // ECR Control
//#define IECRH_0               (EDMA3_TPCC0_BASE + 0x105C) // IECRH Control
//#define IECR_0                (EDMA3_TPCC0_BASE + 0x1058) // IECR Control
//#define EECRH_0               (EDMA3_TPCC0_BASE + 0x102C) // EECRH Control
//#define EECR_0                (EDMA3_TPCC0_BASE + 0x1028) // EECR Control
//
//#define EDMA3_TPCC1_BASE    0x02720000
//#define IERH_1                (EDMA3_TPCC1_BASE + 0x1054) // IERH Control
//#define EERH_1                (EDMA3_TPCC1_BASE + 0x1024) // EERH Control
//#define ICRH_1                (EDMA3_TPCC1_BASE + 0x1074) // ICRH Control
//#define ECRH_1                (EDMA3_TPCC1_BASE + 0x100C) // ECRH Control
//#define IER_1                 (EDMA3_TPCC1_BASE + 0x1050) // IER Control
//#define EER_1                 (EDMA3_TPCC1_BASE + 0x1020) // EER Control
//#define ICR_1                 (EDMA3_TPCC1_BASE + 0x1070) // ICR Control
//#define ECR_1                 (EDMA3_TPCC1_BASE + 0x1008) // ECR Control
//#define IECRH_1               (EDMA3_TPCC1_BASE + 0x105C) // IECRH Control
//#define IECR_1                (EDMA3_TPCC1_BASE + 0x1058) // IECR Control
//#define EECRH_1               (EDMA3_TPCC1_BASE + 0x102C) // EECRH Control
//#define EECR_1                (EDMA3_TPCC1_BASE + 0x1028) // EECR Control
//
//#define EDMA3_TPCC2_BASE    0x02740000
//#define IERH_2                (EDMA3_TPCC2_BASE + 0x1054) // IERH Control
//#define EERH_2                (EDMA3_TPCC2_BASE + 0x1024) // EERH Control
//#define ICRH_2                (EDMA3_TPCC2_BASE + 0x1074) // ICRH Control
//#define ECRH_2                (EDMA3_TPCC2_BASE + 0x100C) // ECRH Control
//#define IER_2                 (EDMA3_TPCC2_BASE + 0x1050) // IER Control
//#define EER_2                 (EDMA3_TPCC2_BASE + 0x1020) // EER Control
//#define ICR_2                 (EDMA3_TPCC2_BASE + 0x1070) // ICR Control
//#define ECR_2                 (EDMA3_TPCC2_BASE + 0x1008) // ECR Control
//#define IECRH_2               (EDMA3_TPCC2_BASE + 0x105C) // IECRH Control
//#define IECR_2                (EDMA3_TPCC2_BASE + 0x1058) // IECR Control
//#define EECRH_2               (EDMA3_TPCC2_BASE + 0x102C) // EECRH Control
//#define EECR_2                (EDMA3_TPCC2_BASE + 0x1028) // EECR Control
//
////*****************************************************
//// GPIO definitions
//#define GPIO_BASE           0x02320000
//#define GPIO_BITEN          (*(unsigned int*)(GPIO_BASE + 0x0008)) // BITEN Control
//#define GPIO_DIR            (*(unsigned int*)(GPIO_BASE + 0x0010)) // DIR Control
//#define GPIO_OUT_DATA       (*(unsigned int*)(GPIO_BASE + 0x0014)) // OUT_DATA Control
//#define GPIO_IN_DATA        (*(unsigned int*)(GPIO_BASE + 0x0020)) // IN_DATA Register
//#define GPIO_CLR_RIS_TRIG   (*(unsigned int*)(GPIO_BASE + 0x0028)) // CLR_RIS_TRIG Control
//#define GPIO_CLR_FAL_TRIG   (*(unsigned int*)(GPIO_BASE + 0x0030)) // CLR_FAL_TRIG Control
//
//#define GPIO_DEFAULT_DIR    0xFFFF2CFF  // GP08,GP09,GP12,GP14,GP15 (Nand) are outputs
//#define GPIO_DEFAULT_OUT    0x0000D000  // GP15, GP14, GP12 default to HIGH
//
//// Used for eeprom programming
//#define LITTLE_END 1
//#define BIG_END 2
//
//// SGMII definitions
//#define SGMII_SERDES_BASE  0x02090000
//#define SGMII_SLIVER_BASE1 0x02090900
//#define SGMII_SLIVER_BASE2 0x02090940
//#define SGMII_SERDES_CONTROL_PORT1      (*(unsigned int*)(SGMII_SERDES_BASE  + 0x210))
//#define SGMII_SERDES_MR_ADV_PORT1       (*(unsigned int*)(SGMII_SERDES_BASE  + 0x218))
//#define SGMII_SERDES_AUX_CFG_PORT1      (*(unsigned int*)(SGMII_SERDES_BASE  + 0x238))
//#define SGMII_SERDES_CONTROL_PORT0      (*(unsigned int*)(SGMII_SERDES_BASE  + 0x110))
//#define SGMII_SERDES_MR_ADV_PORT0       (*(unsigned int*)(SGMII_SERDES_BASE  + 0x118))
//#define SGMII_SERDES_AUX_CFG_PORT0      (*(unsigned int*)(SGMII_SERDES_BASE  + 0x138))
//#define SGMII_SLIVER_MACCONTROL1        (*(unsigned int*)(SGMII_SLIVER_BASE1 + 0x004))
//#define SGMII_SLIVER_MAXLEN1            (*(unsigned int*)(SGMII_SLIVER_BASE1 + 0x010))
//#define SGMII_SLIVER_MACCONTROL2        (*(unsigned int*)(SGMII_SLIVER_BASE2 + 0x004))
//#define SGMII_SLIVER_MAXLEN2            (*(unsigned int*)(SGMII_SLIVER_BASE2 + 0x010))
//
//#define CPSW3G_BASE             0x02090800
//#define CPSW3G_CONTROL_REG              (*(unsigned int*)(CPSW3G_BASE + 0x004))
//#define CPSW3G_STAT_PORT_REG            (*(unsigned int*)(CPSW3G_BASE + 0x00C))
//#define CPSW3G_ALE_CONTROL_REG          (*(unsigned int*)(CPSW3G_BASE + 0x608))
//#define CPSW3G_ALE_PORT_0_CTL_REG       (*(unsigned int*)(CPSW3G_BASE + 0x640))
//#define CPSW3G_ALE_PORT_1_CTL_REG       (*(unsigned int*)(CPSW3G_BASE + 0x644))
//#define CPSW3G_ALE_PORT_2_CTL_REG       (*(unsigned int*)(CPSW3G_BASE + 0x648))
//#define CPSW3G_ALE_PORT_STATE_FORWARD   0x3
//
//
////TIMER 0 definitions
//#define TIMER0_CNTLO                     (*(unsigned int*)(0x02200010))
//#define TIMER0_CNTHI                     (*(unsigned int*)(0x02200014))
//#define TIMER0_PRDLO                     (*(unsigned int*)(0x02200018))
//#define TIMER0_PRDHI                     (*(unsigned int*)(0x0220001C))
//#define TIMER0_TCR                       (*(unsigned int*)(0x02200020))
//#define TIMER0_TGCR                      (*(unsigned int*)(0x02200024))
//
//
//#define L2EDSTAT 0x01846004
//#define L2EDCMD    0x01846008
//#define L2EDADDR 0x0184600C
//#define L2EDCPEC 0x01846018
//#define L2EDCNEC 0x0184601C
//#define L2EDCEN    0x01846030
//
//#define L1PEDSTAT  0x01846404
//#define L1PEDCMD 0x01846408
//#define L1PEDADDR  0x0184640C
//
//#define SMCERRAR 0x0BC00008
//#define  SMCERRXR  0x0BC0000C
//#define SMEDCC   0x0BC00010
//#define SMCEA    0x0BC00014
//#define SMSECC   0x0BC00018
//
//#define PLL_REINIT_MAX_COUNT (10)
//
//int Init_PLL(int pll_mult, int pll_div );

// EDMA Functions

extern Bool Verify_Transfer
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
);

extern Int32 qdma_link_xfer_region (Int32 instNum, Uint8 channelNum, Int32 regionNum);

extern Int32 edma_ping_pong_xfer_gbl_region
(
	Int32 instNum,
	Uint8 channelNum,
	Uint32 ui32SrcAddr,
	Uint32 ui32DstAddr,
	Uint32 ui32TransferSizeWords
);

extern Int32 edma_ping_pong_xfer_region (Int32 instNum, Uint8 channelNum, Int32 regionNum);

// Timing Help Functions

// Definition of TSCL register to use to count CLK cycles
extern unsigned int volatile cregister TSCL;
extern unsigned int volatile cregister TSCH;

//extern platform_info	g_sEvmInfo;

extern uint64_t g_ui64StartTime;
extern uint64_t g_ui64StopTime;
extern uint64_t g_ui64ElapsedTime;

//extern uint64_t Osal_calculateElapsedTime(uint64_t ui64Start, uint64_t ui64Stop);
//
//void CMP_runTests(char * psFuncName, uint32_t ui32Size, char * pcBuffer);
//
//void CMP_spTest(char * psFuncName, uint32_t ui32Size, char * pcBuffer);

#endif
