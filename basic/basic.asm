; ************************************************************************************************
; ************************************************************************************************
;
;       Name:       basic.asm
;       Purpose:    BASIC main program
;       Created:    25th November 2023
;       Reviewed:   No
;       Author:     Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************


; ************************************************************************************************
;
;                                          Main Program
;
; ************************************************************************************************

		.section code

boot:   jmp     ColdStart
		jmp     CheckSpeed
		
ColdStart:
		jmp 	TestTokenising  
		jmp     Command_RUN

		.send   code
		.include "_include.inc"
		.section code

TestTokenising:
		lda 	#_Test1 & $FF
		ldy 	#_Test1 >> 8
		jsr 	LoadTokenCodeYA
		jsr 	TOKTokenise
_h1:		bra 	_h1

_Test1:	.byte 	_Test1End-_Test1-1
		.text 	"  123409"
_Test1End:						
;
;           Temp bodges of various kinds.
;
LoadTokenCodeYA:
		sta 	zTemp0
		sty 	zTemp0+1
		lda 	(zTemp0)
		tax
		inx
		ldy 	#0
_LTCCopy:		
		lda 	(zTemp0),y
		sta 	inputBuffer,y
		iny
		dex
		bne 	_LTCCopy
		rts

CheckSpeed:
		pha
		lda     #100
		ldx     #0
		ldy     #0
_Loop1:    dey
		bne     _Loop1
		dex
		bne     _Loop1
		dec     a
		bne     _Loop1
		pla
		inc     a
		and     #7
		adc     #48
		jsr     $FFF1
		bra     CheckSpeed

WarmStart:
		lda     #$00
		tax
		tay
		.byte   3
		bra     WarmStart


		.align  256
Program:
		.binary "build/tokenised.dat"

		.send code


; ************************************************************************************************
;
;                                   Changes and Updates
;
; ************************************************************************************************
;
;       Date            Notes
;       ====            =====
;
; ************************************************************************************************

