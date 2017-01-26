#include "interrupt.h"
#include "keyboard.h"
#include "utils.h"
#include "video.h"

IDTDescr IDT[256];
char echo_byte;

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

void print_hex(uint32_t val){
	static char str[9];
	str[8] = 0;
	for(int i = 0; i  < 8;  i++){
		uint32_t cv = val % 0x10;
		val = val >> 4;
		switch(cv){
		case 0x00:
			str[7-i] = '0';
			break;
		case 0x01:
			str[7-i] = '1';
			break;
		case 0x02:
			str[7-i] = '2';
			break;
		case 0x03:
			str[7-i] = '3';
			break;
		case 0x04:
			str[7-i] = '4';
			break;
		case 0x05:
			str[7-i] = '5';
			break;
		case 0x06:
			str[7-i] = '6';
			break;
		case 0x07:
			str[7-i] = '7';
			break;
		case 0x08:
			str[7-i] = '8';
			break;
		case 0x09:
			str[7-i] = '9';
			break;
		case 0x0A:
			str[7-i] = 'A';
			break;
		case 0x0B:
			str[7-i] = 'B';
			break;
		case 0x0C:
			str[7-i] = 'C';
			break;
		case 0x0D:
			str[7-i] = 'D';
			break;
		case 0x0E:
			str[7-i] = 'E';
			break;
		case 0x0F:
			str[7-i] = 'F';
			break;
		default:
			str[7-i] = '?';
		}
	}
	printString(str);
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
		if(echo_byte != 0)
			parseKeyOff(k);
		return;
	}
	//if(x == 0xE0){
	//x = inb(0x60);
	//return;
	//}
	//print_hex(x);
	//printCharacter(' ');
	char k = scancodeToAscii(x);
	if(echo_byte != 0)
		parseKey(k);
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

void protection_fault(){
	printString("FAULT: GENERIC PROTECTION");
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