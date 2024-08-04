; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		inputprintline.asm
;		Purpose:	File driven input/print ascii lines
;		Created:	4th August 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;									Handle line input
;
; ************************************************************************************************

InputLineHandler:
		jsr 	IOLCommonChannelCode
_ILHLoop:
		lda 	(codePtr),y 				; check for EOL or colon
		cmp 	#KWD_COLON
		beq 	_ILHExit
		cmp 	#KWD_SYS_END
		beq 	_ILHExit

		ldx	 	#0
		jsr 	EXPEvaluateExpressionAtX	; get an expression, should be a string variable
		lda 	XSControl,x 				; check if reference (e.g. a variable) and type together
		and 	#XS_TYPEMASK|XS_ISVARIABLE
		cmp  	#XS_ISVARIABLE+XS_ISSTRING
		bne 	_ILHType 					; if not, then we have an error.

		ldx 	#1 							; slot to read into.
		lda 	#252 						; maximum string length
		jsr 	StringTempAllocate 			; allocate space for new string.

_ILHReadLoop:
		jsr 	IFHReadByte		
		cmp 	#10 						; LF found, end of string.
		beq 	_ILHReadDone
		cmp 	#32 						; 32+ write character to string
		bcs 	_ILHWriteChar
		cmp 	#9 							; TAB is converted to space
		bne 	_ILHNoWrite 				; other controls are ignored
		lda 	#32
_ILHWriteChar:		
		jsr 	StringTempWrite
_ILHNoWrite:
		bra 	_ILHReadLoop

_ILHReadDone:
		ldx 	#0							; now do assignment code
		jsr 	AssignValueToReference

		lda 	(codePtr),y 				; consume comma if there is one.
		cmp 	#KWD_COMMA
		bne 	_ILHLoop
		iny
		bra 	_ILHLoop
_ILHExit:
		rts

_ILHType:
		.error_type

; ************************************************************************************************
;
;									Handle line output
;
; ************************************************************************************************

OutputLineHandler:
		jsr 	IOLCommonChannelCode		
_OLHLoop:
		lda 	(codePtr),y 				; check for EOL or colon
		cmp 	#KWD_COLON
		beq 	_OLHExit
		cmp 	#KWD_SYS_END
		beq 	_OLHExit
		jsr 	EXPEvalString 				; string, pointer in zTemp0
		lda 	(zTemp0) 					; get length into X
		beq 	_OLHDone 					; handle zero length.
		tax
		phy
		ldy 	#1
_OLHWriteLoop:
		lda 	(zTemp0),y 					; write each character out.
		iny
		jsr 	OFHWriteByte 				
		dex
		bne 	_OLHWriteLoop
		ply
_OLHDone:		
		lda 	#10 						; We use LF to output here
		jsr 	OFHWriteByte

		lda 	(codePtr),y 				; consume comma if there is one.
		cmp 	#KWD_COMMA
		bne 	_OLHLoop
		iny
		bra 	_OLHLoop
_OLHExit:
		rts

; ************************************************************************************************
;
;								process LINE # <channel>,
;
; ************************************************************************************************

IOLCommonChannelCode:
		iny 								; consume line
		lda 	(codePtr),y 				; must be followed by a #
		cmp 	#KWD_HASH
		bne	 	_IOLCSyntax
		iny 								; consume #
		jsr 	FHIdentifyChannel 			; get the current channel.
		jsr 	ERRCheckComma 				; check , follows
		rts
_IOLCSyntax:
		.error_syntax

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

