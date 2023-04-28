#include <uart0.h>


void MCG_Clock_Init()
{
  //____________________PORTA-UART_______________________
  SIM->SCGC5 |= (1<<9);//PORTA
  SIM->SCGC4 |= (1<<10);//UART0
  /*select internal clock */
  MCG->C1 &= ~(MCG_C1_CLKS_MASK);
  MCG->C1 |= MCG_C1_CLKS(1);
  
  /*MCG_C2 = sekect fast internal clock: MCG_IRC = 4MHz - FIRC ( fast...)*/
  MCG->C2 &= ~(MCG_C2_IRCS_MASK);
  MCG->C2 |= MCG_C2_IRCS(1);
  
  /*MCG_SC = f/1(FCRDIV) <-> 0x00 = mask --> MCGIRCLK =4MHz = UART ASYNCH Module Clock*/
  MCG->SC &= ~MCG_SC_FCRDIV_MASK;
  MCG->SC |= MCG_SC_FCRDIV(0);
  
  /*update clock :  SystemCoreClockUpdate();*/
   SystemCoreClockUpdate();
}

void UART0_Init()
{
  // BAURATE  = 9600 = (4.000.000) / (104 * (3 + 1))
  //Config UART - Baurate & Frame & Interrupt?
  //Baurate - 9600
  //Frame - 8 bit Data, 1 bit Stop, Parity = None 
  
  //________________________transmit,receive disable___________________________
  UART0->C2 &= ~(UART0_C2_TE_MASK);         //disable transmit
  UART0->C2 &= ~(UART0_C2_RE_MASK);
  UART0->S1 &= ~(UART0_S1_TC_MASK);         //turn off TC flag
  
  //_____________Enables the internal reference clock for use as MCGIRCLK.
  MCG->C1 &= ~(MCG_C1_IRCLKEN_MASK);
  MCG->C1 |= MCG_C1_IRCLKEN(1);      //1 to make MCGIRCLK active
  
  //select internal clock
  MCG->C1 &= ~(MCG_C1_CLKS_MASK);
  MCG->C1 |= MCG_C1_CLKS(1);
  
  //________________________Fast internal clock 4.000.000Hz
  MCG->C2 &= ~(MCG_C2_IRCS_MASK);
  MCG->C2 |= MCG_C2_IRCS(1);
  //________________________MCG___clock source______________________
  //internal 4.000.000
  MCG->SC &= ~(MCG_SC_FCRDIV_MASK);
  MCG->SC |= MCG_SC_FCRDIV(0);    //divided by 1
  //________________________SIM___________________________________
  SIM->SOPT2 &= ~(SIM_SOPT2_UART0SRC_MASK);
  SIM->SOPT2 = SIM_SOPT2_UART0SRC(0b11U); //  UART0SRC = 11 MCGIRCLK clock
  
  //____________________________enable Both_Edge_Sampling_____________________
  UART0->C5 |= UART0_C5_BOTHEDGE(1);
  
  //________________________set_baurate____UAR-BDH-BDL________________
  //BDH-BDL = 104
  UART0->BDH &= ~(UART0_BDH_SBR_MASK);
  UART0->BDH |= UART0_BDH_SBR(0);
  UART0->BDL &= ~(UART0_BDL_SBR_MASK);
  UART0->BDL |= UART0_BDL_SBR(104);  //59
  //_____Set baurate_____UART_C5.OSR__________________________________ 
  UART0->C4 = (UART0->C4 & ~(0x1FU)) | 0x3;
  
  //___________________________Frame - 8 bit Data, 1 bit Stop, Parity = None _____________
  //--------------------M-----------8 bit data-----------parity disabled
  UART0->C1 &= ~(UART0_C1_M_MASK | UART0_C1_PE_MASK);
  UART0->C1 |= UART0_C1_M(0) | UART0_C1_PE(0);           // M = 0 8 bit data, PE = 0 parity disbaled
  //UART0->C1 &= ~(UART0_C1_M_MASK);
  
  //-------------------------- 1 bit stop ---------------------------
  UART0->BDH &= ~(UART0_BDH_SBNS_MASK);
  UART0->BDH |= UART0_BDH_SBNS(0);
  //-----------------------------transmit,receive enable-------------------
  UART0->C2 |= (1<<3) | (1<<2);
}


void Port_UART0_Init()
{
  //Config Port - PA1, PA2 - AL2 ...
  //Alternative 2/ Pullup
  PORT_ConfigType PA1_uart = {.pin = pin1,
                              .PORTn = PORTA,
                              .pin_mux_control_type = chip_specific_2,
                              .pull_enable_type = pull_enable,
                              .pull_select_type = internal_pullup,
                              };
  PORT_ConfigType PA2_uart = {.pin = pin2,
                              .PORTn = PORTA,
                              .pin_mux_control_type = chip_specific_2,
                              .pull_enable_type = pull_enable,
                              .pull_select_type = internal_pullup,
                              };
  PORTInit(&PA1_uart);
  PORTInit(&PA2_uart);
}

void UART0_Transmit(uint8_t data)
{
  // transmit data into data register UART0->D
    UART0->D = data;
   // waiting for TC flag (Transmission Complete) to be set to 1
    while(  ((UART0->S1) & UART_S1_TC_MASK ) != (1<<6) ); 
}

void UART0_Transmit_String(uint8_t* strPtr, uint32_t length)
{
  uint32_t index = 0;
  for(index = 0; index < length; ++index)
  {
    while(((UART0->S1) & (1<<6)) == 0){}
    UART0->D = strPtr[index];
    while(((UART0->S1) & (1<<7)) == 0){}
  }
}

void enableReceiveInterrupt(void)
{
  //______________________enable receive interrupt___________________________
  UART0->C2 |= UART0_C2_RIE(1);
  NVIC->ISER[0] |= (1 << UART0_IRQn);
}

void disableReceiveInterrupt(void)
{
  //______________________enable receive interrupt___________________________
  UART0->C2 &= ~UART0_C2_RIE_MASK;
}



uint8_t UART0_Receive(void)
{
  while(((UART0->S1) & (1<<5)) == 0){}
  uint8_t receive = UART0->D;
  return receive;
  
}
