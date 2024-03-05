/*
 *  cpu_opcodes.h - 6510 CPU emulation opcode routines
 *
 *  Frodo (C) Copyright 1994-2004 Christian Bauer
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


/*
 *  Routines for documented opcodes
 */

// Load group
case 0xa9:    // LDA #imm
    read_byte_imm(RA);
    set_nz(RA);
    break;

case 0xa5:    // LDA zero
    read_byte_zero(RA);
    set_nz(RA);
    break;

case 0xb5:    // LDA zero,X
    read_byte_zero_x(RA);
    set_nz(RA);
    break;

case 0xad:    // LDA abs
    read_byte_abs(RA);
    set_nz(RA);
    break;

case 0xbd:    // LDA abs,X
    read_byte_abs_x(RA);
    set_nz(RA);
    break;

case 0xb9:    // LDA abs,Y
    read_byte_abs_y(RA);
    set_nz(RA);
    break;

case 0xa1:    // LDA (ind,X)
    read_byte_ind_x(RA);
    set_nz(RA);
    break;
        
case 0xb1:    // LDA (ind),Y
    read_byte_ind_y(RA);
    set_nz(RA);
    break;

case 0xa2:    // LDX #imm
    read_byte_imm(RX);
    set_nz(RX);
    break;

case 0xa6:    // LDX zero
    read_byte_zero(RX);
    set_nz(RX);
    break;

case 0xb6:    // LDX zero,Y
    read_byte_zero_y(RX);
    set_nz(RX);
    break;

case 0xae:    // LDX abs
    read_byte_abs(RX);
    set_nz(RX);
    break;

case 0xbe:    // LDX abs,Y
    read_byte_abs_y(RX);
    set_nz(RX);
    break;

case 0xa0:    // LDY #imm
    read_byte_imm(RY);
    set_nz(RY);
    break;

case 0xa4:    // LDY zero
    read_byte_zero(RY);
    set_nz(RY);
    break;

case 0xb4:    // LDY zero,X
    read_byte_zero_x(RY);
    set_nz(RY);
    break;

case 0xac:    // LDY abs
    read_byte_abs(RY);
    set_nz(RY);
    break;

case 0xbc:    // LDY abs,X
    read_byte_abs_x(RY);
    set_nz(RY);
    break;


// Store group
case 0x85:    // STA zero
    read_adr_zero;
    write_zp(ADR, RA); next_cycle;
    break;

case 0x95:    // STA zero,X
    read_adr_zero_x;
    write_zp(ADR, RA); next_cycle;
    break;

case 0x8d:    // STA abs
    read_adr_abs;
    write_byte(ADR, RA); next_cycle;
    break;

case 0x9d:    // STA abs,X
    read_adr_abs_x;
    write_byte(ADR, RA); next_cycle;
    break;

case 0x99:    // STA abs,Y
    read_adr_abs_y;
    write_byte(ADR, RA); next_cycle;
    break;

case 0x81:    // STA (ind,X)
    read_adr_ind_x;
    write_byte(ADR, RA); next_cycle;
    break;

case 0x91:    // STA (ind),Y
    read_adr_ind_y;
    write_byte(ADR, RA); next_cycle;
    break;

case 0x86:    // STX zero
    read_adr_zero;
    write_zp(ADR, RX); next_cycle;
    break;

case 0x96:    // STX zero,Y
    read_adr_zero_y;
    write_zp(ADR, RX); next_cycle;
    break;

case 0x8e:    // STX abs
    read_adr_abs;
    write_byte(ADR, RX); next_cycle;
    break;

case 0x84:    // STY zero
    read_adr_zero;
    write_zp(ADR, RY); next_cycle;
    break;

case 0x94:    // STY zero,X
    read_adr_zero_x;
    write_zp(ADR, RY); next_cycle;
    break;

case 0x8c:    // STY abs
    read_adr_abs;
    write_byte(ADR, RY); next_cycle;
    break;


// Transfer group
case 0xaa:    // TAX
    set_nz(RX = RA);
    read_idle_opcode; next_cycle;
    break;

case 0x8a:    // TXA
    set_nz(RA = RX);
    read_idle_opcode; next_cycle;
    break;

