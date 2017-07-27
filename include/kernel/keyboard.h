#ifndef __JPS_KEYBOARD_H
#define __JPS_KEYBOARD_H

#include "main.h"
#include <common/common_keyboard.h>

int isShift();
int isCtrl();

void parseKey(short int key);
void parseKeyOff(short int key);

char scancodeToAscii(unsigned char sc);

void init_keyboard();

char getCh();

extern	uint8_t* nextChar;
extern	uint8_t* nextRead;
extern	uint8_t* keyboardBuffer;

char buff_getCh();
char buff_getCharacter();
void buff_getString(uint8_t* string);
void buff_getStringTrm(uint8_t* string, char terminator);
void buff_clean();
void buff_addChar(char c);

#endif