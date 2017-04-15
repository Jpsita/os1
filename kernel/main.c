#define C_STR char*
#include "keyboard.h"
#include "video.h"
#include "utils.h"
#include "interrupt.h"
#include "floppy.h"
#include "rtc.h"
#include "fat.h"

extern uint8_t fat[];
extern uint8_t root_dir[];
extern FAT_DIR_LN_ENTRY root_dir_entries[];
extern FAT_DIR_LN_ENTRY test_dir_entries[];


void entryc(){
	loadVideoPort();
	//printString("Loading IDT and basic interrupt routines...\n");
	PIC_remap(0x20, 0x28);
	create_IDT();
	//printString("Loading RTC...\n");
	init_RTC();
	//printString("Loading Floppy and ISA DMA...\n");
	floppy_init();
	//printString("Basic Functionality Ready.\n");

	printString("os1 Booting...\n");
	//printString("Loading FLOPPY FAT...\n");
	FAT_IMPL fatImpl = initializeFAT(1, fat, 18*512, root_dir, 240 * 32);
	if(fatImpl.fat == 0){
		printString("Error loading FAT. Buffer too small\n");
	}
	printString("Reading directory '/':\n");
	uint16_t n_ent = listFilesFAT(&fatImpl, "/", root_dir_entries, 240);
	if(n_ent != 0xFFFF){
		for(uint16_t i = 0; i < n_ent; i++){
			printString("    ");
			printString(root_dir_entries[i].long_name);
			printString("    ");
			printUint32(root_dir_entries[i].cluster);
			printString("\n");
		}
		uint16_t n_ent_2 = listFilesFAT(&fatImpl, "/test/", test_dir_entries, 240);
	}
	//uint8_t* buff = (uint8_t*) 0xbe00 + (0x15 * 512);
	//loadFileFromCluster(&fatImpl, 0x03, buff, 512 * 16);
	printString("Loading Shell...\n");
	enable_echo();
	while(1){
		//char c = getCh();
		//printCharacterRead (c);
	}
}

uint8_t fat[18 * 512];
uint8_t root_dir[240 * 32];
FAT_DIR_LN_ENTRY	root_dir_entries[240];
FAT_DIR_LN_ENTRY	test_dir_entries[240];