case 0xa8:    // TAY
    set_nz(RY = RA);
    read_idle_opcode; next_cycle;
    break;

case 0x98:    // TYA
    set_nz(RA = RY);
    read_idle_opcode; next_cycle;
    break;

case 0xba:    // TSX
    set_nz(RX = RSP);
    read_idle_opcode; next_cycle;
    break;

case 0x9a:    // TXS
    RSP = RX;
    read_idle_opcode; next_cycle;
    break;


// Arithmetic group
case 0x69: {// ADC #imm
    uint8 t;
    read_byte_imm(t);
    do_adc(t);
}
case 0x65: {// ADC zero
    uint8 t;
    read_byte_zero(t);
    do_adc(t);
}
case 0x75: {// ADC zero,X
    uint8 t;
    read_byte_zero_x(t);
    do_adc(t);
}
case 0x6d: {// ADC abs
    uint8 t;
    read_byte_abs(t);
    do_adc(t);
}
case 0x7d: {// ADC abs,X
    uint8 t;
    read_byte_abs_x(t);
    do_adc(t);
}
case 0x79: {// ADC abs,Y
    uint8 t;
    read_byte_abs_y(t);
    do_adc(t);
}
case 0x61: {// ADC (ind,X)
    uint8 t;
    read_byte_ind_x(t);
    do_adc(t);
}
case 0x71: {// ADC (ind),Y
    uint8 t;
    read_byte_ind_y(t);
    do_adc(t);
}
case 0xe9:    // SBC #imm
case 0xeb: {// Undocumented opcode
    uint8 t;
    read_byte_imm(t);
    do_sbc(t);
}
case 0xe5: {// SBC zero
    uint8 t;
    read_byte_zero(t);
    do_sbc(t);
}
case 0xf5: {// SBC zero,X
    uint8 t;
    read_byte_zero_x(t);
    do_sbc(t);
}
case 0xed: {// SBC abs
    uint8 t;
    read_byte_abs(t);
    do_sbc(t);
}
case 0xfd: {// SBC abs,X
    uint8 t;
    read_byte_abs_x(t);
    do_sbc(t);
}
case 0xf9: {// SBC abs,Y
    uint8 t;
    read_byte_abs_y(t);
    do_sbc(t);
}
case 0xe1: {// SBC (ind,X)
    uint8 t;
    read_byte_ind_x(t);
    do_sbc(t);
}
case 0xf1: {// SBC (ind),Y
    uint8 t;
    read_byte_ind_y(t);
    do_sbc(t);
}


// Increment/decrement group
case 0xe8:    // INX
    set_nz(++RX);
    read_idle_opcode; next_cycle;
    break;

case 0xca:    // DEX
    set_nz(--RX);
    read_idle_opcode; next_cycle;
    break;

case 0xc8:    // INY
    set_nz(++RY);
    read_idle_opcode; next_cycle;
    break;

case 0x88:    // DEY
    set_nz(--RY);
    read_idle_opcode; next_cycle;
    break;

case 0xe6: {// INC zero
    unsigned int t;
    read_byte_zero(t);
    next_cycle; write_zp(ADR, set_nz(t + 1)); next_cycle;
    break;
}
case 0xf6: {// INC zero,X
    unsigned int t;
    read_byte_zero_x(t);
    next_cycle; write_zp(ADR, set_nz(t + 1)); next_cycle;
    break;
}
case 0xee: {// INC abs
    unsigned int t;
    read_byte_abs(t);
    next_cycle; write_byte_rmw(ADR, set_nz(t + 1)); next_cycle;
    break;
}
case 0xfe: {// INC abs,X
    read_adr_abs_x;
    unsigned int t = read_byte(ADR); next_cycle;
    next_cycle; write_byte_rmw(ADR, set_nz(t + 1)); next_cycle;
    break;
}
case 0xc6: {// DEC zero
    unsigned int t;
    read_byte_zero(t);
    next_cycle; write_zp(ADR, set_nz(t - 1)); next_cycle;
    break;
}
case 0xd6: {// DEC zero,X
    unsigned int t;
    read_byte_zero_x(t);
    next_cycle; write_zp(ADR, set_nz(t - 1)); next_cycle;
    break;
}
case 0xce: {// DEC abs
    unsigned int t;
    read_byte_abs(t);
    next_cycle; write_byte_rmw(ADR, set_nz(t - 1)); next_cycle;
    break;
}
case 0xde: {// DEC abs,X
    read_adr_abs_x;
    unsigned int t = read_byte(ADR); next_cycle;
    next_cycle; write_byte_rmw(ADR, set_nz(t - 1)); next_cycle;
    break;
}


