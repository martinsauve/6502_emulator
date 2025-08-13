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

   opcode_table[0xE8].handler = opINX;      opcode_table[0xE8].cycles = 2;
   opcode_table[0xF0].handler = opBEQ;      opcode_table[0xF0].cycles = 2;
   //cycle count only valid if opBEQ does not take the branch, otherwise 3
   opcode_table[0x4C].handler = opJMP_abs;  opcode_table[0x4C].cycles = 3;
   opcode_table[0x6C].handler = opJMP_ind;  opcode_table[0x6C].cycles = 5;
}
