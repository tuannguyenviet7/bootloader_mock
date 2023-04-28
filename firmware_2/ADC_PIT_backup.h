#ifndef __ADC_PIT_backup_h_
#define	__ADC_PIT_backup_h_

#include "MKL46Z4.h"
void ADC_Clock_Init();
void ADC_Start_Converter();
void ADC_Interrupt_Setup();
uint8_t ADC_read_Polling(void);
void PIT_Clock_Init();
void PIT_Interrupt_Setup();







#endif