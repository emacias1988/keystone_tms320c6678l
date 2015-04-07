#include "mathlib_tests.h"

uint32_t pui32DestBuffer[1000*1024];
uint32_t pui32SrcBuffer[1000*1024];

uint16_t g_ui16CPUFrequency;

int main(void)
{
  float * pfDestBuffer = (float*)pui32DestBuffer;
  float * pfSrcBuffer = (float*)pui32SrcBuffer;
  volatile uint32_t ui32Size;

  g_ui16CPUFrequency = 2700;	// MacBookPro 2.7GHz

  /*printf ("The CPU info is %s\n",cpuinfo.);*/
  	
  ui32Size = 4*4*KB;

  ATAN_runTests("atan", ui32Size, pfSrcBuffer, pfDestBuffer);
  ATAN2_runTests("atan2", ui32Size, pfSrcBuffer, pfDestBuffer);
  COS_runTests("cos", ui32Size, pfSrcBuffer, pfDestBuffer);
  DIV_runTests("div", ui32Size, pfSrcBuffer, pfDestBuffer);
  EXP_runTests("exp", ui32Size, pfSrcBuffer, pfDestBuffer);
  EXP2_runTests("exp2", ui32Size, pfSrcBuffer, pfDestBuffer);
  EXP10_runTests("exp10", ui32Size, pfSrcBuffer, pfDestBuffer);
  LOG10_runTests("log10", ui32Size, pfSrcBuffer, pfDestBuffer);
  LOG2_runTests("log2", ui32Size, pfSrcBuffer, pfDestBuffer);
  LOG_runTests("log", ui32Size, pfSrcBuffer, pfDestBuffer);
  POW_runTests("pow", ui32Size, pfSrcBuffer, pfDestBuffer);
  RECIP_runTests("recip", ui32Size, pfSrcBuffer, pfDestBuffer);
  RSQRT_runTests("rsqrt", ui32Size, pfSrcBuffer, pfDestBuffer);
  SIN_runTests("sin", ui32Size, pfSrcBuffer, pfDestBuffer);
  SQRT_runTests("sqrt", ui32Size, pfSrcBuffer, pfDestBuffer);
  CMP_runTests("cmp",ui32Size,(char *) pfSrcBuffer, (char *) pfDestBuffer);

  ui32Size = 4*32*KB;
	
  ATAN_runTests("atan", ui32Size, pfSrcBuffer, pfDestBuffer);
  ATAN2_runTests("atan2", ui32Size, pfSrcBuffer, pfDestBuffer);
  COS_runTests("cos", ui32Size, pfSrcBuffer, pfDestBuffer);
  DIV_runTests("div", ui32Size, pfSrcBuffer, pfDestBuffer);
  EXP_runTests("exp", ui32Size, pfSrcBuffer, pfDestBuffer);
  EXP10_runTests("exp10", ui32Size, pfSrcBuffer, pfDestBuffer);
  EXP2_runTests("exp2", ui32Size, pfSrcBuffer, pfDestBuffer);
  LOG10_runTests("log10", ui32Size, pfSrcBuffer, pfDestBuffer);
  LOG2_runTests("log2", ui32Size, pfSrcBuffer, pfDestBuffer);
  LOG_runTests("log", ui32Size, pfSrcBuffer, pfDestBuffer);
  POW_runTests("pow", ui32Size, pfSrcBuffer, pfDestBuffer);
  RECIP_runTests("recip", ui32Size, pfSrcBuffer, pfDestBuffer);
  RSQRT_runTests("rsqrt", ui32Size, pfSrcBuffer, pfDestBuffer);
  SIN_runTests("sin", ui32Size, pfSrcBuffer, pfDestBuffer);
  SQRT_runTests("sqrt", ui32Size, pfSrcBuffer, pfDestBuffer);
  CMP_runTests("cmp",ui32Size,(char *) pfSrcBuffer, (char *) pfDestBuffer);

  ui32Size = 4*1*MB;
	
  ATAN_runTests("atan", ui32Size, pfSrcBuffer, pfDestBuffer);
  ATAN2_runTests("atan2", ui32Size, pfSrcBuffer, pfDestBuffer);
  COS_runTests("cos", ui32Size, pfSrcBuffer, pfDestBuffer);
  DIV_runTests("div", ui32Size, pfSrcBuffer, pfDestBuffer);
  EXP_runTests("exp", ui32Size, pfSrcBuffer, pfDestBuffer);
  EXP10_runTests("exp10", ui32Size, pfSrcBuffer, pfDestBuffer);
  EXP2_runTests("exp2", ui32Size, pfSrcBuffer, pfDestBuffer);
  LOG10_runTests("log10", ui32Size, pfSrcBuffer, pfDestBuffer);
  LOG2_runTests("log2", ui32Size, pfSrcBuffer, pfDestBuffer);
  LOG_runTests("log", ui32Size, pfSrcBuffer, pfDestBuffer);
  POW_runTests("pow", ui32Size, pfSrcBuffer, pfDestBuffer);
  RECIP_runTests("recip", ui32Size, pfSrcBuffer, pfDestBuffer);
  RSQRT_runTests("rsqrt", ui32Size, pfSrcBuffer, pfDestBuffer);
  SIN_runTests("sin", ui32Size, pfSrcBuffer, pfDestBuffer);
  SQRT_runTests("sqrt", ui32Size, pfSrcBuffer, pfDestBuffer);
  CMP_runTests("cmp",ui32Size,(char *) pfSrcBuffer, (char *) pfDestBuffer);

  ui32Size = 4*1*GB;
	
  ATAN_runTests("atan", ui32Size, pfSrcBuffer, pfDestBuffer);
  ATAN2_runTests("atan2", ui32Size, pfSrcBuffer, pfDestBuffer);
  COS_runTests("cos", ui32Size, pfSrcBuffer, pfDestBuffer);
  DIV_runTests("div", ui32Size, pfSrcBuffer, pfDestBuffer);
  EXP_runTests("exp", ui32Size, pfSrcBuffer, pfDestBuffer);
  EXP10_runTests("exp10", ui32Size, pfSrcBuffer, pfDestBuffer);
  EXP2_runTests("exp2", ui32Size, pfSrcBuffer, pfDestBuffer);
  LOG10_runTests("log10", ui32Size, pfSrcBuffer, pfDestBuffer);
  LOG2_runTests("log2", ui32Size, pfSrcBuffer, pfDestBuffer);
  LOG_runTests("log", ui32Size, pfSrcBuffer, pfDestBuffer);
  POW_runTests("pow", ui32Size, pfSrcBuffer, pfDestBuffer);
  RECIP_runTests("recip", ui32Size, pfSrcBuffer, pfDestBuffer);
  RSQRT_runTests("rsqrt", ui32Size, pfSrcBuffer, pfDestBuffer);
  SIN_runTests("sin", ui32Size, pfSrcBuffer, pfDestBuffer);
  SQRT_runTests("sqrt", ui32Size, pfSrcBuffer, pfDestBuffer);
  CMP_runTests("cmp",ui32Size,(char *) pfSrcBuffer, (char *) pfDestBuffer);

  return(0);
}











