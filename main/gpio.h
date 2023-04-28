#ifndef _GPIO_H_
#define _GPIO_H_

#include <MKL46Z4.h>

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
  GPIO_DIRECTION_INPUT = 0,
  GPIO_DIRECTION_OUTPUT = 1,
}GPIO_DirectionType;

typedef struct
{
  PinNumber pin;
  GPIO_Type* GPIOn;
  GPIO_DirectionType GPIO_direction_type;
}GPIO_ConfigType;

void GPIO_Init(GPIO_ConfigType * GPIO_Config);

//pin_number = 0 -> 31
void GPIO_SetPin(GPIO_Type* GPIOx, uint8_t pin_number);

//pin_number = 0 -> 31
uint8_t GPIO_ReadPin(GPIO_Type* GPIOx, uint8_t pin_number);

void GPIO_TogglePin(GPIO_Type* GPIOx, uint8_t pin_number);

void GPIO_ClearPin(GPIO_Type* GPIOx, uint8_t pin_number);

#endif