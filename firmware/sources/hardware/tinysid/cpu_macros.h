/*
 *  cpu_macros.h - Macro definitions for 6510 CPU emulation opcode routines
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
 *  Status register flag bits
 */

#define PFLAG_N 0x80
#define PFLAG_V 0x40
#define PFLAG_B 0x10
#define PFLAG_D 0x08
#define PFLAG_I 0x04
#define PFLAG_Z 0x02
#define PFLAG_C 0x01


/*
 *  Opcode flag bits
 */

#define OPFLAG_IRQ_DISABLED 0x100
#define OPFLAG_IRQ_ENABLED 0x200
#define OPFLAG_INT_DELAYED 0x400


/*
 *  Address calculation macros
 */

// Read zero page operand address
#define read_adr_zero \
    ADR = read_opcode; inc_pc; next_cycle

// Read zero page x-indexed operand address
#define read_adr_zero_x \
    ADR = read_opcode; inc_pc; next_cycle; \
    read_idle_zp(ADR); next_cycle; \
    ADR = (ADR + RX) & 0xff

// Read zero page y-indexed operand address
#define read_adr_zero_y \
    ADR = read_opcode; inc_pc; next_cycle; \
    read_idle_zp(ADR); next_cycle; \
    ADR = (ADR + RY) & 0xff

// Read absolute operand address
#define read_adr_abs \
    ADR = read_opcode; inc_pc; next_cycle; \
    ADR |= read_opcode << 8; inc_pc; next_cycle

// Read absolute x-indexed operand address (no extra cycle for page crossing)
#define read_adr_abs_x \
{ \
    ADR = read_opcode; inc_pc; next_cycle; \
    unsigned int tmp = ADR + RX; \
    ADR = tmp + (read_opcode << 8); inc_pc; next_cycle; \
    if (tmp >= 0x100) { \
        read_idle((ADR - 0x100) & 0xffff); \
    } else { \
        read_idle(ADR); \
    } \
    next_cycle; \
}

// Read absolute y-indexed operand address (no extra cycle for page crossing)
#define read_adr_abs_y \
{ \
    ADR = read_opcode; inc_pc; next_cycle; \
    unsigned int tmp = ADR + RY; \
    ADR = tmp + (read_opcode << 8); inc_pc; next_cycle; \
    if (tmp >= 0x100) { \
        read_idle((ADR - 0x100) & 0xffff); \
    } else { \
        read_idle(ADR); \
    } \
    next_cycle; \
}

// Read indexed indirect operand address (no extra cycle for page crossing)
#define read_adr_ind_x \
{ \
    unsigned int tmp = read_opcode; inc_pc; next_cycle; \
    read_idle_zp(tmp); next_cycle; \
    tmp += RX; \
    ADR = read_zp(tmp & 0xff); next_cycle; \
    ADR |= read_zp((tmp + 1) & 0xff) << 8; next_cycle; \
}

// Read indirect indexed operand address (no extra cycle for page crossing)
#define read_adr_ind_y \
{ \
    unsigned int tmp = read_opcode; inc_pc; next_cycle; \
    unsigned int tmp2 = read_zp(tmp) + RY; next_cycle; \
    ADR = tmp2 + (read_zp((tmp + 1) & 0xff) << 8); next_cycle; \
    if (tmp2 >= 0x100) { \
        read_idle((ADR - 0x100) & 0xffff); \
    } else { \
        read_idle(ADR); \
    } \
    next_cycle; \
}


/*
 *  Operand fetch macros
 */

// Read immediate operand
#define read_byte_imm(to) \
    to = read_opcode; inc_pc; next_cycle

// Read zero page operand
#define read_byte_zero(to) \
    read_adr_zero; \
    to = read_zp(ADR); next_cycle

// Read zero page x-indexed operand
#define read_byte_zero_x(to) \
    read_adr_zero_x; \
    to = read_zp(ADR); next_cycle

// Read zero page y-indexed operand
#define read_byte_zero_y(to) \
    read_adr_zero_y; \
    to = read_zp(ADR); next_cycle

// Read absolute operand
#define read_byte_abs(to) \
    read_adr_abs; \
    to = read_byte(ADR); next_cycle;

// Read absolute x-indexed operand (extra cycle for page crossing)
#define read_byte_abs_x(to) \
{ \
    ADR = read_opcode; inc_pc; next_cycle; \
    unsigned int tmp = ADR + RX; \
    ADR = tmp + (read_opcode << 8); inc_pc; next_cycle; \
    if (tmp >= 0x100) { \
        /* Page crossed */ \
        read_idle((ADR - 0x100) & 0xffff); next_cycle; \
    } \
    to = read_byte(ADR); next_cycle; \
}

