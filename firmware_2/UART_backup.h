#ifndef __UART_backup_h_
#define	__UART_backup_h_

#include "MKL46Z4.h"
#include "ADC_PIT_backup.h"

void Clock_Init();
void Port_UART0_Init();
void UART0_Init();
void UART0_Transmitt(uint8_t Data);


#endif