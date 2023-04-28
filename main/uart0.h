#ifndef _UART0_H_
#define _UART0_H_

#include "clock.h"
#include <MKL46Z4.h>
#include <port.h>
#include <gpio.h>
#include <flash.h>
#include "uart0.h"


void MCG_Clock_Init();

void UART0_Init();

void Port_UART0_Init();

void UART0_Transmit(uint8_t Data);

void UART0_Transmit_String(uint8_t* strPtr, uint32_t length);

uint8_t UART0_Receive(void);

void enableReceiveInterrupt(void);

void disableReceiveInterrupt(void);

#endif /* _UART0_H_ */