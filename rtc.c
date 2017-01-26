#include "rtc.h"
#include "utils.h"
#include "interrupt.h"

 void NMI_enable(void)
 {
    outb(0x70, inb(0x70)&0x7F);
 }
 
 void NMI_disable(void)
 {
    outb(0x70, inb(0x70)|0x80);
 }
unsigned long long int time = 0;

void handle_irq_8(){
	time ++;
	outb(RTC_SELECT_PORT, RTC_REG_C);
	inb(RTC_DATA_PORT);
	PIC_sendEOI(0x08);
}

unsigned long long int getTime(){
	return (time / 1024);
}

void sleepMs(unsigned long int ms){
	uint64_t startTime = time;
	uint64_t ltime;
	do{
		ltime = time;
	}while(ltime - startTime < ms);
}

void sleep(unsigned int seconds){
	uint64_t startTime = getTime() ;
	uint64_t ltime;
	do{
		ltime = getTime();
	}while(ltime - startTime < seconds);
}

void init_RTC(){
	util_cli();
	outb(RTC_SELECT_PORT, RTC_DISABLE_NMI | RTC_REG_B);
	unsigned char prev = inb(RTC_DATA_PORT);
	outb(RTC_SELECT_PORT, RTC_DISABLE_NMI | RTC_REG_B);
	outb(RTC_DATA_PORT, prev | RTC_ENABLE_IRQ8);
	util_sti();
}