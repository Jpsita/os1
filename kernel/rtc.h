#ifndef __JPS_RTC_H
#define __JPS_RTC_H

#define RTC_REG_A		0x0A
#define RTC_REG_B		0x0B
#define RTC_REG_C		0x0C
#define RTC_REG_D		0x0D

#define RTC_SELECT_PORT	0x70
#define RTC_DATA_PORT	0x71

#define RTC_DISABLE_NMI	0x80
#define RTC_ENABLE_IRQ8	0x40

void handle_irq_8();

unsigned long long int getTime();

void sleep(unsigned int seconds);
void sleepMs(unsigned long int ms);
void init_RTC();

#endif