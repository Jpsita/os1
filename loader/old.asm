
;--------FUN---------------
; FUNCTION print_int32: prints a 32 bit HEXADECIMAL number
; Paramaters;
; 0: (32 bit): number
; return: none
; registers: intact (not BP)
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
