P = &6000:OPT 3
[
.start
bcc start
bcs start
beq start
bit #&89
bit &05
bit &0d0e
bit &34, x
bit &3c3c, x
bmi start
bne start
bpl start
bra start
brk
bvc start
bvs start
adc #&09
adc &05
adc &0d0e
adc &15, x
adc &1919, y
adc &1d1e, x
adc (&01,x)
adc (&11), y
adc (&12)
and #&09
and &05
and &0d0e
and &15, x
and &1919, y
and &1d1e, x
and (&01,x)
and (&11), y
and (&12)
asl
asl &05
asl &0d0e
asl &15, x
asl &1d1e, x
clc
cld
cli
clv
cmp #&09
cmp &05
cmp &0d0e
cmp &15, x
cmp &1919, y
cmp &1d1e, x
cmp (&01,x)
cmp (&11), y
cmp (&12)
cpx #&09
cpx &05
cpx &0d0e
cpy #&09
cpy &05
cpy &0d0e
dec
dec &05
dec &0d0e
dec &15, x
dec &1d1e, x
dex
dey
eor #&09
eor &05
eor &0d0e
eor &15, x
eor &1919, y
eor &1d1e, x
eor (&01,x)
eor (&11), y
eor (&12)
inc
inc &05
inc &0d0e
inc &15, x
inc &1d1e, x
inx
iny
jmp &0d0e
jmp (&6c6c)
jmp (&7c7c,x)
jsr &0d0e
lda #&09
lda &05
lda &0d0e
lda &15, x
lda &1919, y
lda &1d1e, x
lda (&01,x)
lda (&11), y
lda (&12)
ldx #&09
ldx &05
ldx &0d0e
ldx &1919, y
ldx &96, y
ldy #&09
ldy &05
ldy &0d0e
ldy &15, x
ldy &1d1e, x
lsr
lsr &05
lsr &0d0e
lsr &15, x
lsr &1d1e, x
nop
ora #&09
ora &05
ora &0d0e
ora &15, x
ora &1919, y
ora &1d1e, x
ora (&01,x)
ora (&11), y
ora (&12)
pha
php
phx
phy
pla
plp
plx
ply
rol
rol &05
rol &0d0e
rol &15, x
rol &1d1e, x
ror
ror &05
ror &0d0e
ror &15, x
ror &1d1e, x
rti
rts
sbc #&09
sbc &05
sbc &0d0e
sbc &15, x
sbc &1919, y
sbc &1d1e, x
sbc (&01,x)
sbc (&11), y
sbc (&12)
sec
sed
sei
sta &05
sta &0d0e
sta &15, x
sta &1919, y
sta &1d1e, x
sta (&01,x)
sta (&11), y
sta (&12)
stx &05
stx &0d0e
stx &96, y
sty &05
sty &0d0e
sty &15, x
stz &04
stz &0c0c
stz &34, x
stz &3c3c, x
tax
tay
trb &04
trb &0c0c
tsb &04
tsb &0c0c
tsx
txa
txs
tya
]
sys &FFFF