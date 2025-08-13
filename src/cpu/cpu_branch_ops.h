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


#endif // CPU_BRANCH_OPS_H
