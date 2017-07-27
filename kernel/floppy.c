#include "kernel/floppy.h"
#include "kernel/utils.h"
#include "kernel/video.h"
#include "kernel/interrupt.h"
#include "kernel/rtc.h"
#include "kernel/main.h"

uint8_t floppyBuffer[DMA_MAX_TRANSFER_SIZE];

char handled_irq = 0;

void lba_2_chs(uint32_t lba, uint16_t* cyl, uint16_t* head, uint16_t* sector)
{
    *cyl    = lba / (2 * FLOPPY_SPT);

    //*head   = ((lba % (2 * FLOPPY_SPT)) / FLOPPY_SPT);
    *head = ((lba / FLOPPY_SPT) % 2);

	//*sector = ((lba % (2 * FLOPPY_SPT)) % FLOPPY_SPT + 1);
	*sector = ((lba % FLOPPY_SPT) + 1);

}

/*
	Initialise DMA channel 2 for floppy drive
*/
void init_floppy_DMA(uint32_t addr, uint16_t count){
	uint8_t lowAddr = (uint8_t) addr;
	uint8_t highAddr = (uint8_t) (addr >> 8);
	uint8_t exAddr = (uint8_t) ((addr >> 16));

	outb(DMA_1_SING_MASK_REG_IO, 0x06); //mask channels 2 and 0
	outb(DMA_1_FF_REG_IO, 0xFF); //reset master flip flop
	outb(DMA_START_ADDR_REG_IO_2, (uint8_t) addr); //low byte of address
	outb(DMA_START_ADDR_REG_IO_2, (uint8_t) (addr >> 8)); //high byte of address
	outb(DMA_1_FF_REG_IO, 0xFF); //reset master flip flop
	outb(DMA_COUNT_REG_IO_2, (uint8_t) count); //low byte of count
	outb(DMA_COUNT_REG_IO_2, (uint8_t) (count >> 8)); //high byte of count
	outb(DMA_CH_2_PAGE_ADDR_IO, (addr >> 16)); //page register to 0
	outb(DMA_1_SING_MASK_REG_IO, 0x02); //unmask channel 2
	outb(DMA_1_MULTI_MASK_REG_IO, 0x03);
}

//Sets DMA channel 2 to read from memory and write to floppy
void init_DMA_floppy_write(uint16_t count){
	outb(DMA_1_SING_MASK_REG_IO, 0x06); //mask channels 2 and 0	
	outb(DMA_1_FF_REG_IO, 0xFF); //reset master flip flop
	outb(DMA_COUNT_REG_IO_2, (uint8_t) count); //low byte of count
	outb(DMA_COUNT_REG_IO_2, (uint8_t) (count >> 8)); //high byte of count
	outb(DMA_1_MODE_REG_IO, DMA_MOD_REG_MOD_BLOCK | DMA_MOD_REG_BIT_AUTO | DMA_MOD_REG_TRA_READ_MEM | DMA_MOD_REG_SEL_2);
	outb(DMA_1_SING_MASK_REG_IO, 0x02); //mask channel 0
}

//Sets DMA channel 2 to read from floppy and write to memory
void init_DMA_floppy_read(uint16_t count){
	outb(DMA_1_SING_MASK_REG_IO, 0x06); //mask channels 2 and 0
	outb(DMA_1_FF_REG_IO, 0xFF); //reset master flip flop
	outb(DMA_COUNT_REG_IO_2, (uint8_t) count); //low byte of count
	outb(DMA_COUNT_REG_IO_2, (uint8_t) (count >> 8)); //high byte of count
	outb(DMA_1_MODE_REG_IO, DMA_MOD_REG_MOD_BLOCK | DMA_MOD_REG_BIT_AUTO | DMA_MOD_REG_TRA_WRITE_MEM | DMA_MOD_REG_SEL_2);
	outb(DMA_1_SING_MASK_REG_IO, 0x02); //mask channel 0
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
		printUint32(val);
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
	outb(FLOPPY_DATA_FIFO_IO, 7 | FLOPPY_CMD_CONFIGURE_PRM_IS | FLOPPY_CMD_CONFIGURE_PRM_PMO);
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
	inb(FLOPPY_DATA_FIFO_IO);
	floppy_waitForNextParam();
}

void floppy_reset(){
	uint8_t dor = inb(FLOPPY_DIGITAL_OUTPUT_REG_IO);
	dor |= FLOPPY_DOR_IRQ;
	outb(FLOPPY_DIGITAL_OUTPUT_REG_IO, 0x00);
	sleepMs(10);
	outb(FLOPPY_DIGITAL_OUTPUT_REG_IO,dor);
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

void floppy_sense_interrupt(){
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, FLOPPY_CMD_SENSE_INTERRUPT);
	floppy_waitForNextParam();
	inb(FLOPPY_DATA_FIFO_IO);
	floppy_waitForNextParam();
	inb(FLOPPY_DATA_FIFO_IO);
	floppy_waitForNextParam();
}

void floppy_recalibrate(uint32_t disk){
	handled_irq = 0;
	uint8_t msr = inb(FLOPPY_MAIN_STATUS_REG_IO);
	while(msr & 0xC0 == 0x80){
		msr = inb(FLOPPY_MAIN_STATUS_REG_IO);
	}
	outb(FLOPPY_DATA_FIFO_IO, FLOPPY_CMD_RECALIBRATE);
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, disk);
	floppy_waitForNextParam();
	while(handled_irq == 0){
		sleepMs(500);
	}
	handled_irq = 0;

	floppy_sense_interrupt();
}

