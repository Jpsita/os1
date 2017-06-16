
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
	if(header->e_type != ELFCLASS32){
		return ~0;
	}
	if(header->e_machine != ELFDATA2LSB){
		return ~0;
	}
	if(header->e_usage != ET_EXEC){
		return ~0;
	}
	if(header->e_ABI != EM_386){
		return ~0;
	}
	uint8_t* elf_offset = (uint8_t*) 0x100000;
	uint8_t* ph_off = (uint8_t*) header->e_phoff;
	uint32_t ph_size = (uint32_t) header->e_phentsize;
	uint32_t ph_num = (uint32_t) header->e_phnum;
	for(uint32_t i = 0; i < ph_num; i++){
		Elf32_Phdr* pheader = (Elf32_Phdr*)  ((uint32_t) elfBuffer + (uint32_t) ph_off + (ph_size * i) );
		if(pheader->p_type != PT_LOAD)
			continue;
		Elf32_Addr vaddr = pheader->p_vaddr;
		Elf32_Word memSz = pheader->p_memsz;
		Elf32_Word fileSz = pheader->p_filesz;
		Elf32_Word fileOff = pheader->p_offset;

		for(int y = 0; y < memSz; y++){
			elf_offset[vaddr + y] = 0;
		}
		for(int y = 0; y < fileSz; y++){
			elf_offset[vaddr + y] = elfBuffer[fileOff + y];
		}
	}

	return header->e_entry + (uint32_t) elf_offset;
}

uint8_t* elfBuffer = (uint8_t*) 0x36A00;
