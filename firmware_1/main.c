#include <MKL46Z4.h>
#include "clock.h"

#define DELAY_CNT           (1000000)
#define RED_LED_PIN         (1 << 29)
#define GREEN_LED_PIN       (1 << 5)
#define SW1_PIN             (uint32_t)(1 << 3)    //define position of SW1_PIN
#define NVIC_ISER           0xE000E100
#define NVIC_ICER           0xE000E180
/* x = 0->31, there are only 7 registers but 32 NVIC_IPRs are assigned into them */
#define NVIC_IPR(x)         (uint32_t)(0xE000E400+(uint8_t)(x/4)*4)
/* 4 parts in one 32-bits register */
#define NVIC_IPR_SHIFT(x)   (uint8_t)((x%4)*8)
/* 4 parts, 0xFF mask-value for each part */
#define NVIC_IPR_MASK(x)    (uint32_t)(0xFF<<NVIC_IPR_SHIFT(x))
/* x = value that's needed to be set into register, y is the number'th of the NVIC_IPRn */
#define set_NVIC_IPRn(x,y)  (uint32_t)(x<<NVIC_IPR_SHIFT(y))&(uint32_t)(0xFF<<NVIC_IPR_SHIFT(y))

void initLed();
void delay();
void initButtonInterrupt();
uint32_t getR0Interrupt(void);
void concurrentLed();
void seperateLed();
void toggleLed();


volatile uint8_t mode = 0;       // 0 is mode blinking concurrent, 1 is mode not concurrent
volatile uint32_t time_delay = 0;

int main(void)
{
    initLed();
    initButtonInterrupt();
    ClockConfigType config_1Mz = {CLOCK_SOURCE_INTERNAL, CLOCK_CORE_DIV_4, CLOCK_INTERNAL_FIRC};
    core1MHzconfig(&config_1Mz);
    while(1)
    {
      toggleLed();
      delay(200000);
    }
}


void initLed()
{
    /* Enable clock for PORTE & PORTD */
    SIM->SCGC5 |= ( SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK ); //SIM address is 0x40047000
    //>>>>>>SIM_SCGC5_PORTD_MASK, SIM_SCGC5_PORTE_MASK -> clock gate control of PORTD and PORTE
    
   /* Initialize the RED LED (PTE29)*/
    /* Set the PTE29 pin multiplexer to GPIO mode */
    PORTE->PCR[29] = PORT_PCR_MUX(1);
    /* Set the pin's direction to output */
    FPTE->PDDR |= RED_LED_PIN;  //>>>>>>>red led is pin 29 of PORTE
    /* Set the initial output state to high */
    //>>>>>>>set the content of this bin to 1, write 1 tot PSOR to set bit
    //>>>>>>>if output is 1 led turns off, 0 otherwise
    //>>>>>turn led red off
    FPTE->PSOR |= RED_LED_PIN;    

    /* Initialize the Green LED (PTD5) */
    /* Set the PTD5 pin multiplexer to GPIO mode */
    PORTD->PCR[5]= PORT_PCR_MUX(1);
    /* Set the pin's direction to output */
    FPTD->PDDR |= GREEN_LED_PIN;  
    /* Set the initial output state to high to turn led green off */
    //>>>>>>write 1 to PSOR to clear that bit 
    FPTD->PSOR |= GREEN_LED_PIN;
}

void toggleLed()
{
     //toggle led red
     FPTE->PTOR |= RED_LED_PIN;
     //toglge led green 
     FPTD->PTOR |= GREEN_LED_PIN;
}

void initButtonInterrupt()
{
  //button is connected to PTC3
  /* Enable clock for PORTC */
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
  //set-up PORTC, pin3: GPIO mode, pull-up resister enabled, pull-up resister, interrupt on falling edge
  PORTC->PCR[3] |=  PORT_PCR_MUX(1)|PORT_PCR_PE(1)|PORT_PCR_PS(1)|PORT_PCR_IRQC(0b1010);
  /* Set the pin's direction to input, 0 is input */
  FPTC->PDDR &= ~SW1_PIN;
  //set-up NVIC
  //enable PORTC_PORTD_IRQn, IRQs number of this kind of interrupt is 31(PORTC_PORTD_IRQn)
  *(uint32_t*)NVIC_ISER |= (uint32_t)(1<<PORTC_PORTD_IRQn);
  //set priority of the interrupt..................................... register NVIC-IPRx, PIR_31
  //turn off part, set value
  *(uint32_t*)NVIC_IPR(PORTC_PORTD_IRQn) = (*(uint32_t*)NVIC_IPR(PORTC_PORTD_IRQn) & NVIC_IPR_MASK(PORTC_PORTD_IRQn)) | set_NVIC_IPRn(0,PORTC_PORTD_IRQn);
}


void delay(uint32_t delay_time) //delay_time = 200000 for delay 200ms
{
    uint32_t i;
    for (i = 0; i < delay_time; i++)
    {
        __asm("nop");
    }
}


/*
 * brief: find which pin of port C caused the interrupt, can be only use if pin is set pull-up
 */
/*uint8_t pinInterruptPortC_find(uint8_t* pin_PortC)
{
  while()
}*/

//PORTC_PORTD_IRQn handler program 
void PORTC_PORTD_IRQHandler(void)
{
  //clear interrupt flag,
  PORTC->PCR[3] |= PORT_PCR_ISF(1);
  //PORTC->ISFR |= 1<<(getR0Interrupt());
  if(mode == 0)
  {
    mode = 1;   //switch to separate mode
    //turn off led red
    FPTE->PSOR |= RED_LED_PIN;
    //turn on led green
    FPTD->PCOR |= GREEN_LED_PIN;
    time_delay = 100000;
  }
  else if(mode == 1)
  {
    mode = 0;   //switch to concurent mode
    //turn off led red
    FPTE->PSOR |= RED_LED_PIN;
    //turn off led green
    FPTD->PSOR |= GREEN_LED_PIN;
    time_delay = 200000;
  }
  //toggle led red
  //FPTE->PTOR |= RED_LED_PIN;
  //toglge led green 
  //FPTD->PTOR |= GREEN_LED_PIN;
}
