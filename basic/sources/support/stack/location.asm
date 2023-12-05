; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		location.asm
;		Purpose:	Store and retrieve the location from the TOS
;		Created:	5th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;		Save the current code position and offset (in Y) on the stack. By convention, this is
;		stored in the first 3 bytes above the stack frame marker.
;
; ************************************************************************************************

STKSaveCodePosition:
		phy
		tya 								; save Y at offset 1.
		ldy 	#1
		sta 	(basicStack),y
		lda 	codePtr 					; save code (low) at 2
		iny
		sta 	(basicStack),y
		lda 	codePtr+1 					; save code (high) at 3
		iny
		sta 	(basicStack),y
		ply
		rts

; ************************************************************************************************
;
;							Load TOS into current code positions
;
; ************************************************************************************************
		
STKLoadCodePosition:
		ldy 	#3 							; load code pointer back
		lda 	(basicStack),y
		sta 	codePtr+1
		dey
		lda 	(basicStack),y
		sta 	codePtr
		dey
		lda 	(basicStack),y 				; line offset to Y
		tay
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
