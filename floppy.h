#ifndef __JPS_FLOPPY_H
#define __JPS_FLOPPY_H
#include "main.h"

//DMA

#define DMA_START_ADDR_REG_IO_0		0x00
#define DMA_START_ADDR_REG_IO_4		0xC0
#define DMA_COUNT_REG_IO_0			0x01
#define DMA_COUNT_REG_IO_4			0xC2
#define DMA_START_ADDR_REG_IO_1		0x02
#define DMA_START_ADDR_REG_IO_5		0xC4
#define DMA_COUNT_REG_IO_1			0x03
#define DMA_COUNT_REG_IO_5			0xC6
#define DMA_START_ADDR_REG_IO_2		0x04
#define DMA_START_ADDR_REG_IO_6		0xC8
#define DMA_COUNT_REG_IO_2			0x05
#define DMA_COUNT_REG_IO_6			0xCA
#define DMA_START_ADDR_REG_IO_3		0x06
#define DMA_START_ADDR_REG_IO_7		0xCC
#define DMA_COUNT_REG_IO_3			0x07
#define DMA_COUNT_REG_IO_7			0xCE
#define DMA_1_STATUS_REG_IO			0x08
#define DMA_2_STATUS_REG_IO			0xD0
#define DMA_1_CMD_REG_IO				0x08
#define DMA_2_CMD_REG_IO				0xD0
#define DMA_1_REQ_REG_IO				0x09
#define DMA_2_REQ_REG_IO				0xD2
#define DMA_1_SING_MASK_REG_IO			0x0A
#define DMA_2_SING_MASK_REG_IO			0xD4
#define DMA_1_MODE_REG_IO				0x0B
#define DMA_2_MODE_REG_IO				0xD6
#define DMA_1_FF_REG_IO				0x0C
#define DMA_2_FF_REG_IO				0xD8
#define DMA_1_INTRM_REG_IO				0x0D
#define DMA_2_INTRM_REG_IO				0xDA
#define DMA_1_MASTER_RESET_REG_IO		0x0D
#define DMA_2_MASTER_RESET_REG_IO		0xDA
#define DMA_1_MASK_RESET_REG_IO		0x0E
#define DMA_2_MASK_RESET_REG_IO		0xDC
#define DMA_1_MULTI_MASK_REG_IO		0x0F
#define DMA_2_MULTI_MASK_REG_IO		0xDE

#define DMA_CH_0_PAGE_ADDR_IO			0x87
#define DMA_CH_1_PAGE_ADDR_IO			0x83
#define DMA_CH_2_PAGE_ADDR_IO			0x81
#define DMA_CH_3_PAGE_ADDR_IO			0x82
#define DMA_CH_4_PAGE_ADDR_IO			0x8F
#define DMA_CH_5_PAGE_ADDR_IO			0x8B
#define DMA_CH_6_PAGE_ADDR_IO			0x89
#define DMA_CH_7_PAGE_ADDR_IO			0x8A

#define DMA_MAX_TRANSFER_SIZE			0x10000

#define DMA_MOD_REG_BIT_MOD1            0b10000000
#define DMA_MOD_REG_BIT_MOD2            0b01000000
#define DMA_MOD_REG_BIT_DOWN            0b00100000
#define DMA_MOD_REG_BIT_AUTO            0b00010000
#define DMA_MOD_REG_BIT_TRA1            0b00001000
#define DMA_MOD_REG_BIT_TRA0            0b00000100
#define DMA_MOD_REG_BIT_SEL1            0b00000010
#define DMA_MOD_REG_BIT_SEL0            0b00000001

#define DMA_MOD_REG_MOD_ON_DEMAND       0b00000000
#define DMA_MOD_REG_MOD_SINGLE          0b01000000
#define DMA_MOD_REG_MOD_BLOCK           0b10000000
#define DMA_MOD_REG_MOD_CASCADE         0b11000000

#define DMA_MOD_REG_SEL_0               0b00000000
#define DMA_MOD_REG_SEL_1               0b00000001
#define DMA_MOD_REG_SEL_2               0b00000010
#define DMA_MOD_REG_SEL_3               0b00000011

#define DMA_MOD_REG_TRA_TEST            0b00000000
#define DMA_MOD_REG_TRA_WRITE_MEM       0b00000100
#define DMA_MOD_REG_TRA_READ_MEM        0b00001000
#define DMA_MOD_REG_TRA_INVLID          0b00001100

//FLOPPY

#define FLOPPY_HPC                      2
#define FLOPPY_SPT                      18
#define FLOPPY_CYL                      80

#define FLOPPY_STATUS_REG_A_IO          0x3F0 //Read Only
#define FLOPPY_STATUS_REG_B_IO          0x3F1 //Read Only
#define FLOPPY_DIGITAL_OUTPUT_REG_IO    0x3F2
#define FLOPPY_TAPE_DRIVE_REG_IO        0x3F3
#define FLOPPY_MAIN_STATUS_REG_IO       0x3F4 //Read Only
#define FLOPPY_DATARATE_SELECT_REG_IO   0x3F4 //Write Only
#define FLOPPY_DATA_FIFO_IO             0x3F5
#define FLOPPY_DIGITAL_INPUT_REG_IO     0x3F7 //Read Only
#define FLOPPY_CONF_CONTROL_REG_IO      0x3F7 //Write Only

void init_floppy_DMA(uint16_t addr, uint16_t count);

void init_DMA_floppy_write();

void init_DMA_floppy_read();

void lba_2_chs(uint32_t lba, uint16_t* cyl, uint16_t* head, uint16_t* sector);
void floppy_init();

#endif