#include "string.h"

uint32_t strlen(uint8_t* string){
	uint32_t retval = 0;
	while(*string != NULL){
		retval++;
		string++;
	}
	return retval;
}

uint32_t strcmp(uint8_t* first, uint8_t* second){
	uint32_t retval = 0;
	uint32_t size_1 = strlen(first);
	uint32_t size_2 = strlen(second);
	uint32_t size = (size_1 > size_2) ? size_1 : size_2;
	for(int i = 0; i < size; i++){
		if(first[i] > second[i]){
			return 1;
		}else if(first[i] < second[i]){
			return 2;
		}
	}
	return retval;
}