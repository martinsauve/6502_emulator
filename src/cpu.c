#include <stdbool.h>
#include <stdint.h>
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

void opUnknown(Cpu *cpu, Mem *mem){
   printf( "Unrecognized opcode 0x%02x at 0x%04x\n", mem->ROM[cpu->PC], cpu->PC );
   dumpRom(mem, "dump.bin");
   printf("Rom dumped!\n");

}

// untility : set Z and N flags after loading value
static void setZN(Cpu *cpu, uint8_t val) {
   cpu->Z = (val == 0);
   cpu->N = ( (val & 0x80) != 0 );
}


//*******************************************
// LOAD/STORE OPERATIONS
//*******************************************

//*******************************************
// LDA
//*******************************************

// immediate addressing mode (0xA9)
void opLDA_imm(Cpu *cpu, Mem *mem) {
   uint8_t val = mem->ROM[cpu->PC + 1];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}


// zero page (0xA5)
void opLDA_zp(Cpu *cpu, Mem *mem) {
   uint8_t addr = mem->ROM[cpu->PC + 1];
   uint8_t val = mem->ROM[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// zero page with X offset (0xB5)
void opLDA_zpX(Cpu *cpu, Mem *mem) {
   uint8_t addr = (mem->ROM[cpu->PC + 1] + cpu->X) & 0xFF; // wrap around
   uint8_t val = mem->ROM[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// absolute addressing mode (0xAD)
void opLDA_abs(Cpu *cpu, Mem *mem) {
   uint16_t addr = mem->ROM[cpu->PC + 1] | (mem->ROM[cpu->PC + 2] << 8);
   uint8_t val = mem->ROM[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// absolute with X offset (0xBD)
void opLDA_absX(Cpu *cpu, Mem *mem) {
   uint16_t base = mem->ROM[cpu->PC + 1] | (mem->ROM[cpu->PC + 2] << 8);
   uint16_t addr = (base + cpu->X) & 0xFFFF; // wrap around
   uint8_t val = mem->ROM[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// absolute with Y offset (0xB9)
void opLDA_absY(Cpu *cpu, Mem *mem) {
   uint16_t base = mem->ROM[cpu->PC + 1] | (mem->ROM[cpu->PC + 2] << 8);
   uint16_t addr = (base + cpu->Y) & 0xFFFF; // wrap around
   uint8_t val = mem->ROM[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// indirect with X offset to the pointer(0xA1)
void opLDA_indX(Cpu *cpu, Mem *mem) {
   uint8_t zp_addr = (mem->ROM[cpu->PC + 1] + cpu->X) & 0xFF;
   uint16_t addr = mem->ROM[zp_addr] | (mem->ROM[(zp_addr + 1) & 0xFF] << 8);
   uint8_t val = mem->ROM[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// indirect with Y offset to the addressk(0xA1)
void opLDA_indY(Cpu *cpu, Mem *mem) {
   uint8_t zp_addr = mem->ROM[cpu->PC + 1];
   uint16_t base = mem->ROM[zp_addr] | (mem->ROM[(zp_addr + 1) & 0xFF] << 8);
   uint16_t addr = (base + cpu->Y) & 0xFFFF;
   uint8_t val = mem->ROM[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}



// Store value in X, set negative and zero flag
void opLDX(Cpu *cpu, Mem *mem) {
  opUnknown(cpu, mem);
}


// Store value in Y, set negative and zero flag
void opLDY(Cpu *cpu, uint8_t val) {
}

// Store value in A, don't touch the flags
void opSTA(Cpu *cpu, uint8_t val) {
}

// Store value in X, don't touch the flags
void opSTX(Cpu *cpu, uint8_t val) {
}

// Store value in Y, don't touch the flags
void opSTY(Cpu *cpu, uint8_t val) {
}


//*******************************************
// REGISTER TRANSFERS
//*******************************************


void opTAX(Cpu *cpu) { // Transfer accumulator to X
}


void opTAY(Cpu *cpu) { // Transfer accumulator to Y
}
void opTXA(Cpu *cpu) { // Transfer X to accumulator
}

void opTYA(Cpu *cpu) { // Transfer Y to accumulator
}


//*******************************************
// STACK OPERATIONS
//*******************************************

void opTSX(Cpu *cpu, Mem *mem) {
   opUnknown(cpu, mem);
}

void opTXS(Cpu *cpu, Mem *mem) {
   opUnknown(cpu, mem);
}

void opPHA(Cpu *cpu, Mem *mem) {
   opUnknown(cpu, mem);
}

void opPHP(Cpu *cpu, Mem *mem) {
   opUnknown(cpu, mem);
}

void opPLA(Cpu *cpu, Mem *mem) {
   opUnknown(cpu, mem);
}

void opPLP(Cpu *cpu, Mem *mem) {
   opUnknown(cpu, mem);
}

uint16_t readAddr(Cpu *cpu, Mem *mem) {
   return (uint8_t)mem->ROM[cpu->PC] | ((uint8_t)mem->ROM[cpu->PC + 1] << 8);
}

void opNOP(Cpu *cpu, Mem *mem) {
   cpu->PC += 1;
}


void dumpReg(Cpu *cpu) {
   printf(" A | X | C | Z | N\n");
   printf(" %i | %i | %i | %i | %i\n", cpu->A,cpu->X, cpu->C, cpu->Z, cpu->N);
}

typedef struct {
   void (*handler)(Cpu *cpu, Mem *mem);
   uint8_t cycles;
} Opcodes;

Opcodes opcode_table[256];

void initOpcodeTable() {
   for (int i = 0; i < 256; i++) {
      opcode_table[i].handler = opUnknown;
      opcode_table[i].cycles = 2;
   }

   opcode_table[0xEA].handler = opNOP;
   opcode_table[0xEA].cycles = 2;

   opcode_table[0xA9].handler = opLDA_imm;
   opcode_table[0xA9].cycles = 2;

   opcode_table[0xA5].handler = opLDA_zp;
   opcode_table[0xA5].cycles = 3;

   opcode_table[0xB5].handler = opLDA_zpX;
   opcode_table[0xB5].cycles = 4;

   opcode_table[0xAD].handler = opLDA_abs;
   opcode_table[0xAD].cycles = 4;

   opcode_table[0xBD].handler = opLDA_absX;
   opcode_table[0xBD].cycles = 4; // TODO: or 5 if page boundary crossed

   opcode_table[0xB9].handler = opLDA_absY;
   opcode_table[0xB9].cycles = 4; // TODO: or 5 if page boundary crossed

   opcode_table[0xA1].handler = opLDA_indX;
   opcode_table[0xA1].cycles = 6;

   opcode_table[0xB1].handler = opLDA_indY;
   opcode_table[0xB1].cycles = 5; // TODO: or 6 if page boundary crossed
}
