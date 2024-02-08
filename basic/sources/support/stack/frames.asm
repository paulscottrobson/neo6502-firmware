; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		frames.asm
;		Purpose:	Open/Close Frames on the BASIC stack
;		Created:	5th December2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;		Open a frame. A contains the identifier in the upper nibl, and the bytes to claim is
; 		double the lower nibble (includes frame marker) doubled
;
; ************************************************************************************************

StackOpen:
		pha 								; save frame byte
		and 	#$0F 						; shows the bytes to subtract.
		asl 	a 							; claim twice this for storage
		;
		eor 	#$FF 						; 2's complement addition
		sec 								; so basically subtracting from
		adc 	basicStack 	 				; basicStack
		sta 	basicStack
		bcs 	_SONoBorrow

		dec 	basicStack+1
		lda 	basicStack+1 				; reached the bottom ?
		cmp 	basicStackEnd
		beq 	_SOMemory
_SONoBorrow:
		pla 								; get marker back and write at TOS
		sta 	(basicStack)		
		rts
	
_SOMemory:
		.error_stack

; ************************************************************************************************
;
;										Remove Locals
;
; ************************************************************************************************
			
StackRemoveLocals:
		lda 	(basicStack) 				; locals to unstack ?
		cmp 	#STK_LOCALINFO
		bcs 	_SRLExit
		cmp 	#STK_REFPARAM 				; reference parameter
		beq 	_SRLIsReference
		jsr 	LocalPopValue
		bra 	StackRemoveLocals
_SRLIsReference:
		jsr 	CCCopyReferenceBack
		bra 	StackRemoveLocals
				
_SRLExit:
		rts		

; ************************************************************************************************
;
;										Close a frame
;
; ************************************************************************************************

StackClose:
		lda 	(basicStack) 				; get TOS marker
		and 	#$0F 						; bytes to add back
		asl 	a 							; claim twice this.
		adc 	basicStack 					; add to the stack pointer.
		sta 	basicStack
		bcc 	_SCExit
		inc 	basicStack+1
_SCExit:
		rts		

; ************************************************************************************************
;
;								Check in Frame A, if not report Error
;
; ************************************************************************************************

StackCheckFrame:
		cmp		(basicStack) 				; check  frames match
		bne 	_SCFError 					; different, we have structures mixed up
		rts
_SCFError:
		.error_structure
				
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
