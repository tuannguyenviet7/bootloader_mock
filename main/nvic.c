#include <nvic.h>

void nvic_set_priority(uint8_t line, uint8_t priority)
{
  //*(uint32_t*)(0xE000E400UL) |= priority << (line%4);
  NVIC->IP[line] |= priority << (line%4);
}