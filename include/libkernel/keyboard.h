#ifndef __LIBK_KEYBOARD_H
#define __LIBK_KEYBOARD_H

#define CHAR_BKSP		(char) 8
#define CHAR_NEWLINE	'\n'
#define CHAR_ESC		(char) 27
#define CHAR_TAB		'\t'

char k_getCh();
char k_getCharacter();
void k_getString(uint8_t* string);
void k_getStringTrm(uint8_t* string, char terminator);
void k_cleanBuff();

#endif