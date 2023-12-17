; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		indent.asm
;		Purpose:	Get current line indentation
;		Created:	17th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;									Get indent for line (codePtr)
;
; ************************************************************************************************

GetIndent:
		stz 	zTemp1 						; clear structure count.
		ldy 	#3
_GILoop:
		lda 	(codePtr),y 				; end of line ?
		cmp 	#KWD_SYS_END		
		beq 	_GIExit
		jsr 	SkipOneInstruction 			; skip instruction, which adjusts the structure count
		bra 	_GILoop

_GIExit:
		lda 	zTemp1 						; this is now the offset
		asl 	a 							; x 2 to accentuate the indentation.
		rts

		.send code

; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;
; ************************************************************************************************
