	* = $fc00

lfc00	
	cld
	cli
	ldx #$ff
	txs
	jsr 	alphabet
loop2:
	lda 	#1
	jsr 	delay
	lda 	#13
	jsr 	lprintchar
	jsr 	alphabet	
	lda 	#0
	beq 	loop2

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
	ldx 	#10
_loop1:
	txa
	clc
	adc 	#$40
	jsr lprintchar
	dex
	bpl 	_loop1
	lda 	#42
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
	.word 	0
	.word 	$FC00
	.word	0