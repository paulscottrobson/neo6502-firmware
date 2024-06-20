; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		open.asm
;		Purpose:	Open file input/output
;		Created:	20th June 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;							OPEN (INPUT|OUTPUT) <file#>,<name> Command
;
; ************************************************************************************************

		.section code

Command_Open:	;; [open]
		lda 	(codePtr),y 				; look for INPUT
		iny
		cmp 	#KWD_INPUT 				
		beq 	_COHaveDirection
		cmp 	#KWD_SYS_SH1 				; otherwise OUTPUT
		bne 	_COSyntax
		lda 	(codePtr),y
		iny
		cmp 	#KWD_OUTPUT-$100
		beq 	_COHaveDirection
_COSyntax:
		.error_syntax

_COHaveDirection:
		eor 	#KWD_INPUT 					; 0 for input, #0 for output
		beq 	_COIsInput
		lda 	#3 							; 0 for input, 3 for output (create/truncate)
_COIsInput:		
		pha 								; save direction

		ldx 	#0
		jsr 	EXPEvalInteger8 			; channel # to output
		jsr 	ERRCheckComma 				; ,
		inx
		jsr 	EXPEvalString 				; filename string to R1, zTemp0		

		lda 	XSNumber0 					; file handle
		sta 	ControlParameters+0
		lda 	zTemp0 						; file name
		sta 	ControlParameters+1
		lda 	zTemp0+1
		sta 	ControlParameters+2

		pla 								; direction.
		sta 	ControlParameters+3

		.DoSendMessage 						; open file
		.byte 	3,4
		.DoWaitMessage 						; wait for result
		lda 	ControlError 				; error occurred.
		bne 	_COError
		rts
_COError:
		.error_file		

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