// Logic group
case 0x29: {// AND #imm
    uint8 t;
    read_byte_imm(t);
    set_nz(RA &= t);
    break;
}
case 0x25: {// AND zero
    uint8 t;
    read_byte_zero(t);
    set_nz(RA &= t);
    break;
}
case 0x35: {// AND zero,X
    uint8 t;
    read_byte_zero_x(t);
    set_nz(RA &= t);
    break;
}
case 0x2d: {// AND abs
    uint8 t;
    read_byte_abs(t);
    set_nz(RA &= t);
    break;
}
case 0x3d: {// AND abs,X
    uint8 t;
    read_byte_abs_x(t);
    set_nz(RA &= t);
    break;
}
case 0x39: {// AND abs,Y
    uint8 t;
    read_byte_abs_y(t);
    set_nz(RA &= t);
    break;
}
case 0x21: {// AND (ind,X)
    uint8 t;
    read_byte_ind_x(t);
    set_nz(RA &= t);
    break;
}
case 0x31: {// AND (ind),Y
    uint8 t;
    read_byte_ind_y(t);
    set_nz(RA &= t);
    break;
}
case 0x09: {// ORA #imm
    uint8 t;
    read_byte_imm(t);
    set_nz(RA |= t);
    break;
}
case 0x05: {// ORA zero
    uint8 t;
    read_byte_zero(t);
    set_nz(RA |= t);
    break;
}
case 0x15: {// ORA zero,X
    uint8 t;
    read_byte_zero_x(t);
    set_nz(RA |= t);
    break;
}
case 0x0d: {// ORA abs
    uint8 t;
    read_byte_abs(t);
    set_nz(RA |= t);
    break;
}
case 0x1d: {// ORA abs,X
    uint8 t;
    read_byte_abs_x(t);
    set_nz(RA |= t);
    break;
}
case 0x19: {// ORA abs,Y
    uint8 t;
    read_byte_abs_y(t);
    set_nz(RA |= t);
    break;
}
case 0x01: {// ORA (ind,X)
    uint8 t;
    read_byte_ind_x(t);
    set_nz(RA |= t);
    break;
}
case 0x11: {// ORA (ind),Y
    uint8 t;
    read_byte_ind_y(t);
    set_nz(RA |= t);
    break;
}
case 0x49: {// EOR #imm
    uint8 t;
    read_byte_imm(t);
    set_nz(RA ^= t);
    break;
}
case 0x45: {// EOR zero
    uint8 t;
    read_byte_zero(t);
    set_nz(RA ^= t);
    break;
}
case 0x55: {// EOR zero,X
    uint8 t;
    read_byte_zero_x(t);
    set_nz(RA ^= t);
    break;
}
case 0x4d: {// EOR abs
    uint8 t;
    read_byte_abs(t);
    set_nz(RA ^= t);
    break;
}
case 0x5d: {// EOR abs,X
    uint8 t;
    read_byte_abs_x(t);
    set_nz(RA ^= t);
    break;
}
case 0x59: {// EOR abs,Y
    uint8 t;
    read_byte_abs_y(t);
    set_nz(RA ^= t);
    break;
}
case 0x41: {// EOR (ind,X)
    uint8 t;
    read_byte_ind_x(t);
    set_nz(RA ^= t);
    break;
}
case 0x51: {// EOR (ind),Y
    uint8 t;
    read_byte_ind_y(t);
    set_nz(RA ^= t);
    break;
}


