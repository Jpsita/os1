USE32
SECTION .text

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
; FUNCTION outb 
global outb
outb :
	PUSH EBP
	MOV EBP, ESP
	MOV EAX, [EBP + 8]
	MOV EDX, [EBP + 12] 
outb_debug:
	OUT DX, AL
	POP EBP
outb_exit:
	RET
	
;-----------------------

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


