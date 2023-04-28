#ifndef _FLASH_H_
#define _FLAHS_H_

#include <MKL46Z4.h>

void Flash_WriteLongWord(uint32_t Address, uint32_t Data);
void Flash_EraseSector(uint32_t Address);

#endif /* _FLASH_H_ */