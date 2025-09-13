#include "cpu_base.h"
#include "../bus.h"

#ifndef CPU_ADDRESSING_MODES_H
#define CPU_ADDRESSING_MODES_H

typedef struct AddrModeResult {
   Byte value;
   Addr addr;
   bool has_value;
   bool has_addr;
} AddrModeResult;

typedef AddrModeResult (*AddrModeFunc)(Cpu*, Bus*);

AddrModeResult addressingNone(Cpu *cpu, Bus *bus);
AddrModeResult addressingImmediate(Cpu *cpu, Bus *bus);
AddrModeResult addressingZeroPage(Cpu *cpu, Bus *bus);
AddrModeResult addressingZeroPageX(Cpu *cpu, Bus *bus);
AddrModeResult addressingZeroPageY(Cpu *cpu, Bus *bus);
AddrModeResult addressingAbsolute(Cpu *cpu, Bus *bus);
AddrModeResult addressingAbsoluteX(Cpu *cpu, Bus *bus);
AddrModeResult addressingAbsoluteY(Cpu *cpu, Bus *bus);
AddrModeResult addressingIndirectX(Cpu *cpu, Bus *bus);

#endif //CPU_ADDRESSING_MODES_H
