#define C_STR char*
#include "keyboard.h"
#include "video.h"
#include "utils.h"
#include "interrupt.h"
#include "floppy.h"
#include "rtc.h"

uint8_t tmpBuf[1024];

void entryc(){
	printString("Loading IDT and basic interrupt routines...\n");
	PIC_remap(0x20, 0x28);
	create_IDT();
	printString("Loading RTC...\n");
	init_RTC();
	printString("Loading Floppy and ISA DMA...\n");
	floppy_init();
	printString("Basic Functionality Ready.\n");
	//enable_echo();
	printString("Starting test read...\n");
	floppy_read(0, 512, tmpBuf);
	printString("Test Read Complete.\n");
	while(1){
		//char c = getCh();
		//printCharacter(c);
	}


}