// Compare group
case 0xc9: {// CMP #imm
    uint8 t;
    read_byte_imm(t);
    do_cmp;
}
case 0xc5: {// CMP zero
    uint8 t;
    read_byte_zero(t);
    do_cmp;
}
case 0xd5: {// CMP zero,X
    uint8 t;
    read_byte_zero_x(t);
    do_cmp;
}
case 0xcd: {// CMP abs
    uint8 t;
    read_byte_abs(t);
    do_cmp;
}
case 0xdd: {// CMP abs,X
    uint8 t;
    read_byte_abs_x(t);
    do_cmp;
}
case 0xd9: {// CMP abs,Y
    uint8 t;
    read_byte_abs_y(t);
    do_cmp;
}
case 0xc1: {// CMP (ind,X)
    uint8 t;
    read_byte_ind_x(t);
    do_cmp;
}
case 0xd1: {// CMP (ind),Y
    uint8 t;
    read_byte_ind_y(t);
    do_cmp;
}
case 0xe0: {// CPX #imm
    uint8 t;
    read_byte_imm(t);
    do_cpx;
}
case 0xe4: {// CPX zero
    uint8 t;
    read_byte_zero(t);
    do_cpx;
}
case 0xec: {// CPX abs
    uint8 t;
    read_byte_abs(t);
    do_cpx;
}
case 0xc0: {// CPY #imm
    uint8 t;
    read_byte_imm(t);
    do_cpy;
}
case 0xc4: {// CPY zero
    uint8 t;
    read_byte_zero(t);
    do_cpy;
}
case 0xcc: {// CPY abs
    uint8 t;
    read_byte_abs(t);
    do_cpy;
}


// Bit-test group
case 0x24: {// BIT zero
    uint8 t;
    read_byte_zero(t);
    do_bit;
}
case 0x2c: {// BIT abs
    uint8 t;
    read_byte_abs(t);
    do_bit;
}


// Shift/rotate group
case 0x0a:    // ASL A
    (RA & 0x80) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C);
    set_nz(RA <<= 1);
    read_idle_opcode; next_cycle;
    break;

case 0x06: {// ASL zero
    unsigned int t;
    read_byte_zero(t);
    do_asl(write_zp);
}
case 0x16: {// ASL zero,X
    unsigned int t;
    read_byte_zero_x(t);
    do_asl(write_zp);
}
case 0x0e: {// ASL abs
    unsigned int t;
    read_byte_abs(t);
    do_asl(write_byte_rmw);
}
case 0x1e: {// ASL abs,X
    read_adr_abs_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_asl(write_byte_rmw);
}
case 0x4a:    // LSR A
    (RA & 0x01) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C);
    set_nz(RA >>= 1);
    read_idle_opcode; next_cycle;
    break;

case 0x46: {// LSR zero
    unsigned int t;
    read_byte_zero(t);
    do_lsr(write_zp);
}
case 0x56: {// LSR zero,X
    unsigned int t;
    read_byte_zero_x(t);
    do_lsr(write_zp);
}
case 0x4e: {// LSR abs
    unsigned int t;
    read_byte_abs(t);
    do_lsr(write_byte_rmw);
}
case 0x5e: {// LSR abs,X
    read_adr_abs_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_lsr(write_byte_rmw);
}
case 0x2a: {// ROL A
    uint8 t = RA;
    set_nz(RA = (RA << 1) | (PFLAGS & PFLAG_C));
    (t & 0x80) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C);
    read_idle_opcode; next_cycle;
    break;
}
case 0x26: {// ROL zero
    unsigned int t;
    read_byte_zero(t);
    do_rol(write_zp);
}
case 0x36: {// ROL zero,X
    unsigned int t;
    read_byte_zero_x(t);
    do_rol(write_zp);
}
case 0x2e: {// ROL abs
    unsigned int t;
    read_byte_abs(t);
    do_rol(write_byte_rmw);
}
case 0x3e: {// ROL abs,X
    read_adr_abs_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_rol(write_byte_rmw);
}
case 0x6a: {// ROR A
    uint8 t = RA;
    set_nz(RA = ((PFLAGS & PFLAG_C) ? ((RA >> 1) | 0x80) : (RA >> 1)));
    (t & 0x01) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C);
    read_idle_opcode; next_cycle;
    break;
}
case 0x66: {// ROR zero
    unsigned int t;
    read_byte_zero(t);
    do_ror(write_zp);
}
case 0x76: {// ROR zero,X
    unsigned int t;
    read_byte_zero_x(t);
    do_ror(write_zp);
}
case 0x6e:    {// ROR abs
    unsigned int t;
    read_byte_abs(t);
    do_ror(write_byte_rmw);
}
case 0x7e:    {// ROR abs,X
    read_adr_abs_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_ror(write_byte_rmw);
}


