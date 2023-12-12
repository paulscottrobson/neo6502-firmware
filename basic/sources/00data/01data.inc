; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		00data.inc
;		Purpose:	Global Data Objects
;		Created:	25th November 2023
;		Reviewed:	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

ControlCommand = ControlPort+0
ControlFunction = ControlPort+1
ControlError = ControlPort+2
ControlStatus = ControlPort+3
ControlParameters = ControlPort+4

VariableA = Program+2
VariableO = Program+9
VariableP = Program+16
VariableX = Program+23
VariableY = Program+30

; ************************************************************************************************
;	
;											Stack
;
; ************************************************************************************************

StackSize = 8

XS_TYPEMASK = $C0
XS_TYPEBIT = $80
XS_STRING = $80
XS_ISVARIABLE = $20
XS_ISARRAY = $10
XS_ISPROC = $C0

		.section zeropage
XSStack:
		
XSControl:	
		.fill 	StackSize		
XSNumber0:
		.fill 	StackSize
XSNumber1:
		.fill 	StackSize
XSNumber2:
		.fill 	StackSize
XSNumber3:
		.fill 	StackSize

	
		.send zeropage

; ************************************************************************************************
;	
;											Zero Page
;
; ************************************************************************************************

		.section zeropage

CodePtr: 									; basic code
		.fill 	2 
basicStack: 								; basic stack
		.fill 	2		
zsTemp: 									; general string pointer
		.fill 	2
zTemp0:	
		.fill 	2
zTemp1:
		.fill 	2		
		
		.send zeropage

; ************************************************************************************************
;	
;										Normal Memory
;
; ************************************************************************************************

		.section storage
freeMemory: 								; free memory address
		.fill 	2
ERRLine: 									; line number for errors.
		.fill 	2		
InputFlag: 									; Flag indicating input/print.
		.fill 	1
stringInitialised:							; temp string alloc set up
		.fill 	1		
stringMemory: 								; highest string memory concreted.
		.fill 	2		
stringTempPointer: 							; take temp string from here.
		.fill 	2	
basicStackEnd: 								; bottom limit of stack.
		.fill 	1
tempBuffer: 								; generic buffer.
		.fill 	32
		.send storage

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

