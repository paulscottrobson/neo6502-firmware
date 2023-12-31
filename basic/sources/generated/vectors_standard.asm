;
;	This file is automatically generated.
;
	.section code
StandardVectorTable:
	.word	EXPUnaryInlineString     ; $80 !!str
	.word	EXPUnaryDollar           ; $81 $
	.word	UnaryParenthesis         ; $82 (
	.word	UnaryRand                ; $83 rand(
	.word	UnaryRnd                 ; $84 rnd(
	.word	EXPUnaryJoypad           ; $85 joypad(
	.word	UnaryInt                 ; $86 int(
	.word	EXPUnaryTime             ; $87 time(
	.word	EXPUnaryEvent            ; $88 event(
	.word	UnaryInkey               ; $89 inkey$(
	.word	EXPUnaryAsc              ; $8a asc(
	.word	ChrUnary                 ; $8b chr$(
	.word	Unimplemented            ; $8c !!un11
	.word	EXPUnaryLen              ; $8d len(
	.word	UnaryAbs                 ; $8e abs(
	.word	UnarySgn                 ; $8f sgn(
	.word	Unimplemented            ; $90 hit(
	.word	Unimplemented            ; $91 !!un9
	.word	Unary_Mid                ; $92 mid$(
	.word	Unary_Left               ; $93 left$(
	.word	Unary_Right              ; $94 right$(
	.word	ConstTrue                ; $95 true
	.word	ConstFalse               ; $96 false
	.word	Unimplemented            ; $97 !!un4
	.word	Unimplemented            ; $98 !!un5
	.word	Unimplemented            ; $99 !!un6
	.word	Unimplemented            ; $9a !!un7
	.word	UnaryKey                 ; $9b key(
	.word	PeekUnary                ; $9c peek(
	.word	DeekUnary                ; $9d deek(
	.word	EXPUnaryAlloc            ; $9e alloc(
	.word	EXPUnaryMax              ; $9f max(
	.word	EXPUnaryMin              ; $a0 min(
	.word	Unimplemented            ; $a1 
	.word	Unimplemented            ; $a2 
	.word	Unimplemented            ; $a3 
	.word	Unimplemented            ; $a4 
	.word	Unimplemented            ; $a5 
	.word	Unimplemented            ; $a6 
	.word	Unimplemented            ; $a7 
	.word	Unimplemented            ; $a8 
	.word	Unimplemented            ; $a9 
	.word	Unimplemented            ; $aa 
	.word	Unimplemented            ; $ab 
	.word	Unimplemented            ; $ac 
	.word	Unimplemented            ; $ad 
	.word	Unimplemented            ; $ae 
	.word	Unimplemented            ; $af 
	.word	Command_WHILE            ; $b0 while
	.word	Command_WEND             ; $b1 wend
	.word	IfCommand                ; $b2 if
	.word	EndIf                    ; $b3 endif
	.word	Command_DO               ; $b4 do
	.word	Command_LOOP             ; $b5 loop
	.word	Command_REPEAT           ; $b6 repeat
	.word	Command_UNTIL            ; $b7 until
	.word	Command_SN1              ; $b8 proc
	.word	Command_ENDPROC          ; $b9 endproc
	.word	Command_FOR              ; $ba for
	.word	Command_NEXT             ; $bb next
	.word	Unimplemented            ; $bc case
	.word	Unimplemented            ; $bd endcase
	.word	Command_SN11             ; $be !!un1
	.word	Command_SN2              ; $bf then
	.word	RUNEndOfLine             ; $c0 !!end
	.word	Command_Shift1_Handler   ; $c1 !!sh1
	.word	Command_Shift2_Handler   ; $c2 !!sh2
	.word	Command_SN3              ; $c3 !!dec
	.word	Command_SN4              ; $c4 to
	.word	Command_Let              ; $c5 let
	.word	Command_Print            ; $c6 print
	.word	Command_Input            ; $c7 input
	.word	Command_Sys              ; $c8 sys
	.word	Command_EXIT             ; $c9 exit
	.word	Command_SN10             ; $ca ,
	.word	Command_SN7              ; $cb ;
	.word	Command_SN8              ; $cc :
	.word	Command_Comment          ; $cd '
	.word	Command_SN9              ; $ce )
	.word	Command_READ             ; $cf read
	.word	Command_DATA             ; $d0 data
	.word	ElseCode                 ; $d1 else
	.word	Unimplemented            ; $d2 when
	.word	Command_SN5              ; $d3 downto
	.word	PokeCommand              ; $d4 poke
	.word	DokeCommand              ; $d5 doke
	.word	Command_LOCAL            ; $d6 local
	.word	Command_CALL             ; $d7 call
	.word	Unimplemented            ; $d8 #
	.word	LabelCommand             ; $d9 .
	.word	Command_Line             ; $da line
	.word	Command_Rect             ; $db rect
	.word	Command_Move             ; $dc move
	.word	Command_Plot             ; $dd plot
	.word	Command_Ellipse          ; $de ellipse
	.word	Command_Text             ; $df text
	.word	Command_Image            ; $e0 image
	.word	Command_Sprite           ; $e1 sprite
	.send code
