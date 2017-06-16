#include "keyboard.h"
#include "scancodes.h"
#include "video.h"
#include "utils.h"
#include "rtc.h"
#include "interrupt.h"

unsigned char KEYB_STATUS_BYTE = 0;
uint8_t currentChar = 0;

uint8_t* keyboardBuffer;
uint8_t* nextChar;
uint8_t* nextRead;
uint8_t* keyboardBufferEnd;

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
			if(echo_byte != 0)
				deleteCurrentChar();
			buff_addChar((char) 8);
			break;
		case KEY_TAB:
			if(echo_byte != 0)
				printTab();
			buff_addChar('\t');
			break;
		case KEY_ESC:
			buff_addChar((char) 27);
			break;
		case KEY_RETURN:
			if(echo_byte != 0)
				newLine();
			buff_addChar('\n');
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
			currentChar = (uint8_t) key;
			if(echo_byte != 0)
				printCharacter(key);
			buff_addChar(key);
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
	keyboardBuffer = (uint8_t*) 0x32E00;
	keyboardBufferEnd = (uint8_t*) 0x33200;
	nextChar = keyboardBuffer + 1;
	nextRead = keyboardBuffer;
}

char getCh(){
	while(red == 0){
		sleepMs(10);
	}
	red = 0;
	return currentChar;
}

void buff_addChar(char c){
	*nextChar = c;

	nextChar++;
	if(nextChar == nextRead){
		printString("WARNING: Keyboard Buffer overflow\n");
	}
	if(nextChar == keyboardBufferEnd){
		nextChar = keyboardBuffer;
	}
	*nextChar = 0;
}

void buff_clean(){
	nextChar = keyboardBuffer;
	nextRead = keyboardBuffer;
	*keyboardBuffer = 0;
}

char nextCh(){
	if(*nextRead != 0){
		char c = *nextRead;
		*nextRead = 0;
		nextRead++;
		if(nextRead == keyboardBufferEnd){
			nextRead = keyboardBuffer;
		}
		return c;
	}
	return 0;
}

char buff_getCh(){
	char c;
	do{
		c = nextCh();
		if(c == 0){
			sleepMs(100);
		}else{
			return c;
		}
	}while(c == 0);
	return 0;
}

char buff_getCharacter(){
	return nextCh();
}

void buff_getString(uint8_t* string){
	buff_getStringTrm(string, '\n');
}

void buff_getStringTrm(uint8_t* string, char terminator){
	char c;
	do{
		c = nextCh();
		if(c == 0 || c == terminator){
			return;
		}else{
			*string = c;
			string++;
			*string = 0;
		}
	}while(c != 0 && c != terminator);
}
