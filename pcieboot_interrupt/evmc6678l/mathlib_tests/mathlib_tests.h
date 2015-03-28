#ifndef _MATHLIB_TEST_
#define _MATHLIB_TEST_

#include "EVM6678.h"
#include <C:/ti/mathlib_c66x_3_0_1_1/inc/mathlib.h>

//typedef float (*FUNC_PTR_SP_ONE_ARG)(float);
//typedef double (*FUNC_PTR_DP_ONE_ARG)(double);
//
//typedef struct
//{
//	char * psFuncName;
//	uint32_t ui32Size;
//	float * pfBuffer;
//	FUNC_PTR_SP_ONE_ARG spFunc;
//	FUNC_PTR_SP_ONE_ARG spFunc_i;
//	FUNC_PTR_DP_ONE_ARG dpFunc;
//	FUNC_PTR_DP_ONE_ARG dpFunc_i;
//}sSingleInputFunction;

//typedef struct
//{
//	char * psFuncName;
//	uint32_t ui32Size;
//	float * pfBuffer;
//	FUNC_PTR_SP_ONE_ARG spFunc;
//	FUNC_PTR_DP_ONE_ARG dpFunc;
//}sDoubleInputFunction;

//extern void LOG10_runSinglePrecisionTest(sSingleInputFunction sInput);
//
//extern void LOG10_runTestSingleInput(sSingleInputFunction sInput);

void LOG10_runTests(char * psFuncName, uint32_t ui32Size, float * pfBuffer);

extern void LOG10_spTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);

extern void LOG10_dpTest(char * psFuncName, uint32_t ui32Size, float * pfBuffer);



#endif
