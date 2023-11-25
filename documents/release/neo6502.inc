ControlPort = $ff00
ReadCharacter = $ffee
WriteCharacter = $fff1
WaitMessage = $fff4
SendMessage = $fff7


SendMessage .macro
	jsr 	SendMessage
	.endm

WaitMessage .macro
	jsr 	WaitMessage
	.endm
