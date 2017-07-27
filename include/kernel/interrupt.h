#ifndef __JPS_INTERRUPT_H
#define __JPS_INTERRUPT_H
#include "main.h"
#include <common/common_interrupt.h>

void PIC_sendEOI(unsigned char irq);

void PIC_remap(int offset1, int offset2);

void create_IDT();
void c_dbl_flt();


extern IDTDescr IDT[];

void zero_IDT(IDTDescr * descr);


void disable_echo();
void enable_echo();

extern char red; 
extern char echo_byte;

#endif