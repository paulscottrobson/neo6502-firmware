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
		.byte 	3

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

