#ifndef __JPS_KEYBOARD_H
#define __JPS_KEYBOARD_H

char getScancode();

char getCh();

void parseKey(short int key);

char scancodeToAscii(unsigned char sc);
#endif