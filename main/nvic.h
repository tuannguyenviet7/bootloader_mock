#ifndef _NVIC_H_
#define _NVIC_H_

#include "clock.h"
#include <MKL46Z4.h>
#include <port.h>
#include <gpio.h>
#include <flash.h>
#include "uart0.h"

void nvic_set_priority(uint8_t line, uint8_t priority);

#endif  //_NVIC_H_