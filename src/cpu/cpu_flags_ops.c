#include "cpu.h"
#include "../bus.h"
#include "../6502_types.h"


void opCLC(Cpu *cpu, Bus *bus) { // clear carry flag
   (void)bus; // Unused parameter
   cpu->C = 0;
   cpu->PC += 1;
}
void opCLD(Cpu *cpu, Bus *bus) { // clear decimal flag
   (void)bus; // Unused parameter
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
   cpu->D = 1;
   cpu->PC += 1;
}

void opSEI(Cpu *cpu, Bus *bus) { // set interrupt disable status
   (void)bus; // Unused parameter
   cpu->I = 1;
   cpu->PC += 1;
}
