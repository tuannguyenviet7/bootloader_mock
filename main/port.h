#ifndef _PORT_H_
#define _PORT_H_

#include <MKL46Z4.h>

typedef enum
{
  interrupt_DMA_request_disabled = 0x0000,
  DMA_request_on_rising_edge = 0x0001,
  DMA_request_on_falling_edge = 0x0010, 
  DMA_request_on_either_edge = 0x0011, 
  interrupt_when_logic_zero = 0x1000, 
  interrupt_on_rising_edge = 0x1001, 
  interrupt_on_falling_edge = 0x1010, 
  interrupt_on_either_edge = 0x1011,
  interrupt_when_logic_one = 0x1100, 
}InterruptConfigType;

typedef enum
{
  analog = 0,
  GPIO = 1,
  chip_specific_2 = 0b010U,
  chip_specific_3 = 0b011U,
  chip_specific_4 = 0b100U,
  chip_specific_5 = 0b101U,
  chip_specific_6 = 0b110U,
  chip_specific_7 = 0b111U, 
}PinMuxControlType;

#ifndef _PIN_NUMBER_
#define _PIN_NUMBER_
typedef enum
{
  //pin[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31},
  pin0 = 0,
  pin1 = 1,
  pin2 = 2,
  pin3 = 3,
  pin4 = 4,
  pin5 = 5, 
  pin6 = 6,
  pin7 = 7,
  pin8 = 8,
  pin9 = 9,
  pin10 = 10,
  pin11 = 11,
  pin12 = 12,
  pin13 = 13,
  pin14 = 14,
  pin15 = 15,
  pin16 = 16,
  pin17 = 17,
  pin18 = 18,
  pin19 = 19,
  pin20 = 20,
  pin21 = 21,
  pin22 = 22,
  pin23 = 23,
  pin24 = 24,
  pin25 = 25,
  pin26 = 26,
  pin27 = 27,
  pin28 = 28,
  pin29 = 29,
  pin30 = 30,
  pin31 = 31,
}PinNumber;
#endif

typedef enum
{
  pull_disable = 0,
  pull_enable = 1,
}PullEnableType;

typedef enum
{
  internal_pulldown = 0,
  internal_pullup = 1,
}PullSelectType;

typedef struct
{
  PinNumber pin;
  PORT_Type* PORTn;
  InterruptConfigType interrupt_config_type;
  PinMuxControlType pin_mux_control_type;
  PullEnableType pull_enable_type;
  PullSelectType pull_select_type;
}PORT_ConfigType;

void PORTInit(PORT_ConfigType * PORT_Config);
void PORT_EXTI_Config (PORT_ConfigType * PORT_Config);

#endif