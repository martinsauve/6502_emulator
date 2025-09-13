#include "cpu.h"
#include "../bus.h"
#include "../6502_types.h"
#include <stdint.h>

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

void opBVC(Cpu *cpu, Bus *bus) { // branch if V is clear
   if (cpu->V) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)busRead(bus, cpu->PC+1); //cast to signed int for negative offsets
   return;
}

void opBVS(Cpu *cpu, Bus *bus) { // branch if V is NOT clear
   if (!cpu->V) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)busRead(bus, cpu->PC+1); //cast to signed int for negative offsets
   return;
}


// JUMP instructions



void opJMP_abs(Cpu *cpu, Bus *bus) {
   cpu->PC = readAddr(cpu, bus);
}

void opJMP_ind_buggy(Cpu *cpu, Bus *bus) {
    Addr ptr = (Addr)busRead(bus, cpu->PC + 1) | ((Byte)busRead(bus, cpu->PC + 2) << 8);
    Byte lo = busRead(bus, ptr);
    // Emulate 6502 bug: if low byte is $FF, high byte wraps around
    Byte hi = busRead(bus, (ptr & 0xFF00) | ((ptr + 1) & 0x00FF));
    cpu->PC = (Addr)lo | ((Addr)hi << 8);
}

void opJMP_ind_fixed(Cpu *cpu, Bus *bus) {
    Addr ptr = (Addr)busRead(bus, cpu->PC + 1) | ((Byte)busRead(bus, cpu->PC + 2) << 8);
    Byte lo = busRead(bus, ptr);
    Byte hi = busRead(bus, (ptr + 1) & 0xFFFF); // Correctly read the next byte
    cpu->PC = (Addr)lo | ((Addr)hi << 8);
}

void opJSR(Cpu *cpu, Bus *bus) {
   Addr target = busRead(bus, cpu->PC +1) | (busRead(bus, cpu->PC + 2) << 8);
   Addr ret = cpu->PC + 2; // 3-1
   pushStack(cpu, bus, (ret >> 8) & 0xFF); //high byte
   pushStack(cpu, bus, ret & 0xFF); //high byte
   cpu->PC = target;
}

void opRTS(Cpu *cpu, Bus *bus) {
   Byte lo = pullStack(cpu, bus);
   Byte hi = pullStack(cpu, bus);
   Addr ret = ((Addr)hi<<8 | lo);
   cpu->PC = ret + 1;
}

void opRTI(Cpu *cpu, Bus *bus) {
   handleRTI(cpu, bus);
}

void handleBBS(Cpu *cpu, Bus *bus, Byte mask) {
   Byte zp_addr = busRead(bus, cpu->PC + 1);
   Byte rel_offset = busRead(bus, cpu->PC + 2);
   Byte zp_value = busRead(bus, zp_addr);

   cpu->PC += 3;

   if ((zp_value & mask) != 0) {
      // branch if bit is clear
      // cast to signed as offset is signed
      cpu->PC += (int8_t)rel_offset;
   }
}

void handleBBR(Cpu *cpu, Bus *bus, Byte mask) {
   Byte zp_addr = busRead(bus, cpu->PC + 1);
   Byte rel_offset = busRead(bus, cpu->PC + 2);
   Byte zp_value = busRead(bus, zp_addr);

   cpu->PC += 3;

   if ((zp_value & mask) == 0) {
      // branch if bit is clear
      // cast to signed as offset is signed
      cpu->PC += (int8_t)rel_offset;
   }
}
void opBBS0(Cpu *cpu, Bus *bus) { handleBBS(cpu, bus, 0x01); }
void opBBS1(Cpu *cpu, Bus *bus) { handleBBS(cpu, bus, 0x02); }
void opBBS2(Cpu *cpu, Bus *bus) { handleBBS(cpu, bus, 0x04); }
void opBBS3(Cpu *cpu, Bus *bus) { handleBBS(cpu, bus, 0x08); }
void opBBS4(Cpu *cpu, Bus *bus) { handleBBS(cpu, bus, 0x10); }
void opBBS5(Cpu *cpu, Bus *bus) { handleBBS(cpu, bus, 0x20); }
void opBBS6(Cpu *cpu, Bus *bus) { handleBBS(cpu, bus, 0x40); }
void opBBS7(Cpu *cpu, Bus *bus) { handleBBS(cpu, bus, 0x80); }

void opBBR0(Cpu *cpu, Bus *bus) { handleBBR(cpu, bus, 0x01); }
void opBBR1(Cpu *cpu, Bus *bus) { handleBBR(cpu, bus, 0x02); }
void opBBR2(Cpu *cpu, Bus *bus) { handleBBR(cpu, bus, 0x04); }
void opBBR3(Cpu *cpu, Bus *bus) { handleBBR(cpu, bus, 0x08); }
void opBBR4(Cpu *cpu, Bus *bus) { handleBBR(cpu, bus, 0x10); }
void opBBR5(Cpu *cpu, Bus *bus) { handleBBR(cpu, bus, 0x20); }
void opBBR6(Cpu *cpu, Bus *bus) { handleBBR(cpu, bus, 0x40); }
void opBBR7(Cpu *cpu, Bus *bus) { handleBBR(cpu, bus, 0x80); }



void opBRK(Cpu *cpu, Bus *bus) {
   cpu->PC +=1;
   handleBRK(cpu, bus);
}
