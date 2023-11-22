; ***************************************************************************************
; ***************************************************************************************
;
;      Name :      support.asm
;      Authors :   Paul Robson (paul@robsons.org.uk)
;      Date :      23rd November 2023
;      Reviewed :  No
;      Purpose :   Support functions
;
; ***************************************************************************************
; ***************************************************************************************

; ***************************************************************************************
;
;					  Send message in the following 2 bytes
;
; ***************************************************************************************

KSendMessage:
		jsr		KWaitMessage 				; wait for command to be released.

		sta 	_KSMReturnA+1 				; save A reloaded at end.

		pla 								; pop return address to the read instruction
		sta 	_KSMRAddress+1 			
		pla
		sta 	_KSMRAddress+2

		jsr 	_KSMReadAdvance 			; read the command.
		pha 								; save, write it after the command.
		jsr 	_KSMReadAdvance 			; read the function number
		sta 	DFunction 					
		pla
		sta 	DCommand 					; save the command, starting the message.
		jsr 	_KSMReadAdvance 			; use jmp indirect so advance it again.

_KSMReturnA:		
		lda 	#$FF 						; original A value
		jmp 	(_KSMRAddress+1)

_KSMReadAdvance:
		inc 	_KSMRAddress+1 				; pre-inc because of 6502 RTS behaviour
		bne 	_KSMRAddress
		inc 	_KSMRAddress+2
_KSMRAddress:
		lda 	$FFFF 						; holds the return address.
		rts

; ***************************************************************************************
;
;							Write A to the current console
;
; ***************************************************************************************

KWriteCharacter:	
		sta 	DParameters 				; sending A
		jsr 	KSendMessage 				; command 1,0 write character
		.byte 	1,0
		rts

; ***************************************************************************************
;
;							Read keystroke from the console
;
; ***************************************************************************************

KReadCharacter:
		jsr 	KSendMessage 				; send command 1,1 read keyboard
		.byte 	1,1
		lda 	DParameters 				; read result
		beq 	KReadCharacter 				; no key, yet.
		rts

; ***************************************************************************************
;
;			Wait for the handler process to acknowledge or return a value
;
; ***************************************************************************************

KWaitMessage:
		pha
KWaitMessage1:
		lda 	DCommand 					; wait until the handler has finished.
		bne 	KWaitMessage1
		pla
		rts
	