#define C_STR char*
#include "keyboard.h"
#include "video.h"
#include "utils.h"
#include "interrupt.h"
#include "floppy.h"
#include "rtc.h"
#include "fat.h"

extern uint8_t tmpBuf[];
extern uint8_t fat[];
extern uint8_t root_dir[];

void entryc(){
	loadVideoPort();
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
	printString("Loading FLOPPY FAT...\n");
	FAT_IMPL fatImpl = initializeFAT(1, fat, 18*512, root_dir, 240 * 32);
	if(fatImpl.fat == 0){
		printString("Error loading FAT. Buffer too small\n");
	}
	printString("Read done!\n");
	enable_echo();
	while(1){
		//char c = getCh();
		//printCharacter(c);
	}
}

uint8_t tmpBuf[1024];
uint8_t fat[18 * 512];
uint8_t root_dir[240 * 32];