; ***************************************************************************************
; ***************************************************************************************
;
;      Name :      wozmon.asm
;      Authors :   Paul Robson (paul@robsons.org.uk)
;      Date :      23rd November 2023
;      Reviewed :  No
;      Purpose :   Wozmon monitor variant, uses no zero page at all.
;
; ***************************************************************************************
; ***************************************************************************************

WMMode = WMModeAddress + 1 					; current mode of operation
WMCurrent = WMCurrentAddress + 1  			; current address
WMCurrentWrite = WMCurrentWriteAddress+1 	; current store address

lineBuffer = inputBuffer					; line stored here.

		.section code

; ***************************************************************************************
;
;										Start it up.
;
; ***************************************************************************************

WozMonStart: ;; [mon]
	jsr 	WriteCharacterInline			; write \
	.byte 	$5C
WMNewCommand: 					
	lda 	#13								; write CR
;
;		New Input Line to buffer
;	
WMStartOfLine:	
	ldx 	#$FF							; index into line.
WMNextCharacter:
	inx
WMNoIncrement:
	jsr 	WriteCharacter 					; echo it.	
	jsr 	ReadCharacter 					; get character
	and 	#$7F 							; drop bit 7
	cmp 	#'a'							; capitalise.
	bcc 	WMNotUpper
	eor 	#$20
WMNotUpper:
	sta 	lineBuffer,x 					; write character out.
	cmp		#' '							; is it ASCII ? 
	bcs 	WMNextCharacter 				; echo, store and next.
	cmp 	#8  							; if not BS end of line
	bne 	WMCommand
	lda 	#'_' 							; displayed character
	dex
	bmi 	WMStartOfLine 					; past the start
	bra 	WMNoIncrement 					; otherwise do next character without preincrement

; ***************************************************************************************
;
; 									Handle buffer commands
;
; ***************************************************************************************

WMCommand:	
	jsr 	WriteCharacter 					; write CR out
	ldx 	#$FF							; line pointer.
	lda 	#$80
WMSetMode: 									; set mode to A, next character
	adc 	#8 								; here $80 (display) $2E (list) $3A (store)
	sta 	WMMode	 						; $88 (display) $36 (list) $42 (store) maybe +1 if carry set.
;
;		Process next character group.
;
WMNextProcess:
	inx
WMNextProcessNoInc: 
	lda 	lineBuffer,x 					; next character
	cmp 	#32+1 							; new line/control character
	bcc 	WMNewCommand
	cmp 	#'.' 							; '.' and ':' set mode.
	beq 	WMSetMode
	bcc 	WMNextProcess 					; < '.' we ignore
	cmp 	#':' 							
	beq 	WMSetMode
	jsr 	WMCommandHandler

; ***************************************************************************************
;
;									Get hex number
;
; ***************************************************************************************

	stz 	WMNewHex 						; zero new hex
	stz 	WMNewHex+1	
	ldy 	#0 								; count of hex digits
WMHexLoop:
	cmp 	#'9'+1 							; Hex shift ?
	beq 	WHEndHex 						; if it is : then don't use it.
	bcc 	WHNotHexAF
	sbc 	#7 								; adjust.
WHNotHexAF:
	sec 									; shift into range.
	sbc 	#'0'	
	cmp 	#16 							; out of range
	bcs 	WHEndHex 						; decide what next

	pha 									; save 
	lda 	#4 								; shift newhex left.
WHShift:
	asl 	WMNewHex	 	
	rol 	WMNewHex+1
	dec 	a
	bne 	WHShift
	pla 									; OR the value in.
	ora 	WMNewHex
	sta 	WMNewHex
	iny 									; one more hex character.
	inx 									; look at the next character
	lda 	lineBuffer,x
	bra 	WMHexLoop

; ***************************************************************************************
;
;							Process hex number accordingly
;
; ***************************************************************************************

WHEndHex:
	cpy 	#0 								; received any hex digits
	beq 	WozMonStart 					; if so, restart,
	bit 	WMMode 							; is bit 6 set (e.g. : x 2, store mode)
	bvc 	WMNotStore
;
;						In ':' mode, e.g. storing to memory
;
	lda 	WMNewHex 						; byte to save
WMCurrentWriteAddress:  		
	sta 	@w $0000 						; write at the current address.
	inc 	WMCurrentWrite 					; increment store pointer and do next command
	bne 	WMNextProcessNoInc
	inc 	WMCurrentWrite+1
WMNextProcessNoInc2:	
	bra 	WMNextProcessNoInc

;
;		So either in single byte mode ($FF) or List mode ($74)
;
WMNotStore:
	
WMModeAddress:
	lda 	#$FF 							; get the mode
	bpl 	WMCheckCompare 					; if . mode then list from WMCurrent -> WMNewHex

	ldy 	#2 								; copy WMNewHex -> WMCurrent & WMCurrentWrite
WMCopyAddress:
	lda 	WMNewHex-1,y
	sta 	WMCurrent-1,y
	sta 	WMCurrentWrite-1,y
	dey
	bne 	WMCopyAddress
;
;		Print one location (WMCurrent), the address if xxx0 or xxx8 and increment that address.
;
WMNextDisplay:
	bne 	WMNoAddress
	jsr 	WriteCharacterInline
	.byte 	13
	lda 	WMCurrent+1 					; address
	jsr 	PrintHexByte
	lda 	WMCurrent
	jsr 	PrintHexByte
	jsr 	WriteCharacterInline
	.byte 	':'
WMNoAddress:	
	lda 	#' ' 							; print space
	jsr 	WriteCharacter
WMCurrentAddress:
	lda 	@w $0000 						; print current address
	jsr 	PrintHexByte

WMCheckCompare:
	lda 	WMCurrent 						; check current < newhex 
	cmp 	WMNewHex
	lda 	WMCurrent+1
	sbc 	WMNewHex+1
	bcs 	WMNextProcessNoInc2

	inc 	WMCurrent 						; and bump the pointer
	bne 	WMCheckAddressReqd
	inc 	WMCurrent+1
WMCheckAddressReqd:
	lda 	WMCurrent
	and 	#7
	bra 	WMNextDisplay

WMNewHex:
	.word 	0

; ***************************************************************************************
;
;										Special commands
;
; ***************************************************************************************

WMCommandHandler:
	cmp 	#'R' 							; R command from WozMon
	beq 	WMRunCode
	cmp 	#'N' 							; N runs NeoBASIC
	beq 	WMRunBasic
	rts
	
WMRunCode:
 	jmp 	(WMCurrent)

WMRunBasic:
	jmp 	WarmStart

; ***************************************************************************************
;
;								Print A as a byte
;
; ***************************************************************************************

PrintHexByte:
	pha
	lsr 	a
	lsr 	a
	lsr 	a
	lsr 	a
	jsr 	PrintHexNibble
	pla

PrintHexNibble:	
	and 	#15
	cmp 	#10
	bcc 	nothex
	adc 	#6
nothex:
	adc 	#48
	jsr 	WriteCharacter
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

