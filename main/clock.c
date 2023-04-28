#include "clock.h"
#include <MKL46Z4.h>


void core1MHzconfig(ClockConfigType* clock_config)
{
  //internal clock
  MCG->C1 &= ~MCG_C1_CLKS_MASK;
  MCG->C1 |= MCG_C1_CLKS(clock_config->clock_source);
  
  //fast internal clock - if ................................
  MCG->C2 &= ~MCG_C2_IRCS_MASK;
  MCG->C2 |= MCG_C2_IRCS(clock_config->clock_internal);     
  
  MCG->SC &= ~MCG_SC_FCRDIV_MASK; //default divide by 1
  
  SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV1_MASK;
  SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(clock_config->clock_core_div);        //divide by 4
}
