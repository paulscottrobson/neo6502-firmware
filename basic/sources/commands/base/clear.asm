; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		clear.asm
;		Purpose:	Clear variables / general reset
;		Created:	3rd December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										CLR Command
;
; ************************************************************************************************

		.section code

Command_CLEAR:	;; [clear]
		lda 	(codePtr),y 				; followed by : or EOL
		cmp 	#KWD_COLON
		beq 	_CCSetFullMemory
		cmp 	#KWD_SYS_END
		beq 	_CCSetFullMemory
		;
		ldx 	#0 							; where does himem go.
		jsr 	EXPEvalInteger16
		lda 	XSNumber1 					; check range
		cmp 	#HIGHMEMORY >> 8  			; past end of memory
		bcs 	_CCRange
											; Program space, stack space, and 2k required.
		cmp 	#((Program >> 8)+StackPages+(2048 >> 8))
		bcc 	_CCRange
		jsr 	ClearCodeSetMemoryA
		rts
_CCRange:
		.error_range		
_CCSetFullMemory:		
		jsr 	ClearCode
		rts

; ************************************************************************************************
;
;									Do CLEAR functionality
;
; ************************************************************************************************


ClearCode:
		lda 	#HIGHMEMORY >> 8 			; reset to full memory.
ClearCodeSetMemoryA:
		sta 	highMemoryUpper				; remember the high page
		phy
		;
		;		Reset memory and variables
		;
		jsr 	ClearResetFreeMemory
		jsr 	ClearVariables
		;
		;		Reset stack
		;	
		lda 	highMemoryUpper
		jsr 	StackReset 			
		;
		;		Initialise string usage.
		;
		jsr 	StringSystemInitialise 		
		;
		;		Scan for procedures
		;
		jsr 	ScanProcedures
		;
		;		Reset READ/DATA
		;
		jsr 	Command_RESTORE
		;
		;		Reset Graphics
		;
		jsr 	GraphicsReset
		;
		;		Reset sprite data and clear sprite layer
		;
		.DoSendMessage 					
		.byte 	6,1
		.DoWaitMessage
		;
		;		Reset turtle system
		;
		jsr 	TurtleResetSystem
		;
		;		Clearer the error handler link
		;
		stz		errCode
		stz 	errCode+1
		;
		; 		Reset the I/O system
		;
		.DoSendMessage 					
		.byte 	10,1
		.DoWaitMessage		
		;
		; 		Reset to Degrees
		;
		lda 	#1
		sta 	ControlParameters+0
		.DoSendMessage 					
		.byte 	4,35
		.DoWaitMessage		
		ply
		rts

; ************************************************************************************************
;
;							Reset free memory pointer, put program end in zTemp0
;
; ************************************************************************************************

ClearResetFreeMemory:
		lda 	Program 					; number of pages of code.
		clc
		adc 	#(Program >> 8) 			; set zTemp0 to start of program
		sta 	zTemp0+1
		stz 	zTemp0
_CCFindEnd:
		lda 	(zTemp0) 					; found end ?
		beq 	_CCFoundEnd
		clc
		adc 	zTemp0
		sta 	zTemp0
		bcc 	_CCFindEnd
		inc 	zTemp0+1
		bra 	_CCFindEnd
_CCFoundEnd:
		clc 								; space for allocating memory after program.
		lda 	zTemp0
		adc 	#8
		sta 	freeMemory
		lda 	zTemp0+1
		adc 	#0
		sta 	freeMemory+1
		rts

; ************************************************************************************************
;
;									Allocate Memory & Clear it.
;
; ************************************************************************************************

MemoryAllocateYA:
		sta 	zTemp0 						; save LSB of count in zTemp0

		lda 	freeMemory+1 				; save start address on stack
		sta 	zTemp1+1 					; & copy to zTemp1.
		pha
		lda 	freeMemory
		sta 	zTemp1
		pha

		clc 								; add Y:ZTemp0 to free memory
		adc 	zTemp0
		sta 	freeMemory
		tya
		adc 	freeMemory+1
		sta 	freeMemory+1
		bcs 	_MYAError 					; overflow
		jsr 	MemoryCheck 				; past string pos
		bcs 	_MYAError

_MAYAClear: 								; erase the memory.
		lda 	zTemp1
		cmp 	freeMemory
		bne 	_MAYAZero
		lda 	zTemp1+1
		cmp 	freeMemory+1
		beq 	_MAYAExit
_MAYAZero:		
		lda 	#$00
		sta 	(zTemp1)
		inc 	zTemp1
		bne 	_MAYAClear
		inc 	zTemp1+1
		bra 	_MAYAClear

_MAYAExit:
		pla 								; return value in YA
		ply 									
		rts
_MYAError:
		.error_memory

; ************************************************************************************************
;
;								Check out of memory, CS if so.
;
; ************************************************************************************************

MemoryCheck:
		lda 	freeMemory+1
		clc
		adc 	#6
		cmp 	stringMemory+1
		rts

; ************************************************************************************************
;
;										Clear all variables
;
; ************************************************************************************************

ClearVariables:
		lda 	#Program >> 8 				; start of identifier link
		sta 	zTemp0+1
		lda 	#1	
		sta 	zTemp0
_CVLoop:
		lda 	(zTemp0) 					; check complete
		beq 	_CVExit

		ldy 	#1 							; zero the variable values.
		lda 	#0
		sta 	(zTemp0),y
		iny
		sta 	(zTemp0),y
		iny
		sta 	(zTemp0),y
		iny
		sta 	(zTemp0),y

		iny 								; set the type to integer or string, preserving array
		lda 	(zTemp0),y
		and 	#$80+$10
		sta 	(zTemp0),y

		clc
		lda 	(zTemp0)
		adc 	zTemp0
		sta 	zTemp0
		bcc 	_CVLoop
		inc 	zTemp0+1
		bra 	_CVLoop
_CVExit:
		rts				

		.send 	code

; ************************************************************************************************
;
;									Changes and Updates
;
; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;		16-01-24		CLEAR was looping (Y was reset to 3 causing it to loop infinitely)
;		23-01-24 		CLEAR resets all sprite settings as well.
; 		30-01-24 		CLEAR clears the sprite layer to maintain consistency.
;		22-02-24 		CLEAR initialises UEXT
;
; ************************************************************************************************

