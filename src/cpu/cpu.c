#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpu.h"
#include "../bus.h"
#include "../6502_types.h"
#include "../acia.h"



// initialize a CPU structure
Cpu* initCpu(CpuType type) {
   Cpu *cpu = NULL;
   cpu = malloc(sizeof *cpu);

   cpu->PC = 0;
   cpu->SP = 0x00;
   cpu->A  = 0;
   cpu->X  = 0;
   cpu->Y  = 0;

   cpu->C  = 0;
   cpu->Z  = 0;
   cpu->I  = 1;
   cpu->D  = 0;
   cpu->B  = 0;
   cpu->V  = 0;
   cpu->N  = 0;

   cpu->irq_pending = 0;
   cpu->nmi_pending = 0;

   cpu->type = type;
   return cpu;

}


// free a CPU structure
void freeCpu(Cpu *cpu) {
   if (!cpu) return;
   free(cpu);
}

// Pack the flags bits in a byte to push on the stack for interrupts
Byte packStatusFlags(Cpu *cpu, bool break_flag) {
   return
      (cpu->N << 7) |
      (cpu->V << 6) |
      (1 << 5)      | // unused
      ((break_flag ? 1:  0) << 4) | // B flag (set for brk/irq, clear for nmi)
      (cpu->D << 3) |
      (cpu->I << 2) |
      (cpu->Z << 1) |
      (cpu->C << 0);
}

// set cpu flags from a packed byte (used when pulling flags from stack)
void unpackStatusFlags(Cpu *cpu, Byte status) {
   cpu->N = (status >> 7) & 1;
   cpu->V = (status >> 6) & 1;
   // not bit 5
   cpu->B = (status >> 4) & 1;
   cpu->D = (status >> 3) & 1;
   cpu->I = (status >> 2) & 1;
   cpu->Z = (status >> 1) & 1;
   cpu->C = (status >> 0) & 1;
}

// reset the CPU (equivalent to pulling the reset pin on pysical CPU)
void cpuReset(Cpu *cpu, Bus *bus) {
   Byte lo = busRead(bus, RESET_VECTOR_ADDR);
   Byte hi  = busRead(bus, RESET_VECTOR_ADDR + 1);
   cpu->PC = ((Addr)hi << 8) | lo;
   cpu->A = 0x00;
   cpu->X = 0x00;
   cpu->Y = 0x00;
   cpu->I = 1;
   cpu->D = 0;
   cpu->B = 0;
   cpu->N = 0;
   cpu->V = 0;
   cpu->Z = 0;
   cpu->C = 0;
   cpu->SP = 0xFD;
   cpu->irq_pending = 0;
   cpu->nmi_pending = 0;
}

// push flags and PC on stack, set PC to IRQBRK_VECTOR_ADDR
void handleBRK(Cpu *cpu, Bus *bus) {
   // Push PC high then low
   pushStack(cpu, bus, (cpu->PC >> 8) & 0xFF);
   pushStack(cpu, bus, cpu->PC & 0xFF);

   // Push status flags
   pushStack(cpu, bus, packStatusFlags(cpu, true));

   // set interrupt flag
   cpu->I = 1;

   // set pc to irq addr
   cpu->PC = (Addr)busRead(bus, IRQBRK_VECTOR_ADDR) | ((Byte)busRead(bus, IRQBRK_VECTOR_ADDR + 1) << 8);
}


// push flags and PC on stack, set PC to IRQBRK_VECTOR_ADDR
void handleIRQ(Cpu *cpu, Bus *bus) {
   // Push PC high then low
   pushStack(cpu, bus, (cpu->PC >> 8) & 0xFF);
   pushStack(cpu, bus, cpu->PC & 0xFF);

   // Push status flags
   pushStack(cpu, bus, packStatusFlags(cpu, false));

   // set interrupt flag
   cpu->I = 1;

   // set pc to irq addr
   cpu->PC = (Addr)busRead(bus, IRQBRK_VECTOR_ADDR) | ((Byte)busRead(bus, IRQBRK_VECTOR_ADDR + 1) << 8);
}

