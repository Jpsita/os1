#ifndef __LIBK_STRING_H
#define __LIBK_STRING_H

#include "definitions.h"

void s_strcpy(uint8_t* first, uint8_t* second);

uint32_t s_strlen(uint8_t* string);

uint32_t s_strcmp(uint8_t* first, uint8_t* second);


uint32_t s_strcat(uint8_t* first, uint8_t* second);
uint32_t s_strcat_n(uint8_t* first, uint8_t* second, uint32_t max_chars);
uint32_t s_strcat_nn(uint8_t* first, uint8_t* second, uint32_t max_chrs, uint8_t myNull);
uint32_t s_strpos_s(uint8_t* string, uint8_t needle, uint32_t start);
uint32_t s_strpos(uint8_t* string, uint8_t needle);
uint32_t s_substr(uint8_t* string, uint8_t* buffer, uint32_t start, uint32_t end);
void s_stradd(uint8_t* string, uint8_t chr);

#endif