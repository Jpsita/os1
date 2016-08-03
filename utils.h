#ifndef __JPS_UTILS_H
#define __JPS_UTILS_H
#include "main.h"
char inb(int a);
char outb(int port, int val);
void waitLoop();
void keybSp();

void load_IDT(uint32_t IDT);

void io_wait();
void irq_1();

#endif