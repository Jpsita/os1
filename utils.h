#ifndef __JPS_UTILS_H
#define __JPS_UTILS_H
char inb(int a);
char outb(int port, int val);
void waitLoop();
void keybSp();

void io_wait();

#endif