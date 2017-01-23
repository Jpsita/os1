#include "floppy.h"
#include "utils.h"
#include "video.h"
#include "interrupt.h"

uint8_t floppyBuffer[DMA_MAX_TRANSFER_SIZE];

char handled_irq = 0;

void lba_2_chs(uint32_t lba, uint16_t* cyl, uint16_t* head, uint16_t* sector)
{
    *cyl    = lba / (2 * FLOPPY_SPT);
    *head   = ((lba % (2 * FLOPPY_SPT)) / FLOPPY_SPT);
    *sector = ((lba % (2 * FLOPPY_SPT)) % FLOPPY_SPT + 1);
}

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
/*
Check for floppy controller version (0x90 for controller 82077AA)
*/
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

void floppy_waitForNextParam(){
	uint8_t msr;
	do{
		msr = inb(FLOPPY_MAIN_STATUS_REG_IO);
	}while(msr & FLOPPY_MSR_RQM_BIT == 0 && msr & FLOPPY_MSR_DIO_BIT != 0);
}

/*
Sends configure command with default parameters
*/
void send_configure_command(){
	uint8_t msr = inb(FLOPPY_MAIN_STATUS_REG_IO);
	while(msr & 0xC0 == 0x80){
		msr = inb(FLOPPY_MAIN_STATUS_REG_IO);
	}
	outb(FLOPPY_DATA_FIFO_IO, FLOPPY_CMD_CONFIGURE);
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, 0);
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, 7 | FLOPPY_CMD_CONFIGURE_PRM_IS | FLOPPY_CMD_CONFIGURE_PRM_PMO );
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, 0);
}
/*
Locks FIFO and Treshold values from being changed with controller reset
*/
void send_lock_command(){
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, FLOPPY_CMD_LOCK | FLOPPY_CMD_OPTION_MT);
	floppy_waitForNextParam();
}

void floppy_reset(){
	outb(FLOPPY_DATARATE_SELECT_REG_IO, 0x80);
	while(handled_irq == 0){

	}
	handled_irq = 0;
}

void send_specify_command(){
	outb(FLOPPY_DATA_FIFO_IO, FLOPPY_CMD_SPECIFY);
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, FLOPPY_CMD_SPECIFY_DEFAULT_SRT << 4 | FLOPPY_CMD_SPECIFY_DEFAULT_HUT);
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, FLOPPY_CMD_SPECIFY_DEFAULT_HLT << 1 );
	floppy_waitForNextParam();
}

void floppy_select_drive(uint32_t drive){
	if(drive > 4){
		return;
	}
	send_specify_command();
	outb(FLOPPY_DIGITAL_OUTPUT_REG_IO, (FLOPPY_DOR_DSEL_0 + drive) | FLOPPY_DOR_IRQ | FLOPPY_DOR_RESET);
}

void floppy_init(){
	uint32_t floppy_addr = (uint32_t) floppyBuffer;
	init_floppy_DMA((uint16_t) floppy_addr, DMA_MAX_TRANSFER_SIZE - 1); //-1 is required by DMA
	
	check_floppy_version();

	send_configure_command();

	send_lock_command();

	floppy_reset();

	floppy_select_drive(0);
}

void floppy_startMotor(uint32_t motorId){

}

void floppy_stopMotor(uint32_t motorId){

}

void floppy_read(uint32_t lba, uint16_t bytecount, void* targetAddress){
	floppy_startMotor(0);
	
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, FLOPPY_CMD_READ_DATA);
	floppy_waitForNextParam();
	
	floppy_stopMotor(0);

	
}

void handle_irq_6(){
	handled_irq = 1;
	PIC_sendEOI(0x06);
}

