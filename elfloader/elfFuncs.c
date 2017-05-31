#include "elfloader/elf_defs.h"
#include "elfloader/elfFuncs.h"

uint32_t is_header_valid(Elf32_Ehdr* header){
	return header->e_ident[0] == ELFMAG0 && header->e_ident[1] == ELFMAG1 && header->e_ident[2] == ELFMAG2 && header->e_ident[3] == ELFMAG3;
}