// push flags and PC on stack, set PC to NMI_VECTOR_ADDR
void handleNMI(Cpu *cpu, Bus *bus) {
   // Push PC high then low
   pushStack(cpu, bus, (cpu->PC >> 8) & 0xFF);
   pushStack(cpu, bus, cpu->PC & 0xFF);

   // Push status flags
   pushStack(cpu, bus, packStatusFlags(cpu, false));

   // set interrupt flag
   cpu->I = 1;

   // set pc to nmi addr
   cpu->PC = (Addr)busRead(bus, NMI_VECTOR_ADDR) | ((Byte)busRead(bus,  NMI_VECTOR_ADDR + 1) << 8);
}

void handleRTI(Cpu *cpu, Bus *bus) {
   Byte status = pullStack(cpu, bus);
   unpackStatusFlags(cpu, status);
   Byte lo = pullStack(cpu, bus);
   Byte hi = pullStack(cpu, bus);
   cpu->PC = ((Addr)hi << 8) | lo;
}

// clear flags with given mask in argument
void opREP(Cpu *cpu, Bus *bus) {
   Byte mask = busRead(bus, cpu->PC + 1);
   if(mask & 0x80) cpu->N = 0;
   if(mask & 0x40) cpu->V = 0;
   // ignore bit  5
   // ignore bit  4
   if(mask & 0x08) cpu->D = 0;
   if(mask & 0x04) cpu->I = 0;
   if(mask & 0x02) cpu->Z = 0;
   if(mask & 0x01) cpu->C = 0;

   cpu->PC += 2;
}

// Handle unknown opcode
void opUnknown(Cpu *cpu, Bus *bus){
   printf( "Unrecognized opcode 0x%02x at 0x%04x\n", busRead(bus, cpu->PC), cpu->PC );
   printf("Please make sure you are using the correct CPU type.\n");
   //dumpRom(bus, "dump.bin");
   exit(1);

}

// No Operation
void opNOP(Cpu *cpu, Bus *bus) {
   (void)bus; // Unused parameter
   cpu->PC += 1;
}


// dump CPU state to console
void dumpCpu(Cpu *cpu) {
   printf(" ┌──────────────┬──────┬───────────────────────────┐\n");
   printf(" │ CPU Registers│      │        CPU FLAGS          │\n");
   printf(" ├────┬────┬────┼──────┼───┬───┬───┬───┬───┬───┬───┤\n");
   printf(" │  A │  X │  Y │  SP  │ C │ Z │ I │ D │ B │ V │ N │ PC\n");
   printf(" ├────┼────┼────┼──────┼───┼───┼───┼───┼───┼───┼───┤\n");
   printf(" │ %02x │ %02x │ %02x │  %02x  │ %i │ %i │ %i │ %i │ %i │ %i │ %i │ %04x\n",
          cpu->A, cpu->X, cpu->Y, cpu->SP, cpu->C, cpu->Z, cpu->I, cpu->D, cpu->B, cpu->V, cpu->N, cpu->PC);
   printf(" └────┴────┴────┴──────┴───┴───┴───┴───┴───┴───┴───┘\n");
}

// Execute a single CPU operation and return the number of cycles it took
Cycles step(Cpu *cpu, Bus *bus, Opcodes *table){
   if (cpu->D) {
      fprintf(stderr, "Decimal mode not supported\n");
      exit(1);
   }
   if (cpu->nmi_pending) {
      handleNMI(cpu, bus);
      cpu->nmi_pending = false;
   } else if (cpu->irq_pending && !cpu->I) {
      handleIRQ(cpu, bus);
      cpu->irq_pending = false;
   }
   Byte op = busRead(bus, cpu->PC);
   table[op].handler(cpu, bus);
   return table[op].cycles;
}

