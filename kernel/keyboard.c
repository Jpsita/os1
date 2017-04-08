#include "keyboard.h"
#include "scancodes.h"
#include "video.h"
#include "utils.h"
#include "interrupt.h"

unsigned char KEYB_STATUS_BYTE = 0;

int isShift(){
	if(KEYB_STATUS_BYTE & STATUS_BIT_SHIFT){
		return 1;
	}
	return 0;
}

void doShift(){
	KEYB_STATUS_BYTE |= STATUS_BIT_SHIFT;
}

void unShift(){
	KEYB_STATUS_BYTE &= (!STATUS_BIT_SHIFT);
}

int isCtrl(){
	if(KEYB_STATUS_BYTE & STATUS_BIT_CTRL){
		return 1;
	}
	return 0;
}

void doCtrl(){
	KEYB_STATUS_BYTE |= STATUS_BIT_CTRL;
}

void unCtrl(){
	KEYB_STATUS_BYTE &= (!STATUS_BIT_CTRL);
}

char scancodeToAscii(unsigned char sc){
	if(!isShift()){
		return scancode_tbl[sc - 1]; 
	}else{
		return scancode_tbl_shift[sc - 1];
	}
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
			doCtrl();
			break;
		case KEY_SHIFT:
			doShift();
			break;
		case NULL:
			break;
		default:
			printCharacter(key);
	}
}

void parseKeyOff(short int key){
	switch(key){
		case KEY_CTRL:
			unCtrl();
			break;
		case KEY_SHIFT:
			unShift();
			break;
		default:
			return;
	}
}

void PS2_wait(){
	unsigned char y = inb(PS2_COMM_PORT);
	do{
		y  = inb(PS2_COMM_PORT);
	}while(y & 0x01);
}

void init_keyboard(){
	/*PS2_wait();
	outb(PS2_DATA_PORT, CMD_SCANCODE_SET_GET);
	unsigned char x = inb(PS2_DATA_PORT);
	while(x == PS2_RSD){
		PS2_wait();
		outb(PS2_DATA_PORT, CMD_SCANCODE_SET_GET);
		x = inb(PS2_DATA_PORT);	
		printCharacter('r');
	}
	print_hex(x);
	printCharacter(' ');
	PS2_wait();
	outb(PS2_DATA_PORT, PRM_SCANCODE_SET_SC2);
	x = inb(PS2_DATA_PORT);
	while(x == PS2_RSD){
		PS2_wait();
		outb(PS2_DATA_PORT, PRM_SCANCODE_SET_SC2);
		x = inb(PS2_DATA_PORT);	
		printCharacter('r');
	}
	print_hex(x);
	printCharacter(' ');
	*/
	
	PS2_wait();
	outb(PS2_DATA_PORT, CMD_LED_CTRL);
	unsigned  char x = inb(PS2_DATA_PORT);
	while(x == PS2_RSD){
		PS2_wait();
		outb(PS2_DATA_PORT, CMD_LED_CTRL);
		x = inb(PS2_DATA_PORT);	
		//printCharacter('r');
	}
	PS2_wait();
	outb(PS2_DATA_PORT, LED_SCR_LCK | LED_CAPS_LCK | LED_NUM_LCK);
	x = inb(PS2_DATA_PORT);
	while(x == PS2_RSD){
		PS2_wait();
		outb(PS2_DATA_PORT, LED_SCR_LCK | LED_CAPS_LCK | LED_NUM_LCK);
		x = inb(PS2_DATA_PORT);	
		//printCharacter('r');
	}
}