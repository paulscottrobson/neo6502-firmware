; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		inputprintfile.asm
;		Purpose:	File driven input/print (e.g. input# print#)
;		Created:	20th June 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
; ************************************************************************************************
;
;		The format for files is
;
;			String values : <length> <string data>
;			Number : 		$FF 	 <type> <n0> <n1> <n2> <n3>
;
;			Strings cannot be 255 bytes long hence the use of the $FF marker.
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;						Entry point for INPUT# ; points to #
;
; ************************************************************************************************

InputFileHandler:
		iny 								; consume #
		jsr 	FHIdentifyChannel 			; get the current channel.
_IFHLoop:
		lda 	(codePtr),y 				; check for ,
		cmp 	#KWD_COMMA 					; not found, then exit.
		bne 	_IFHExit
		iny 								; consume comma

		ldx	 	#0
		jsr 	EXPEvaluateExpressionAtX	; get an expression, should be a variable
		lda 	XSControl,x 				; check if reference (e.g. a variable)
		and 	#XS_ISVARIABLE
		beq 	_IFHType 					; if not, then we have an error.
		lda 	XSControl,x 				; is it a string
		bmi 	_IFHString
;
;		Input a number
;
		jsr 	IFHReadByte 				; check it is a number in the stream
		cmp 	#$FF
		bne 	_IFHType

		ldx 	#1 							; read number to slot 1
		jsr 	IFHReadByte
		sta 	XSControl,x
		jsr 	IFHReadByte
		sta 	XSNumber0,x
		jsr 	IFHReadByte
		sta 	XSNumber1,x
		jsr 	IFHReadByte
		sta 	XSNumber2,x
		jsr 	IFHReadByte
		sta 	XSNumber3,x
		dex 								; now do assignment code
		jsr 	AssignValueToReference
		bra 	_IFHLoop
;
;		Input a string
;
_IFHString:
		jsr 	IFHReadByte 				; check it is a string in the stream
		cmp 	#$FF 						; this will be the length of the string to read in.
		beq 	_IFHType 	

		pha 								; save length
		ldx 	#1 							; slot to read into.
		jsr 	StringTempAllocate
		plx 								; this is a count now.
_IFHSLoop:	
		cpx 	#0 							; done ?
		beq 	_IFHSReadAll
		dex  								; do one more byte
		jsr 	IFHReadByte		
		jsr 	StringTempWrite
		bra 	_IFHSLoop
_IFHSReadAll:		
		ldx 	#0							; now do assignment code
		jsr 	AssignValueToReference
		bra 	_IFHLoop


_IFHType:
		.error_type
_IFHExit:
		rts


; ************************************************************************************************
;
;						Entry point for OUTPUT# ; points to #
;
; ************************************************************************************************

OutputFileHandler:
		iny 								; consume #
		jsr 	FHIdentifyChannel 			; get the current channel.

_OFHLoop:
		lda 	(codePtr),y 				; check for ,
		cmp 	#KWD_COMMA 					; not found, then exit.
		bne 	_OFHExit
		iny 								; consume comma

		ldx	 	#0
		jsr 	EXPEvaluateExpressionAtX	; get a value
		jsr 	DereferenceTOS				; dereference it
		bit 	XSControl,x	 				; check if string
		bmi 	_OFHString
;
;		Output number
;
		lda 	#$FF 						; output number marker
		jsr 	OFHWriteByte 				
		lda 	XSControl 					; output actual number
		jsr 	OFHWriteByte
		lda 	XSNumber0
		jsr 	OFHWriteByte
		lda 	XSNumber1
		jsr 	OFHWriteByte
		lda 	XSNumber2
		jsr 	OFHWriteByte
		lda 	XSNumber3
		jsr 	OFHWriteByte
		bra 	_OFHLoop
;
;		Output string
;
_OFHString:
		lda 	XSNumber0 					; copy address to zTemp0
		sta 	zTemp0
		lda 	XSNumber1
		sta 	zTemp0+1
		lda 	(zTemp0)					; length to read.
		pha
		jsr 	OFHWriteByte 				; output it
		plx 								; length in X
		beq 	_OFHLoop 					; zero length.
		phy
		ldy 	#1
_OFHSLoop:
		lda 	(zTemp0),y 					; output the string.
		iny
		jsr 	OFHWriteByte
		dex
		bne 	_OFHSLoop		
		ply
		bra 	_OFHLoop
_OFHExit:
		rts

; ************************************************************************************************
;
;										Read Byte to A
;
; ************************************************************************************************

IFHReadByte:
		jsr 	SetUpReadWrite 				; set up to read/write one byte
		.DoSendMessage 						; read byte to file.
		.byte 	3,8
		.DoWaitMessage		
		lda 	ControlError 				; file I/O error ?
		bne 	_OFHRError
		lda 	fhBuffer 					; return value
		rts
_OFHRError:
		.error_file		

; ************************************************************************************************
;
;										Write byte in A
;
; ************************************************************************************************

OFHWriteByte:
		sta 	fhBuffer 					; save in the buffer
		jsr 	SetUpReadWrite 				; set up to read/write one byte
		.DoSendMessage 						; write to file.
		.byte 	3,9
		.DoWaitMessage		
		rts

; ************************************************************************************************
;
;								Set up for read/write one byte
;		
; ************************************************************************************************

SetUpReadWrite:
		lda 	fhCurrentChannel 			; P0 channel
		sta 	ControlParameters+0
		lda 	#fhBuffer & $FF 			; P1.2 buffer
		sta 	ControlParameters+1
		lda 	#fhBuffer >> 8
		sta 	ControlParameters+2
		lda 	#1 							; P3.4 size
		sta 	ControlParameters+3
		stz 	ControlParameters+4
		rts

; ************************************************************************************************
;
;										 Set up channel
;
; ************************************************************************************************

FHIdentifyChannel:
		ldx 	#0 							; get channel number.
		jsr 	EXPEvalInteger8
		sta 	fhCurrentChannel 			; and save it
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

