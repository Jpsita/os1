#include "keyboard.h"
#include "scancodes.h"
#include "video.h"
#include "utils.h"

char getScancode()
{
	char c=0;
	char d = 0;
	do {
		d = inb(0x60);
		if(d != c){
			c = inb(0x60);
			if(c> 0)
				return c;
		}
	}while(1);
}

char getCh(){
	return scancode_tbl_it[getScancode() - 1];
}

void parseKey(short int key){
	switch(key){
		case KEY_BKSP:
			deleteCurrentChar();
			break;
		case KEY_TAB:
			printTab();
			break;
		case KEY_ESC:
			break;
		case KEY_RETURN:
			newLine();
			break;
		case KEY_CTRL:
			break;
		case KEY_SHIFT:
			break;
		case NULL:
			break;
		default:
			printCharacter(key);
	}
}