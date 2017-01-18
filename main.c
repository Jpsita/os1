#define C_STR char*
#include "keyboard.h"
#include "video.h"
#include "utils.h"
#include "interrupt.h"

void entryc(){
	PIC_remap(0x20, 0x28);
	create_IDT();
	printString("Hello fools!!\n");
	//printString("TEST_3\n");
	while(1){
		//char c = getCh();
		//printCharacter(c);
	}
}