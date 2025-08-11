#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "bus.h"
#include "6502_types.h"


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

void opUnknown(Cpu *cpu, Bus *bus){
   printf( "Unrecognized opcode 0x%02x at 0x%04x\n", bus->memory[cpu->PC], cpu->PC );
   dumpBus(bus, "dump.bin");
   printf("Rom dumped!\n");

}

// untility : set Z and N flags after loading value
static void setZN(Cpu *cpu, Byte val) {
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
void opLDA_imm(Cpu *cpu, Bus *bus) {
   Byte val = bus->memory[cpu->PC + 1];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}


// zero page (0xA5)
void opLDA_zp(Cpu *cpu, Bus *bus) {
   Byte addr = bus->memory[cpu->PC + 1];
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// zero page with X offset (0xB5)
void opLDA_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (bus->memory[cpu->PC + 1] + cpu->X) & 0xFF; // wrap around
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// absolute addressing mode (0xAD)
void opLDA_abs(Cpu *cpu, Bus *bus) {
   Addr addr = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// absolute with X offset (0xBD)
void opLDA_absX(Cpu *cpu, Bus *bus) {
   Addr base = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Addr addr = (base + cpu->X) & 0xFFFF; // wrap around
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// absolute with Y offset (0xB9)
void opLDA_absY(Cpu *cpu, Bus *bus) {
   Addr base = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF; // wrap around
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// indirect with X offset to the pointer(0xA1)
void opLDA_indX(Cpu *cpu, Bus *bus) {
   Byte zp_addr = (bus->memory[cpu->PC + 1] + cpu->X) & 0xFF;
   Addr addr = bus->memory[zp_addr] | (bus->memory[(zp_addr + 1) & 0xFF] << 8);
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// indirect with Y offset to the addressk(0xA1)
void opLDA_indY(Cpu *cpu, Bus *bus) {
   Byte zp_addr = bus->memory[cpu->PC + 1];
   Addr base = bus->memory[zp_addr] | (bus->memory[(zp_addr + 1) & 0xFF] << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF;
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}



// Store value in X, set negative and zero flag
void opLDX(Cpu *cpu, Bus *bus) {
  opUnknown(cpu, bus);
}


// Store value in Y, set negative and zero flag
void opLDY(Cpu *cpu, Byte val) {
}

// Store value in A, don't touch the flags
void opSTA(Cpu *cpu, Byte val) {
}

// Store value in X, don't touch the flags
void opSTX(Cpu *cpu, Byte val) {
}

// Store value in Y, don't touch the flags
void opSTY(Cpu *cpu, Byte val) {
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

void opTSX(Cpu *cpu, Bus *bus) {
   opUnknown(cpu, bus);
}

void opTXS(Cpu *cpu, Bus *bus) {
   opUnknown(cpu, bus);
}

void opPHA(Cpu *cpu, Bus *bus) {
   opUnknown(cpu, bus);
}

void opPHP(Cpu *cpu, Bus *bus) {
   opUnknown(cpu, bus);
}

void opPLA(Cpu *cpu, Bus *bus) {
   opUnknown(cpu, bus);
}

void opPLP(Cpu *cpu, Bus *bus) {
   opUnknown(cpu, bus);
}

Addr readAddr(Cpu *cpu, Bus *bus) {
   return (Byte)bus->memory[cpu->PC] | ((Byte)bus->memory[cpu->PC + 1] << 8);
}

void opNOP(Cpu *cpu, Bus *bus) {
   cpu->PC += 1;
}


void dumpReg(Cpu *cpu) {
   printf(" A | X | C | Z | N\n");
   printf(" %i | %i | %i | %i | %i\n", cpu->A,cpu->X, cpu->C, cpu->Z, cpu->N);
}

typedef struct {
   void (*handler)(Cpu *cpu, Bus *bus);
   unsigned cycles;
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
