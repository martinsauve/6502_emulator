#include <stdint.h>
#include <stdbool.h>
#include "bus.h"
#include "6502_types.h"
#include "cpu_base.h"
#include "cpu_load_store_ops.h"

#ifndef CPU_H
#define CPU_H

void opBEQ(Cpu*, Bus*);
void opBNE(Cpu*, Bus*);
void opBCC(Cpu*, Bus*);
void opBCS(Cpu*, Bus*);
void opBMI(Cpu*, Bus*);
void opBPL(Cpu*, Bus*);
void opBVC(Cpu*, Bus*);
void opBVS(Cpu*, Bus*);

void opCLC(Cpu*, Bus*);
void opCLD(Cpu*, Bus*);
void opCLI(Cpu*, Bus*);
void opCLV(Cpu*, Bus*);
void opSEC(Cpu*, Bus*);
void opSED(Cpu*, Bus*);
void opSEI(Cpu*, Bus*);


void opINX(Cpu*, Bus*);

void opJMP_abs(Cpu*, Bus*);
void opJMP_ind(Cpu*, Bus*);

void opPHA(Cpu*, Bus*);
void opPLA(Cpu*, Bus*);

void opJSR(Cpu*, Bus*);
void opRTS(Cpu*, Bus*);

#endif // CPU_H
