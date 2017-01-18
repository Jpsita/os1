#include "floppy.h"
#include "utils.h"
#include "video.h"
#include "main.h"

void init_floppy_DMA(uint16_t addr, uint16_t count){
	outb(DMA_1_SING_MASK_REG_IO, 0x06); //mask channels 2 and 0
	outb(DMA_1_FF_REG_IO, 0xFF); //reset master flip flop
	outb(DMA_START_ADDR_REG_IO_2, (uint8_t) addr); //low byte of address
	outb(DMA_START_ADDR_REG_IO_2, (uint8_t) (addr >> 8)); //high byte of address
	outb(DMA_1_FF_REG_IO, 0xFF); //reset master flip flop
	outb(DMA_COUNT_REG_IO_2, (uint8_t) count); //low byte of count
	outb(DMA_COUNT_REG_IO_2, (uint8_t) (count >> 8)); //high byte of count
	outb(DMA_CH_2_PAGE_ADDR_IO, 0); //page register to 0
	outb(DMA_1_SIN_MASK_REG_IO, 0x02); //unmask channel 2
}

