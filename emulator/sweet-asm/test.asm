;
;		No direct addresses ¬!
;

		org $C000
s2:
		set 0,1
		set 12,$E000
		bk
		byte $0E,$13
		