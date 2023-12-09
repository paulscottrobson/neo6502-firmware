; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		scanproc.asm
;		Purpose:	Scan Procedures
;		Created:	9th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;								Scan code looking for procedures
;
; ************************************************************************************************

ScanProcedures:
		lda 	Program 					; back to the program start
		clc
		adc 	#Program >> 8
		sta 	zTemp0+1
		stz 	zTemp0	
		;
_SPLoop:
		lda 	(zTemp0) 					; end of program
		beq 	_SPExit
		ldy 	#3 							; first token PROC ?
		lda 	(zTemp0),y
		cmp 	#KWD_PROC
		bne 	_SPNext
		jsr 	_SPSetupRecord
_SPNext:	
		clc 								; forward to next			
		lda 	(zTemp0)
		adc 	zTemp0
		sta 	zTemp0
		bcc 	_SPLoop
		inc 	zTemp0+1
		bra 	_SPLoop
_SPExit:
		rts
		;
		;		Set up a new procedure record.
		;
_SPSetupRecord:
		ldy 	#4 							; check identifier follows
		lda 	(zTemp0),y
		cmp 	#$20
		bcs 	_SPSyntax

		clc 								; make address in zTemp1
		adc 	#(Program >> 8)	
		sta 	zTemp1+1
		iny
		lda 	(zTemp0),y
		sta 	zTemp1
		;
		lda 	zTemp0 						; copy address of line to data
		sta 	(zTemp1)
		lda 	zTemp0+1
		ldy 	#1
		sta 	(zTemp1),y
		ldy 	#4 							; set type to ISPROC
		lda 	#XS_ISPROC 
		sta 	(zTemp1),y
		;
		rts

_SPSyntax:		
		.error_syntax

		.send code
		
; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;
; ************************************************************************************************
