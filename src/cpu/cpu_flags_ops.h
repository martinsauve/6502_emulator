#include "../bus.h"
#include "../6502_types.h"
#include "cpu_base.h"

#ifndef CPU_FLAGS_OPS_H
#define CPU_FLAGS_OPS_H

void opCLC(Cpu*, Bus*);
void opCLD(Cpu*, Bus*);
void opCLI(Cpu*, Bus*);
void opCLV(Cpu*, Bus*);
void opSEC(Cpu*, Bus*);
void opSED(Cpu*, Bus*);
void opSEI(Cpu*, Bus*);
void opTXS(Cpu *cpu, Bus *bus);
void opTSX(Cpu *cpu, Bus *bus);
void opPHP(Cpu *cpu, Bus *bus);
void opPLP(Cpu *cpu, Bus *bus);

#endif // CPU_FLAGS_OPS_H