// Read absolute y-indexed operand (extra cycle for page crossing)
#define read_byte_abs_y(to) \
{ \
    ADR = read_opcode; inc_pc; next_cycle; \
    unsigned int tmp = ADR + RY; \
    ADR = tmp + (read_opcode << 8); inc_pc; next_cycle; \
    if (tmp >= 0x100) { \
        /* Page crossed */ \
        read_idle((ADR - 0x100) & 0xffff); next_cycle; \
    } \
    to = read_byte(ADR); next_cycle; \
}

// Read indexed indirect operand (no extra cycle for page crossing)
#define read_byte_ind_x(to) \
    read_adr_ind_x; \
    to = read_byte(ADR); next_cycle;

// Read indirect indexed operand (extra cycle for page crossing)
#define read_byte_ind_y(to) \
{ \
    unsigned int tmp = read_opcode; inc_pc; next_cycle; \
    unsigned int tmp2 = read_zp(tmp) + RY; next_cycle; \
    ADR = tmp2 + (read_zp((tmp + 1) & 0xff) << 8); next_cycle; \
    if (tmp2 >= 0x100) { \
        /* Page crossed */ \
        read_idle((ADR - 0x100) & 0xffff); next_cycle; \
    } \
    to = read_byte(ADR); next_cycle; \
}


/*
 *  Other macros
 */

// Set N and Z flags
#define set_nz(val) \
    N_FLAG = Z_FLAG = val

// Pop status flags
#define pop_flags \
    N_FLAG = PFLAGS = pop_byte & 0xcf; next_cycle; \
    Z_FLAG = !(PFLAGS & PFLAG_Z)

// Push status flags
#ifdef DRIVE_CPU
#define push_flags(b_flag) \
{ \
    gcr_drive->Update(current_cycle); \
    if (gcr_drive->ByteReady()) \
        PFLAGS |= PFLAG_V; \
    (N_FLAG & 0x80) ? (PFLAGS |= PFLAG_N) : (PFLAGS &= ~PFLAG_N); \
    Z_FLAG ? (PFLAGS &= ~PFLAG_Z) : (PFLAGS |= PFLAG_Z); \
    push_byte(PFLAGS | 0x20 | b_flag); next_cycle; \
}
#else
#define push_flags(b_flag) \
    (N_FLAG & 0x80) ? (PFLAGS |= PFLAG_N) : (PFLAGS &= ~PFLAG_N); \
    Z_FLAG ? (PFLAGS &= ~PFLAG_Z) : (PFLAGS |= PFLAG_Z); \
    push_byte(PFLAGS | 0x20 | b_flag); next_cycle
#endif

// ADC operation
#define do_adc(byte) \
    if (PFLAGS & PFLAG_D) { \
        unsigned int tmp, tmp2; \
        tmp = (RA & 0x0f) + (byte & 0x0f) + (PFLAGS & PFLAG_C); \
        if (tmp > 9) tmp += 6; \
        tmp2 = (tmp & 0x0f) + (RA & 0xf0) + (byte & 0xf0); \
        if (tmp > 0x0f) tmp2 += 0x10; \
        Z_FLAG = RA + byte + (PFLAGS & PFLAG_C); \
        N_FLAG = tmp2; \
        (!((RA ^ byte) & 0x80) && ((RA ^ tmp2) & 0x80)) ? (PFLAGS |= PFLAG_V) : (PFLAGS &= ~PFLAG_V); \
        if ((tmp2 & 0x1f0) > 0x90) tmp2 += 0x60; \
        (tmp2 & 0xf00) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
        RA = tmp2; \
    } else { \
        unsigned int tmp = RA + byte + (PFLAGS & PFLAG_C); \
        (tmp > 0xff) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
        (!((RA ^ byte) & 0x80) && ((RA ^ tmp) & 0x80)) ? (PFLAGS |= PFLAG_V) : (PFLAGS &= ~PFLAG_V); \
        set_nz(RA = tmp); \
    } \
    break

