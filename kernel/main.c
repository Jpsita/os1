#define C_STR char*
#include "keyboard.h"
#include "video.h"
#include "utils.h"
#include "interrupt.h"
#include "floppy.h"
#include "rtc.h"
#include "fat.h"
#include "string.h"
#include "kerneldefs.h"

extern uint8_t fat[];
extern uint8_t root_dir[];
extern FAT_DIR_LN_ENTRY root_dir_entries[240];
extern FAT_DIR_LN_ENTRY test_dir_entries[];

extern uint8_t* tmpPtr;

extern void (*callShell)();

void entryc(){
	loadVideoPort();

	PIC_remap(0x20, 0x28);

	create_IDT();

	init_RTC();

	floppy_init();

	printString("os1 Booting...\n");
	fa_impl = initializeFAT(1, fat, 18*512, root_dir, 240 * 32);
	if(fa_impl.fat == 0){
		printString("Error loading FAT. Buffer too small\n");
	}
	printString("Reading directory '/':\n");
	uint16_t n_ent = listFilesFAT(&fa_impl, "/", root_dir_entries, 240);
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
	int shell_id = -1;
	for(int i = 0; i < n_ent; i++){
		printString("> ");
		printString(root_dir_entries[i].long_name);
		printString("\n");
		if(strcmp((uint8_t*) root_dir_entries[i].long_name, "SHELL.RNB") == 0){
			shell_id = i;
		}
	}
	if(shell_id != -1){
		printString("Loading Shell...\n");
		//enable_echo();
		tmpPtr = (uint8_t*) 0x33200;
		uint32_t size = loadFileFromCluster(&fa_impl, root_dir_entries[shell_id].cluster, tmpPtr, 0xE00);
		//printUint32(size);
		//printString("Size: ");
		//printUint32(size);
		//floppy_read(0x38, 1024, tmpPtr);
		//enable_echo();
		sleepMs(1000);
		printString("Calling shell...\n");
		sleepMs(100);
		callShell = (void (*)) tmpPtr;
		callShell();
	}else
		printString("Shell not found.\n");
	while(1){
		//char c = getCh();
		//printCharacterRead (c);
	}
}

uint8_t* tmpPtr;
void (*callShell)();

uint8_t fat[18 * 512];
uint8_t root_dir[240 * 32];
FAT_DIR_LN_ENTRY	root_dir_entries[240];
FAT_IMPL			fa_impl;
