#define C_STR char*
#include "keyboard.h"
#include "video.h"

void entryc(){
	posX = 0;
	posY = 0;
	printString("Hello fools!!\n");
	while(1){
		char key = getCh();
		parseKey(key);
	}
}