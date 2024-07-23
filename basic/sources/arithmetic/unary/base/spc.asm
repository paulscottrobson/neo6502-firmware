; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		spc.asm
;		Purpose:	Function returning a given number of strings.
;		Created:	6th December 2003
;		Reviewed: 	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
; 										STR$() function
;
; ************************************************************************************************

UnarySpc: ;; [spc(]	

		jsr 	EXPEvalInteger8 			; count of spaces.
		pha
		jsr 	ERRCheckRParen
		pla

		phy 								; save Y
		ldy 	#32 						; character to display
		jsr 	StringCreateDuplicate
		ply 								; restore Y

		rts

StringCreateDuplicate:
		phy 								; save character and count
		pha 
		;
		jsr 	StringTempAllocate 			; allocate string space.

		pla 								; count in to first byte
		sta 	(zsTemp) 					
		tay 								; count in Y
		pla 								; character in A
_CSDLoop:
		cpy 	#0 							; complete
		beq 	_CSDExit
		sta 	(zsTemp),y
		dey
		bra 	_CSDLoop
_CSDExit:		
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