// Execute a batch of CPU operations and sleep to maintain the desired frequency
void stepBatch(Cpu *cpu, Bus *bus, Opcodes *table, int batch_size, float freq) {
   Cycles total_cycles = 0;
   pollAciaInput(bus, cpu);
   for (int i = 0; i< batch_size; ++i) {
      total_cycles += step(cpu, bus, table);

   }
   sleep_ms((int)((total_cycles / freq) * 1000));
}



 void initOpcodeTable(Opcodes opcode_table[256], CpuType type) {
   for (int i = 0; i < 256; i++) {
      opcode_table[i].handler = opUnknown;
      opcode_table[i].cycles = 2;
   }

   opcode_table[0xEA].handler = opNOP;       opcode_table[0xEA].cycles = 2;

   // *******************************************
   // LOAD/STORE OPERATIONS
   // *******************************************

   // LDA
   opcode_table[0xA9].handler = opLDA_imm;  opcode_table[0xA9].cycles = 2;
   opcode_table[0xA5].handler = opLDA_zp;   opcode_table[0xA5].cycles = 3;
   opcode_table[0xB5].handler = opLDA_zpX;  opcode_table[0xB5].cycles = 4;
   opcode_table[0xAD].handler = opLDA_abs;  opcode_table[0xAD].cycles = 4;
   opcode_table[0xBD].handler = opLDA_absX; opcode_table[0xBD].cycles = 4;
   // TODO: or 5 if page boundary crossed
   opcode_table[0xB9].handler = opLDA_absY; opcode_table[0xB9].cycles = 4;
   // TODO: or 5 if page boundary crossed
   opcode_table[0xA1].handler = opLDA_indX; opcode_table[0xA1].cycles = 6;
   opcode_table[0xB1].handler = opLDA_indY; opcode_table[0xB1].cycles = 5;
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

   // TRANSFER

   opcode_table[0xAA].handler = opTAX;      opcode_table[0xAA].cycles = 2;
   opcode_table[0xA8].handler = opTAY;      opcode_table[0xA8].cycles = 2;
   opcode_table[0x8A].handler = opTXA;      opcode_table[0x8A].cycles = 2;
   opcode_table[0x98].handler = opTYA;      opcode_table[0x98].cycles = 2;
   // INCREMENT/DECREMENT
   opcode_table[0xE8].handler = opINX;      opcode_table[0xE8].cycles = 2;
   opcode_table[0xC8].handler = opINY;      opcode_table[0xC8].cycles = 2;
   opcode_table[0x88].handler = opDEY;      opcode_table[0x88].cycles = 2;
   opcode_table[0xCA].handler = opDEX;      opcode_table[0xCA].cycles = 2;
   opcode_table[0xE6].handler = opINC_zp;   opcode_table[0xE6].cycles = 5;
   opcode_table[0xF6].handler = opINC_zpX;  opcode_table[0xF6].cycles = 6;
   opcode_table[0xC6].handler = opDEC_zp;   opcode_table[0xC6].cycles = 5;
   opcode_table[0xD6].handler = opDEC_zpX;  opcode_table[0xD6].cycles = 6;


   // BRANCH
   opcode_table[0x00].handler = opBRK;      opcode_table[0x00].cycles = 7;
   opcode_table[0xF0].handler = opBEQ;      opcode_table[0xF0].cycles = 2;
   opcode_table[0xD0].handler = opBNE;      opcode_table[0xD0].cycles = 2;
   opcode_table[0x90].handler = opBCC;      opcode_table[0x90].cycles = 2;
   opcode_table[0xB0].handler = opBCS;      opcode_table[0xB0].cycles = 2;
   opcode_table[0x30].handler = opBMI;      opcode_table[0x30].cycles = 2;
   opcode_table[0x10].handler = opBPL;      opcode_table[0x10].cycles = 2;
   opcode_table[0x50].handler = opBVC;      opcode_table[0x50].cycles = 2;
   opcode_table[0x70].handler = opBVS;      opcode_table[0x70].cycles = 2;
   //cycle count only valid if opBEQ does not take the branch, otherwise 3
   // TODO: fix the cycle counting by returning them from the handler
   // /!\ this will need to be done at some point i guess



   // STACK
   opcode_table[0x08].handler = opPHP;      opcode_table[0x08].cycles = 3;
   opcode_table[0x28].handler = opPLP;      opcode_table[0x28].cycles = 4;
   opcode_table[0x48].handler = opPHA;      opcode_table[0x48].cycles = 3;
   opcode_table[0x68].handler = opPLA;      opcode_table[0x68].cycles = 4;

   opcode_table[0xDA].handler = opPHX;      opcode_table[0xDA].cycles = 3;
   opcode_table[0xFA].handler = opPLX;      opcode_table[0xFA].cycles = 4;
   // SUBROUTINE
   opcode_table[0x20].handler = opJSR;      opcode_table[0x20].cycles = 6;
   opcode_table[0x60].handler = opRTS;      opcode_table[0x60].cycles = 6;

   // INTERRUPTS
   opcode_table[0x40].handler = opRTI;      opcode_table[0x40].cycles = 6;

   // FLAGS
   opcode_table[0x18].handler = opCLC;      opcode_table[0x18].cycles = 2;
   opcode_table[0xD8].handler = opCLD;      opcode_table[0xD8].cycles = 2;
   opcode_table[0x58].handler = opCLI;      opcode_table[0x58].cycles = 2;
   opcode_table[0xB8].handler = opCLV;      opcode_table[0xB8].cycles = 2;
   opcode_table[0x38].handler = opSEC;      opcode_table[0x38].cycles = 2;
   opcode_table[0xF8].handler = opSED;      opcode_table[0xF8].cycles = 2;
   opcode_table[0x78].handler = opSEI;      opcode_table[0x78].cycles = 2;
   opcode_table[0x9A].handler = opTXS;      opcode_table[0x9A].cycles = 2;
   opcode_table[0xBA].handler = opTSX;      opcode_table[0xBA].cycles = 2;

   // ARITHMETIC
   opcode_table[0xC9].handler = opCMP_imm;   opcode_table[0xC9].cycles = 2;
   opcode_table[0xC5].handler = opCMP_zp;    opcode_table[0xC5].cycles = 3;
   opcode_table[0xD1].handler = opCMP_indY;  opcode_table[0xD1].cycles = 5;

   opcode_table[0xDD].handler = opCMP_absX;  opcode_table[0xDD].cycles = 4;

   opcode_table[0xC0].handler = opCPY_imm;   opcode_table[0xC0].cycles = 2;
   opcode_table[0xC4].handler = opCPY_zp;    opcode_table[0xC4].cycles = 3;

   opcode_table[0xE0].handler = opCPX_imm;   opcode_table[0xE0].cycles = 2;
   opcode_table[0xE4].handler = opCPX_zp;    opcode_table[0xE4].cycles = 3;

   opcode_table[0x29].handler = opAND_imm;   opcode_table[0x29].cycles = 2;
   opcode_table[0x2D].handler = opAND_abs;   opcode_table[0x2D].cycles = 4;
   opcode_table[0x3D].handler = opAND_absX;  opcode_table[0x3D].cycles = 4;
   opcode_table[0x25].handler = opAND_zp;    opcode_table[0x25].cycles = 4;

   opcode_table[0x24].handler = opBIT_zp;    opcode_table[0x24].cycles = 3;
   opcode_table[0x2C].handler = opBIT_abs;   opcode_table[0x2C].cycles = 4;
   opcode_table[0x3C].handler = opBIT_absX;  opcode_table[0x3C].cycles = 4;

   opcode_table[0x49].handler = opEOR_imm;   opcode_table[0x49].cycles = 2;
   opcode_table[0x45].handler = opEOR_zp;   opcode_table[0x45].cycles = 2;

   opcode_table[0x0A].handler = opASL_A;     opcode_table[0x0A].cycles = 2;
   opcode_table[0x06].handler = opASL_zp;    opcode_table[0x06].cycles = 5;
   opcode_table[0x16].handler = opASL_zpX;   opcode_table[0x16].cycles = 6;

   opcode_table[0x4A].handler = opLSR_A;     opcode_table[0x4A].cycles = 2;
   opcode_table[0x46].handler = opLSR_zp;    opcode_table[0x46].cycles = 5;
   opcode_table[0x56].handler = opLSR_zpX;   opcode_table[0x56].cycles = 6;

   opcode_table[0x05].handler = opORA_zp;    opcode_table[0x05].cycles = 2;
   opcode_table[0x09].handler = opORA_imm;   opcode_table[0x09].cycles = 2;
   opcode_table[0x0D].handler = opORA_abs;   opcode_table[0x0D].cycles = 4;
   opcode_table[0x01].handler = opORA_indX;  opcode_table[0x01].cycles = 6;

   opcode_table[0x2A].handler = opROL_A;     opcode_table[0x2A].cycles = 2;
   opcode_table[0x26].handler = opROL_zp;    opcode_table[0x26].cycles = 5;
   opcode_table[0x36].handler = opROL_zpX;   opcode_table[0x36].cycles = 6;

   opcode_table[0x6A].handler = opROR_A;     opcode_table[0x6A].cycles = 2;
   opcode_table[0x66].handler = opROR_zp;    opcode_table[0x66].cycles = 5;
   opcode_table[0x76].handler = opROR_zpX;   opcode_table[0x76].cycles = 6;

   opcode_table[0x69].handler = opADC_imm;   opcode_table[0x69].cycles = 2;
   opcode_table[0x65].handler = opADC_zp;    opcode_table[0x65].cycles = 3;
   opcode_table[0x75].handler = opADC_zpX;   opcode_table[0x75].cycles = 4;
   opcode_table[0x6D].handler = opADC_abs;   opcode_table[0x6D].cycles = 4;
   opcode_table[0x7D].handler = opADC_absX;  opcode_table[0x7D].cycles = 4;
   opcode_table[0x79].handler = opADC_absY;  opcode_table[0x79].cycles = 4;

   opcode_table[0xE5].handler = opSBC_zp;    opcode_table[0xE5].cycles = 3;
   opcode_table[0xF5].handler = opSBC_zpX;   opcode_table[0xF5].cycles = 3;
   opcode_table[0xE9].handler = opSBC_imm;   opcode_table[0xE9].cycles = 3;
   opcode_table[0xED].handler = opSBC_abs;   opcode_table[0xED].cycles = 3;
   opcode_table[0xFD].handler = opSBC_absX;  opcode_table[0xFD].cycles = 4;
   opcode_table[0xF9].handler = opSBC_absY;  opcode_table[0xF9].cycles = 4;
   opcode_table[0xF1].handler = opSBC_indY;  opcode_table[0xF1].cycles = 5;

   opcode_table[0x04].handler = opTSB_zp;    opcode_table[0x04].cycles = 5;
   opcode_table[0x0C].handler = opTSB_abs;   opcode_table[0x0C].cycles = 5;
   //JMP
   opcode_table[0x4C].handler = opJMP_abs;  opcode_table[0x4C].cycles = 3;



   if (type == CPU_6502) { // if we want to emulate the 6502 JMP bug
      opcode_table[0x6C].handler = opJMP_ind_buggy;  opcode_table[0x6C].cycles = 5;


   } else if (type == CPU_65C02) { // 65C02 fixed JMP behavior and other 65C02 instructions

      opcode_table[0x6C].handler = opJMP_ind_fixed;   opcode_table[0x6C].cycles = 5;
      opcode_table[0x3A].handler = opDEC_A;           opcode_table[0x3A].cycles = 2;
      opcode_table[0x1A].handler = opINC_A;           opcode_table[0x1A].cycles = 2;
      // BBR
      opcode_table[0x0F].handler = opBBR0;      opcode_table[0x0F].cycles = 4;
      opcode_table[0x1F].handler = opBBR1;      opcode_table[0x1F].cycles = 4;
      opcode_table[0x2F].handler = opBBR2;      opcode_table[0x2F].cycles = 4;
      opcode_table[0x3F].handler = opBBR3;      opcode_table[0x3F].cycles = 4;
      opcode_table[0x4F].handler = opBBR4;      opcode_table[0x4F].cycles = 4;
      opcode_table[0x5F].handler = opBBR5;      opcode_table[0x5F].cycles = 4;
      opcode_table[0x6F].handler = opBBR6;      opcode_table[0x6F].cycles = 4;
      opcode_table[0x7F].handler = opBBR7;      opcode_table[0x7F].cycles = 4;
      // BBS
      opcode_table[0x8F].handler = opBBS0;      opcode_table[0x8F].cycles = 4;
      opcode_table[0x9F].handler = opBBS1;      opcode_table[0x9F].cycles = 4;
      opcode_table[0xAF].handler = opBBS2;      opcode_table[0xAF].cycles = 4;
      opcode_table[0xBF].handler = opBBS3;      opcode_table[0xBF].cycles = 4;
      opcode_table[0xCF].handler = opBBS4;      opcode_table[0xCF].cycles = 4;
      opcode_table[0xDF].handler = opBBS5;      opcode_table[0xDF].cycles = 4;
      opcode_table[0xEF].handler = opBBS6;      opcode_table[0xEF].cycles = 4;
      opcode_table[0xFF].handler = opBBS7;      opcode_table[0xFF].cycles = 4;

      opcode_table[0xC2].handler = opREP;       opcode_table[0xC2].cycles = 3;

   }

}
