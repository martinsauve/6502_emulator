#include <stdint.h>
#include <stdbool.h>
#include "../bus.h"
#include "../6502_types.h"
#include "cpu_base.h"
#include "cpu_load_store_ops.h"
#include "cpu_branch_ops.h"
#include "cpu_flags_ops.h"

#ifndef CPU_H
#define CPU_H


void opINX(Cpu*, Bus*);

void opJMP_abs(Cpu*, Bus*);
void opJMP_ind(Cpu*, Bus*);

void opPHA(Cpu*, Bus*);
void opPLA(Cpu*, Bus*);

void opJSR(Cpu*, Bus*);
void opRTS(Cpu*, Bus*);

#endif // CPU_H
