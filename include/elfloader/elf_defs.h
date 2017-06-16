#ifndef __LEF_ELF_DEFS_H
#define __LEF_ELF_DEFS_H

#include "libkernel/definitions.h"

typedef	unsigned int	Elf32_Addr;
typedef unsigned short	Elf32_Half;
typedef unsigned int 	Elf32_Off;
typedef	int				Elf32_Sword;
typedef	unsigned int	Elf32_Word;
typedef unsigned char	Elf32_Uchar;

typedef int elf32_main(int, char**); 

#define EI_MAG0				0
#define EI_MAG1				1
#define EI_MAG2				2
#define EI_MAG3				3
#define EI_CLASS			4			//File Class
#define EI_DATA				5			//Data Encoding
#define EI_VERSION			6			//File Version
#define EI_PAD				7			//Start of padding bytes
#define EI_NIDENT			16

#define ELFMAG0				0x7f		//e_ident[EI_MAG0]
#define ELFMAG1				'E'			//e_ident[EI_MAG1]
#define ELFMAG2				'L'			//e_ident[EI_MAG2]
#define ELFMAG3				'F'			//e_ident[EI_MAG3]

#define ELFCLASSNONE		0			//Invalid Class
#define ELFCLASS32			1			//32-bit objects
#define ELFCLASS64			2			//64-bit objects

#define ELFDATANONE			0			//Invalid Data Encoding
#define ELFDATA2LSB			1
#define ELFDATA2MSB			2

#define ET_NONE				0
#define ET_REL				1
#define ET_EXEC				2
#define ET_DYN				3
#define ET_CORE				4
#define ET_LOPROC			0xff00		//Processor specific
#define ET_HIPROC			0xffff		//Processor specific

#define EM_NONE				0
#define EM_M32				1			//AT&T WE 32100
#define EM_SPARC			2			//SPARC
#define EM_386				3			//Intel 80386
#define EM_68K				4			//Motorola 68000
#define EM_88K				5			//Motorloa 88000
#define EM_860				7			//Intel 80860
#define EM_MIPS				8			//MIPS RS3000

#define EV_NONE				0			//No version
#define EV_CURRENT			1			//Current (first) version

typedef struct PACKED{
	unsigned char		e_ident[EI_NIDENT]; //Object file Marker
	Elf32_Uchar			e_type; 			//Object Architecture()
	Elf32_Uchar			e_machine;			//Machine Type
	Elf32_Uchar			e_version;			//Object File Version
	Elf32_Uchar			e_ABI;				//OS Type (usually 0)
	Elf32_Word			e_padding0;			//padding
	Elf32_Word			e_padding1;			//padding
	Elf32_Half			e_usage;			//object type (executable, relocatable, ecc)
	Elf32_Half			e_instrSet;			//Instruction set
	Elf32_Word			e_version2;			//ELF Version (again)
	Elf32_Addr			e_entry;			//Virtual entry point
	Elf32_Off			e_phoff;			//Program Header Table Offset (bytes)
	Elf32_Off			e_shoff;			//Section Header Table Offset (bytes)
	Elf32_Word			e_flags;			//Machine-Related flags
	Elf32_Half			e_ehsize;			//ELF Header Size
	Elf32_Half			e_phentsize;		//Size of entry of Program Header Table (bytes)
	Elf32_Half			e_phnum;			//Number of entries in Program Header Table
	Elf32_Half			e_shentsize;		//Size of Section Header (Section Header Table Entry) (bytes)
	Elf32_Half			e_shnum;			//Number of entries in Section Header Table
	Elf32_Half			e_shstrndx;			//Section Header Table index of the String Table Entry
} Elf32_Ehdr;

#define SHN_UNDEF			0				//Undefined, Missing, Irrelevant
#define SHN_LORESERVE		0xff00			//Lower bound of Reserved Indices
#define SHN_LOPROC			0xff00			//Processor-Specific Begin
#define SHN_HIPROC			0xff1f			//Processor-Specific End
#define SHN_ABS				0xfff1			//Absolute Values for Corresponding References
#define SHN_COMMON			0xfff2			//FORTRAN COMMON or Unallocated C
#define SHN_HIRESERVE		0xffff			//Upper Bound of Reserved Indices

#define SHT_NULL			0				//Inactive
#define SHT_PROGBITS		1				//Format and meaning determined by the program
#define SHT_SYMTAB			2				//Holds a Symbol Table
#define SHT_STRTAB			3				//Section holds a String Table
#define SHT_RELA			4				//Relocations with Explicit Addends
#define SHT_HASH			5				//Contains a Hash table
#define SHT_DYNAMIC			6				//Information for Dynamic Linking
#define SHT_NOTE			7				//Generic Info
#define SHT_NOBITS			8				//Section with no bits in the file, but has to be treated as SHT_PROGBITS
#define SHT_REL				9				//Relocations without explicit addends
#define SHT_SHLIB			10				//Unused and Deprecated
#define SHT_DYNSYM			11				//Same as SHT_SYMTAB, but with less symbols, used mainly for dynamic linking
#define SHT_LOPROC			0x70000000		//			RESERVED FOR
#define SHT_HIPROC			0x7fffffff		//			PROCESSOR SPECIFIC
#define SHT_LOUSER			0x80000000		//Lower bounds of indexes for applications
#define SHT_HIUSER			0xffffffff		//Higher bounds of indexes for application

