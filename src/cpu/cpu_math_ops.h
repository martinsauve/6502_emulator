#ifndef CPU_MATH_OPS_H
#define CPU_MATH_OPS_H

#include "../bus.h"
#include "../6502_types.h"
#include "cpu_base.h"


//*******************************************
// LOGIC AND ARITHMETIC OPERATIONS
//*******************************************
//
//
//NEW IMPLEMTATIONS

void opCMP(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc);
void opCPX(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc);
void opCPY(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc);

void opAND(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc);


////

void opDEY(Cpu *cpu, Bus *bus);
void opDEX(Cpu *cpu, Bus *bus);
void opDEC_A(Cpu *cpu, Bus *bus);
void opDEC_zp(Cpu *cpu, Bus *bus);
void opDEC_zpX(Cpu *cpu, Bus *bus);

void opINX(Cpu *cpu, Bus *bus);
void opINY(Cpu *cpu, Bus *bus);
void opINC_A(Cpu *cpu, Bus *bus);
void opINC_zp(Cpu *cpu, Bus *bus);
void opINC_zpX(Cpu *cpu, Bus *bus);


void opADC_imm(Cpu *cpu, Bus *bus);
void opADC_zp(Cpu *cpu, Bus *bus);
void opADC_zpX(Cpu *cpu, Bus *bus);
void opADC_abs(Cpu *cpu, Bus *bus);
void opADC_absX(Cpu *cpu, Bus *bus);
void opADC_absY(Cpu *cpu, Bus *bus);

void opSBC_zp(Cpu *cpu, Bus *bus);
void opSBC_zpX(Cpu *cpu, Bus *bus);
void opSBC_abs(Cpu *cpu, Bus *bus);
void opSBC_absX(Cpu *cpu, Bus *bus);
void opSBC_absY(Cpu *cpu, Bus *bus);
void opSBC_indY(Cpu *cpu, Bus *bus);
void opSBC_imm(Cpu *cpu, Bus *bus);

void opCMP_imm(Cpu *cpu, Bus *bus);
void opCMP_zp(Cpu *cpu, Bus *bus);
void opCMP_indY(Cpu *cpu, Bus *bus);

void opCPX_imm(Cpu *cpu, Bus *bus);
void opCPX_zp(Cpu *cpu, Bus *bus);

void opCPY_imm(Cpu *cpu, Bus *bus);
void opCPY_zp(Cpu *cpu, Bus *bus);


void opAND_imm(Cpu *cpu, Bus *bus);


void opCMP_absX(Cpu *cpu, Bus *bus);

void opAND_abs(Cpu *cpu, Bus *bus);
void opAND_absX(Cpu *cpu, Bus *bus);

void opAND_zp(Cpu *cpu, Bus *bus);

void opEOR_imm(Cpu *cpu, Bus *bus);
void opEOR_zp(Cpu *cpu, Bus *bus);

void opORA_imm(Cpu *cpu, Bus *bus);
void opORA_abs(Cpu *cpu, Bus *bus);
void opORA_indX(Cpu *cpu, Bus *bus);
void opORA_zp(Cpu *cpu, Bus *bus);

void opROL_A(Cpu *cpu, Bus *bus);
void opROL_zp(Cpu *cpu, Bus *bus);
void opROL_zpX(Cpu *cpu, Bus *bus);

void opROR_A(Cpu *cpu, Bus *bus);
void opROR_zp(Cpu *cpu, Bus *bus);
void opROR_zpX(Cpu *cpu, Bus *bus);

void opBIT_zp(Cpu *cpu, Bus *bus);
void opBIT_abs(Cpu *cpu, Bus *bus);
void opBIT_absX(Cpu *cpu, Bus *bus);

void opASL_A(Cpu *cpu, Bus *bus);
void opASL_zp(Cpu *cpu, Bus *bus);
void opASL_zpX(Cpu *cpu, Bus *bus);

void opLSR_A(Cpu *cpu, Bus *bus);
void opLSR_zp(Cpu *cpu, Bus *bus);
void opLSR_zpX(Cpu *cpu, Bus *bus);


void opTSB_abs(Cpu *cpu, Bus *bus);
void opTSB_zp(Cpu *cpu, Bus *bus);

#endif // CPU_MATH_OPS_H
