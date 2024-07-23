; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		gosub.asm
;		Purpose:	Gosub/Return
;		Created:	8th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;										GOSUB command
;
; ************************************************************************************************

Command_GOSUB:	;; [gosub]
		ldx 	#0
		jsr 	EXPEvalInteger16 			; get line number
		lda 	#STK_GOSUB
		jsr 	StackOpen 
		jsr 	STKSaveCodePosition 		; save return address on stack.
		jmp 	GotoR0

; ************************************************************************************************
;
;										RETURN command
;
; ************************************************************************************************

Command_RETURN:	;; [return]
		jsr 	StackGetFrame 				; look at top of frame stack
		cmp 	#STK_GOSUB 					; found a RETURN
		beq 	_CRReturn
		and 	#$0F 						; x0 is impossible as can't have empty frame
		beq 	_CRStructure
		jsr 	StackClose 					; unpick the frame
		bra 	Command_RETURN

_CRReturn:		
		jsr 	StackCheckFrame
		jsr 	STKLoadCodePosition 		; return
		jsr 	StackClose		 			
		rts

_CRStructure:
		.error_structure

		.send code

; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;
; ************************************************************************************************
