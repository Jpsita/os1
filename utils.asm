USE32
SECTION .text

extern PIC_sendEOI

;----------FUN----------
; FUNCTION inb 
global inb
inb:
	MOV EAX, 0
inb_debug2:
	ENTER 0, 0
	MOV EDX, [EBP + 8]
inb_debug:
	IN AL, DX
	LEAVE
inb_exit:
	RET
	
;----------END----------

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
	ENTER 0, 0
	MOV EDX, [EBP + 8]	;get port
	MOV EAX, [EBP + 12] 	;get data
outb_debug:
	OUT DX, AL
	LEAVE
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
	PUSHAD
irq_1_dbg:
	CALL 	handle_irq_1	;move to C Function
	XOR 	EAX, EAX
	MOV	AL,	0x01
	PUSH	EAX
	CALL	PIC_sendEOI
	POP 	EAX
	POPAD
irq_1_exit:
	IRET
	
;----------END----------

;----------FUN----------
; FUNCTION: load_IDT(uint32_t addr)
; params: addr Address of the IDT Descriptor
;-----------------------
global load_IDT
load_IDT:
	ENTER 0, 0
	MOV		EAX, [EBP + 8]	;load addr
load_IDT_debug:
	LIDT	[EAX]
load_IDT_sti:
	STI
	LEAVE
load_IDT_ret:
	RET
	
;----------END----------


;----------FUN---------
; FUNCTION double_fault_HNDLR
; params: 0 default 0 value for double faults.
;----------------------
extern c_dbl_flt
global double_fault_HNDLR
double_fault_HNDLR:
	MOV		EBX,	EBP
	MOV 	EBP, 	ESP
	ADD		EBP,	4
	MOV		ESP,	EBP
	MOV		EBP,	EBX
	CALL	c_dbl_flt
double_fault_HNDLR_exit:
	IRET
	
;----------END----------
	

;----------FUN---------
; FUNCTION d_b_0_HNDLR
; params: 0 default 0 value for double faults.
;----------------------
extern db0_fault
global d_b_0_HNDLR
d_b_0_HNDLR:

	CALL	db0_fault
d_b_0_HNDLR_exit:
	IRET

;----------END----------

;----------FUN---------
; FUNCTION segment_fault_HNDLR
; params: 0 default 0 value for double faults.
;----------------------

extern segment_fault
global segment_fault_HNDLR
segment_fault_HNDLR:
	CALL	segment_fault
segment_fault_HNDLR_exit:
	IRET
	
;----------END----------


extern stack_fault
global stack_fault_HNDLR
stack_fault_HNDLR:
	CALL stack_fault
stack_fault_HNDLR_exit:
	IRET
	
;----------END----------

extern overflow_fault
global overflow_fault_HNDLR
overflow_fault_HNDLR:
	CALL overflow_fault
overflow_fault_HNDLR_exit:
	IRET

;----------END----------

extern protection_fault
global protection_fault_HNDLR
protection_fault_HNDLR:
	CALL	protection_fault
protection_fault_HNDLR_exit:
	IRET

;----------END----------

global PIC_timer
PIC_timer:
	IRET

;----------END----------

global util_cli
util_cli:
	CLI
	RET

;----------END----------

global util_sti
util_sti:
	STI
	RET

;----------END----------

extern handle_irq_6
global floppy_HNDLR
floppy_HNDLR:
	PUSHAD
	CALL handle_irq_6
	POPAD
	IRET

;----------END----------

extern handle_irq_8
global rtc_HNDLR
rtc_HNDLR:
	PUSHAD
	CALL handle_irq_8
	POPAD
	IRET

;----------END----------