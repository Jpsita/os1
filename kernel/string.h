#ifndef __JPS_STRING_H
#define __JPS_STRING_H

#include "main.h"

uint32_t strlen(uint8_t* string);

uint32_t strcmp(uint8_t* first, uint8_t* second);


uint32_t strcat(uint8_t* first, uint8_t* second);
uint32_t strcat_n(uint8_t* first, uint8_t* second, uint32_t max_chars);
uint32_t strcat_nn(uint8_t* first, uint8_t* second, uint32_t max_chrs, uint8_t myNull);
uint32_t strpos_s(uint8_t* string, uint8_t needle, uint32_t start);
uint32_t strpos(uint8_t* string, uint8_t needle);
uint32_t substr(uint8_t* string, uint8_t* buffer, uint32_t start, uint32_t end);
void stradd(uint8_t* string, uint8_t chr);

#endif