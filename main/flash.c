#include <flash.h>

void Flash_WriteLongWord(uint32_t Address, uint32_t Data)
{
  //scheme to write to flash
  //__________________wait for the previous commmand complete, wait until CCIF = 1__________________________
  while((FTFA->FSTAT & (1 << 7)) == 0)
  {
  }
  //_____________________check error_____________________________
  if((FTFA->FSTAT & (1 << 5)) != 0 || (FTFA->FSTAT & (1 << 4)) != 0)
  {
    FTFA->FSTAT = 0x30;
  }
  //_______________________setup command____________________
  FTFA->FCCOB0 = 0x06u;
  FTFA->FCCOB1 = (uint8_t)(Address >> 16);
  FTFA->FCCOB2 = (uint8_t)(Address >> 8);
  FTFA->FCCOB3 = (uint8_t)(Address);     
  FTFA->FCCOB4 = (uint8_t)(Data >> 24); //12 - MSB - Flash (Max Address)              
  FTFA->FCCOB5 = (uint8_t)(Data >> 16);
  FTFA->FCCOB6 = (uint8_t)(Data >> 8);  //78 - LSB - Flash (Low Address)
  
  FTFA->FCCOB7 = (uint8_t)(Data); 
  //___________________________start command___________________________
  FTFA->FSTAT = 0x80u;
}

void Flash_EraseSector(uint32_t Address)
{
  //scheme to erase to flash
  //____________________DO NOT ERASE LICENSE AREA IN FLASH_________________________
  if(Address >= 0x400u && Address <= 0x7FFu)
  {
    return;     
  }
  //__________________wait for the previous commmand complete, wait until CCIF = 1__________________________
  while((FTFA->FSTAT & (1 << 7)) == 0)
  {
  }
  //_____________________check error_____________________________
  if((FTFA->FSTAT & (1 << 5)) != 0 || (FTFA->FSTAT & (1 << 4)) != 0)
  {
    FTFA->FSTAT = 0x30; //erase one sector = 1KB
  }
  //_______________________setup command____________________
  FTFA->FCCOB0 = 0x09u;
  FTFA->FCCOB1 = (uint8_t)(Address >> 16);
  FTFA->FCCOB2 = (uint8_t)(Address >> 8);
  FTFA->FCCOB3 = (uint8_t)(Address);     
  
  //___________________________start command___________________________
  FTFA->FSTAT = 0x80u;
}