// Stack group
case 0x48:    // PHA
    read_idle_opcode; next_cycle;
    push_byte(RA); next_cycle;
    break;

case 0x68:    // PLA
    read_idle_opcode; next_cycle;
    read_idle_stack(RSP); next_cycle;
    set_nz(RA = pop_byte); next_cycle;
    break;

case 0x08:    // PHP
    read_idle_opcode; next_cycle;
    push_flags(PFLAG_B);
    break;

case 0x28: {// PLP
    read_idle_opcode; next_cycle;
    read_idle_stack(RSP); next_cycle;
    uint8 old_pflags = PFLAGS;
    pop_flags;
    if (!(old_pflags & PFLAG_I) && (PFLAGS & PFLAG_I))
        opcode |= OPFLAG_IRQ_DISABLED;
    else if ((old_pflags & PFLAG_I) && !(PFLAGS & PFLAG_I))
        opcode |= OPFLAG_IRQ_ENABLED;
    break;
}


// Jump/branch group
case 0x4c:    // JMP abs
    read_adr_abs;
    jump(ADR);
    break;

case 0x6c: {// JMP (ind)
    read_adr_abs;
    uint8 t = read_byte(ADR); next_cycle;
    jump(t | (read_byte(((ADR + 1) & 0xff) | (ADR & 0xff00)) << 8)); next_cycle;
    break;
}
case 0x20: {// JSR abs
    uint8 t = read_opcode; inc_pc; next_cycle;
    read_idle_stack(RSP); next_cycle;
    push_byte(RPC >> 8); next_cycle;
    push_byte(RPC); next_cycle;
    jump(t | (read_opcode << 8)); next_cycle;
    break;
}
case 0x60: {// RTS
    read_idle_opcode; next_cycle;
    read_idle_stack(RSP); next_cycle;
    uint8 t = pop_byte; next_cycle;
    jump(t | (pop_byte << 8)); inc_pc; next_cycle;
    read_idle_opcode; next_cycle;
    break;
}
case 0x40: {// RTI
    quit = true;
    break;
}
case 0x00: {// BRK
    read_idle_opcode; inc_pc; next_cycle;
    push_byte(RPC >> 8); next_cycle;
    push_byte(RPC); next_cycle;
    push_flags(PFLAG_B);
    PFLAGS |= PFLAG_I;
    uint8 t = read_byte(0xfffe); next_cycle;
    jump(t | (read_byte(0xffff) << 8 )); next_cycle;
    break;
}
case 0xb0:    // BCS rel
    branch(PFLAGS & PFLAG_C);

case 0x90:    // BCC rel
    branch(!(PFLAGS & PFLAG_C));

case 0xf0:    // BEQ rel
    branch(!Z_FLAG);

case 0xd0:    // BNE rel
    branch(Z_FLAG);

case 0x70:    // BVS rel
#ifdef DRIVE_CPU
    gcr_drive->Update(current_cycle);
    if (gcr_drive->ByteReady())
        branch(1)
    else
#endif
    branch(PFLAGS & PFLAG_V);

case 0x50:    // BVC rel
#ifdef DRIVE_CPU
    gcr_drive->Update(current_cycle);
    if (gcr_drive->ByteReady())
        branch(0)
    else
#endif
    branch(!(PFLAGS & PFLAG_V));

case 0x30:    // BMI rel
    branch(N_FLAG & 0x80);

case 0x10:    // BPL rel
    branch(!(N_FLAG & 0x80));


// Flags group
case 0x38:    // SEC
    PFLAGS |= PFLAG_C;
    read_idle_opcode; next_cycle;
    break;

case 0x18:    // CLC
    PFLAGS &= ~PFLAG_C;
    read_idle_opcode; next_cycle;
    break;

case 0xf8:    // SED
    PFLAGS |= PFLAG_D;
    read_idle_opcode; next_cycle;
    break;

case 0xd8:    // CLD
    PFLAGS &= ~PFLAG_D;
    read_idle_opcode; next_cycle;
    break;

