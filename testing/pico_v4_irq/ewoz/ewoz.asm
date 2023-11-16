	* = $fc00

lfc00	cld
	cli
	ldx #$ff
	txs
	lda #$0d
	jsr lfd1d
	lda #$df
	sta $2c
	lda #$fd
	sta $2d
	jsr lfd2c
	lda #$0d
	jsr lfd1d
lfc1a	lda #$9b
lfc1c	cmp #$88
	beq lfc33
lfc20	cmp #$9b
	beq lfc27
lfc24	iny
	bpl lfc40
lfc27	lda #$dc
	jsr lfd1d
lfc2c	lda #$8d
	jsr lfd1d
	ldy #$01
lfc33	dey
	bmi lfc2c
lfc36	lda #$a0
	jsr lfd1d
	lda #$88
	jsr lfd1d
lfc40	lda $d010
	beq lfc40
lfc45	pha
	lda #$00
	sta $d010
	pla
	sta $0200,y
	jsr lfd1d
	cmp #$8d
	bne lfc1c
lfc56	ldy #$ff
	lda #$00
	tax
lfc5b	asl
lfc5c	sta $2b
lfc5e	iny
lfc5f	lda $0200,y
	cmp #$8d
	beq lfc2c
lfc66	cmp #$ae
	bcc lfc5e
lfc6a	beq lfc5c
lfc6c	cmp #$ba
	beq lfc5b
lfc70	cmp #$d2
	beq lfca5
lfc74	cmp #$cc
	beq lfcae
lfc78	stx $28
	stx $29
	sty $2a
lfc7e	lda $0200,y
	eor #$b0
	cmp #$0a
	bcc lfc8d
lfc87	adc #$88
	cmp #$fa
	bcc lfc9e
lfc8d	asl
	asl
	asl
	asl
	ldx #$04
lfc93	asl
	rol $28
	rol $29
	dex
	bne lfc93
lfc9b	iny
	bne lfc7e
lfc9e	cpy $2a
	bne lfcb4
lfca2	jmp lfc27
lfca5	jsr lfcab
	jmp lfc1a
lfcab	jmp ($0024)
lfcae	jsr lfd46
	jmp lfc1a
lfcb4	bit $2b
	bvc lfcc5
lfcb8	lda $28
	sta ($26,x)
	inc $26
	bne lfc5f
lfcc0	inc $27
lfcc2	jmp lfc5f
lfcc5	bmi lfcf2
lfcc7	ldx #$02
lfcc9	lda $27,x
	sta $25,x
	sta $23,x
	dex
	bne lfcc9
lfcd2	bne lfce8
lfcd4	lda #$8d
	jsr lfd1d
	lda $25
	jsr lfd0a
	lda $24
	jsr lfd0a
	lda #$ba
	jsr lfd1d
lfce8	lda #$a0
	jsr lfd1d
	lda ($24,x)
	jsr lfd0a
lfcf2	stx $2b
	lda $24
	cmp $28
	lda $25
	sbc $29
	bcs lfcc2
lfcfe	inc $24
	bne lfd04
lfd02	inc $25
lfd04	lda $24
	and #$0f
	bpl lfcd2
lfd0a	pha
	lsr
	lsr
	lsr
	lsr
	jsr lfd13
	pla
lfd13	and #$0f
	ora #$b0
	cmp #$ba
	bcc lfd1d
lfd1b	adc #$06
lfd1d	pha
	pha
lfd1f	lda $d012
	bne lfd1f
lfd24	pla
	and #$7f
	sta $d012
	pla
	rts
lfd2c	ldy #$00
lfd2e	lda ($2c),y
	beq lfd38
lfd32	jsr lfd1d
	iny
	bne lfd2e
lfd38	rts
lfd39	lda $d010
	beq lfd39
lfd3e	pha
	lda #$00
	sta $d010
	pla
	rts
lfd46	rts
lfd47	lda #$0d
	jsr lfd1d
	lda #$19
	sta $2c
	lda #$fe
	sta $2d
	jsr lfd2c
	lda #$0d
	jsr lfd1d
	rts
lfd5d	lda $0200,y
	eor #$30
	cmp #$0a
	bcc lfd68
lfd66	adc #$08
lfd68	asl
	asl
	asl
	asl
	sta $28
	iny
	lda $0200,y
	eor #$30
	cmp #$0a
	bcc lfd7a
lfd78	adc #$08
lfd7a	and #$0f
	ora $28
	iny
	rts
lfd80	lda $d010
	beq lfd80
lfd85	pha
	lda #$00
	sta $d010
	pla
	rts
lfd8d	sta $43
	stx $44
	sty $45
	pla
	sta $42
	pla
	sta $41
	pla
	sta $40
	cld
	tsx
	stx $46
	cli
	lda #$0d
	jsr lfd1d
	lda #$58
	sta $2c
	lda #$fe
	sta $2d
	jsr lfd2c
	lda #$0d
	jsr lfd1d
	lda #$20
	jsr lfd1d
	lda $40
	jsr lfd0a
	lda $41
	jsr lfd0a
	ldy #$01
lfdc7	lda #$20
	jsr lfd1d
	lda !$0041,y
	jsr lfd0a
	iny
	cpy #$06
	bcc lfdc7
lfdd7	lda #$0d
	jsr lfd1d
	jmp lfc1a
	.byt $57,$65
lfde1	jmp ($6f63)
	.byt $6d,$65,$20
	.byt $74,$6f,$20
	.byt $45,$57,$4f
	.byt $5a,$20,$31
	.byt $2e,$32,$2e
	.byt $00,$53,$74
	.byt $61,$72,$74
	.byt $20,$49
lfdfb	ror $6574
	jmp ($4820)
	.byt $65,$78,$20
	.byt $63,$6f,$64
	.byt $65,$20,$54
	.byt $72,$61,$6e
	.byt $73,$66,$65
	.byt $72,$20,$28
	.byt $54,$42
lfe15	eor #$29
	rol $4900
	ror $6574
	jmp ($4820)
lfe20	adc $78
	jsr $6d49
	bvs $fe96
	.byt $72,$74
lfe29	adc $64
	jsr $4b4f
	rol $4900
	ror $6574
	jmp ($4820)
lfe37	adc $78
	jsr $6d49
	bvs $fead
	.byt $72,$74,$65
	.byt $64,$20,$77
	.byt $69,$74,$68
	.byt $20,$63,$68
	.byt $65,$63,$6b
	.byt $73,$75,$6d
	.byt $20,$65,$72
	.byt $72,$6f,$72
	.byt $2e,$00,$20
	.byt $20
lfe5a	bvc $fe9f
	.byt $20,$20,$53
	.byt $52,$20,$41
	.byt $43,$20,$58
	.byt $52,$20,$59
	.byt $52,$20,$53
lfe6b	bvc $fe6d
