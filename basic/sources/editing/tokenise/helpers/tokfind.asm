; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tokfind.asm
;		Purpose:	Find a token in the element buffer in upper case.
;		Created:	14th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;										Search macro
;
; ************************************************************************************************

tsearch 	.macro
		ldx 	#\1 & $FF 					; do this table
		ldy 	#\1 >> 8
		jsr 	TOKFindTokenXY 				; find it, or not
		bcs 	\2
		.endm

; ************************************************************************************************
;
;			Find a token in the element buffer, return X:A/CS if found, CC if not found)
;
; ************************************************************************************************

TOKFindToken:
		.tsearch 	BinaryTokenText,TOKBinaryToken
		.tsearch	BaseTokenText,TOKBaseToken
		.tsearch 	Shift1TokenText,TOKShift1Token
		.tsearch 	Shift2TokenText,TOKShift2Token
		clc
		rts

TOKBinaryToken: 							; binary token $20-$3F
		sec
		sbc 	#$60
TOKBaseToken:
		ldx 	#0 							; come here for unshifted (e.g. X = 0)
		sec
		rts

TOKShift1Token: 							; shifted tokens.
		ldx 	#KWD_SYS_SH1
		sec
		rts

TOKShift2Token:
		ldx 	#KWD_SYS_SH2
		sec
		rts


; ************************************************************************************************
;
;								General token finder using table at YX
;
; ************************************************************************************************

TOKFindTokenXY:
		stx 	zTemp0 						; save token table address
		sty 	zTemp0+1
		lda 	#$80 						; table starts at $80
		sta 	zTemp1
_TOKFindLoop:				
		lda 	(zTemp0) 					; get token length from table
		clc 		 						; clear carry in case of fail
		bmi 	_TOKExit 					; end of table, fail, so return CC.
		cmp 	TOKElement 					; compare against the element length
		bne 	_TOKNext 					; different, try next.
		tax 								; number of chars to compare.
		ldy 	#1 							; offset to actual text.
_TOKCompare:
		lda 	(zTemp0),y 					; compare the characters 
		eor 	TOKElement,y
		and 	#$7F 						; ignoring bit 7
		bne 	_TOKNext 					; different ? try next
		iny 								; compare next two
		dex
		bne 	_TOKCompare 				; until done X characters.
		sec
		lda 	zTemp1 	 					; return current ID.
_TOKExit:		
		rts

_TOKNext:
		inc 	zTemp1 						; increment token #
		sec 								; add length+1 to ptr
		lda 	(zTemp0) 					
		adc 	zTemp0
		sta 	zTemp0
		bcc 	_TOKFindLoop
		inc 	zTemp0+1
		bra 	_TOKFindLoop

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
