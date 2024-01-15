; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		readdata.asm
;		Purpose:	Read & Data commands
;		Created:	26th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;											READ command
;
; ************************************************************************************************

		.section code

Command_READ: ;; [read]
		ldx 	#0 							; get term to R0
		jsr 	EvaluateTerm		
		lda 	XSControl					; fail if not a reference.
		and 	#XS_ISVARIABLE
		beq 	_CRSyntax 	
		;
		;		Now find something to be DATA
		;
		jsr 	SwapCodeDataPointers 		; swap code and data pointers over

		lda 	dataInStatement 			; if in a data statement, we don't need to search
		bne 	_CRHaveData  				; forward for the next one.

_CRNextLine:
		lda 	(codePtr)					; check end of program, e.g. nothing more to READ.
		beq 	_CRNoData
		;
		;		Look for Data.
		;
_CRKeepSearching:		
		lda 	#KWD_DATA 					; scan for instruction DATA or EOL.
		ldx 	#KWD_SYS_END
		jsr 	ScanForward
		cmp 	#KWD_DATA 					; found data ?
		beq 	_CRHaveData 				; found it

		ldy 	#3 							; position in next line.
		clc 								; try the next line, keep going.
		lda 	(codePtr)
		adc 	codePtr
		sta 	codePtr
		bcc 	_CRNextLine 
		inc 	codePtr+1
		bra 	_CRNextLine

_CRNoData:		 							; error , no more data
		.error_data
_CLType: 									; error , type mismatch
		.error_type		
_CRSyntax: 									; error , syntax
		.error_syntax
		;
		; 		Now have codePtr (dataPtr really) pointing at DATA keyword
		;
_CRHaveData:
		ldx 	#1 							; expression into slot 1.
		jsr 	EXPEvaluateExpressionAtX
		jsr 	DereferenceTOS				; dereference it

		lda 	XSControl 					; type of l-expr
		eor 	XSControl+1 				; check types match
		bmi 	_CLType
		;
		dex 								; assign the value.
		jsr 	AssignValueToReference
		;
		stz 	dataInStatement 			; clear in data flag
		lda 	(codePtr),y 				; data followed by a comma,e.g. more data follows
		cmp 	#KWD_COMMA 					; if not, end of data statement and exit
		bne 	_CRSwapBack
		iny 								; consume comma
		inc 	dataInStatement 			; set in data statement flag.
		;
_CRSwapBack:		
		jsr 	SwapCodeDataPointers		; swap them back.		
		lda 	(codePtr),y 				; l-expr was followed by a comma
		iny
		cmp 	#KWD_COMMA
		beq 	Command_READ 				; if so go round again.
		dey 								; unpick get.
		rts

; ************************************************************************************************
;
;									DATA command - effectively NOP
;
; ************************************************************************************************
		
Command_DATA: ;; [data]
		lda 	#KWD_COLON 					; scan forward to : or EOL
		ldx 	#KWD_SYS_END
		jsr 	ScanForward
		cmp 	#KWD_SYS_END 			; unconsume the EOL
		bne 	_CDNotEOL
		dey
_CDNotEOL:		
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

