	* = $fc00

	nop
	nop
lfc00	
	cld
	cli
	ldx #$ff
	txs
	lda 	#-1
	sta 	$81
loop2:
	lda 	#10
	jsr 	delay
	jsr 	alphabet	
	inc 	$81
	lda 	$81
	jsr 	lprinthex
	lda 	#13
	jsr 	lprintchar
	lda 	#0
_opcode:	
	bra 	loop2			

delay:
	ldy 	#0
	ldx 	#0
loop1:
	dey
	bne 	loop1
	dex 
	bne 	loop1	
	dec		a
	bne 	loop1
	rts

alphabet:	
	ldx 	#5
	phx
	ply
_loop1:
	tya
	clc
	adc 	#$1C
	jsr lprinthex
	dey
	bpl 	_loop1
	lda 	#42
	jsr 	lprinthex
	rts

lprinthex:
	pha
	pha
	lda 	#32
	jsr 	lprintchar
	pla
	lsr 	a
	lsr 	a
	lsr 	a
	lsr 	a
	jsr 	lprintnibble
	pla
	jsr 	lprintnibble
	rts

lprintnibble:
	and 	#15
	cmp 	#10
	bcc 	_nothex
	adc 	#6
_nothex:
	adc 	#48
	jsr 	lprintchar
	rts

lprintchar:	
	pha
waitfree:
	lda $d012
	bne waitfree
	pla	
	and #$7f
	sta $d012
	rts

	* = $FFFA
	.word 	lfc00
	.word 	lfc00
	.word	lfc00
	
