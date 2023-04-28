#include <gpio.h>

void GPIO_Init(GPIO_ConfigType * GPIO_Config)
{
  //reset the GPIO_PDDR
  (GPIO_Config->GPIOn)->PDDR &= ~(1<<(GPIO_Config->pin));
  //set GPIO_PDDR
  (GPIO_Config->GPIOn)->PDDR |= 1<<(GPIO_Config->pin);
}


//pin_number = 0 -> 31
void GPIO_SetPin(GPIO_Type* GPIOx, uint8_t pin_number)
{
  GPIOx->PSOR |= (1<<pin_number);
}

void GPIO_ClearPin(GPIO_Type* GPIOx, uint8_t pin_number)
{
  GPIOx->PCOR |= (1<<pin_number);
}

//pin_number = 0 -> 31
uint8_t GPIO_ReadPin(GPIO_Type* GPIOx, uint8_t pin_number)
{
  return ((GPIOx->PDIR)>>pin_number) & ((uint32_t)1);
}

void GPIO_TogglePin(GPIO_Type* GPIOx, uint8_t pin_number)
{
  (GPIOx->PTOR) |= (1<<pin_number);
}

