#include "cpu.h"
#include "../bus.h"
#include "../6502_types.h"
#include <stdio.h>
#include <stdlib.h>



void opCLC(Cpu *cpu, Bus *bus) { // clear carry flag
   (void)bus; // Unused parameter
   cpu->C = 0;
   cpu->PC += 1;
}
void opCLD(Cpu *cpu, Bus *bus) { // clear decimal flag
   (void)bus; // Unused parameter
//   fprintf(stderr, "Warning: Decimal mode not supported in this implementation.\n");
//   dumpCpu(cpu);
//   exit(1);
   cpu->D = 0;
   cpu->PC += 1;
}

void opCLI(Cpu *cpu, Bus *bus) { // clear interrupt disable
   (void)bus; // Unused parameter
   cpu->I = 0;
   cpu->PC += 1;
}

void opCLV(Cpu *cpu, Bus *bus) { // clear overflow
   (void)bus; // Unused parameter
   cpu->V = 0;
   cpu->PC += 1;
}

void opSEC(Cpu *cpu, Bus *bus) { // set carry flag
   (void)bus; // Unused parameter
   cpu->C = 1;
   cpu->PC += 1;
}

void opSED(Cpu *cpu, Bus *bus) { // set decimal flag
   (void)bus; // Unused parameter
//   fprintf(stderr, "Warning: Decimal mode not supported in this implementation.\n");
//   exit(1);
//   dumpCpu(cpu);
   cpu->D = 1;
   cpu->PC += 1;
}

void opSEI(Cpu *cpu, Bus *bus) { // set interrupt disable status
   (void)bus; // Unused parameter
   cpu->I = 1;
   cpu->PC += 1;
}

// transfer x to stack pointer
void opTXS(Cpu *cpu, Bus *bus){
   (void)bus; // Unused parameter
   cpu->SP = cpu->X;
   cpu->PC += 1;
}
// transfer stack pointer to x
void opTSX(Cpu *cpu, Bus *bus){
   (void)bus; // Unused parameter
   cpu->X = cpu->SP;
   cpu->PC += 1;
}

// push processor status on stack
void opPHP(Cpu *cpu, Bus *bus){
   pushStack(cpu, bus, packStatusFlags(cpu, true));
   cpu->PC +=1;
}

// push processor status on stack
void opPLP(Cpu *cpu, Bus *bus){
   unpackStatusFlags(cpu, pullStack(cpu, bus));
   cpu->PC +=1;
}
