#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <MKL46Z4.h>

typedef enum
{
  CLOCK_SOURCE_PLL        = 0u,
  CLOCK_SOURCE_EXTERNAL   = 2u,
  CLOCK_SOURCE_INTERNAL   = 1u,
}ClockSourceType;

typedef enum
{
  CLOCK_INTERNAL_FIRC = 1u,
  CLOCK_INTERNAL_SIRC = 0u,
}ClockInternalType;

typedef enum
{
  CLOCK_CORE_DIV_1 = 0u,
  CLOCK_CORE_DIV_2 = 1u,
  CLOCK_CORE_DIV_3 = 2u,
  CLOCK_CORE_DIV_4 = 3u,
}ClockCoreDivType;

typedef struct
{
  ClockSourceType       clock_source;
  ClockCoreDivType      clock_core_div; 
  ClockInternalType     clock_internal;
}ClockConfigType;

void core1MHzconfig(ClockConfigType* clock_config);

#endif