// SBC operation
#define do_sbc(byte) \
{ \
    unsigned int tmp = RA - byte - ((PFLAGS & PFLAG_C) ? 0 : 1); \
    (((RA ^ tmp) & 0x80) && ((RA ^ byte) & 0x80)) ? (PFLAGS |= PFLAG_V) : (PFLAGS &= ~PFLAG_V); \
    if (PFLAGS & PFLAG_D) { \
        unsigned int tmp2 = (RA & 0x0f) - (byte & 0x0f) - ((PFLAGS & PFLAG_C) ? 0 : 1); \
        if (tmp2 & 0x10) \
            tmp2 = ((tmp2 - 6) & 0x0f) | ((RA & 0xf0) - (byte & 0xf0) - 0x10); \
        else \
            tmp2 = (tmp2 & 0x0f) | ((RA & 0xf0) - (byte & 0xf0)); \
        if (tmp2 & 0x100) tmp2 -= 0x60; \
        RA = tmp2; \
    } else { \
        RA = tmp; \
    } \
    (tmp < 0x100) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
    set_nz(tmp); \
    break; \
}

// CMP operation
#define do_cmp \
    unsigned int tmp = RA - t; \
    set_nz(tmp); \
    (tmp < 0x100) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
    break

// CPX operation
#define do_cpx \
    unsigned int tmp = RX - t; \
    set_nz(tmp); \
    (tmp < 0x100) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
    break

// CPY operation
#define do_cpy \
    unsigned int tmp = RY - t; \
    set_nz(tmp); \
    (tmp < 0x100) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
    break

// BIT operation
#define do_bit \
    Z_FLAG = RA & t; \
    N_FLAG = t; \
    (t & 0x40) ? (PFLAGS |= PFLAG_V) : (PFLAGS &= ~PFLAG_V); \
    break

// ASL operation
#define do_asl(write_cmd) \
    (t & 0x80) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
    next_cycle; write_cmd(ADR, set_nz(t << 1)); next_cycle; \
    break

// LSR operation
#define do_lsr(write_cmd) \
    (t & 0x01) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
    next_cycle; write_cmd(ADR, set_nz(t >> 1)); next_cycle; \
    break

// ROL operation
#define do_rol(write_cmd) \
    next_cycle; write_cmd(ADR, set_nz((t << 1) | (PFLAGS & PFLAG_C))); next_cycle; \
    (t & 0x80) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
    break

// ROR operation
#define do_ror(write_cmd) \
    next_cycle; write_cmd(ADR, set_nz((PFLAGS & PFLAG_C) ? (t >> 1) | 0x80 : (t >> 1))); next_cycle; \
    (t & 0x01) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
    break

// Branch operation
#define branch(flag) \
{ \
    int8 tmp = read_opcode; inc_pc; next_cycle; \
    if (flag) { \
        /* Branch taken */ \
        ADR = RPC + (int8)tmp; \
        if ((ADR ^ RPC) & 0xff00) { \
            /* Page crossed */ \
            read_idle_opcode; next_cycle; \
            if (tmp & 0x80) { \
                read_idle(ADR + 0x100); next_cycle; \
            } else { \
                read_idle(ADR - 0x100); next_cycle; \
            } \
            jump(ADR); \
        } else { \
            /* No page crossed */ \
            opcode |= OPFLAG_INT_DELAYED; \
            read_idle_opcode; next_cycle; \
            jump(ADR); \
        } \
    } \
    break; \
}

// SLO operation
#define do_slo(write_cmd) \
    (t & 0x80) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
    next_cycle; write_cmd(ADR, t <<= 1); next_cycle; \
    set_nz(RA |= t); \
    break

// RLA operation
#define do_rla(write_cmd) \
    unsigned int t2 = (t << 1) | (PFLAGS & PFLAG_C); \
    next_cycle; write_cmd(ADR, t2); next_cycle; \
    (t & 0x80) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
    set_nz(RA &= t2); \
    break

// SRE operation
#define do_sre(write_cmd) \
    (t & 0x01) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
    next_cycle; write_cmd(ADR, t >>= 1); next_cycle; \
    set_nz(RA ^= t); \
    break

// RRA operation
#define do_rra(write_cmd) \
    unsigned int t2 = (PFLAGS & PFLAG_C) ? (t >> 1) | 0x80 : (t >> 1); \
    next_cycle; write_cmd(ADR, t2); next_cycle; \
    (t & 0x01) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
    do_adc(t2);

// DCP operation
#define do_dcp(write_cmd) \
    t = (t - 1) & 0xff; \
    next_cycle; write_cmd(ADR, t); next_cycle; \
    t = RA - t; \
    set_nz(t); \
    (t < 0x100) ? (PFLAGS |= PFLAG_C) : (PFLAGS &= ~PFLAG_C); \
    break

// ISB operation
#define do_isb(write_cmd) \
    t = (t + 1) & 0xff; \
    next_cycle; write_cmd(ADR, t); next_cycle; \
    do_sbc(t);
