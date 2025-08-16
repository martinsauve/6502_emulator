#include "../bus.h"
#include "../6502_types.h"
#include "cpu_base.h"

#ifndef CPU_MATH_OPS_H
#define CPU_MATH_OPS_H
//*******************************************
// LOGIC AND ARITHMETIC OPERATIONS
//*******************************************

void opDEY(Cpu *cpu, Bus *bus);
void opDEX(Cpu *cpu, Bus *bus);
void opDEC_A(Cpu *cpu, Bus *bus);

void opINX(Cpu *cpu, Bus *bus);
void opINY(Cpu *cpu, Bus *bus);
void opINC_A(Cpu *cpu, Bus *bus);
void opINC_zp(Cpu *cpu, Bus *bus);


void opADC_imm(Cpu *cpu, Bus *bus);

void opSBC_zp(Cpu *cpu, Bus *bus);

void opCMP_imm(Cpu *cpu, Bus *bus);
void opCMP_zp(Cpu *cpu, Bus *bus);

void opCPY_imm(Cpu *cpu, Bus *bus);
void opCPY_zp(Cpu *cpu, Bus *bus);


void opAND_imm(Cpu *cpu, Bus *bus);

void opEOR_imm(Cpu *cpu, Bus *bus);

void opORA_imm(Cpu *cpu, Bus *bus);

void opROL_A(Cpu *cpu, Bus *bus);
void opROL_zp(Cpu *cpu, Bus *bus);

void opBIT_zp(Cpu *cpu, Bus *bus);

void opASL_A(Cpu *cpu, Bus *bus);
void opLSR_A(Cpu *cpu, Bus *bus);

#endif // CPU_MATH_OPS_H