case 0x78:    // SEI
    if (!(PFLAGS & PFLAG_I))
        opcode |= OPFLAG_IRQ_DISABLED;
    PFLAGS |= PFLAG_I;
    read_idle_opcode; next_cycle;
    break;

case 0x58:    // CLI
    if (PFLAGS & PFLAG_I)
        opcode |= OPFLAG_IRQ_ENABLED;
    PFLAGS &= ~PFLAG_I;
    read_idle_opcode; next_cycle;
    break;

case 0xb8:    // CLV
    PFLAGS &= ~PFLAG_V;
    read_idle_opcode; next_cycle;
    break;


// NOP group
case 0xea:    // NOP
    read_idle_opcode; next_cycle;
    break;


/*
 *  Routines for undocumented opcodes
 */

// Load A/X group
case 0xa7:    // LAX zero
    read_byte_zero(RA);
    set_nz(RX = RA);
    break;

case 0xb7:    // LAX zero,Y
    read_byte_zero_y(RA);
    set_nz(RX = RA);
    break;

case 0xaf:    // LAX abs
    read_byte_abs(RA);
    set_nz(RX = RA);
    break;

case 0xbf:    // LAX abs,Y
    read_byte_abs_y(RA);
    set_nz(RX = RA);
    break;

case 0xa3:    // LAX (ind,X)
    read_byte_ind_x(RA);
    set_nz(RX = RA);
    break;

case 0xb3:    // LAX (ind),Y
    read_byte_ind_y(RA);
    set_nz(RX = RA);
    break;


// Store A/X group
case 0x87:    // SAX zero
    read_adr_zero;
    write_zp(ADR, RA & RX); next_cycle;
    break;

case 0x97:    // SAX zero,Y
    read_adr_zero_y;
    write_zp(ADR, RA & RX); next_cycle;
    break;

case 0x8f:    // SAX abs
    read_adr_abs;
    write_byte(ADR, RA & RX); next_cycle;
    break;

case 0x83:    // SAX (ind,X)
    read_adr_ind_x;
    write_byte(ADR, RA & RX); next_cycle;
    break;


// ASL/ORA group
case 0x07: {// SLO zero
    unsigned int t;
    read_byte_zero(t);
    do_slo(write_zp);
}
case 0x17: {// SLO zero,X
    unsigned int t;
    read_byte_zero_x(t);
    do_slo(write_zp);
}
case 0x0f: {// SLO abs
    unsigned int t;
    read_byte_abs(t);
    do_slo(write_byte_rmw);
}
case 0x1f: {// SLO abs,X
    read_adr_abs_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_slo(write_byte_rmw);
}
case 0x1b: {// SLO abs,Y
    read_adr_abs_y;
    unsigned int t = read_byte(ADR); next_cycle;
    do_slo(write_byte_rmw);
}
case 0x03: {// SLO (ind,X)
    read_adr_ind_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_slo(write_byte_rmw);
}
case 0x13: {// SLO (ind),Y
    read_adr_ind_y;
    unsigned int t = read_byte(ADR); next_cycle;
    do_slo(write_byte_rmw);
}


// ROL/AND group
case 0x27: {// RLA zero
    unsigned int t;
    read_byte_zero(t);
    do_rla(write_zp);
}
case 0x37: {// RLA zero,X
    unsigned int t;
    read_byte_zero_x(t);
    do_rla(write_zp);
}
case 0x2f: {// RLA abs
    unsigned int t;
    read_byte_abs(t);
    do_rla(write_byte_rmw);
}
case 0x3f: {// RLA abs,X
    read_adr_abs_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_rla(write_byte_rmw);
}
case 0x3b: {// RLA abs,Y
    read_adr_abs_y;
    unsigned int t = read_byte(ADR); next_cycle;
    do_rla(write_byte_rmw);
}
case 0x23: {// RLA (ind,X)
    read_adr_ind_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_rla(write_byte_rmw);
}
case 0x33: {// RLA (ind),Y
    read_adr_ind_y;
    unsigned int t = read_byte(ADR); next_cycle;
    do_rla(write_byte_rmw);
}


