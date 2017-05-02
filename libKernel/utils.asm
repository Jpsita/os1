global util_int49
util_int49:
	PUSH EBP
	MOV EBP, ESP
	MOV EAX, [EBP + 8]
	INT 0x49
	POP EBP
	RET

;----------END----------