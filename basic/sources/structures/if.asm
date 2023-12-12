; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		if.asm
;		Purpose:	IF command
;		Created:	8th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************

; ************************************************************************************************
;
;										IF (two forms)
;
; ************************************************************************************************

		.section code

IfCommand: ;; [if]
		ldx 	#0
		jsr 	EXPEvalNumber 				; work out the test
		;
		lda 	(codePtr),y					; what follows ?
		cmp 	#KWD_THEN  					; could be THEN <stuff> 
		bne 	_IfStructured 				; we still support it.

		; ------------------------------------------------------------------------
		;
		;						 IF ... THEN <statement> 
		;
		; ------------------------------------------------------------------------

		iny 								; consume THEN
		jsr 	CheckIfZero
		beq 	_IfFail 					; if fail, go to next line
		lda 	(codePtr),y 				; line # follows e.g. IF .. THEN 100
		and 	#$C0
		cmp 	#$40
		beq 	_IfThenConst
		rts 								; if THEN just continue
_IfFail:
		jmp 	RUNEndOfLine
_IfThenConst:
		jmp 	Command_GOTO		

		; ------------------------------------------------------------------------
		;
		;		   The modern, structured, nicer IF ... ELSE ... ENDIF
		;
		; ------------------------------------------------------------------------

_IfStructured:
		jsr 	CheckIfZero 				; is it zero
		bne 	_IfExit 					; if not, then continue normally.
		lda 	#KWD_ELSE 					; look for else/endif 
		ldx 	#KWD_ENDIF
		jsr 	ScanForward 				; and run from there
_IfExit: 
		rts

; ************************************************************************************************
;
;					ELSE code - should be found when running a successful IF clause
;
; ************************************************************************************************

ElseCode: ;; [else] 					
		lda 	#KWD_ENDIF 					; else is only run after the if clause succeeds
		tax 								; so just go to the structure exit
		jsr 	ScanForward
		rts

; ************************************************************************************************
;
;										ENDIF code
;
; ************************************************************************************************

EndIf:	;; [endif]							
		rts 								; endif code does nothing

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
