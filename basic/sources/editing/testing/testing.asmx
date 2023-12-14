; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		testing.asmx
;		Purpose:	Testing code
;		Created:	29th May 2023
;		Reviewed:
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

		.include "testdat.incx"

TestCode:
		jsr 	PGMNewProgram
		stz 	$FFFF
		.include "testrun.incx"
		jmp 	$FFFF


; ************************************************************************************************
;
;									Process one input line
;
; ************************************************************************************************

TOKOneLine:
		stx 	zTemp2
		sty 	zTemp2+1
		ldx 	#TOKGetSource & $FF
		ldy 	#TOKGetSource >> 8
		sec
		jsr 	TOKTokenise
		jsr 	PGMDeleteLine
		lda 	TOKLineSize
		cmp 	#4
		beq 	_TOKNoInsert
		jsr 	PGMInsertLine
_TOKNoInsert:
		rts		

; ************************************************************************************************
;
;		Get data, test for the library, Get CC, Get advance CS, return zero when out of data
;
; ************************************************************************************************

TOKGetSource:
		lda 	(zTemp2)
		bcc 	_GSNoIncrement
		inc 	zTemp2
		bne 	_GSNoIncrement
		inc 	zTemp2+1
_GSNoIncrement:
		cmp 	#0
		rts
		.send code

		.section storage
SrcPosition:
		.fill 		1				
		.send storage


		
; ************************************************************************************************
;
;									Changes and Updates
;
; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;
; ************************************************************************************************

