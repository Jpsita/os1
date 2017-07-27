#include "kernel/string.h"

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

uint32_t strcat(uint8_t*first, uint8_t* second){
	uint8_t* ptr = first;
	while(*ptr != 0){
		ptr++;
	}
	uint8_t* second_ptr = second;
	while(*second_ptr != 0){
		*ptr = *second_ptr;
		ptr++;
		second_ptr++;
	}
	*ptr = 0;
	return 0;
}

uint32_t strcat_n(uint8_t*first, uint8_t* second, uint32_t max_chars){
	uint8_t* ptr = first;
	while(*ptr != 0){
		ptr++;
	}
	uint32_t chars = 0;
	uint8_t* second_ptr = second;
	while(*second_ptr != 0 && chars < max_chars){
		*ptr = *second_ptr;
		ptr++;
		second_ptr++;
		chars++;
	}
	*ptr = 0;
	return chars;
}

uint32_t strcat_nn(uint8_t* first, uint8_t* second, uint32_t max_chrs, uint8_t myNull){
	uint8_t* ptr = first;
	while(*ptr != 0){
		ptr++;
	}
	uint32_t chars = 0;
	uint8_t* second_ptr = second;
	while(*second_ptr != myNull && chars < max_chrs){
		*ptr = *second_ptr;
		ptr++;
		second_ptr++;
		chars++;
	}
	*ptr = 0;
	return chars;
}

void stradd(uint8_t* string, uint8_t chr){
	uint8_t* ptr = string;
	while(*ptr != 0){
		ptr++;
	}
	*ptr = chr;
	ptr++;
	*ptr = 0;
}

uint32_t strpos_s(uint8_t* string, uint8_t needle, uint32_t start){
	uint8_t* ptr = string;
	ptr += start;
	uint32_t i = start;
	while(*ptr != 0 ){
		if(*ptr == needle){
			return i;
		}
		i++;
		ptr++;
	}
	return ~0;
}
uint32_t strpos_rs(uint8_t* string, uint8_t needle, uint32_t start){
	uint32_t len = strlen(string);
	uint8_t* ptr = &string[len - start - 1];
	uint32_t i = len - start - 1;
	while(i >= 0){
		if(*ptr == needle){
			return i;
		}
		i--;
		ptr--;
	}
	return ~0;
}

uint32_t strpos(uint8_t* string, uint8_t needle){
	return strpos_s(string, needle, 0);
}

uint32_t strpos_r(uint8_t* string, uint8_t needle){
	return strpos_rs(string, needle, 0);
}

uint32_t substr(uint8_t* string, uint8_t* buffer, uint32_t start, uint32_t end){
	uint32_t i = start;
	uint32_t chrs = 0;
	while(string[i] != 0 && i < end){
		buffer[chrs] = string[i];
		chrs++;
		i++;
	}
	buffer[chrs] = 0;
	return chrs;
}