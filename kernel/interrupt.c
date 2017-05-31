#include "interrupt.h"
#include "keyboard.h"
#include "utils.h"
#include "video.h"
#include "string.h"
#include "functionIds.h"
#include "fat.h"
#include "kerneldefs.h"

IDTDescr IDT[256];
char echo_byte;
char red = 0;

void PIC_sendEOI(unsigned char IRQ){
	if(IRQ >= 8){
		outb(PIC2_COMMAND, PIC_EOI);
	}
	outb(PIC1_COMMAND, PIC_EOI);
}

void c_dbl_flt(){
	printString("FAULT: DOBULE FAULT. IRRECOVERABLE ERROR. Press any key to reset. (Coming Soon)\n");
	while(1){
		
	}
}

void ZeroMemory(uint8_t* addr, uint32_t size) {
	for (int i = 0; i < size; i++) {
		addr[i] = 0;
	}
}

void PIC_remap(int offset1, int offset2){

	unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);
 
	outb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);	
}

void handle_irq_1(){
	unsigned char status = inb(0x64);
	if(!(status & 0x01)){
		return;
	}
	unsigned char x = inb(0x60);
	if(x == 0xFA){
		return;
	}
	if(x >= 128){
		char k = scancodeToAscii(x - 128);
		//if(echo_byte != 0)
			parseKeyOff(k);
		return;
	}
	char k = scancodeToAscii(x);
	red = 1;
	//if(echo_byte != 0){
	parseKey(k);
	//}
}



void zero_IDT(IDTDescr * idt){
	idt->offset_1 = 0;
	idt->selector = 0;
	idt->zero = 0;
	idt->type_attr = 0;
	idt->offset_2 = 0;
}

void db0_fault(){
	printString("FAULT: DB0. Check ESP for faulting address,");
	while(1){
		
	}
}

void segment_fault(){
	printString("FAULT: SEGMENT NOT PRESENT. Check ESP for faulting segment.");
	while(1){
		
	}
}

void stack_fault(){
	printString("FAULT: STACK SEGMENT NOT PRESENT.");
	while(1){
		
	}
}

void overflow_fault(){
	printString("FAULT: OVERFLOW");
	while(1){
		
	}
}

void protection_fault(uint32_t addr){
	printString("FAULT: GENERIC PROTECTION\n");
	printUint32(addr);
	while(1){
		
	}
}



void fill_IDT_entry(uint16_t vector, uint32_t address, uint16_t trap){
	IDT[vector].offset_1 = (uint16_t) address;
	IDT[vector].offset_2 = (uint16_t) ((address) >> 16);
	IDT[vector].selector = 0x10;
	if(trap != 0){
		IDT[vector].type_attr = 0x8F;
	}else{
		IDT[vector].type_attr = 0x8E;
	}
}	

void create_IDT(){
	IDTPointer p;
	p.address = (uint32_t) IDT;
	p.length = sizeof(IDT) - 1;
	for(int i = 0; i < 256; i++){
		zero_IDT(&IDT[i]);
	}
	echo_byte = 0;
	uint32_t db0_addr = (uint32_t) &d_b_0_HNDLR;
	fill_IDT_entry(0x00, db0_addr, 0);
	fill_IDT_entry(0x4, (uint32_t) &overflow_fault_HNDLR, 0);
	uint32_t sgf_addr = (uint32_t) &segment_fault_HNDLR;
	fill_IDT_entry(0x0B, sgf_addr, 0);
	uint32_t stf_addr = (uint32_t) &stack_fault_HNDLR;
	fill_IDT_entry(0x09, stf_addr, 0);
	uint32_t irq_addr =(uint32_t) &irq_1;
	fill_IDT_entry(0x0D, (uint32_t) &protection_fault_HNDLR, 0);
	fill_IDT_entry(0x20, (uint32_t) &PIC_timer, 1);
	fill_IDT_entry(0x21, irq_addr, 1);
	uint32_t dfh_addr = (uint32_t) &double_fault_HNDLR;
	fill_IDT_entry(0x08, dfh_addr, 0);
	uint32_t floppy_addr = (uint32_t) &floppy_HNDLR;
	fill_IDT_entry(0x26, floppy_addr, 0);
	uint32_t rtc_addr = (uint32_t) &rtc_HNDLR;
	fill_IDT_entry(0x28, rtc_addr, 0);
	uint32_t x49_addr = (uint32_t) &int49_HNDLR;
	fill_IDT_entry(0x49, x49_addr, 0);
	load_IDT((uint32_t) &p);
	init_keyboard();
	

	outb(PIC1_DATA, 0b10111001);
	outb(PIC2_DATA, 0xFE);
}

void disable_echo(){
	echo_byte = 0;
}

void enable_echo(){
	echo_byte = 1;
}

uint32_t handle_int_49(uint32_t id){
	switch(id){
		//video
		case PRINTCHARACTERATPOS_ID:
			return (uint32_t) &printCharacterAtPos;
		case CLEARSCREEN_ID:
			return (uint32_t) &clearScreen;
		case PRINTTAB_ID:
			return (uint32_t) &printTab;
		case NEWLINE_ID:
			return (uint32_t) &newLine;
		case DELETECURRENTCHAR_ID:
			return (uint32_t) &deleteCurrentChar;
		case PRINTCHARACTER_ID:
			return (uint32_t) &printCharacter;
		case PRINTSTRING_ID:
			return (uint32_t) &printString;
		//keyboard
		case BUFF_GETCH_ID:
			return (uint32_t) &buff_getCh;
		case BUFF_GETCHARACTER_ID:
			return (uint32_t) &buff_getCharacter;
		case BUFF_GETSTRING_ID:
			return (uint32_t) &buff_getString;
		case BUFF_GETSTRINGTRM_ID:
			return (uint32_t) &buff_getStringTrm;
		case BUFF_CLEAN_ID:
			return (uint32_t) &buff_clean;
		//string
		case STRLEN_ID:
			return (uint32_t) &strlen;
		case STRCMP_ID:
			return (uint32_t) &strcmp;
		case STRCAT_ID:
			return (uint32_t) &strcat;
		case STRCAT_N_ID:
			return (uint32_t) &strcat_n;
		case STRCAT_NN_ID:
			return (uint32_t) &strcat_nn;
		case STRPOS_ID:
			return (uint32_t) &strpos;
		case STRPOS_S_ID:
			return (uint32_t) &strpos_s;
		case SUBSTR_ID:
			return (uint32_t) &substr;
		case STRADD_ID:
			return (uint32_t) &stradd;
		case STRPOS_R_ID:
			return (uint32_t) &strpos_r;
		case STRPOS_RS_ID:
			return (uint32_t) &strpos_rs;

		//FAT
		case INITIALIZEFAT_ID:
			return (uint32_t) &initializeFAT;
		case LISTFILESFAT_ID:
			return (uint32_t) &listFilesFAT;
		case LOADFILEFROMCLUSTER_ID:
			return (uint32_t) &loadFileFromCluster;
		case RES_DEFAULTFAT_ID:
			return (uint32_t) &fa_impl;
	}
	return 0;
}