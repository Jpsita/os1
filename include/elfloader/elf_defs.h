#ifndef __LEF_ELF_DEFS_H
#define __LEF_ELF_DEFS_H

typedef	unsigned int	Elf32_Addr;
typedef unsigned short	Elf32_Half;
typedef unsigned int 	Elf32_Off;
typedef	int				Elf32_Sword;
typedef	unsigned int	Elf32_Word;
typedef unsigned char	Elf32_Uchar;


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

typedef struct{
	unsigned char		e_ident[EI_NIDENT]; //Object file Marker
	Elf32_Half			e_type; 			//Object Type
	Elf32_Half			e_machine;			//Machine Type
	Elf32_Word			e_version;			//Object File Version
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

#endif