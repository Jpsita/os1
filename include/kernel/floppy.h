#ifndef __JPS_FLOPPY_H
#define __JPS_FLOPPY_H
#include "main.h"
#include <common/common_floppy.h>

void init_floppy_DMA(uint32_t addr, uint16_t count);

void init_DMA_floppy_write(uint16_t count);

void init_DMA_floppy_read(uint16_t count);

void floppy_read(uint32_t lba, uint16_t byteCount, uint8_t* outAddr);
void handle_irq_6();
void lba_2_chs(uint32_t lba, uint16_t* cyl, uint16_t* head, uint16_t* sector);
void floppy_init();

#endif