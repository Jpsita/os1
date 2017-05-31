
#include "elfloader/elf_defs.h"
#include "elfloader/elfFuncs.h"
#include "libkernel.h"

extern uint8_t* elfBuffer;

uint32_t entryElf(char* path){
	uint16_t res = f_loadFile(f_getFAT(), path, elfBuffer, 0x1400);
	if(res == ~0){
		return ~0;
	}
	Elf32_Ehdr* header = (Elf32_Ehdr*) elfBuffer;
	if(!is_header_valid(header)){
		return ~0;
	}
}

uint8_t* elfBuffer = (uint8_t*) 0x13800;
