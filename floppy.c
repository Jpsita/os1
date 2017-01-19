#include "floppy.h"
#include "utils.h"
#include "video.h"
#include "interrupt.h"

uint8_t floppyBuffer[DMA_MAX_TRANSFER_SIZE];

char handled_irq = 0;
/*
	Initialise DMA channel 2 for floppy drive
*/
void init_floppy_DMA(uint16_t addr, uint16_t count){
	outb(DMA_1_SING_MASK_REG_IO, 0x06); //mask channels 2 and 0
	outb(DMA_1_FF_REG_IO, 0xFF); //reset master flip flop
	outb(DMA_START_ADDR_REG_IO_2, (uint8_t) addr); //low byte of address
	outb(DMA_START_ADDR_REG_IO_2, (uint8_t) (addr >> 8)); //high byte of address
	outb(DMA_1_FF_REG_IO, 0xFF); //reset master flip flop
	outb(DMA_COUNT_REG_IO_2, (uint8_t) count); //low byte of count
	outb(DMA_COUNT_REG_IO_2, (uint8_t) (count >> 8)); //high byte of count
	outb(DMA_CH_2_PAGE_ADDR_IO, 0); //page register to 0
	outb(DMA_1_SING_MASK_REG_IO, 0x02); //unmask channel 2
}

//Sets DMA channel 2 to read from memory and write to floppy
void init_DMA_floppy_write(){
	outb(DMA_1_SING_MASK_REG_IO, 0x06); //mask channels 2 and 0
	outb(DMA_1_MODE_REG_IO, DMA_MOD_REG_MOD_SINGLE | DMA_MOD_REG_BIT_AUTO | DMA_MOD_REG_TRA_READ_MEM | DMA_MOD_REG_SEL_2);
	outb(DMA_1_SING_MASK_REG_IO, 0x02); //mansk channel 0
}

//Sets DMA channel 2 to read from floppy and write to memory
void init_DMA_floppy_read(){
	outb(DMA_1_SING_MASK_REG_IO, 0x06); //mask channels 2 and 0
	outb(DMA_1_MODE_REG_IO, DMA_MOD_REG_MOD_SINGLE | DMA_MOD_REG_BIT_AUTO | DMA_MOD_REG_TRA_WRITE_MEM | DMA_MOD_REG_SEL_2);
	outb(DMA_1_SING_MASK_REG_IO, 0x02); //mansk channel 0
}

void check_floppy_version(){
	uint8_t mst = inb(FLOPPY_MAIN_STATUS_REG_IO);
	outb(FLOPPY_DATA_FIFO_IO, FLOPPY_CMD_VERSION);
	do{
		mst = inb(FLOPPY_MAIN_STATUS_REG_IO);
	}while(mst & FLOPPY_MSR_RQM_BIT == 0);
	uint8_t val = inb(FLOPPY_DATA_FIFO_IO);
	if(val != 0x90){
		printString("ERROR 2. Value: ");
		print_hex(val);
		while(1){
		}
	}

}

void send_configure_command(){
	uint8_t msr = inb(FLOPPY_MAIN_STATUS_REG_IO);
	while(msr & 0xC0 == 0x80){
		msr = inb(FLOPPY_MAIN_STATUS_REG_IO);
	}
	outb(FLOPPY_DATA_FIFO_IO, FLOPPY_CMD_CONFIGURE);
	printString("DEBUG 1");
}

void floppy_init(){
	uint32_t floppy_addr = (uint32_t) floppyBuffer;
	init_floppy_DMA((uint16_t) floppy_addr, DMA_MAX_TRANSFER_SIZE - 1); //-1 is required by DMA
	
	check_floppy_version();
	

}

void handle_irq_6(){
	printString("GOT IRQ 6\n");
	handled_irq = 1;
}

void lba_2_chs(uint32_t lba, uint16_t* cyl, uint16_t* head, uint16_t* sector)
{
    *cyl    = lba / (2 * FLOPPY_SPT);
    *head   = ((lba % (2 * FLOPPY_SPT)) / FLOPPY_SPT);
    *sector = ((lba % (2 * FLOPPY_SPT)) % FLOPPY_SPT + 1);
}
