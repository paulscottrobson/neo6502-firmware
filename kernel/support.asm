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

		sta 	KSMReturnA+1 				; save A reloaded at end.
		pla 								; pop return address to the read instruction
		sta 	KSMRAddress+1 			
		pla
		sta 	KSMRAddress+2

		jsr 	KSMReadAdvance 				; read the command.
		pha 								; save, write it after the command.
		jsr 	KSMReadAdvance 				; read the function number
		sta 	DFunction 					
		pla
		sta 	DCommand 					; save the command, starting the message.
KSMAdvanceReturn:		
		jsr 	KSMReadAdvance 				; use jmp indirect so advance it again.
KSMReturnA:		
		lda 	#$FF 						; original A value
		jmp 	(KSMRAddress+1)

KSMReadAdvance:
		inc 	KSMRAddress+1 				; pre-inc because of 6502 RTS behaviour
		bne 	KSMRAddress
		inc 	KSMRAddress+2
KSMRAddress:
		lda 	$FFFF 						; holds the return address.
		rts

; ***************************************************************************************
;
;							Write character inlined (following byte)
;
; ***************************************************************************************

KWriteCharacterInLine:
		sta 	KSMReturnA+1 				; save A reloaded at end.
		pla 								; pop return address to the read instruction
		sta 	KSMRAddress+1 			
		pla
		sta 	KSMRAddress+2
		jsr 	KSMReadAdvance 				; output a character
		jsr 	KWriteCharacter
		bra 	KSMAdvanceReturn

; ***************************************************************************************
;
;							Write A to the current console
;
; ***************************************************************************************

KWriteCharacter:	
		pha
		sta 	DParameters 				; sending A
		lda 	#6
		sta 	DFunction 					; we don't inline it because inline uses it
		lda 	#2
		sta 	DCommand
		pla
		rts

; ***************************************************************************************
;
;							Read keystroke from the console
;
; ***************************************************************************************

KReadCharacter:
		jsr 	KWriteCharacterInline		; control X (reverse at cursor)
		.byte 	24
_KRCWait:		
		jsr 	KSendMessage  				; check serial interface.
		.byte 	1,5
		jsr 	KWaitMessage

		jsr 	KSendMessage 				; send command 2,1 read keyboard
		.byte 	2,1
		jsr 	KWaitMessage
		lda 	DParameters 				; read result
		beq 	_KRCWait 					; no key, yet.
		jsr 	KWriteCharacterInline		; control X (reverse at cursor)
		.byte 	24
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

; ***************************************************************************************
;
;									Screen Editor input
;
; ***************************************************************************************

KReadLine:
		phx 								; save target address
		phy
_KRLLoop:
		jsr 	KReadCharacter 				; read and echo character
		cmp 	#13 						; exit if CR pressed
		beq 	_KRLExit
		jsr 	KWriteCharacter
		bra 	_KRLLoop
_KRLExit:
		ply
		plx
		stx 	DParameters 				; where the string goes.
		sty 	DParameters+1
		jsr 	KSendMessage 				; send the 'get line' message.
		.byte 	2,3
		jsr 	KWaitMessage
		rts				
