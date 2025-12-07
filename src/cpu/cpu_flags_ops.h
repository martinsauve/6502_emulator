#ifndef CPU_FLAGS_OPS_H
#define CPU_FLAGS_OPS_H

#include "../bus.h"
#include "../6502_types.h"
#include "cpu_base.h"

void opCLC(Cpu*, Bus*, AddrModeFunc addrModeFunc);
void opCLD(Cpu*, Bus*, AddrModeFunc addrModeFunc);
void opCLI(Cpu*, Bus*, AddrModeFunc addrModeFunc);
void opCLV(Cpu*, Bus*, AddrModeFunc addrModeFunc);
void opSEC(Cpu*, Bus*, AddrModeFunc addrModeFunc);
void opSED(Cpu*, Bus*, AddrModeFunc addrModeFunc);
void opSEI(Cpu*, Bus*, AddrModeFunc addrModeFunc);
void opTXS(Cpu*, Bus*, AddrModeFunc addrModeFunc);
void opTSX(Cpu*, Bus*, AddrModeFunc addrModeFunc);
void opPHP(Cpu*, Bus*, AddrModeFunc addrModeFunc);
void opPLP(Cpu*, Bus*, AddrModeFunc addrModeFunc);

#endif // CPU_FLAGS_OPS_H
