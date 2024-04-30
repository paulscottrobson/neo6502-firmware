;
;		No direct addresses ¬!
;

		org $C000
s2:
		set r1,32767
		bk
		set r0,42
		bs s1
		bsl s1
		bra s1
		bk
		ld r1
		st r2
		ld @r3
		st @r4
		ldd @r5
		std @r6
		pop @r7
		stp @r8
		add r9
		sub r10
		popd @r11
		cpr r12
		inr r13
		dcr r14

		rtn
s1:		
		rtn
		bra  s1		
		bnc  s1		
		bc  s1		
		bp  s1		
		bm  s1		
		bz  s1		
		bnz  s1		
		bm1  s1		
		bnm1  s1		
		bk 
		rs
		bs 	s1
		bsl s1

