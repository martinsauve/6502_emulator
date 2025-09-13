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
void opNOP(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc)
{
   (void)cpu;
   (void)bus;
   (void)addrModeFunc;
}

//void opNOP(Cpu *cpu, Bus *bus) {
//   (void)bus; // Unused parameter
//   cpu->PC += 1;
//}


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
   if (cpu->nmi_pending) {
      handleNMI(cpu, bus);
      cpu->nmi_pending = false;
   } else if (cpu->irq_pending && !cpu->I) {
      handleIRQ(cpu, bus);
      cpu->irq_pending = false;
   }
   Byte op = busRead(bus, cpu->PC);
   if (table[op].usesNewHandler && table[op].newHandler) {
      cpu->PC++;
      table[op].newHandler(cpu, bus, table[op].addrModeFunc);
   } else if (table[op].handler) {
      table[op].handler(cpu, bus);
   }
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
      opcode_table[i].newHandler = NULL;
      opcode_table[i].usesNewHandler = false;
      opcode_table[i].cycles = 2;
      opcode_table[i].addrModeFunc = NULL;
   }

   opcode_table[0xEA] = (Opcodes){ .newHandler = opNOP, .usesNewHandler = true, .addrModeFunc = addressingNone, .cycles = 2 };

   // *******************************************
   // LOAD/STORE OPERATIONS
   // *******************************************

   // LDA
   //opcode_table[0xA9] = (Opcodes){ .handler = opLDA_imm,  .cycles = 2 };
   //opcode_table[0xA5] = (Opcodes){ .handler = opLDA_zp,   .cycles = 3 };
   //opcode_table[0xAD] = (Opcodes){ .handler = opLDA_abs,  .cycles = 4 };
   //opcode_table[0xB5] = (Opcodes){ .handler = opLDA_zpX,  .cycles = 4 };
   //opcode_table[0xBD] = (Opcodes){ .handler = opLDA_absX, .cycles = 4 };
   //opcode_table[0xB9] = (Opcodes){ .handler = opLDA_absY, .cycles = 4 };
   //opcode_table[0xA1] = (Opcodes){ .handler = opLDA_indX, .cycles = 6 };
   //opcode_table[0xB1] = (Opcodes){ .handler = opLDA_indY, .cycles = 5 };

   opcode_table[0xA9] = (Opcodes){ .newHandler = opLDA,   .usesNewHandler = true, .addrModeFunc = addressingImmediate, .cycles = 2 };
   opcode_table[0xA5] = (Opcodes){ .newHandler = opLDA,   .usesNewHandler = true, .addrModeFunc = addressingZeroPage,  .cycles = 3 };
   opcode_table[0xAD] = (Opcodes){ .newHandler = opLDA,   .usesNewHandler = true, .addrModeFunc = addressingAbsolute,  .cycles = 4 };
   opcode_table[0xB5] = (Opcodes){ .newHandler = opLDA,   .usesNewHandler = true, .addrModeFunc = addressingZeroPageX,  .cycles = 4 };
   opcode_table[0xBD] = (Opcodes){ .newHandler = opLDA,   .usesNewHandler = true, .addrModeFunc = addressingAbsoluteX,  .cycles = 4 };
   opcode_table[0xB9] = (Opcodes){ .newHandler = opLDA,   .usesNewHandler = true, .addrModeFunc = addressingAbsoluteY,  .cycles = 4 };
   opcode_table[0xA1] = (Opcodes){ .newHandler = opLDA,   .usesNewHandler = true, .addrModeFunc = addressingIndirectX, .cycles = 6 };
   opcode_table[0xB1] = (Opcodes){ .newHandler = opLDA,   .usesNewHandler = true, .addrModeFunc = addressingIndirectY, .cycles = 5 };

   // TODO: or 6 if page boundary crossed }

   // LDX
   opcode_table[0xA2] = (Opcodes){ .handler = opLDX_imm,  .cycles = 2 };
   opcode_table[0xA6] = (Opcodes){ .handler = opLDX_zp,   .cycles = 3 };
   opcode_table[0xB6] = (Opcodes){ .handler = opLDX_zpY,  .cycles = 4 };
   opcode_table[0xAE] = (Opcodes){ .handler = opLDX_abs,  .cycles = 4 };
   opcode_table[0xBE] = (Opcodes){ .handler = opLDX_absY, .cycles = 4 };
   // +1 if page crossed

   // LDY
   opcode_table[0xA0] = (Opcodes){ .handler = opLDY_imm,  .cycles = 2 };
   opcode_table[0xA4] = (Opcodes){ .handler = opLDY_zp,   .cycles = 3 };
   opcode_table[0xB4] = (Opcodes){ .handler = opLDY_zpX,  .cycles = 4 };
   opcode_table[0xAC] = (Opcodes){ .handler = opLDY_abs,  .cycles = 4 };
   opcode_table[0xBC] = (Opcodes){ .handler = opLDY_absX, .cycles = 4 };
   // +1 if page crossed

   // STA
   opcode_table[0x85] = (Opcodes){ .handler = opSTA_zp,   .cycles = 3 };
   opcode_table[0x95] = (Opcodes){ .handler = opSTA_zpX,  .cycles = 4 };
   opcode_table[0x8D] = (Opcodes){ .handler = opSTA_abs,  .cycles = 4 };
   opcode_table[0x9D] = (Opcodes){ .handler = opSTA_absX, .cycles = 5 };
   opcode_table[0x99] = (Opcodes){ .handler = opSTA_absY, .cycles = 5 };
   opcode_table[0x81] = (Opcodes){ .handler = opSTA_indX, .cycles = 6 };
   opcode_table[0x91] = (Opcodes){ .handler = opSTA_indY, .cycles = 6 };

   // STX
   opcode_table[0x86] = (Opcodes){ .handler = opSTX_zp,   .cycles = 3 };
   opcode_table[0x96] = (Opcodes){ .handler = opSTX_zpY,  .cycles = 4 };
   opcode_table[0x8E] = (Opcodes){ .handler = opSTX_abs,  .cycles = 4 };

   // STY
   opcode_table[0x84] = (Opcodes){ .handler = opSTY_zp,   .cycles = 3 };
   opcode_table[0x94] = (Opcodes){ .handler = opSTY_zpX,  .cycles = 4 };
   opcode_table[0x8C] = (Opcodes){ .handler = opSTY_abs,  .cycles = 4 };

   // TRANSFER

   opcode_table[0xAA] = (Opcodes){ .handler = opTAX, .cycles = 2 };
   opcode_table[0xA8] = (Opcodes){ .handler = opTAY, .cycles = 2 };
   opcode_table[0x8A] = (Opcodes){ .handler = opTXA, .cycles = 2 };
   opcode_table[0x98] = (Opcodes){ .handler = opTYA, .cycles = 2 };
   // INCREMENT/DECREMENT
   opcode_table[0xE8] = (Opcodes){ .handler = opINX,     .cycles = 2 };
   opcode_table[0xC8] = (Opcodes){ .handler = opINY,     .cycles = 2 };
   opcode_table[0x88] = (Opcodes){ .handler = opDEY,     .cycles = 2 };
   opcode_table[0xCA] = (Opcodes){ .handler = opDEX,     .cycles = 2 };
   opcode_table[0xE6] = (Opcodes){ .handler = opINC_zp,  .cycles = 5 };
   opcode_table[0xF6] = (Opcodes){ .handler = opINC_zpX, .cycles = 6 };
   opcode_table[0xC6] = (Opcodes){ .handler = opDEC_zp,  .cycles = 5 };
   opcode_table[0xD6] = (Opcodes){ .handler = opDEC_zpX, .cycles = 6 };


   // BRANCH
   opcode_table[0x00] = (Opcodes){ .handler = opBRK, .cycles = 7 };
   opcode_table[0xF0] = (Opcodes){ .handler = opBEQ, .cycles = 2 };
   opcode_table[0xD0] = (Opcodes){ .handler = opBNE, .cycles = 2 };
   opcode_table[0x90] = (Opcodes){ .handler = opBCC, .cycles = 2 };
   opcode_table[0xB0] = (Opcodes){ .handler = opBCS, .cycles = 2 };
   opcode_table[0x30] = (Opcodes){ .handler = opBMI, .cycles = 2 };
   opcode_table[0x10] = (Opcodes){ .handler = opBPL, .cycles = 2 };
   opcode_table[0x50] = (Opcodes){ .handler = opBVC, .cycles = 2 };
   opcode_table[0x70] = (Opcodes){ .handler = opBVS, .cycles = 2 };
   //cycle count only valid if opBEQ does not take the branch, otherwise 3
   // TODO: fix the cycle counting by returning them from the handler
   // /!\ this will need to be done at some point i guess



   // STACK
   opcode_table[0x08] = (Opcodes){ .handler = opPHP, .cycles = 3 };
   opcode_table[0x28] = (Opcodes){ .handler = opPLP, .cycles = 4 };
   opcode_table[0x48] = (Opcodes){ .handler = opPHA, .cycles = 3 };
   opcode_table[0x68] = (Opcodes){ .handler = opPLA, .cycles = 4 };

   opcode_table[0xDA] = (Opcodes){ .handler = opPHX, .cycles = 3 };
   opcode_table[0xFA] = (Opcodes){ .handler = opPLX, .cycles = 4 };
   // SUBROUTINE
   opcode_table[0x20] = (Opcodes){ .handler = opJSR, .cycles = 6 };
   opcode_table[0x60] = (Opcodes){ .handler = opRTS, .cycles = 6 };

   // INTERRUPTS
   opcode_table[0x40] = (Opcodes){ .handler = opRTI, .cycles = 6 };

   // FLAGS
   opcode_table[0x18] = (Opcodes){ .handler = opCLC, .cycles = 2 };
   opcode_table[0xD8] = (Opcodes){ .handler = opCLD, .cycles = 2 };
   opcode_table[0x58] = (Opcodes){ .handler = opCLI, .cycles = 2 };
   opcode_table[0xB8] = (Opcodes){ .handler = opCLV, .cycles = 2 };
   opcode_table[0x38] = (Opcodes){ .handler = opSEC, .cycles = 2 };
   opcode_table[0xF8] = (Opcodes){ .handler = opSED, .cycles = 2 };
   opcode_table[0x78] = (Opcodes){ .handler = opSEI, .cycles = 2 };
   opcode_table[0x9A] = (Opcodes){ .handler = opTXS, .cycles = 2 };
   opcode_table[0xBA] = (Opcodes){ .handler = opTSX, .cycles = 2 };

   // ARITHMETIC
   opcode_table[0xC9] = (Opcodes){ .handler = opCMP_imm,  .cycles = 2 };
   opcode_table[0xC5] = (Opcodes){ .handler = opCMP_zp,   .cycles = 3 };
   opcode_table[0xD1] = (Opcodes){ .handler = opCMP_indY, .cycles = 5 };

   opcode_table[0xDD] = (Opcodes){ .handler = opCMP_absX, .cycles = 4 };

   opcode_table[0xC0] = (Opcodes){ .handler = opCPY_imm,  .cycles = 2 };
   opcode_table[0xC4] = (Opcodes){ .handler = opCPY_zp,   .cycles = 3 };

   opcode_table[0xE0] = (Opcodes){ .handler = opCPX_imm,  .cycles = 2 };
   opcode_table[0xE4] = (Opcodes){ .handler = opCPX_zp,   .cycles = 3 };

   opcode_table[0x29] = (Opcodes){ .handler = opAND_imm,  .cycles = 2 };
   opcode_table[0x2D] = (Opcodes){ .handler = opAND_abs,  .cycles = 4 };
   opcode_table[0x3D] = (Opcodes){ .handler = opAND_absX, .cycles = 4 };
   opcode_table[0x25] = (Opcodes){ .handler = opAND_zp,   .cycles = 4 };

   opcode_table[0x24] = (Opcodes){ .handler = opBIT_zp,   .cycles = 3 };
   opcode_table[0x2C] = (Opcodes){ .handler = opBIT_abs,  .cycles = 4 };
   opcode_table[0x3C] = (Opcodes){ .handler = opBIT_absX, .cycles = 4 };

   opcode_table[0x49] = (Opcodes){ .handler = opEOR_imm,  .cycles = 2 };
   opcode_table[0x45] = (Opcodes){ .handler = opEOR_zp,   .cycles = 2 };

   opcode_table[0x0A] = (Opcodes){ .handler = opASL_A,    .cycles = 2 };
   opcode_table[0x06] = (Opcodes){ .handler = opASL_zp,   .cycles = 5 };
   opcode_table[0x16] = (Opcodes){ .handler = opASL_zpX,  .cycles = 6 };

   opcode_table[0x4A] = (Opcodes){ .handler = opLSR_A,    .cycles = 2 };
   opcode_table[0x46] = (Opcodes){ .handler = opLSR_zp,   .cycles = 5 };
   opcode_table[0x56] = (Opcodes){ .handler = opLSR_zpX,  .cycles = 6 };

   opcode_table[0x05] = (Opcodes){ .handler = opORA_zp,   .cycles = 2 };
   opcode_table[0x09] = (Opcodes){ .handler = opORA_imm,  .cycles = 2 };
   opcode_table[0x0D] = (Opcodes){ .handler = opORA_abs,  .cycles = 4 };
   opcode_table[0x01] = (Opcodes){ .handler = opORA_indX, .cycles = 6 };

   opcode_table[0x2A] = (Opcodes){ .handler = opROL_A,    .cycles = 2 };
   opcode_table[0x26] = (Opcodes){ .handler = opROL_zp,   .cycles = 5 };
   opcode_table[0x36] = (Opcodes){ .handler = opROL_zpX,  .cycles = 6 };

   opcode_table[0x6A] = (Opcodes){ .handler = opROR_A,    .cycles = 2 };
   opcode_table[0x66] = (Opcodes){ .handler = opROR_zp,   .cycles = 5 };
   opcode_table[0x76] = (Opcodes){ .handler = opROR_zpX,  .cycles = 6 };

   opcode_table[0x69] = (Opcodes){ .handler = opADC_imm,  .cycles = 2 };
   opcode_table[0x65] = (Opcodes){ .handler = opADC_zp,   .cycles = 3 };
   opcode_table[0x75] = (Opcodes){ .handler = opADC_zpX,  .cycles = 4 };
   opcode_table[0x6D] = (Opcodes){ .handler = opADC_abs,  .cycles = 4 };
   opcode_table[0x7D] = (Opcodes){ .handler = opADC_absX, .cycles = 4 };
   opcode_table[0x79] = (Opcodes){ .handler = opADC_absY, .cycles = 4 };

   opcode_table[0xE5] = (Opcodes){ .handler = opSBC_zp,   .cycles = 3 };
   opcode_table[0xF5] = (Opcodes){ .handler = opSBC_zpX,  .cycles = 3 };
   opcode_table[0xE9] = (Opcodes){ .handler = opSBC_imm,  .cycles = 3 };
   opcode_table[0xED] = (Opcodes){ .handler = opSBC_abs,  .cycles = 3 };
   opcode_table[0xFD] = (Opcodes){ .handler = opSBC_absX, .cycles = 4 };
   opcode_table[0xF9] = (Opcodes){ .handler = opSBC_absY, .cycles = 4 };
   opcode_table[0xF1] = (Opcodes){ .handler = opSBC_indY, .cycles = 5 };

   opcode_table[0x04] = (Opcodes){ .handler = opTSB_zp,   .cycles = 5 };
   opcode_table[0x0C] = (Opcodes){ .handler = opTSB_abs,  .cycles = 5 };
   //JMP
   opcode_table[0x4C] = (Opcodes){ .handler = opJMP_abs,  .cycles = 3 };



   if (type == CPU_6502) { // if we want to emulate the 6502 JMP bug
      opcode_table[0x6C] = (Opcodes){ .handler = opJMP_ind_buggy, .cycles = 5 };


   } else if (type == CPU_65C02) { // 65C02 fixed JMP behavior and other 65C02 instructions

      opcode_table[0x6C] = (Opcodes){ .handler = opJMP_ind_fixed, .cycles = 5 };
      opcode_table[0x3A] = (Opcodes){ .handler = opDEC_A, .cycles = 2 };
      opcode_table[0x1A] = (Opcodes){ .handler = opINC_A, .cycles = 2 };
      // BBR
      opcode_table[0x0F] = (Opcodes){ .handler = opBBR0, .cycles = 4 };
      opcode_table[0x1F] = (Opcodes){ .handler = opBBR1, .cycles = 4 };
      opcode_table[0x2F] = (Opcodes){ .handler = opBBR2, .cycles = 4 };
      opcode_table[0x3F] = (Opcodes){ .handler = opBBR3, .cycles = 4 };
      opcode_table[0x4F] = (Opcodes){ .handler = opBBR4, .cycles = 4 };
      opcode_table[0x5F] = (Opcodes){ .handler = opBBR5, .cycles = 4 };
      opcode_table[0x6F] = (Opcodes){ .handler = opBBR6, .cycles = 4 };
      opcode_table[0x7F] = (Opcodes){ .handler = opBBR7, .cycles = 4 };
      // BBS
      opcode_table[0x8F] = (Opcodes){ .handler = opBBS0, .cycles = 4 };
      opcode_table[0x9F] = (Opcodes){ .handler = opBBS1, .cycles = 4 };
      opcode_table[0xAF] = (Opcodes){ .handler = opBBS2, .cycles = 4 };
      opcode_table[0xBF] = (Opcodes){ .handler = opBBS3, .cycles = 4 };
      opcode_table[0xCF] = (Opcodes){ .handler = opBBS4, .cycles = 4 };
      opcode_table[0xDF] = (Opcodes){ .handler = opBBS5, .cycles = 4 };
      opcode_table[0xEF] = (Opcodes){ .handler = opBBS6, .cycles = 4 };
      opcode_table[0xFF] = (Opcodes){ .handler = opBBS7, .cycles = 4 };

      opcode_table[0xC2] = (Opcodes){ .handler = opREP,  .cycles = 3 };

   }

}
