; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		bytes.asm
;		Purpose:	Push/Pull single bytes on the stack
;		Created:	5th December 2023
;		Reviewed:	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;								Push A on the BASIC stack
;
; ************************************************************************************************

StackPushByte:
		pha 								; save byte on stack
		lda 	basicStack 					; decrement basic stack pointer
		bne 	_SPBNoBorrow
		dec 	basicStack+1 				; borrow
_SPBNoBorrow:
		dec 	basicStack
		pla 								; get back and write
		sta 	(basicStack)
		rts				
		
; ************************************************************************************************
;
;								Pop A off the BASIC stack
;
; ************************************************************************************************				

StackPopByte:
		lda 	(basicStack) 				; bump the stack pointer.
		inc 	basicStack
		bne 	_SPBNoCarry
		inc 	basicStack+1
_SPBNoCarry:
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
