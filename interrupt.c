#include "interrupt.h"
#include "utils.h"
#include "video.h"

IDTDescr IDT[33];

void PIC_sendEOI(unsigned char IRQ){
	if(IRQ >= 8){
		outb(PIC2_COMMAND, PIC_EOI);
	}
	outb(PIC1_COMMAND, PIC_EOI);
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
	printCharacter('I');
}

void zero_IDT(IDTDescr * idt){
	idt->offset_1 = 0;
	idt->selector = 0;
	idt->zero = 0;
	idt->type_attr = 0;
	idt->offset_2 = 0;
}

void create_IDT(){
	IDTPointer p;
	p.address = (uint32_t) IDT;
	p.length = sizeof(IDT);
	zero_IDT(&IDT[0]);
	zero_IDT(&IDT[1]);
	zero_IDT(&IDT[2]);
	zero_IDT(&IDT[3]);
	zero_IDT(&IDT[4]);
	zero_IDT(&IDT[5]);
	zero_IDT(&IDT[6]);
	zero_IDT(&IDT[7]);
	zero_IDT(&IDT[8]);
	zero_IDT(&IDT[9]);
	zero_IDT(&IDT[10]);
	zero_IDT(&IDT[11]);
	zero_IDT(&IDT[12]);
	zero_IDT(&IDT[13]);
	zero_IDT(&IDT[14]);
	zero_IDT(&IDT[15]);
	zero_IDT(&IDT[16]);
	zero_IDT(&IDT[17]);
	zero_IDT(&IDT[18]);
	zero_IDT(&IDT[19]);
	zero_IDT(&IDT[20]);
	zero_IDT(&IDT[21]);
	zero_IDT(&IDT[22]);
	zero_IDT(&IDT[23]);
	zero_IDT(&IDT[24]);
	zero_IDT(&IDT[25]);
	zero_IDT(&IDT[26]);
	zero_IDT(&IDT[27]);
	zero_IDT(&IDT[28]);
	zero_IDT(&IDT[29]);
	zero_IDT(&IDT[30]);
	zero_IDT(&IDT[31]);
	uint32_t irq_addr =(uint32_t) &irq_1;
	IDT[32].offset_1 = (uint16_t) irq_addr;
	IDT[32].offset_2 = (uint16_t) ((irq_addr) >> 16);
	IDT[32].zero = 0;
	IDT[32].selector = 0b0000100000000000;
	IDT[32].type_attr = 0b1110;
	load_IDT((uint32_t) &IDT[0]);
}