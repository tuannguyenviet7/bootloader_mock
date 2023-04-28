#include "MKL46Z4.h"
#include "ADC_PIT_backup.h"
#include "UART_backup.h"

uint8_t data[10];
uint8_t i=0;
uint8_t data_check;
uint8_t chac = 0xC;
void ADC0_IRQHandler()
{     
       for(i=0;i<10;i++)
       {
          data[i] = ADC_read_Polling();
       }

}
void PIT_IRQHandler()
{
    PIT->CHANNEL[0].TCTRL &= ~ PIT_TCTRL_TEN_MASK;
    PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
  
    ADC_Start_Converter();
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN(1);
}
void UART0_IRQHandler(void)
{       
  
     data_check = UART0->D;
}
void main(void)
{
    SIM->SCGC5 |=  SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTD_MASK;
    ADC_Clock_Init();
    ADC_Interrupt_Setup();
    PIT_Clock_Init();
    PORTE->PCR[22] &= ~PORT_PCR_MUX_MASK;
    PIT_Interrupt_Setup();
    Clock_Init();
    Port_UART0_Init();
    UART0_Init();
    UART0_Transmitt(chac);
    while(1)
    {
      if(data_check == 0x55)
      {
          for(i=0;i<10;i++)
          {
            UART0_Transmitt(data[i]);
            
            UART0_Transmitt('\n');
          }
          data_check = 0;
      }
    }
}