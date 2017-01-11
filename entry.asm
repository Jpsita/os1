USE16

;DEFINES

NULL					EQU	0
FLOPPY_144_SECTORS_PER_TRACK 	EQU 	18
PRINT_FUNC_AH			EQU	0Eh
PRINT_FUNC_INT			EQU	10h
MAIN_FLOPPY_DRIVE			EQU	0
FLOPPY_READ_FUNC_AH		EQU	02h
FLOPPY_READ_FUNC_INT		EQU	13h

SECTION .text
extern entryc
global _main
_main:
	JMP startup		;moving to actual startup code

;--------FUN---------------
; FUNCTION print_int32: prints a 32 bit HEXADECIMAL number
; Paramaters;
; 0: (32 bit): number
; return: none
; registers: intact
;-------------------------- 

print_int32:
	MOV BP, SP		;
	PUSHA			;
	MOV EBX, [BP + 2]	
	MOV AH, PRINT_FUNC_AH
	MOV AL, '0'
	INT PRINT_FUNC_INT
	MOV AH, PRINT_FUNC_AH
	MOV AL, 'x'
	INT PRINT_FUNC_INT
	
	MOV CX, 8
print_int32_loop:
	MOV DL, 0
	SHL EBX, 1
	JNC print_int32_next_4
	ADD DL, 8
print_int32_next_4:
	SHL EBX, 1
	JNC print_int32_next_2
	ADD DL, 4
print_int32_next_2:
	SHL EBX, 1
	JNC print_int32_next_1
	ADD DL, 2
print_int32_next_1:
	SHL EBX, 1
	JNC print_int32_next_over
	ADD DL, 1
print_int32_next_over:
	CMP DL, 10
	JGE print_int32_moreThanTen
	MOV AL, '0'
	ADD AL, DL
	MOV AH, PRINT_FUNC_AH
	INT PRINT_FUNC_INT
	JMP print_int32_do_loop
print_int32_moreThanTen:
	MOV AL, 'A'
	SUB DL, 10
	ADD AL, DL
	MOV AH, PRINT_FUNC_AH
	INT PRINT_FUNC_INT
print_int32_do_loop:
	LOOP print_int32_loop
	POPA
	RET

;--------END--------------- 

;--------FUN---------------
; FUNCTION print: prints a string using bios functions
; parameters:
; 0 (16 bit, +2): string_addr: address of the string to print
; return: none
; registers: intact
;--------------------------

print:
	MOV BP, SP			;preserving stack??
	MOV SI, [BP + 2]		;loading string address
	PUSHA				;saving registers
print_loop:
	LODSB				;loading next character in al
	MOV AH, PRINT_FUNC_AH		;int 10h ah 0Eh: print character stored in AL
	CMP AL, NULL			;check is character in NULL
	JE print_exit			;if null, exit function
	INT PRINT_FUNC_INT		;else, print (call in 10h)
	JMP print_loop			;repeat for next character
print_exit:
	POPA				;loading registers
	RET				;returning

;--------END--------------

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
	MOV CH, BL				;put the lower part of the cylinder in ch
	SHL BH, 6				;rotate left cylinder by 6
	OR CL, BH				;put in the 2 high bits of CL the remeining number of cylinder
	MOV DH, DL				;put the HEAD value in DH
	MOV DL, MAIN_FLOPPY_DRIVE		;put 0 (disc number in DL)
	MOV AX, [BP + 6]			;get N
	MOV AH, FLOPPY_READ_FUNC_AH		;set the instruction value
	MOV BX, ES				;get ES
	PUSH BX					;and save it
	MOV BX, [BP + 10]			;load effective address high
	MOV ES, BX
	MOV BX, [DS:BP + 8]
eff_read:
	INT FLOPPY_READ_FUNC_INT
	POP BX
	MOV ES, BX
eff_read_ret:
	RET

;--------END--------------

;--------FUN--------------
; FUNCTION eff_2_chs: converts a element number to chs
; Parameters:
; EAX (32 bit): N (The effective sector number)
; Return:
; ECX: Sector (16 bit)
; EDX: Head (16 bit)
; EBX: Cylinder (16 bit)
; Registers: modified
; Temp values: NOT-MODIFIED
;------------------------

eff_2_chs:
	XOR EDX, EDX
	MOV EBX, 2 * FLOPPY_144_SECTORS_PER_TRACK	;by EBX (36)
	DIV EBX						;= (in EAX, leftover in EDX)
	PUSH EAX					;push result (cylinder)
eff_2_div_1:
	MOV EAX, EDX					;divide EAX % EBX
	XOR EDX, EDX					;reset dividend
	MOV EBX, FLOPPY_144_SECTORS_PER_TRACK		;by EBX (18)
	DIV EBX						;= (in EAX, leftover in EDX)
eff_2_div_2:
	INC EDX						;increase left
	MOV ECX, EDX					;put sector in ECX
	MOV EDX, EAX					;put head in EDX
	POP EBX						;put cylinder in EBX
	RET

;-------END--------------


;-------FUN--------------
;WaitLoop16: wiats for the keyboard to be ready
WaitLoop16:	in     al, 64h       ; Read Status byte 
		and    al, 01b       ; Test OBF flag (Status<0>) 
		jnz    WaitLoop16    ; Wait for OBF = 0 
		ret
;-------END--------------

startup:


	PUSHA
	MOV BX, ES
	PUSH BX
	MOV CX, 7E00h
	PUSH CX
	MOV BX, 1
	PUSH BX
	MOV EAX, 1
	PUSH EAX
rd1:
	CALL read
	POPA
	
	PUSHA
	MOV DX, 0x800
	PUSH DX
	MOV CX, 0x0
	PUSH CX
	MOV BX, 14
	PUSH BX
	MOV EAX, 2
	PUSH EAX
rd2:
	CALL read
	POPA


	
	
	
rdOver:
	MOV AX, msg2
	PUSH AX
	CALL print
	
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
	JMP 10h:from_protected
from_protected:
	USE32
loop_32:
	MOV AX, 0x8
	MOV DS, AX
	MOV ES, AX
	MOV SS, AX
	jmp entryc
	

SECTION .tmpdata
tmpb1		db		0
tmpb2		db		0
tmpb3		db		0
tmpb4		db		0

SECTION .data
msg2		db		"Going in graphics mode...", 0

;-------GDTD--------------

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