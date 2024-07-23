; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		instr.asm
;		Purpose:	Instring (string position of s2 in s1)
;		Created:	24th March 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;									String length
;
; ************************************************************************************************

		.section code	

EXPUnaryInstr: ;; [instr(]
		jsr 	EXPEvalString 					; search string
		inx
		jsr 	ERRCheckComma 					; ,
		jsr 	EXPEvalString 					; match string pointer in R0.
		jsr 	ERRCheckRParen 					; )
		dex

		lda 	XSNumber0,x 					; address of search string in R1
		sta 	zTemp1
		lda 	XSNumber1,x
		sta 	zTemp1+1
		;
		phy 									; save position.
		sec  									; calculate len(search)-len(match), this is the number of substrings to work out.
		lda 	(zTemp1)
		sbc 	(zTemp0)
		bcc 	_EUIFail 						; cannot possibly work :)
		sta 	zsTemp 							; number of matches to try, plus one extra.
		stz 	zsTemp+1  						; char offset in result.
_EUIMatchLoop1:
		inc 	zsTemp+1 						; pre-increment result so first is 1.
		lda 	(zTemp0) 						; length of match string, also offset to last character of it.
		tay 									; to Y
		beq 	_EUISucceed 					; if zero we have a match (may match "")
_EUMatchLoop2:
		lda 	(zTemp1),y 						; check if string matches
		cmp 	(zTemp0),y
		bne 	_EUMatchNext 					; no, maybe try the next
		dey 									; done all characters
		bne 	_EUMatchLoop2
_EUISucceed:		
		ply
		lda 	zsTemp+1 						; get the match position.
		jmp 	EXPUnaryReturnA 				; return it.
_EUMatchNext:
		lda 	zsTemp 							; done the lot ?
		beq 	_EUIFail
		dec 	zsTemp 							; decrement try counter
		inc 	zTemp1 							; bump string pointer, next character along
		bne 	_EUIMatchLoop1
		inc 	zTemp1+1
		bra 	_EUIMatchLoop1

_EUIFail:
		ply
		jmp 	ConstFalse						; return false which is zero.

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

