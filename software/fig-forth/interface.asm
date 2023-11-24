        .include "neo6502.inc"
	
oswrch:
	jmp 	WriteCharacter
osrdch:
	jmp 	ReadCharacter
osnewl:
	pha
	lda 	#13
	jsr 	WriteCharacter
	pla
	rts
