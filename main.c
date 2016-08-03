#define C_STR char*
#include "keyboard.h"
#include "video.h"
#include "utils.h"
#include "interrupt.h"

void entryc(){
	printString("Hello fools!!\n");
	PIC_remap(0x20, 0x28);
	create_IDT();
	while(1){
		char key = getCh();
	}
}