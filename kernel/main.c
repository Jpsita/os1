#define C_STR char*
#include "keyboard.h"
#include "video.h"
#include "utils.h"
#include "interrupt.h"
#include "floppy.h"
#include "rtc.h"
#include "fat.h"
#include "string.h"

extern uint8_t fat[];
extern uint8_t root_dir[];
extern FAT_DIR_LN_ENTRY root_dir_entries[];
extern FAT_DIR_LN_ENTRY test_dir_entries[];

uint8_t* tmpPtr;

void (*callShell)();

void entryc(){
	loadVideoPort();

	PIC_remap(0x20, 0x28);
	
	create_IDT();
	
	init_RTC();
	
	floppy_init();

	printString("os1 Booting...\n");
	FAT_IMPL fatImpl = initializeFAT(1, fat, 18*512, root_dir, 240 * 32);
	if(fatImpl.fat == 0){
		printString("Error loading FAT. Buffer too small\n");
	}
	printString("Reading directory '/':\n");
	uint16_t n_ent = listFilesFAT(&fatImpl, "/", root_dir_entries, 240);
	if(n_ent == 0){
		printString("Error: empty folder! (But then, where the fuck did you load me from?)\n");
		while(1){
			sleepMs(1000);
		}
	}else if(n_ent == ~0){
		printString("Error reading folder.\n");
		while(1){
			sleepMs(1000);
		}
	}
	for(int i = 0; i < n_ent; i++){
		printString("> ");
		printString(root_dir_entries[i].long_name);
		printString("\n");
		if(strcmp((uint8_t*) root_dir_entries[i].long_name, "SHELL.RNB") == 0){

			printString("Loading Shell...\n");
			//enable_echo();
			tmpPtr = (uint8_t*) 0x80000;
			//uint32_t size = loadFileFromCluster(&fatImpl, root_dir_entries[i].cluster, tmpPtr, 0x600);
			//printString("Size: ");
			//printUint32(size);
			floppy_read(0x37, 512, tmpPtr);
			callShell = (void (*)) 0x80000;
			callShell();
			break;
		}
	}
	printString("Shell not found.\n");
	while(1){
		//char c = getCh();
		//printCharacterRead (c);
	}
}

uint8_t fat[18 * 512];
uint8_t root_dir[240 * 32];
FAT_DIR_LN_ENTRY	root_dir_entries[240];