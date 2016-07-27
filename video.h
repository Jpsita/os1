#ifndef __JPS_VIDEO_H
#define __JPS_VIDEO_H
#define COLOR_WHITE_BLACK 0x0F

char * videoMem = (char * ) 0x000B8000;
int posX = 0;
int posY = 0;

void printCharacterAtPos(char c, char color, int x, int y){
	char * tmp = videoMem;
	tmp += (y * 160);
	tmp += (x * 2);
	*tmp++ = c;
	*tmp = color;
}

void clearScreen(){
	char * tmp = videoMem;
	int i = 0;
	for(; i < 2000; i++){
		*tmp++ = 0;
		*tmp++ = COLOR_WHITE_BLACK;
	}
}

void printTab(){
	posX += 4;
	if(posX >= 80){
		posX = 79;
	}
}

void newLine(){
	posX = 0;
	posY+= 1;
	if(posY >= 25){
		clearScreen();
		posY = 0;
	}
}



void deleteCurrentChar(){
	printCharacterAtPos(NULL, COLOR_WHITE_BLACK, posX, posY);
	posX--;
	if(posX < 0){
		posX = 79;
		posY--;
		if(posY < 0){
			posY = 0;
			posX = 0;
		}
	}
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
}



#endif