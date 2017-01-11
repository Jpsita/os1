#ifndef __JPS_UTILS_H
#define __JPS_UTILS_H
#include "main.h"
char inb(int a);
char outb(int port, int val);
void waitLoop();
void keybSp();

void load_IDT(uint32_t IDT);

void double_fault_HNDLR();
void fault_HNDLR();
void io_wait();
void irq_1();
void d_b_0_HNDLR();
void segment_fault_HNDLR();
void stack_fault_HNDLR();
void overflow_fault_HNDLR();
void protection_fault_HNDLR();

#endif