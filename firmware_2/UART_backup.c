#include "UART_backup.h"
void Clock_Init()
{
    SIM->SCGC5 |= (1 << 9);
    SIM->SCGC4 |= (1 << 10);
    SIM->SOPT2 |= (3 << 26);
    MCG->SC &= ~(MCG_SC_FCRDIV_MASK);
    MCG->C2 |= (1 << 0);
    MCG->C1 |= MCG_C1_IRCLKEN(1);
}

void Port_UART0_Init()
{
    PORTA->PCR[1] |= (2 << 8);
    PORTA->PCR[2] |= (2 << 8);
    
    PORTA->PCR[1] |= (3 << 0);
    PORTA->PCR[2] |= (3 << 0);
}

void UART0_Init()
{
   // Baudate - 9600 - OSR = 4, BSR = 83
   // (4.000.000) / (104 * (3 + 1)) = 9615
//   uint8_t temp =  UART0->C4;
//   temp &= ~(0x1F);
//   temp |= 0x3;
//   UART0->C4 = temp;
   // Read - Modify - Write
   UART0->C4 = ( UART0->C4 & ~(0x1F)) | 0x3;
   UART0->BDL = 104;
  
   // Frame - 8 bit Data, 1 bit Stop, Parity = None
   UART0->C1 &= ~(1 << 4);
   UART0->BDH &= ~(1 << 5);
   UART0->C1 &= ~(1 << 1);
   
   UART0->C5 |= (1<<1);
   // Enable Transmit/Receive
   UART0->C2 |= (1 << 3);
   UART0->C2 |= (1<< 2);
    UART0->C2 |= (1<<5);
   NVIC->ISER[0] |= 1 <<  UART0_IRQn;
}
void UART0_Transmitt(uint8_t Data)
{

    UART0->D = Data;
    
    while((UART0->S1 & (1 << 6)) == 0);
}

