; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tokidfind.asm
;		Purpose:	Find existing identifier.
;		Created:	15th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code


; ************************************************************************************************
;
;									See if identifier exists					
;
; ************************************************************************************************

TOKFindIdentifier:
		ldx 	tokElement 					; set the MSB of the last character of the identifier.
		lda 	tokElement,x
		ora 	#$80
		sta 	tokElement,x

		lda 	#Program >> 8				; point zTemp0 to first record.
		sta 	zTemp0+1
		lda 	#1
		sta 	zTemp0
_TOKFISearch:
		lda 	(zTemp0)					; reached the end ?
		beq 	_TOKFIFail
		;
		ldy 	#5 							; start matching, pre-increment
_TOKFIMatch:
		iny
		lda 	(zTemp0),y 					; check match
		cmp 	tokElement-5,y
		bne 	_TOKFINext 					; no, do next.
		asl 	a 							; bit 7 set
		bcc 	_TOKFIMatch 				; no, keep going till a match.
		;
		lda 	zTemp0+1 					; XA = record offset
		sec
		sbc 	#Program >> 8
		tax
		lda 	zTemp0 						
		inc 	a 							; add 1 to point to data
		bne 	_TOKFINoCarry
		inx
_TOKFINoCarry:
		sec 								; return success
		rts		

_TOKFINext:
		clc 								; go to next record.
		lda 	(zTemp0)
		adc 	zTemp0
		sta 	zTemp0
		bcc 	_TOKFISearch
		inc 	zTemp0+1
		bra 	_TOKFISearch

_TOKFIFail:
		clc
		rts

		.send code

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