#define SHF_WRITE			0x1				//Writable Data
#define SHF_ALLOC			0x2				//Occupies memory
#define SHF_EXECINSTR		0x4				//Executable Data
#define SHF_MASKPROC		0xf0000000		//Bits reseved for processor specific semantics

typedef struct { 
	Elf32_Word 			sh_name;			//Index to section name
	Elf32_Word 			sh_type;			//Contents and Semantics type
	Elf32_Word 			sh_flags;			//Miscellaneous Attributes (bitfield)
	Elf32_Addr			sh_addr;			//Address in virtual memory
	Elf32_Off			sh_offset;			//Offset from the beginning of file (bytes)
	Elf32_Word			sh_size;			//Bytes occupied in file
	Elf32_Word			sh_link;			//Section Header table Index Link
	Elf32_Word			sh_info;			//Extra info (type-dependant)
	Elf32_Word			sh_addralign;		//Alignment (or lack there of)
	Elf32_Word			sh_entsize;			//Size of each entry
}Elf32_Shdr;

//Sections:
//	.bss				Uninitalized data
//	.comment			Version Control Info
//	.data
//	.data1				initialized data
//	.debug				used for debugging
//	.dynamic			Dynamic Linking information
//	.dynstr				Dynamic Linking related strings
//	.dynsym				Dynamic Linking Symbol Table
//	.fini				Executable code used to finish execution
//	.got				Global Offset Table
//	.hash				Symbolic Hash Table
//	.init				Initialization Code
//	.interp				Path to interpreter
//	.line				Line numbers used for debugging
//	.note				Extra Information
//	.plt				Procedure Linkage Table
//	.rel(name)
//	.rela(name)			Relocation Information
//	.rodata
//	.rodata1			Read-only data
//	.shstrtab			Section Names
//	.strtab				Strings (generally used for Symbol Tables)
//	.symtab				Symbol Table
//	.text				Main Code Section

typedef struct{
	Elf32_Word			st_name;		//Index into string table
	Elf32_Addr			st_value;		//Value of the symbol (varies)
	Elf32_Word			st_size;		//Size. 0 for Null or unknown
	unsigned char		st_info;		//Type and Binding
	unsigned char		st_other;		//Undefined Meaning
	Elf32_Half			st_shndx;		//Section indices
} Elf32_Sym;

#define STB_LOCAL		0				//Local Symbols, not visible outside object
#define STB_GLOBAL		1				//Global Symbols, visible to every object combined
#define STB_WEAK		2				//Like Global, but with lower precedence
#define STB_LOPROC		13				//			RESERVED FOR
#define STB_HIPROC		15				//			PROCESSOR SPECIFIC

#define STT_NOTYPE		0				//Type not specified
#define STT_OBJECT		1				//Associated with a data object
#define STT_FUNC		2				//Symbol is associated with a function or executable code
#define STT_SECTION		3				//Symbol is associated with a section
#define STT_FILE		4				//Source file associated with object file
#define STT_LOPROC		13				//			RESERVED FOR
#define STT_HIPROC		15				//			PROCESSOR SPECIFIC


#define ELF32_ST_BIND(i)   ((i)>>4)
#define ELF32_ST_TYPE(i)   ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf)

typedef struct{
	Elf32_Addr			r_offset;		//Location to apply the relocation action
	Elf32_Word			r_info;			//Symbol table index AND type of relocation
} Elf32_Rel;

typedef struct{
	Elf32_Addr			r_offset;		//Location to apply the relocation action
	Elf32_Word			r_info;			//Symbol table index AND type of relocation
	Elf32_Sword			r_addend;		//Constant addend to be added to relocation field
} Elf32_Rela;

#define PT_NULL			0				//Element unused
#define PT_LOAD			1				//Lodable segment
#define PT_DYNAMIC		2				//Dynamic linking information
#define PT_INTERP		3				//Interpreter location
#define PT_NOTE			4				//Location and size of extra info
#define PT_SHLIB		5				//Reserved and unspecified
#define PT_PHDR			6				//Location and size of Program Header Table
#define PT_LOPROC		0x70000000		//		RESERVED FOR
#define PT_HIPROC		0x7fffffff		//		PROCESSOR SPECIFIC

#define PFLAGS_READ		4
#define PFLAGS_WRITE	2
#define PFLAGS_EXEC		1

typedef struct PACKED {
	Elf32_Word      p_type;				//Type of segment
	Elf32_Off       p_offset;			//Offset from beginning of file
	Elf32_Addr      p_vaddr;			//Segment's Virtual address
	Elf32_Addr      p_paddr;			//Segment's physical address (not used)
	Elf32_Word      p_filesz;			//Size in the file image
	Elf32_Word      p_memsz;			//Size in memory
	Elf32_Word      p_flags;			//Flags
	Elf32_Word      p_align;			//Align
} Elf32_Phdr;

#endif