#include "MKL46Z4.h"
#include <uart0.h>
#include <flash.h>
#define RED_LED_PIN         (1 << 29)
#define GREEN_LED_PIN       (1 << 5)
#define MAIN_FIRMWARE     (uint32_t)(0x00020000UL)
#define BACKUP_FIRMWARE   0x2800
/* Can switch two firmware by button  */

volatile uint8_t status = 0 ; // this variable check what button is clicked
volatile uint8_t count_check = 0; // this variable check 5 times PIT interrupt

void clockInit()
{
  //-----------------Default Clock Source PLL - 00---------------------------------
  //----------------------------PORTD - bus clock PORTD----------------------------
  SIM->SCGC5 |= (1 << 12);
  //----------------------------PortC - bus clock PORTC----------------------------
  SIM->SCGC5 |= (1 << 11);
  //----------------------------PORTE - bus clock PORTE----------------------------
  SIM->SCGC5 |= (1 << 13);
  //____________________PORTA-UART_______________________
  SIM->SCGC5 |= (1<<9);//PORTA
  SIM->SCGC4 |= (1<<10);//UART0
}

void Toggle_Green_LED()
{ 
  /* Toggle Green Led */
  FPTD->PTOR |= GREEN_LED_PIN;
    
}
void Toggle_Red_LED()
{
  /* Toggle RED Led */
  FPTE->PTOR |= RED_LED_PIN;
}
/* Set initial value for green and red led*/
void Led_Init()
{
  SIM->SCGC5 |=  SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTD_MASK;
  
  PORTE->PCR[29] = PORT_PCR_MUX(1);
  /* Set the pin's direction to output */ 
  FPTE->PDDR |= RED_LED_PIN;
  /* Set the initial output state to high */
  FPTE->PSOR |= RED_LED_PIN;


  /* Initialize the Green LED (PTD5) */
  /* Set the PTD5 pin multiplexer to GPIO mode */
  PORTD->PCR[5]= PORT_PCR_MUX(1);
  /* Set the pin's direction to output */
  FPTD->PDDR |= GREEN_LED_PIN;
  /* Set the initial output state to high */
   FPTD->PSOR |= GREEN_LED_PIN;
}
/*Set initial value for Botton*/
void Button_Init()
{
  // enable clock for port C- SW1
  SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
  // Set PORTC_PCR3: Interrup falling, pullup, GPIO.
  PORTC->PCR[3] |= PORT_PCR_IRQC(10) | PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1) ; // SW 1
  PORTC->PCR[12] |= PORT_PCR_IRQC(10) | PORT_PCR_MUX(1) | PORT_PCR_PE(1) |PORT_PCR_PS(1) ; // SW2
  // set enable input- default
  // setup for NVIC: priority, enable
  NVIC->ISER[0] |= (uint32_t)(1U<<31);
}
//Interrup service routine
void PORTC_PORTD_IRQHandler()
{
  if((PORTC->ISFR & (1<<3) ) != 0)
  {
    status = 1;
  }
  else if ((PORTC->ISFR & (1<<12) ) != 0)
  {
    status = 2;
  }
  // delete ifr of line interrup
  PORTC->ISFR |= PORT_ISFR_ISF(1);
}
/*- Mode 1: Green and Red Led blink concurrent with T = 200ms*/

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
    PIT->CHANNEL[0].LDVAL |= PIT_LDVAL_TSV(999999);//set up PIT 1s to interrupt 1s
    NVIC->ISER[0] |= 1 <<  PIT_IRQn;
    // Interrupt will be request when TIF is set
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE(1);
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN(1);
      
}
void PIT_IRQHandler()
{
    PIT->CHANNEL[0].TCTRL &= ~ PIT_TCTRL_TEN_MASK;
    PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
    count_check++; // check for waiting 5s
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN(1);
}
void bootloader(uint32_t PROGRAM_START_ADDRESS )
{
    
    // Disable all interrupt and turn off SysTick Timer
    __disable_irq();
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    
    // clear pending interrupt request
     SCB->SHCSR &= ~(SCB_SHCSR_SVCALLPENDED_Msk);
    // Set Main Stack Pointer
    
    __set_MSP(*((volatile uint32_t*)PROGRAM_START_ADDRESS));
    __DMB();
    SCB->VTOR = PROGRAM_START_ADDRESS;
    __DSB();
    // Set Program counter to APP Current address
    // set PC  -> Reset Handler
    void (*Reset_Handler)(void) = (void(*)(void))(*((volatile uint32_t *)(PROGRAM_START_ADDRESS+ 4U)));
    
    // Reset lai chuong trinh bang cach Jump to Reset Hander
    Reset_Handler();
    
}

void UART_Clear_Default()               // this function clear UART back to default before go to two firmware
{
    
    PORTA->PCR[1] &= ~PORT_PCR_MUX_MASK;
    PORTA->PCR[1] &= ~(PORT_PCR_PE_MASK | PORT_PCR_PS_MASK)  ;
    
    PORTA->PCR[2] &= ~PORT_PCR_MUX_MASK;
    PORTA->PCR[2] &= ~(PORT_PCR_PE_MASK | PORT_PCR_PS_MASK)  ;

   UART0->BDL &= ~UART_BDL_SBR_MASK ;
    UART0->C4 &= ~UART0_C4_OSR_MASK;
}
void main()
{
  clockInit();
  Port_UART0_Init();
  //Config UART - Baurate & Frame & Interrupt?
  //Baurate - 9600
  //Frame - 8 bit Data, 1 bit Stop, Parity = None 
  MCG_Clock_Init();                 
  UART0_Init();

  //______________________enable receive interrupt___________________________
  enableReceiveInterrupt();
  
  //_________________________flash erase___________________________
  Flash_EraseSector(MAIN_FIRMWARE);
  Flash_EraseSector(BACKUP_FIRMWARE);
  
  Button_Init();
  Led_Init();
  PIT_Clock_Init();
  PIT_Interrupt_Setup();
  UART_Clear_Default();
  while(1)
  {
    if(status == 1 || count_check == 5) // if button 1 is clicked or waiting 5s for count_check = 5
    {
      Toggle_Green_LED();
      bootloader(MAIN_FIRMWARE); // go to main firmware
    }
    else if(status == 2)
    {
        Toggle_Red_LED();
        bootloader(BACKUP_FIRMWARE); // go to backup firmware
    }
  }
}