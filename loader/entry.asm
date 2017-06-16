USE16

;DEFINES

NULL					EQU	0
FLOPPY_144_SECTORS_PER_TRACK	EQU	18
PRINT_FUNC_AH			EQU	0Eh
PRINT_FUNC_INT			EQU	10h
MAIN_FLOPPY_DRIVE			EQU	0
FLOPPY_READ_FUNC_AH		EQU	02h
FLOPPY_READ_FUNC_INT		EQU	13h

SECTION .text
global _main
_main:
	JMP startup		;moving to actual startup code
;Common FAT header

tst			db		'OS1.0   '	;OEM Header
bps			dw		0x200		;bytes per sector
spc			db		0x01		;sectors per cluster
rsv			dw		0x01		;reserved logical sectors
fats		db		0x02		;number of FATs
dirs		dw		240		;Max number of root directories
secs		dw		2880		;total logical sectors
tp			db		0xF0		;media descriptor
spft		dw		0x09		;logical sectors per FAT
spt			dw		0x12		;sectors per track
hds			dw		0x02		;number of heads
hdn			dd		0x00		;count of hidden sectors

;extended FAT12 - 16 floppy header

lrg			dd		0x00			;extra sectors(?)
drn			db		0x00			;drive number
entrycx		db		0x00			;reserved
sig			db		0x28			;extended boot signature
id			dd		0x49494949		;volume id
lbl			db		'OS1INSTALL '	;volume label
tpx			db		'FAT12   '		;File system label


;--------FUN--------------
; FUNCTION read: reads N sectors from floppy and puts them at memory address X
; Parameters:
; 0: (+2): Base (Starting Sector, 32 bit)
; 1: (+6): N (Number of sectors to read, 16 bit)
; 2: (+8): Buff (Pointer to memory buffer, 16 bit)
; 3: (+10): Buff_high (Segment pointer to memory buffer, 16bit)
; Return: none
; Registers: modified
; Temp values: modified
;------------------------

read:
	MOV BP, SP
	MOV EAX, [BP + 2]
	CALL eff_2_chs		;converting effective address to CHS
;----------------------
;INT 13h AH 02h
;AL: Sectors to read
;	CX:
;sector:   ________ __543210
;cylinder: 76543210 98______
;DH: head
;DL: drive
;ES:BX buffer
;----------------------
eff_ret:
	;MOV CH, BL				;put the lower part of the cylinder in ch
	;SHL BH, 6				;shift left cylinder by 6
	;OR CL, BH				;put in the 2 high bits of CL the remeining number of cylinder
	;MOV DH, DL				;put the HEAD value in DH
	;MOV DL, MAIN_FLOPPY_DRIVE		;put 0 (disc number in DL)
	;MOV AX, [BP + 6]			;get N
	;MOV AH, FLOPPY_READ_FUNC_AH		;set the instruction value
	;MOV BX, ES				;get ES
	;PUSH BX					;and save it
	;MOV BX, [BP + 10]			;load effective address high
	;MOV ES, BX
	;MOV BX, [DS:BP + 8]
	MOV AX, [BP + 6]			;load N
	MOV AH, 02h					;set INT function
	MOV CL, [sector]			;load sector
	MOV DX, [track]				;load TRACK(cylinder)
	MOV CH, DL					;set low part of TRACK
	SHL DH, 6					;get only 2 high bits of TRACK
	OR CL, DH					;add 2 high bits of TRACK
	MOV DL, MAIN_FLOPPY_DRIVE	;set MAIN_FLOPPY_DRIVE
	MOV DH, [head]				;load HEAD
	MOV BX, [BP + 8]			;load BUFF
	PUSH ES						;save ES
	MOV ES, [DS:BP + 12]		;load BUFF_HIGH
eff_read:
	INT FLOPPY_READ_FUNC_INT	;launch interrupt
	POP ES						;restore ES
eff_read_ret:
	RET

;--------END--------------

;--------FUN--------------
; FUNCTION eff_2_chs: converts a lba number to chs
; Parameters:
; EAX (32 bit): N (The LBA sector number)
; Return:
; track, head and sector in RAM
; Registers: modified
; Temp values: MODIFIED
;------------------------

eff_2_chs:
	;cylinder
	PUSH EBP					;saving ebp
	MOV EBP, EAX				;using ebp as backup LBA
	XOR EDX, EDX				;zeroing EDX
	MOV EBX, 2 * FLOPPY_144_SECTORS_PER_TRACK	;by EBX (36)
	DIV EBX						;= (in EAX, leftover in EDX)
	MOV [track], AX 			;save result
	;head
	XOR EDX, EDX				;zeroing leftover
	MOV EAX, EBP				;reloading LBA
	MOV EBX, FLOPPY_144_SECTORS_PER_TRACK ; divide EAX by EBX(18)
	DIV EBX						;= (in EAX, leftover in EDX)
	XOR EDX, EDX				;zeroing leftover
	MOV EBX, 2					;number of HEADS
	DIV EBX						;= divide EAX by EBX(2)
	MOV [head], DL				;save effective HEAD
	;sector
	XOR EDX, EDX				;zeroing leftover
	MOV EAX, EBP				;loading LBA
	MOV EBX, FLOPPY_144_SECTORS_PER_TRACK ;divide EAX by EBX(18)
	DIV EBX						;= (in EAX, leftover in EDX)
	MOV ECX, EDX				;put sector in ECX
	INC ECX						;increase (sectors are counted from 1)
	MOV [sector], CL			;saving sector
	POP EBP
eff_2_ret:
	RET

;-------END--------------


;-------FUN--------------
;WaitLoop16: waits for the keyboard to be ready
;WaitLoop16:	in     al, 64h       ; Read Status byte
;		and    al, 01b       ; Test OBF flag (Status<0>)
;		jnz    WaitLoop16    ; Wait for OBF = 0
;		ret
;-------END--------------


;-------FUN--------------
;ReadFATEntry: reads a single FAT12 12bit value from the given address
;Params:
;0 (+2) (16 bit) address: Address of the value to read.
;Return:
;AX: Value
;-----------------------


startup:


	PUSHA
	MOV DX, 0x0	;High buffer
	PUSH DX
	MOV CX, 0x8e00	;low buffer
	PUSH CX
	MOV BX, 0x17	;number of sectors to read
	PUSH BX
	MOV EAX, 0x23	;starting sector
	PUSH EAX
rd2:
	CALL read
	POPA


rdOver:
	MOV AX, 2h
	INT 10h

to_protected:
	MOV EAX, gdt
	MOV [gdt_offset], EAX
	MOV EAX, gdt_end
	SUB EAX, gdt
	MOV [gdt_size], AX
	CLI
	LGDT [gdt_descriptor]
	MOV EAX, cr0
	OR EAX, 1
	MOV cr0, EAX
	JMP 10h:at_32
at_32:
	USE32
	MOV AX, 0x8
	MOV DS, AX
	MOV ES, AX
	MOV SS, AX
	jmp 0x8e00


SECTION .data

track 		dw		0
head		db		0
sector		db		0

gdt_descriptor:
gdt_size:	dw		0
gdt_offset:	dd		0

;-------END--------------

;-------GDT--------------

gdt:
null_seg	dq		0
data_seg	dq		58430246923337727
code_seg	dq		54216918365634660
gdt_end:
;-------END--------------
