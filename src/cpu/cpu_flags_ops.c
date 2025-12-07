#include "cpu.h"
#include "../bus.h"
#include "../6502_types.h"
#include <stdio.h>
#include <stdlib.h>



void opCLC(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc) { // set decimal flag
   (void)addrModeFunc; // Unused parameter
   (void)bus; // Unused parameter
   cpu->C = 0;
}
void opCLD(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc) { // set decimal flag
   (void)addrModeFunc; // Unused parameter
   (void)bus; // Unused parameter
   cpu->D = 0;
}

void opCLI(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc) { // set decimal flag
   (void)addrModeFunc; // Unused parameter
   (void)bus; // Unused parameter
   cpu->I = 0;
}

void opCLV(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc) { // set decimal flag
   (void)addrModeFunc; // Unused parameter
   (void)bus; // Unused parameter
   cpu->V = 0;
}

void opSEC(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc) { // set decimal flag
   (void)addrModeFunc; // Unused parameter
   (void)bus; // Unused parameter
   cpu->C = 1;
}

void opSED(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc) { // set decimal flag
   (void)addrModeFunc; // Unused parameter
   (void)bus; // Unused parameter
   cpu->D = 1;
}

void opSEI(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc) { // set interrupt disable status
   (void)addrModeFunc; // Unused parameter
   (void)bus; // Unused parameter
   cpu->I = 1;
}

// transfer x to stack pointer
void opTXS(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc){
   (void)addrModeFunc; // Unused parameter
   (void)bus; // Unused parameter
   cpu->SP = cpu->X;
}
// transfer stack pointer to x
void opTSX(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc){
   (void)addrModeFunc; // Unused parameter
   (void)bus; // Unused parameter
   cpu->X = cpu->SP;
}

// push processor status on stack

void opPHP(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc)
{
   (void)addrModeFunc;
   pushStack(cpu, bus, packStatusFlags(cpu, true));
}

// push processor status on stack
void opPLP(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc){
   (void)addrModeFunc;
   unpackStatusFlags(cpu, pullStack(cpu, bus));
}
