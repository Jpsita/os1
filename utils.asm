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