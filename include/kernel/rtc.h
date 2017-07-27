#ifndef __JPS_RTC_H
#define __JPS_RTC_H

#include <common/common_rtc.h>

void handle_irq_8();

unsigned long long int getTime();

void sleep(unsigned int seconds);
void sleepMs(unsigned long int ms);
void init_RTC();

#endif