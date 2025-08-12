#include <stdint.h>
#include <stdbool.h>
#include "bus.h"
#include "6502_types.h"
#include "cpu_base.h"
#include "cpu_load_store_ops.h"

#ifndef CPU_H
#define CPU_H

void opBEQ(Cpu*, Bus*);
void opINX(Cpu*, Bus*);
void opJMP(Cpu*, Bus*);
#endif // CPU_H
