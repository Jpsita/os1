#ifndef	__ELFL_ELF_FUNCS_H
#define __ELFL_ELF_FUNCS_H

#include "libkernel/definitions.h"
#include "elfloader/elf_defs.h"

uint32_t is_header_valid(Elf32_Ehdr* header);


#endif