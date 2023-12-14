; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		dtkdataitem.asm
;		Purpose:	Detokenise data item
;		Created:	28th May 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;										Detokenise data item
;
; ************************************************************************************************

TOKDDataItem:
		tay 								; type in Y
		lda 	#'"'						; start with " or . depending on type.
		cpy 	#PR_LSQLSQSTRINGRSQRSQ
		beq 	_TOKDDIsString
		lda 	#'.'
_TOKDDIsString:
		;
		jsr 	TOKDOutput 					; dump it
		jsr 	TOKDGet 					; get length into X
		tax 
		;
_TOKDDOutput:
		dex 								; output all the data.
		cpx 	#255
		beq 	_TOKDDEnd
		jsr 	TOKDGet 					; get character and output it
		jsr 	TOKDOutput
		bra 	_TOKDDOutput
_TOKDDEnd:
		cpy 	#PR_LSQLSQSTRINGRSQRSQ 		; if string, do closing quote
		bne 	_TOKDDNotString
		lda 	#'"'
		jsr 	TOKDOutput
_TOKDDNotString:
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
;	 	30/06/23 		Test was using bmi for complete so didn't work for > 127.
;
; ************************************************************************************************

