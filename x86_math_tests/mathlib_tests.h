#ifndef _MATHLIB_TEST_
#define _MATHLIB_TEST_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define KB								(256)
#define MB								(256*1024)
#define GB								(256*1024*1024)
#define DESTBUFFERSIZE (300*KB)

#define uint32_t unsigned int
#define uint64_t unsigned long

#define bool int
#define true 1
#define false 0

extern uint16_t g_ui16CPUFrequency;

//
// ATAN
//
void ATAN_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void ATAN_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void ATAN_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);

//
// ATAN2
//
void ATAN2_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void ATAN2_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void ATAN2_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);


//
// CMP
//
void CMP_runTests(char * psFuncName, uint32_t ui32Size, char * pcSrcBuffer, char * pcDestBuffer);
void CMP_spTest(char * psFuncName, uint32_t ui32Size, char * pcBuffer);

//
// COS
//
void COS_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void COS_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void COS_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);


//
// DIV
//
void DIV_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void DIV_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void DIV_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);

//
// EXP
//
void EXP_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void EXP_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void EXP_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);


//
// EXP2
//
void EXP2_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void EXP2_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void EXP2_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);

//
// EXP10
//
void EXP10_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void EXP10_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void EXP10_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);

//
// LOG10 Functions
//
void LOG10_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void LOG10_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void LOG10_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);

//
// LOG2 Functions
//
void LOG2_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void LOG2_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void LOG2_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);

//
// LOG Functions
//
void LOG_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void LOG_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void LOG_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);


//
// POW
//
void POW_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void POW_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void POW_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);

//
// RECIP Functions
//
void RECIP_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void RECIP_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void RECIP_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);

//
// RSQRT Functions
//
void RSQRT_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void RSQRT_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void RSQRT_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);

//
// SIN Functions
//
void SIN_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void SIN_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void SIN_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);

//
// SQRT Functions
//
void SQRT_runTests(char * psFuncName, uint32_t ui32Size, float * pfSrcBuffer, float * pfDestBuffer);
void SQRT_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);
void SQRT_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);

#endif
