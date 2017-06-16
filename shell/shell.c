#include "libkernel/definitions.h"
#include "libkernel/video.h"
#include "libkernel/keyboard.h"
#include "libkernel/string.h"
#include "libkernel/fat.h"

uint8_t	currentPath[256];
uint8_t commandLine[256];
uint32_t lineWidth = 0;

void printLine();
void parseLine();

void entryShell(){
	v_printString("Shell loaded\n Starting ELF Loader\n");
	FAT_IMPL* ft = f_getFAT();
	f_loadFile(ft, "/elfl.rnb", (uint8_t*) 0x34E00, 0x1C00);
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
		v_printString("Copyright (c) John Stefanelli, 2017");
	}else if(s_strcmp(commandLine, "cls") == 0){
		v_clear();
		commandLine[0] = 0;
		return;
	}else{
		v_printString("Unknown command: '");
		v_printString(commandLine);
		v_printCharacter('\'');
	}
	commandLine[0] = 0;
	v_printString("\n");
}
