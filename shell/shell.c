#include "libkernel/definitions.h"
#include "libkernel/video.h"
#include "libkernel/keyboard.h"
#include "libkernel/string.h"
#include "libkernel/fat.h"
#include "common/staticmemorymap.h"
#include "elfloader.h"

typedef int(*APP_MAIN)(int, char**);

extern uint8_t	currentPath[];
extern uint8_t commandLine[];
extern uint32_t lineWidth;
extern uint8_t isElfLoaderLoaded;
extern FAT_IMPL *ft;
extern ELF_MAIN elf_main;
extern APP_MAIN app_main;

extern void printLine();
extern void parseLine();

void entryShell(){
	s_strcpy("/", currentPath);
	commandLine[0] = 0;
	v_clear();
	k_cleanBuff();
	printLine();
	while(1){
		char c = k_getCh();
		switch(c){
			case CHAR_BKSP:
				if(lineWidth > 0){
					v_bksp();
					lineWidth--;
				}
				uint32_t ln = s_strlen(commandLine);
				if(ln > 0){
					commandLine[ln - 1] = 0;
				}
				break;
			case CHAR_TAB:
				v_printCharacter(' ');
				v_printCharacter(' ');
				v_printCharacter(' ');
				v_printCharacter(' ');
				lineWidth += 4;
				break;
			case CHAR_NEWLINE:
				parseLine();
				printLine();
				break;
			default:
				lineWidth++;
				s_stradd(commandLine, c);
				v_printCharacter(c);
		}
	}
	v_printString("Quitting shell...");
}

void printLine(){
	v_printString(" '");
	v_printString(currentPath);
	v_printString("' @ os1: ");
	lineWidth = 0;
}

void parseLine(){
	v_printString("\n");
	if(s_strcmp(commandLine, "ls") == 0){

	}else if(s_strcmp(commandLine, "welcome") == 0){
		v_printString("Welcome to JStefanelli's OS1.\nPlease, report bugs to github!");
	}else if(s_strcmp(commandLine, "help") == 0){
		v_printString("JStefanelli's OS1 v. 0.01a. Available commands:\n");
		v_printString("    welcome: Show welcome message.\n");
		v_printString("    cls: Clear the screen\n");
		v_printString("    help: Show this message.\n");
		v_printString("    lelf: Load the ELF Loader.\n");
		v_printString("    elf [elf_path]: Execute the ELF file located at [elf_path]\n");
		v_printString("Copyright (c) John Stefanelli, 2017");
	}else if(s_strcmp(commandLine, "cls") == 0){
		v_clear();
		commandLine[0] = 0;
		return;
	}else if(s_strcmp(commandLine, "lelf") == 0){
		if(isElfLoaderLoaded != 0){
			v_printString("ELF Loader already loaded");
		}else{
			v_printString("Trying to load ELF Loader...\n");
			ft = f_getFAT();
			uint32_t res = f_loadFile(ft, "/ELFL.RNB", (uint8_t*) ELFL_BUFF_START, ELFL_BUFF_SIZE);
			if(res != ~0){
				v_printString("ELF Loader loaded succesfully.");
				isElfLoaderLoaded = 1;
			}else{
				v_printString("ELF Loader loading failed.");
			}
		}
	}else if(s_strcmp(commandLine, "elf") == 0){
		v_printString("Usage: elf file");
	}else if(s_strcmp(commandLine, "elf") == 1){
		if(isElfLoaderLoaded == 0){
			v_printString("Please load the ELF Loader before executing this command.\n");
			v_printString("(Run \"lelf\" or see the help screen)");
		}else{
			uint32_t res = elf_main(commandLine + 4);
			v_printString("ELF Loader returned: ");
			v_printUint32(res);
			v_printString("\nPress a button to continue \n");
			k_getCh();
			if(res != ~0){
				v_printString("\n");
				app_main = (APP_MAIN) res;
				int32_t app_res = app_main(0, (char**) 0);
				v_printString("App returned: ");
				v_printUint32(app_res);
			}else{
				v_printString("ELF loading failed");
			}
		}
	}else{
		v_printString("Unknown command: '");
		v_printString(commandLine);
		v_printCharacter('\'');
	}
	commandLine[0] = 0;
	v_printString("\n");
}

uint8_t	currentPath[256];
uint8_t commandLine[256];
uint32_t lineWidth = 0;
uint8_t isElfLoaderLoaded = 0;
ELF_MAIN elf_main = (ELF_MAIN) ELFL_INIT_ADDR;
FAT_IMPL *ft;
APP_MAIN app_main;
