#ifndef _FUNC_H_
#define _FUNC_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * brief: convert a HEX digit to decimal value
 * params: uint8_t hex_digit - a chareacter that presents a HEX digit 
 * reval: uint32_t - decimal value of that HEX digit
 */
uint8_t hexToDex4bits(uint8_t hex_digit);

/*
 * brief: convert a HEX number with 2 digits to decimal value
 * params: uint8_t* aByte - array that contains 2 digits of HEX number
 * reval: uint32_t - decimal value of that HEX number
 */
uint32_t hex_to_dec(uint8_t*);

/*
 * brief: check checksum byte
 * params: uint8_t buffer[100][3] - array containing all characters in one line, 2 each
 *         uint32_t numberOfByte - number of bytes in that line 
 * output: uint8_t - 1 if checksum is correct
 *                   0 if checksum is not correct
 */
uint8_t check_checksum(uint8_t[100][3], uint32_t);

/* brief: check if a character is a HEX character
 * params: uint8_t character - uint8_t
 * reval: uint8_t - 1 if that character is HEX format
 *                - 0 if that character is not HEX format
 */
uint8_t check_character(uint8_t);

/*******************************************************************************
 * API
 ******************************************************************************/

#endif /* _FUNC_H_ */