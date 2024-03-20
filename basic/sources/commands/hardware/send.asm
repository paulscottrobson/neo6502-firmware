; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		send.asm
;		Purpose:	Data send to i2c/spi
;		Created:	2nd March 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										isend Command
;
; ************************************************************************************************

		.section code

Command_ISend:	;; [isend]
		ldx 	#0
		jsr 	EXPEvalInteger8 			; channel #
		jsr 	ERRCheckComma

		jsr 	SendGetDataBuffer 			; get the data to send.

		.DoSendMessage 						; write to i2c
		.byte 	10,10
		.DoWaitMessage
		lda 	ControlError
		bne 	_CSHardware
		rts		

_CSHardware:
		.error_hardware

; ************************************************************************************************
;
;										ssend Command
;
; ************************************************************************************************

Command_SSend:	;; [ssend]
		jsr 	SendGetDataBuffer 			; get the data to send.
		.DoSendMessage 						; write to spi
		.byte 	10,12
		.DoWaitMessage
		lda 	ControlError
		bne 	_CSHardware
		rts		
_CSHardware:
		.error_hardware

; ************************************************************************************************
;
;										usend Command
;
; ************************************************************************************************

Command_USend:	;; [usend]
		jsr 	SendGetDataBuffer 			; get the data to send.
		.DoSendMessage 						; write to uart
		.byte 	10,14
		.DoWaitMessage
		lda 	ControlError
		bne 	_CSHardware
		rts		
_CSHardware:
		.error_hardware

; ************************************************************************************************
;
;									Create data Buffer to send
;
; ************************************************************************************************

SendGetDataBuffer:
		stz 	transmitDataSize
_SDGBLoop:
		lda 	(codePtr),y 				; : or EOL, exit
		cmp 	#KWD_SYS_END
		beq 	_SGDBExit
		cmp 	#KWD_COLON
		beq 	_SGDBExit
		;
		ldx 	#1
		jsr 	EXPEvaluateExpressionAtX	; get a value
		jsr 	DereferenceTOS				; dereference it
		bit 	XSControl,x	 				; check if string
		bmi 	_SDGBSendString
		;
		;		Handle constant, or constant ;
		;
		beq 	_SGDBInteger 				; is it integer
		lda 	#17 						; no, make it integer
		jsr 	DOMathCommand
_SGDBInteger:		
		lda 	XSNumber0+1 				; send LSB anyway
		jsr 	_SDGBAddByte

		lda 	(codePtr),y 				; what follows ?

		cmp 	#KWD_COLON 					; : or end, exit out
		beq 	_SGDBExit
		cmp 	#KWD_SYS_END
		beq 	_SGDBExit

		iny 								; , do another.
		cmp 	#KWD_COMMA
		beq 	_SDGBLoop 		 		

		cmp 	#KWD_SEMICOLON				; followed by ; ?
		bne 	_SDGBSyntax
		lda 	XSNumber1+1 				; send the MSB
		jsr 	_SDGBAddByte
		bra 	_SDGBLoop  					; and do another.

_SDGBSyntax:
		.error_syntax
		;
		;		Add A to the buffer
		;
_SDGBAddByte:
		inc 	transmitDataSize 			; bump data count
		beq 	_SDGBSyntax 				; overflow
		phx 								; copy to buffer
		ldx 	transmitDataSize
		sta 	transmitDataBuffer-1,x
		plx
		rts

_SDGBSendString:		
		lda 	XSNumber0+1 				; copy string address to zTemp0
		sta 	zTemp0
		lda 	XSNumber1+1
		sta 	zTemp0+1
		lda 	(zTemp0) 					; length -> X
		tax 
		beq 	_SDGBStringExit 			; exit if zero
		phy
		ldy 	#1 							; start here
_SDSBSSLoop: 								; send the whole string
		lda 	(zTemp0),y
		iny
		jsr 	_SDGBAddByte
		dex
		bne 	_SDSBSSLoop	
		ply
_SDGBStringExit:
		lda 	(codePtr),y 				; comma follows
		cmp 	#KWD_COMMA
		bne 	_SDGBLoop
		iny 								; consume it
		bra 	_SDGBLoop
		
_SGDBExit:		
		lda 	XSNumber0  					; set it up for transmission
		sta 	ControlParameters+0
		lda 	#transmitDataBuffer & $FF
		sta 	ControlParameters+1
		lda 	#transmitDataBuffer >> 8
		sta 	ControlParameters+2
		lda 	transmitDataSize
		sta 	ControlParameters+3
		stz 	ControlParameters+4
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
; 		21-01-24 		SFX always plays immediately.
;
; ************************************************************************************************

