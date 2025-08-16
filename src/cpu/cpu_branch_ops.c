#include "cpu.h"
#include "../bus.h"
#include "../6502_types.h"

void opBEQ(Cpu *cpu, Bus *bus) { // branch if Z is set
   if (!cpu->Z) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)busRead(bus, cpu->PC+1); //cast to signed int for negative offsets
   return;
}

void opBNE(Cpu *cpu, Bus *bus) { // branch if Z is NOT set
   if (cpu->Z) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)busRead(bus, cpu->PC+1); //cast to signed int for negative offsets
   return;
}

void opBCC(Cpu *cpu, Bus *bus) { // branch if C is NOT set
   if (cpu->C) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)busRead(bus, cpu->PC+1); //cast to signed int for negative offsets
   return;
}

void opBCS(Cpu *cpu, Bus *bus) { // branch if C is set
   if (!cpu->C) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)busRead(bus, cpu->PC+1); //cast to signed int for negative offsets
   return;
}

void opBMI(Cpu *cpu, Bus *bus) { // branch if N is set
   if (!cpu->N) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)busRead(bus, cpu->PC+1); //cast to signed int for negative offsets
   return;
}

void opBPL(Cpu *cpu, Bus *bus) { // branch if N is NOT set
   if (cpu->N) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)busRead(bus, cpu->PC+1); //cast to signed int for negative offsets
   return;
}

// CHECKMEEEEE
void opBVC(Cpu *cpu, Bus *bus) { // branch if V is clear
   if (cpu->V) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)busRead(bus, cpu->PC+1); //cast to signed int for negative offsets
   return;
}

// CHECKME THIS WAS MY POTENTIAL BUG!!!!!
void opBVS(Cpu *cpu, Bus *bus) { // branch if V is NOT clear
   if (!cpu->V) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)busRead(bus, cpu->PC+1); //cast to signed int for negative offsets
   return;
}
