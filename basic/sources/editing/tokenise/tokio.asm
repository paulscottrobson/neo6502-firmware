; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tokio.asm
;		Purpose:	Read/Write functions
;		Created:	14th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;									Output token A
;
; ************************************************************************************************

TOKWriteA:
		phx
		ldx 	tokLineSize
		sta 	tokLineSize,x
		inc 	tokLineSize
		plx
		rts

; ************************************************************************************************
;
;		  Get current character in token buffer. Z flag set at End, with/without advance
;
; ************************************************************************************************				

TOKGet:
		phx
		lda 	#0
		ldx 	inputPos 					; end of buffer, return zero.
		cpx 	inputBuffer
		beq		_TGExit
		lda 	inputBuffer+1,x 			; read next character skipping length prefix.
_TGExit:	
		plx
		cmp 	#0
		rts		

TOKGetNext:
		jsr 	TOKGet
		beq 	_TGNExit
		inc 	inputPos
_TGNExit:
		cmp 	#0
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

