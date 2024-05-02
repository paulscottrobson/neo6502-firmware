;
;		No direct addresses ¬!
;

		org $C000
s2:
		set 0,1
		set 12,$E000
		bk
		bs	 sbr
		bk
		bk
		bk
		bk

		word 0,0,0,0


sbr:		
		add 	0
		add 	0
		rs