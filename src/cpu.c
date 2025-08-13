#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpu.h"
#include "bus.h"
#include "6502_types.h"




Cpu* initCpu() {
   Cpu *cpu = NULL;
   cpu = malloc(sizeof *cpu);

   cpu->PC = 0;
   cpu->SP = 0xFF;
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
   exit(0);

}

void opNOP(Cpu *cpu, Bus *bus) {
   (void)bus; // Unused parameter
   cpu->PC += 1;
}

void opBEQ(Cpu *cpu, Bus *bus) { // branch if Z is set
   if (!cpu->Z) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)bus->memory[cpu->PC+1]; //cast to signed int for negative offsets
   return;
}

void opBNE(Cpu *cpu, Bus *bus) { // branch if Z is NOT set
   if (cpu->Z) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)bus->memory[cpu->PC+1]; //cast to signed int for negative offsets
   return;
}

void opBCC(Cpu *cpu, Bus *bus) { // branch if C is NOT set
   if (cpu->C) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)bus->memory[cpu->PC+1]; //cast to signed int for negative offsets
   return;
}

void opBCS(Cpu *cpu, Bus *bus) { // branch if C is set
   if (!cpu->C) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)bus->memory[cpu->PC+1]; //cast to signed int for negative offsets
   return;
}

void opBMI(Cpu *cpu, Bus *bus) { // branch if N is set
   if (!cpu->N) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)bus->memory[cpu->PC+1]; //cast to signed int for negative offsets
   return;
}

void opBPL(Cpu *cpu, Bus *bus) { // branch if N is NOT set
   if (cpu->N) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)bus->memory[cpu->PC+1]; //cast to signed int for negative offsets
   return;
}

void opBVC(Cpu *cpu, Bus *bus) { // branch if V is clear
   if (!cpu->V) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)bus->memory[cpu->PC+1]; //cast to signed int for negative offsets
   return;
}

void opBVS(Cpu *cpu, Bus *bus) { // branch if V is NOT clear
   if (cpu->V) {
      cpu->PC+=2;
      return;
   }
   cpu->PC = cpu->PC + 2 + (int8_t)bus->memory[cpu->PC+1]; //cast to signed int for negative offsets
   return;
}

void opCLC(Cpu *cpu, Bus *bus) { // clear carry flag
   cpu->C = 0;
   cpu->PC += 1;
}
void opCLD(Cpu *cpu, Bus *bus) { // clear decimal flag
   cpu->D = 0;
   cpu->PC += 1;
}

void opCLI(Cpu *cpu, Bus *bus) { // clear interrupt disable
   cpu->I = 0;
   cpu->PC += 1;
}

void opCLV(Cpu *cpu, Bus *bus) { // clear overflow
   cpu->V = 0;
   cpu->PC += 1;
}

void opSEC(Cpu *cpu, Bus *bus) { // set carry flag
   cpu->C = 1;
   cpu->PC += 1;
}

void opSED(Cpu *cpu, Bus *bus) { // set decimal flag
   cpu->D = 1;
   cpu->PC += 1;
}

void opSEI(Cpu *cpu, Bus *bus) { // set interrupt disable status
   cpu->I = 1;
   cpu->PC += 1;
}

void opINX(Cpu *cpu, Bus *bus) {
   (void)bus; // Unused
   cpu->X = (cpu->X + 1) & 0xff;
   cpu->Z = (cpu->X == 0);
   cpu->N = ( (cpu->X & 0x80) != 0 );
   cpu->PC += 1;
}
   // TODO: factor out setZN

void opJMP_abs(Cpu *cpu, Bus *bus) { // only absolute for the hello world
   cpu->PC = readAddr(cpu, bus);
}

void opJMP_ind(Cpu *cpu, Bus *bus) {
    Addr ptr = readAddr(cpu, bus);
    Byte lo = bus->memory[ptr];
    // Emulate 6502 bug: if low byte is $FF, high byte wraps around
    Byte hi = bus->memory[(ptr & 0xFF00) | ((ptr + 1) & 0x00FF)];
    cpu->PC = (Addr)lo | ((Addr)hi << 8);
}

