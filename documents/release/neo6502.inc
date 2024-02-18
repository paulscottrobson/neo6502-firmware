ControlPort = $ff00
LoadExtended = $ffe8
ReadLine = $ffeb
ReadCharacter = $ffee
WriteCharacter = $fff1
WaitMessage = $fff4
SendMessage = $fff7


DoSendMessage .macro
	jsr 	SendMessage
	.endm

DoWaitMessage .macro
	jsr 	WaitMessage
	.endm
