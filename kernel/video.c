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

//1
void printCharacterAtPos(char c, char color, int x, int y){
	char * tmp = videoMem;
	tmp += (y * 160);
	tmp += (x * 2);
	*tmp++ = c;
	*tmp = color;
	updateCursor();
}

//2
void clearScreen(){
	char * tmp = videoMem;
	int i = 0;
	for(; i < 2000; i++){
		*tmp++ = 0;
		*tmp++ = COLOR_WHITE_BLACK;
	}
	updateCursor();
}

//3
void printTab(){
	posX += 4;
	if(posX >= 80){
		posX = 79;
	}
	updateCursor();
}

//4
void newLine(){
	posX = 0;
	posY+= 1;
	if(posY >= 25){
		clearScreen();
		posY = 0;
	}
	updateCursor();
}


//5
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

//6
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

//7
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

void printHex(uint32_t val){
	switch(val){
		case 0:
		printCharacter('0');
		break;
		case 0x1:
		printCharacter('1');
		break;
		case 0x2:
		printCharacter('2');
		break;
		case 0x3:
		printCharacter('3');
		break;
		case 0x4:
		printCharacter('4');
		break;
		case 0x5:
		printCharacter('5');
		break;
		case 0x6:
		printCharacter('6');
		break;
		case 0x7:
		printCharacter('7');
		break;
		case 0x8:
		printCharacter('8');
		break;
		case 0x9:
		printCharacter('9');
		break;
		case 0xA:
		printCharacter('A');
		break;
		case 0xB:
		printCharacter('B');
		break;
		case 0xC:
		printCharacter('C');
		break;
		case 0xD:
		printCharacter('D');
		break;
		case 0xE:
		printCharacter('E');
		break;
		case 0xF:
		printCharacter('F');
		break;
		
	}
}

//8
void printUint32(uint32_t val){
	printString("0x");
	uint32_t i = 28;
	uint8_t printed = 0;
	uint32_t mask = 0x0000000F;
	while(i != 0){
		uint32_t vl = val >> i;
		vl &= mask;
		i-= 4;
		if(vl != 0 || printed == 1){
			printHex(vl);
			printed = 1;
		}
	}
	uint32_t vl = val & mask;
	printHex(vl);
}