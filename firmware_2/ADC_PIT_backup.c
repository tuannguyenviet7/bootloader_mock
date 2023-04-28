
#include "ADC_PIT_backup.h"

void ADC_Clock_Init()
{
      SIM->SCGC6 |= SIM_SCGC6_ADC0(1);  // Enable ADC Clock
     // PORTE->PCR[22] |= PORT_PCR_MUX(1);
}
void ADC_Start_Converter()
{
       //Trigger - Start Convert
      ADC0->SC1[0] &= ~ADC_SC1_ADCH_MASK;
      ADC0->SC1[0] |= ADC_SC1_ADCH(3);
}
void ADC_Interrupt_Setup()
{
      // Select Chnanel A 
       ADC0->CFG2 &= ~ADC_CFG2_MUXSEL_MASK;
    //Configure Clock- Clock Source, frequency - Bus clock
     
      ADC0->CFG1 &= ~ADC_CFG1_ADICLK_MASK;      // ADC Clock source = Bus Clock
       ADC0->CFG1 &= ~ADC_CFG1_ADIV_MASK;       // Div = 1
    // Voltage Reference - 3,3V
      ADC0->SC2 &= ~ADC_SC2_REFSEL_MASK; // 00-Vrefh  - Vrefl =  3.3V
    // Resolutin (N-bit?) - 16bits
      ADC0->SC1[0] &= ~ADC_SC1_DIFF_MASK;       // Single-end
      ADC0->CFG1 &= ~ADC_CFG1_MODE_MASK;
      ADC0->CFG1 |= ~ADC_CFG1_MODE(3);  // 16 bits
    //Setup Polling or Interrupt - Polling
      ADC0->SC1[0] |= ADC_SC1_AIEN(1);
     NVIC->ISER[0] |= 1 <<  ADC0_IRQn;
  
}

uint8_t ADC_read_Polling(void)
{
    return ADC0->R[0];
}

void PIT_Clock_Init()
{       
      // enable clock for PIT so we can access to PIT register 
    SIM->SCGC6 |= SIM_SCGC6_PIT(1);
      // select internal clock
     MCG->C1 |= MCG_C1_CLKS(1);
     // select fast interal clock
     MCG->C2 |= MCG_C2_IRCS(1);
     // div 4 for 1MHz
     MCG->SC |= MCG_SC_FCRDIV(2);
     // Select OUTDIV1 and OUTDIV4 for 1Mhz
     SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0);
     SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV4_MASK;
}


void PIT_Interrupt_Setup()
{
    // turn on clock for PIT timer
    PIT->MCR &= ~ PIT_MCR_MDIS_MASK;
    // Set up Reload value - Interrupt - Enable Timer
    PIT->CHANNEL[0].LDVAL |= PIT_LDVAL_TSV(999999);//set up PIT 1s to interrupt
    NVIC->ISER[0] |= 1 <<  PIT_IRQn;
    // Interrupt will be request when TIF is set
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE(1);
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN(1);
      
}
