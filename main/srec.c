#include "srec.h"

uint32_t hex_to_dec(uint8_t* aByte)
{
    uint8_t firstDec;
    uint8_t lastDec;
    /* aByte[0] -> decimal */
    if(aByte[0]>=48 && aByte[0]<=57)
    {
        firstDec = aByte[0] - '0';
    }
    else if(aByte[0] >= 'A' && aByte[0] <= 'F')
    {
        firstDec = aByte[0] - 55;
    }
    /* aByte[1] -> decimal */
    if(aByte[1]>=48 && aByte[1]<=57)
    {
        lastDec = aByte[1] - '0';
    }
    else if(aByte[1] >= 'A' && aByte[0] <= 'F')
    {
        lastDec = aByte[1] - 55;
    }
    return ((firstDec << 4) | lastDec);
}

uint8_t hexToDex4bits(uint8_t hex_digit)
{
    if(hex_digit>=48 && hex_digit<=57)
    {
        hex_digit = hex_digit - '0';
    }
    else if(hex_digit >= 'A' && hex_digit <= 'F')
    {
        hex_digit = hex_digit - 55;
    }
    return hex_digit;
}


uint8_t check_checksum(uint8_t buffer[100][3], uint32_t numberOfByte)
{
    uint8_t result = 0;
    uint32_t sumOfAllByte=0;
    uint8_t LSB;
    uint32_t index;
    uint8_t checksum = hex_to_dec(buffer[numberOfByte-1]);
    /* add each byte excluding RecordType and CheckSum */
    for(index=1; index<numberOfByte-1; ++index)
    {
        sumOfAllByte += hex_to_dec(buffer[index]);
    }
    /* get the LBS of sumOfALlByte */
    LSB = sumOfAllByte;
    /*  compute the ones' complement of the LSB */
    LSB ^= (uint8_t)0xFF;
    /* if the result is equal to CheckSum, the CheckSum is correct*/
    if(LSB == checksum)
    {
        result = 1;
    }
    else
    {
        result = 0;
    }
    return result;
}

uint8_t check_character(uint8_t character)
{
    if((48 <= character && character <=57) || (65 <= character && character <=70))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************
 * Code
 ******************************************************************************/