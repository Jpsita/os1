USE32
SECTION .text

extern PIC_sendEOI

;----------FUN----------
; FUNCTION inb 
global inb
inb:
	XOR AL, AL
inb_debug2:
	PUSH EBP
	MOV EBP, ESP
	MOV EDX, [EBP + 8]
inb_debug:
	IN AL, DX
	POP EBP
inb_exit:
	RET
	
;-----------------------

;----------FUN----------
; FUNCTION io_wait()
; Waits for io processing to be complete
;-----------------------

global io_wait
io_wait:
	MOV DX, 0x80
	MOV AL, 0
	OUT DX, AL
	RET

;----------END----------


;----------FUN----------
; FUNCTION outb (INT_32 port, INT_32 data)
;-----------------------
global outb
outb :
	PUSH EBP
	MOV EBP, ESP
	MOV EDX, [EBP + 8]	;get port
	MOV EAX, [EBP + 12] 	;get data
outb_debug:
	OUT DX, AL
	POP EBP
outb_exit:
	RET
	
;----------END----------

;----------FUN----------
; FUNCTION WaitLoop
global waitLoop
waitLoop:
	IN	AL, 64h     ; Read Status byte 
	AND	AL, 10b     ; Test OBF flag (Status<0>) 
	JNZ	waitLoop    ; Wait for OBF = 0 
	RET
;----------END----------

;----------FUN----------
; Function keybSp
global keybSp
keybSp:
	MOV AL, 0xF0
	OUT 0x60, AL 
	CALL waitLoop
	NOP
	NOP
	MOV AL, 2
	OUT 0x60, AL
	CALL waitLoop
	RET
;----------END----------	

;----------SEC----------
; INTERRUPT HANDLERS
;-----------------------

;----------HDN----------
; HANDLER: hndl1: handles interrupt with IRQ 1.
extern handle_irq_1
; this will only redirect to c handler funxtion
global irq_1
irq_1:
	PUSHA
	XOR 	EAX,	EAX
	CALL 	handle_irq_1	;move to C Function
	MOV	AL,	0x01
	PUSH	EAX
	CALL	PIC_sendEOI
	POPA	
	IRET
	
;----------END----------

;----------FUN----------
; FUNCTION: load_IDT(uint32_t addr)
; params: addr Address of the IDT Descriptor
;-----------------------
global load_IDT
load_IDT:
	PUSH	EBP
	MOV	EBP, ESP
	MOV	EAX, [EBP + 8]	;load addr
load_IDT_debug:
	LIDT	[EAX]
load_IDT_sti:
	STI
	POP 	EBP
load_IDT_ret:
	RET
	
;----------END----------
