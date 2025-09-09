#include "../bus.h"
#include "../6502_types.h"
#include "cpu_base.h"

#ifndef CPU_BRANCH_OPS_H
#define CPU_BRANCH_OPS_H

void opBEQ(Cpu*, Bus*);
void opBNE(Cpu*, Bus*);
void opBCC(Cpu*, Bus*);
void opBCS(Cpu*, Bus*);
void opBMI(Cpu*, Bus*);
void opBPL(Cpu*, Bus*);
void opBVC(Cpu*, Bus*);
void opBVS(Cpu*, Bus*);

void opJMP_abs(Cpu*, Bus*);
void opJMP_ind_buggy(Cpu*, Bus*);
void opJMP_ind_fixed(Cpu*, Bus*);

void opJSR(Cpu*, Bus*);
void opRTS(Cpu*, Bus*);
void opRTI(Cpu*, Bus*);

void opBBR0(Cpu *cpu, Bus *bus);
void opBBR1(Cpu *cpu, Bus *bus);
void opBBR2(Cpu *cpu, Bus *bus);
void opBBR3(Cpu *cpu, Bus *bus);
void opBBR4(Cpu *cpu, Bus *bus);
void opBBR5(Cpu *cpu, Bus *bus);
void opBBR6(Cpu *cpu, Bus *bus);
void opBBR7(Cpu *cpu, Bus *bus);

void opBBS0(Cpu *cpu, Bus *bus);
void opBBS1(Cpu *cpu, Bus *bus);
void opBBS2(Cpu *cpu, Bus *bus);
void opBBS3(Cpu *cpu, Bus *bus);
void opBBS4(Cpu *cpu, Bus *bus);
void opBBS5(Cpu *cpu, Bus *bus);
void opBBS6(Cpu *cpu, Bus *bus);
void opBBS7(Cpu *cpu, Bus *bus);

void opBRK(Cpu *cpu, Bus *bus);

#endif // CPU_BRANCH_OPS_H
