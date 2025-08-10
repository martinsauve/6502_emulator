#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"


Cpu* initCpu() {
   Cpu *cpu = NULL;
   cpu = malloc(sizeof *cpu);

   cpu->PC = 0;
   cpu->SP = 0;
   cpu->A  = 0;
   cpu->X  = 0;
   cpu->Y  = 0;

   cpu->C  = false;
   cpu->Z  = false;
   cpu->I  = false;
   cpu->D  = false;
   cpu->B  = false;
   cpu->V  = false;
   cpu->N  = false;
   return cpu;

}




//*******************************************
// LOAD/STORE OPERATIONS
//*******************************************

// Store value in A, set negative and zero flag
void opLDA(Cpu *cpu, Mem *mem) {
  cpu->A = val;
  if (val==0) cpu->Z = true; else cpu->Z = false;
  if (val<0)  cpu->N = true; else cpu->N = false;
}


// Store value in X, set negative and zero flag
void opLDX(Cpu *cpu, uint8_t val) {
  cpu->X = val;
  if (val==0) cpu->Z = true; else cpu->Z = false;
  if (val<0)  cpu->N = true; else cpu->N = false;
}


// Store value in Y, set negative and zero flag
void opLDY(Cpu *cpu, uint8_t val) {
  cpu->Y = val;
  if (val==0) cpu->Z = true; else cpu->Z = false;
  if (val<0)  cpu->N = true; else cpu->N = false;
}

// Store value in A, don't touch the flags
void opSTA(Cpu *cpu, uint8_t val) {
  cpu->A = val;
}

// Store value in X, don't touch the flags
void opSTX(Cpu *cpu, uint8_t val) {
  cpu->X = val;
}

// Store value in Y, don't touch the flags
void opSTY(Cpu *cpu, uint8_t val) {
  cpu->Y = val;
}


//*******************************************
// REGISTER TRANSFERS
//*******************************************


void opTAX(Cpu *cpu) { // Transfer accumulator to X
  cpu->X = cpu->A;
  if (cpu->A == 0) cpu->Z = true; else cpu->Z = false;
  if (cpu->A < 0)  cpu->N = true; else cpu->N = false;
}


void opTAY(Cpu *cpu) { // Transfer accumulator to Y
  cpu->Y = cpu->A;
  if (cpu->A == 0) cpu->Z = true; else cpu->Z = false;
  if (cpu->A < 0)  cpu->N = true; else cpu->N = false;
}
void opTXA(Cpu *cpu) { // Transfer X to accumulator
  cpu->A = cpu->X;
  if (cpu->A == 0) cpu->Z = true; else cpu->Z = false;
  if (cpu->A < 0)  cpu->N = true; else cpu->N = false;
}

void opTYA(Cpu *cpu) { // Transfer Y to accumulator
  cpu->A = cpu->Y;
  if (cpu->A == 0) cpu->Z = true; else cpu->Z = false;
  if (cpu->A < 0)  cpu->N = true; else cpu->N = false;
}


//*******************************************
// STACK OPERATIONS
//*******************************************

void opTSX(Cpu *cpu, Mem *mem) {
}

void opTXS(Cpu *cpu, Mem *mem) {
}

void opPHA(Cpu *cpu, Mem *mem) {
}

void opPHP(Cpu *cpu, Mem *mem) {
}

void opPLA(Cpu *cpu, Mem *mem) {
}

void opPLP(Cpu *cpu, Mem *mem) {
}


void step(Cpu *cpu) {
   cpu->PC++;
}

uint16_t readAddr(Cpu *cpu, Mem *mem) {
   return (uint8_t)mem->ROM[cpu->PC] | ((uint8_t)mem->ROM[cpu->PC + 1] << 8);
}

void opNOP(Cpu *cpu, Mem *mem) {
   step(cpu);
}


void opJMPdirect(Cpu *cpu, uint16_t val) {
   cpu->PC = val;
}

void opJMPindirect(Cpu *cpu, Mem *mem) {
   cpu->PC = readAddr(cpu, mem);
}

void opUnknown(Cpu *cpu, Mem *mem){
   printf("Unrecognized opcode %02x at %04x", cpu->PC, mem->ROM[cpu->PC]);
   abort();
}

void dumpReg(Cpu *cpu) {
   printf(" A | X | C | Z | N\n");
   printf(" %i | %i | %i | %i | %i\n", cpu->A,cpu->X, cpu->C, cpu->Z, cpu->N);
}

typedef void (*OpFunc)(Cpu *cpu, Mem *mem); // function pointers to opXXX to construct a table matching them with opcodes

OpFunc opcode_table[256];

void initOpcodeTable() {
   for (int i = 0; i < 256; i++) opcode_table[i] = opUnknown;
   opcode_table[0xEA] = opNOP;
   opcode_table[0xA9] = opLDA;
}