void opJSR(Cpu *cpu, Bus *bus) {
   Addr target = bus->memory[cpu->PC +1] | (bus->memory[cpu->PC + 2] << 8);
   Addr ret = cpu->PC + 2;
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

void opPHA(Cpu *cpu, Bus *bus) {
   pushStack(cpu, bus, cpu->A);
   cpu->PC += 1;
}

void opPLA(Cpu *cpu, Bus *bus) {
   cpu->A = pullStack(cpu, bus);
   setZN(cpu, cpu->A);
   cpu->PC += 1;
}


//void dumpReg(Cpu *cpu) {
//   printf(" ┌────────────────────────────────┐\n");
//   printf(" │ CPU Registers                  │\n");
//   printf(" ├────┬────┬────┬─────┬───┬───┬───┤\n");
//   printf(" │  A │  X │  Y │     │ C │ Z │ N │\n");
//   printf(" ├────┼────┼────┼─────┼───┼───┼───┤\n");
//   printf(" │ %02x │ %02x │ %02x │     │ %i │ %i │ %i │\n", cpu->A,cpu->X,cpu->Y, cpu->C, cpu->Z, cpu->N);
//   printf(" └────┴────┴────┴─────┴───┴───┴───┘\n");
//}
void dumpReg(Cpu *cpu) {
   printf(" ┌─────────────────────────────────────────────────┐\n");
   printf(" │ CPU Registers                                   │\n");
   printf(" ├────┬────┬────┬──────┬───┬───┬───┬───┬───┬───┬───┤\n");
   printf(" │  A │  X │  Y │  SP  │ C │ Z │ I │ D │ B │ V │ N │ PC\n");
   printf(" ├────┼────┼────┼──────┼───┼───┼───┼───┼───┼───┼───┤\n");
   printf(" │ %02x │ %02x │ %02x │  %02x  │ %i │ %i │ %i │ %i │ %i │ %i │ %i │ %04x\n",
          cpu->A, cpu->X, cpu->Y, cpu->SP, cpu->C, cpu->Z, cpu->I, cpu->D, cpu->B, cpu->V, cpu->N, cpu->PC);
   printf(" └────┴────┴────┴──────┴───┴───┴───┴───┴───┴───┴───┘\n");
}

void step(Cpu *cpu, Bus *bus, float freq, Opcodes *table){
   Byte op = bus->memory[cpu->PC];
   table[op].handler(cpu, bus);
   sleep_ms(table[op].cycles / freq);
}



 void initOpcodeTable(Opcodes opcode_table[256]) {
   for (int i = 0; i < 256; i++) {
      opcode_table[i].handler = opUnknown;
      opcode_table[i].cycles = 2;
   }

   opcode_table[0xEA].handler = opNOP;       opcode_table[0xEA].cycles = 2;

   // *******************************************
   // LOAD/STORE OPERATIONS
   // *******************************************

   // LDA
   opcode_table[0xA9].handler = opLDA_imm;   opcode_table[0xA9].cycles = 2;
   opcode_table[0xA5].handler = opLDA_zp;    opcode_table[0xA5].cycles = 3;
   opcode_table[0xB5].handler = opLDA_zpX;   opcode_table[0xB5].cycles = 4;
   opcode_table[0xAD].handler = opLDA_abs;   opcode_table[0xAD].cycles = 4;
   opcode_table[0xBD].handler = opLDA_absX;  opcode_table[0xBD].cycles = 4;
   // TODO: or 5 if page boundary crossed
   opcode_table[0xB9].handler = opLDA_absY;  opcode_table[0xB9].cycles = 4;
   // TODO: or 5 if page boundary crossed
   opcode_table[0xA1].handler = opLDA_indX;  opcode_table[0xA1].cycles = 6;
   opcode_table[0xB1].handler = opLDA_indY;  opcode_table[0xB1].cycles = 5;
   // TODO: or 6 if page boundary crossed }

   // LDX
   opcode_table[0xA2].handler = opLDX_imm;  opcode_table[0xA2].cycles = 2;
   opcode_table[0xA6].handler = opLDX_zp;   opcode_table[0xA6].cycles = 3;
   opcode_table[0xB6].handler = opLDX_zpY;  opcode_table[0xB6].cycles = 4;
   opcode_table[0xAE].handler = opLDX_abs;  opcode_table[0xAE].cycles = 4;
   opcode_table[0xBE].handler = opLDX_absY; opcode_table[0xBE].cycles = 4;
   // +1 if page crossed

   // LDY
   opcode_table[0xA0].handler = opLDY_imm;  opcode_table[0xA0].cycles = 2;
   opcode_table[0xA4].handler = opLDY_zp;   opcode_table[0xA4].cycles = 3;
   opcode_table[0xB4].handler = opLDY_zpX;  opcode_table[0xB4].cycles = 4;
   opcode_table[0xAC].handler = opLDY_abs;  opcode_table[0xAC].cycles = 4;
   opcode_table[0xBC].handler = opLDY_absX; opcode_table[0xBC].cycles = 4;
   // +1 if page crossed

   // STA
   opcode_table[0x85].handler = opSTA_zp;   opcode_table[0x85].cycles = 3;
   opcode_table[0x95].handler = opSTA_zpX;  opcode_table[0x95].cycles = 4;
   opcode_table[0x8D].handler = opSTA_abs;  opcode_table[0x8D].cycles = 4;
   opcode_table[0x9D].handler = opSTA_absX; opcode_table[0x9D].cycles = 5;
   opcode_table[0x99].handler = opSTA_absY; opcode_table[0x99].cycles = 5;
   opcode_table[0x81].handler = opSTA_indX; opcode_table[0x81].cycles = 6;
   opcode_table[0x91].handler = opSTA_indY; opcode_table[0x91].cycles = 6;

   // STX
   opcode_table[0x86].handler = opSTX_zp;   opcode_table[0x86].cycles = 3;
   opcode_table[0x96].handler = opSTX_zpY;  opcode_table[0x96].cycles = 4;
   opcode_table[0x8E].handler = opSTX_abs;  opcode_table[0x8E].cycles = 4;

   // STY
   opcode_table[0x84].handler = opSTY_zp;   opcode_table[0x84].cycles = 3;
   opcode_table[0x94].handler = opSTY_zpX;  opcode_table[0x94].cycles = 4;
   opcode_table[0x8C].handler = opSTY_abs;  opcode_table[0x8C].cycles = 4;

   // INCREMENT X
   opcode_table[0xE8].handler = opINX;      opcode_table[0xE8].cycles = 2;

   // BRANCH
   opcode_table[0xF0].handler = opBEQ;      opcode_table[0xF0].cycles = 2; // Branch if Z == 0
   opcode_table[0x0D].handler = opBNE;      opcode_table[0x0D].cycles = 2; // Branch if Z == 1
   opcode_table[0x90].handler = opBCC;      opcode_table[0x90].cycles = 2; // Branch if C == 0
   opcode_table[0xB0].handler = opBCS;      opcode_table[0xB0].cycles = 2; // Branch if C == 1
   opcode_table[0x30].handler = opBMI;      opcode_table[0x30].cycles = 2; // Branch if N == 1
   opcode_table[0x10].handler = opBPL;      opcode_table[0x10].cycles = 2; // Branch if N == 0
   opcode_table[0x50].handler = opBVC;      opcode_table[0x50].cycles = 2; // Branch if V == 0
   opcode_table[0x70].handler = opBVS;      opcode_table[0x70].cycles = 2; // Branch if V == 1
   //cycle count only valid if opBEQ does not take the branch, otherwise 3
   // TODO: fix the cycle counting by returning them from the handler
   // /!\ this will need to be done at some point i guess

   //JMP
   opcode_table[0x4C].handler = opJMP_abs;  opcode_table[0x4C].cycles = 3;
   opcode_table[0x6C].handler = opJMP_ind;  opcode_table[0x6C].cycles = 5;

   // STACK
   opcode_table[0x48].handler = opPHA;      opcode_table[0x48].cycles = 3;
   opcode_table[0x68].handler = opPLA;      opcode_table[0x68].cycles = 3;

   // SUBROUTINE
   opcode_table[0x20].handler = opJSR;      opcode_table[0x20].cycles = 6;
   opcode_table[0x60].handler = opRTS;      opcode_table[0x60].cycles = 6;

   // FLAGS
   opcode_table[0x18].handler = opCLC;      opcode_table[0x18].cycles = 2;
   opcode_table[0xD8].handler = opCLD;      opcode_table[0xD8].cycles = 2;
   opcode_table[0x58].handler = opCLI;      opcode_table[0x58].cycles = 2;
   opcode_table[0xB8].handler = opCLV;      opcode_table[0xB8].cycles = 2;
   opcode_table[0x38].handler = opSEC;      opcode_table[0x38].cycles = 2;
   opcode_table[0xF8].handler = opSED;      opcode_table[0xF8].cycles = 2;
   opcode_table[0x78].handler = opSEI;      opcode_table[0x78].cycles = 2;
}
