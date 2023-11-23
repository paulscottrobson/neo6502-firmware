	.if NEO!=0

FlipCharacterCase:        
	cmp 	#"A"
	bcc 	_FCCExit
	cmp 	#"Z"+1
	bcc 	_FCCFlip
	cmp 	#"a"
	bcc 	_FCCExit
	cmp 	#"z"+1
	bcs 	_FCCExit
_FCCFlip:	
	eor 	#$20
_FCCExit:
	rts	

	.endif