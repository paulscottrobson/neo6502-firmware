#pragma once
// 
// Author: Rien Matthijsse
// 
#define NEO6502

// 65c02 connections
//
// NEO6502 board v1.0
//
#define uP_RESET  28 // RESB(40) <-- UEXT pin 3
#define uP_CLOCK  21 // PHI2
#define uP_RW     11 // RW#

#define uP_BUZZ   20

#define pDebug    25 // UEXT pin 10

const uint32_t en_MASK =  0b00000000000011100000000;
const uint32_t en_NONE =  0b00000000000011100000000;
const uint32_t en_A0_7 =  0b00000000000011000000000;
const uint32_t en_A8_15 = 0b00000000000010100000000;
const uint32_t en_D0_7 =  0b00000000000001100000000;