// LSR/EOR group
case 0x47: {// SRE zero
    unsigned int t;
    read_byte_zero(t);
    do_sre(write_zp);
}
case 0x57: {// SRE zero,X
    unsigned int t;
    read_byte_zero_x(t);
    do_sre(write_zp);
}
case 0x4f: {// SRE abs
    unsigned int t;
    read_byte_abs(t);
    do_sre(write_byte_rmw);
}
case 0x5f: {// SRE abs,X
    read_adr_abs_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_sre(write_byte_rmw);
}
case 0x5b: {// SRE abs,Y
    read_adr_abs_y;
    unsigned int t = read_byte(ADR); next_cycle;
    do_sre(write_byte_rmw);
}
case 0x43: {// SRE (ind,X)
    read_adr_ind_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_sre(write_byte_rmw);
}
case 0x53: {// SRE (ind),Y
    read_adr_ind_y;
    unsigned int t = read_byte(ADR); next_cycle;
    do_sre(write_byte_rmw);
}


// ROR/ADC group
case 0x67: {// RRA zero
    unsigned int t;
    read_byte_zero(t);
    do_rra(write_zp);
}
case 0x77: {// RRA zero,X
    unsigned int t;
    read_byte_zero_x(t);
    do_rra(write_zp);
}
case 0x6f: {// RRA abs
    unsigned int t;
    read_byte_abs(t);
    do_rra(write_byte_rmw);
}
case 0x7f: {// RRA abs,X
    read_adr_abs_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_rra(write_byte_rmw);
}
case 0x7b: {// RRA abs,Y
    read_adr_abs_y;
    unsigned int t = read_byte(ADR); next_cycle;
    do_rra(write_byte_rmw);
}
case 0x63: {// RRA (ind,X)
    read_adr_ind_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_rra(write_byte_rmw);
}
case 0x73: {// RRA (ind),Y
    read_adr_ind_y;
    unsigned int t = read_byte(ADR); next_cycle;
    do_rra(write_byte_rmw);
}


// DEC/CMP group
case 0xc7: {// DCP zero
    unsigned int t;
    read_byte_zero(t);
    do_dcp(write_zp);
}
case 0xd7: {// DCP zero,X
    unsigned int t;
    read_byte_zero_x(t);
    do_dcp(write_zp);
}
case 0xcf: {// DCP abs
    unsigned int t;
    read_byte_abs(t);
    do_dcp(write_byte_rmw);
}
case 0xdf: {// DCP abs,X
    read_adr_abs_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_dcp(write_byte_rmw);
}
case 0xdb: {// DCP abs,Y
    read_adr_abs_y;
    unsigned int t = read_byte(ADR); next_cycle;
    do_dcp(write_byte_rmw);
}
case 0xc3: {// DCP (ind,X)
    read_adr_ind_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_dcp(write_byte_rmw);
}
case 0xd3: {// DCP (ind),Y
    read_adr_ind_y;
    unsigned int t = read_byte(ADR); next_cycle;
    do_dcp(write_byte_rmw);
}


// INC/SBC group
case 0xe7: {// ISB zero
    unsigned int t;
    read_byte_zero(t);
    do_isb(write_zp);
}
case 0xf7: {// ISB zero,X
    unsigned int t;
    read_byte_zero_x(t);
    do_isb(write_zp);
}
case 0xef: {// ISB abs
    unsigned int t;
    read_byte_abs(t);
    do_isb(write_byte_rmw);
}
case 0xff: {// ISB abs,X
    read_adr_abs_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_isb(write_byte_rmw);
}
case 0xfb: {// ISB abs,Y
    read_adr_abs_y;
    unsigned int t = read_byte(ADR); next_cycle;
    do_isb(write_byte_rmw);
}
case 0xe3: {// ISB (ind,X)
    read_adr_ind_x;
    unsigned int t = read_byte(ADR); next_cycle;
    do_isb(write_byte_rmw);
}
case 0xf3: {// ISB (ind),Y
    read_adr_ind_y;
    unsigned int t = read_byte(ADR); next_cycle;
    do_isb(write_byte_rmw);
}