void floppy_init(){
	uint32_t floppy_addr = (uint32_t) floppyBuffer;
	init_floppy_DMA(floppy_addr, DMA_MAX_TRANSFER_SIZE - 1); //-1 is required by DMA
	
	check_floppy_version();

	floppy_reset();

	send_configure_command();

	send_lock_command();


	floppy_select_drive(0);

	floppy_recalibrate(0);
}

void floppy_startMotor(uint32_t motorId){
	char dor = inb(FLOPPY_DIGITAL_OUTPUT_REG_IO);
	switch(motorId){
		case 0:
		dor |= FLOPPY_DOR_MOTA;
		break;
		case 1:
		dor |= FLOPPY_DOR_MOTB;
		break;
		case 2:
		dor |= FLOPPY_DOR_MOTC;
		break;
		case 3:
		dor |= FLOPPY_DOR_MOTD;
		break;
	}
	outb(FLOPPY_DIGITAL_OUTPUT_REG_IO, dor);
}

void floppy_stopMotor(uint32_t motorId){
	char dor = inb(FLOPPY_DIGITAL_OUTPUT_REG_IO);
	switch(motorId){
		case 0:
		dor &= ~FLOPPY_DOR_MOTA;
		break;
		case 1:
		dor &= ~FLOPPY_DOR_MOTB;
		break;
		case 2:
		dor &= ~FLOPPY_DOR_MOTC;
		break;
		case 3:
		dor &= ~FLOPPY_DOR_MOTD;
		break;
	}
	outb(FLOPPY_DIGITAL_OUTPUT_REG_IO, dor);
}

void floppy_read(uint32_t lba, uint16_t bytecount, uint8_t* addr){
	uint16_t eff = (((bytecount - (bytecount % 512)) + ((bytecount % 512 == 0) ? 0 : 1) * (512))) -1;
	init_DMA_floppy_read(eff) ;
	floppy_startMotor(0);
	sleepMs(1500);
	handled_irq = 0;
	uint16_t cyl;
	uint16_t head;
	uint16_t sec;

	uint16_t sector_count = (bytecount / 512) + ((bytecount % 512 == 0)? 0: 1);

	lba_2_chs(lba, &cyl, &head, &sec);
	
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, FLOPPY_CMD_OPTION_MT | FLOPPY_CMD_OPTION_MF | FLOPPY_CMD_READ_DATA);
	floppy_waitForNextParam();

	outb(FLOPPY_DATA_FIFO_IO, head << 2 | 0);
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, cyl);
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, head);
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, sec);
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, 2);
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, sector_count);
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, FLOPPY_CMD_RW_PARAM_GAP1);
	floppy_waitForNextParam();
	outb(FLOPPY_DATA_FIFO_IO, 0xFF);
	floppy_waitForNextParam();
	while(handled_irq == 0){
		sleepMs(500);
	}
	handled_irq = 0;
	unsigned char b;
	unsigned char st0;
	unsigned char st1;
	unsigned char st2;

	unsigned char eCyl;
	unsigned char eHead;
	unsigned char eSector;

	unsigned char eRes;
	do{
		b = inb(FLOPPY_MAIN_STATUS_REG_IO);
	}while((b & FLOPPY_MSR_RQM_BIT) == 0);
	st0 = inb(FLOPPY_DATA_FIFO_IO);
	do{
		b = inb(FLOPPY_MAIN_STATUS_REG_IO);
	}while((b & 0x50) != 0x50);

	st1 = inb(FLOPPY_DATA_FIFO_IO);
	do{
		b = inb(FLOPPY_MAIN_STATUS_REG_IO);
	}while((b & 0x50) != 0x50);

	st2 = inb(FLOPPY_DATA_FIFO_IO);
	do{
		b = inb(FLOPPY_MAIN_STATUS_REG_IO);
	}while((b & 0x50) != 0x50);

	eCyl = inb(FLOPPY_DATA_FIFO_IO);
	do{
		b = inb(FLOPPY_MAIN_STATUS_REG_IO);
	}while((b & 0x50) != 0x50);

	eHead = inb(FLOPPY_DATA_FIFO_IO);
	do{
		b = inb(FLOPPY_MAIN_STATUS_REG_IO);
	}while((b & 0x50) != 0x50);

	eSector = inb(FLOPPY_DATA_FIFO_IO);
	do{
		b = inb(FLOPPY_MAIN_STATUS_REG_IO);
	}while((b & 0x50) != 0x50);

	eRes = inb(FLOPPY_DATA_FIFO_IO);


	if(eRes != 2){
		printString("ERROR ON READING FLOPPY\n");
		while(1){
			
		}
	}

	floppy_stopMotor(0);
	sleepMs(1000);

	for(int i = 0; i < bytecount; i++){
		addr[i] = floppyBuffer[i];
	}
	ZeroMemory(floppyBuffer, sizeof(floppyBuffer));
}

void handle_irq_6(){
	handled_irq = 1;
	PIC_sendEOI(0x06);
}

