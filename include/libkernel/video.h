#ifndef __LIBK_VIDEO_H
#define __LIBK_VIDEO_H

#include "definitions.h"

void v_printCharacterAtPos(char c, char color, int x, int y);

void v_printCharacter(char c);
void v_printString(char* s);
void v_clear();
void v_bksp();
void v_printUint32(uint32_t num);

#endif