// Complex functions
case 0x0b:    // ANC #imm
case 0x2b: {
    uint8 t;
    read_byte_imm(t);
    set_nz(RA &= t);
    (N_FLAG & 0x80) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C);
    break;
}
case 0x4b: {// ASR #imm
    uint8 t;
    read_byte_imm(t);
    RA &= t;
    (RA & 0x01) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C);
    set_nz(RA >>= 1);
    break;
}
case 0x6b: {// ARR #imm
    unsigned int t;
    read_byte_imm(t);
    t &= RA;
    RA = ((PFLAGS & PFLAG_C) ? (t >> 1) | 0x80 : (t >> 1));
    if (PFLAGS & PFLAG_D) {
        N_FLAG = (PFLAGS & PFLAG_C) << 7;
        Z_FLAG = RA;
        ((t ^ RA) & 0x40) ? (PFLAGS |= PFLAG_V) : (PFLAGS &= ~PFLAG_V);
        if ((t & 0x0f) + (t & 0x01) > 5)
            RA = (RA & 0xf0) | ((RA + 6) & 0x0f);
        (((t + (t & 0x10)) & 0x1f0) > 0x50) ? (PFLAGS |= PFLAG_C, RA += 0x60) : (PFLAGS &= ~PFLAG_C);
    } else {
        set_nz(RA);
        (RA & 0x40) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C);
        ((RA & 0x40) ^ ((RA & 0x20) << 1)) ? (PFLAGS |= PFLAG_V) : (PFLAGS &= ~PFLAG_V);
    }
    break;
}
case 0x8b: {// ANE #imm
    uint8 t;
    read_byte_imm(t);
    set_nz(RA = (RA | 0xee) & RX & t);
    break;
}
case 0x93: {// SHA (ind),Y
    read_adr_ind_y;
    write_byte(ADR, RA & RX & (((ADR - RY) >> 8) + 1)); next_cycle;
    break;
}
case 0x9b: {// SHS abs,Y
    read_adr_abs_y;
    write_byte(ADR, (RSP = RA & RX) & (((ADR - RY) >> 8) + 1)); next_cycle;
    break;
}
case 0x9c: {// SHY abs,X
    read_adr_abs_x;
    write_byte(ADR, RY & (((ADR - RX) >> 8) + 1)); next_cycle;
    break;
}
case 0x9e: {// SHX abs,Y
    read_adr_abs_y;
    write_byte(ADR, RX & (((ADR - RY) >> 8) + 1)); next_cycle;
    break;
}
case 0x9f: {// SHA abs,Y
    read_adr_abs_y;
    write_byte(ADR, RA & RX & (((ADR - RY) >> 8) + 1)); next_cycle;
    break;
}
case 0xab: {// LXA #imm
    uint8 t;
    read_byte_imm(t);
    set_nz(RA = RX = (RA | 0xee) & t);
    break;
}
case 0xbb: {// LAS abs,Y
    uint8 t;
    read_byte_abs_y(t);
    set_nz(RA = RX = RSP = t & RSP);
    break;
}
case 0xcb: {// SBX #imm
    unsigned int t;
    read_byte_imm(t);
    set_nz(RX = t = (RA & RX) - t);
    (t < 0x100) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C);
    break;
}


// NOP group
case 0x1a:    // NOP
case 0x3a:
case 0x5a:
case 0x7a:
case 0xda:
case 0xfa:
    read_idle_opcode; next_cycle;
    break;

case 0x80:    // NOP #imm
case 0x82:
case 0x89:
case 0xc2:
case 0xe2:
    read_idle_opcode; inc_pc; next_cycle;
    break;

case 0x04:    // NOP zero
case 0x44:
case 0x64:
    read_adr_zero;
    read_idle_zp(ADR); next_cycle;
    break;

case 0x14:    // NOP zero,X
case 0x34:
case 0x54:
case 0x74:
case 0xd4:
case 0xf4:
    read_adr_zero_x;
    read_idle_zp(ADR); next_cycle;
    break;

case 0x0c:    // NOP abs
    read_adr_abs;
    read_idle(ADR); next_cycle;
    break;

case 0x1c:    // NOP abs,X
case 0x3c:
case 0x5c:
case 0x7c:
case 0xdc:
case 0xfc: {
    uint8 t;
    read_byte_abs_x(t);
    break;
}


// Jam group
case 0x02:
case 0x12:
case 0x22:
case 0x32:
case 0x42:
case 0x52:
case 0x62:
case 0x72:
case 0x92:
case 0xb2:
case 0xd2:
    goto illegal_op;
