#include "video.h"
#include "main.h"
#include "utils.h"

#define VGA_INDEX_REGISTER_PORT		0x000
#define VGA_INDEX_COMMAND_PORT		0x001

#define VGA_INDEX_LOW_PORT			0x0F
#define VGA_INDEX_HIGH_PORT			0x0E

unsigned short video_base;

char * videoMem = (char * ) 0x000B8000;
int posX = 0;
int posY = 0;

void loadVideoPort(){
	video_base = *(unsigned short*)(0x0463);
}

void updateCursor(){
	unsigned short cursorPos = (posY * 80) + posX;

	//Low part
	outb(video_base + VGA_INDEX_REGISTER_PORT, VGA_INDEX_LOW_PORT);
	outb(video_base + VGA_INDEX_COMMAND_PORT, (unsigned char)((cursorPos)&0xFF));

	//High Part
	outb(video_base + VGA_INDEX_REGISTER_PORT, VGA_INDEX_HIGH_PORT);
	outb(video_base + VGA_INDEX_COMMAND_PORT, (unsigned char)((cursorPos >> 8)&0xFF));
}

void printCharacterAtPos(char c, char color, int x, int y){
	char * tmp = videoMem;
	tmp += (y * 160);
	tmp += (x * 2);
	*tmp++ = c;
	*tmp = color;
	updateCursor();
}

void clearScreen(){
	char * tmp = videoMem;
	int i = 0;
	for(; i < 2000; i++){
		*tmp++ = 0;
		*tmp++ = COLOR_WHITE_BLACK;
	}
	updateCursor();
}

void printTab(){
	posX += 4;
	if(posX >= 80){
		posX = 79;
	}
	updateCursor();
}

void newLine(){
	posX = 0;
	posY+= 1;
	if(posY >= 25){
		clearScreen();
		posY = 0;
	}
	updateCursor();
}



void deleteCurrentChar(){
	posX--;
	if(posX < 0){
		posX = 79;
		posY--;
		if(posY < 0){
			posY = 0;
			posX = 0;
		}
	}
	printCharacterAtPos(NULL, COLOR_WHITE_BLACK, posX, posY);

}

void printString(char * s){
	while(*s){
		if(*s != '\n'){
			printCharacterAtPos(*s++, COLOR_WHITE_BLACK, posX, posY);
			posX+= 1;
			if(posX >= 80){
				posY+= 1;
				if(posY >= 25){
					clearScreen();
					posY = 0;
				}
				posX = 0;
			}
		}else{
			s++;
			newLine();
		}
	}
	updateCursor();
}

void printCharacter(char chr){
	printCharacterAtPos(chr, COLOR_WHITE_BLACK, posX, posY);
	posX++;
	if(posX >= 80){
		posX = 0;
		posY++;
		if(posY >= 25){
			clearScreen();
			posY = 0;
		}
	}
	updateCursor();
}