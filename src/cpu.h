#include <stdint.h>
#include <stdbool.h>
#include "memory.h"

#ifndef CPU_H
#define CPU_H

typedef struct {
   uint16_t PC; // Program Counter
   uint8_t  SP; // Stack Pointer
   uint8_t  A;  // Accumulator
   uint8_t  X;  // Index Register
   uint8_t  Y;  // Index Register

   bool    C;  // Carry Flag
   bool    Z;  // Zero Flag
   bool    I;  // Interupt Disable
   bool    D;  // Decimal Mode
   bool    B;  // Break Command
   bool    V;  // Overflow Flag
   bool    N;  // Negative Flag

} Cpu;



Cpu* initCpu();

void opUnknown(Cpu *cpu, Mem *mem);
//*******************************************
// LOAD/STORE OPERATIONS
// //*******************************************
void opLDA_imm(Cpu*, Mem*);
void opLDA_zp(Cpu*, Mem*);
void opLDA_zpX(Cpu*, Mem*);
void opLDA_abs(Cpu*, Mem*);
void opLDA_absX(Cpu*, Mem*);
void opLDA_absY(Cpu*, Mem*);
void opLDA_indX(Cpu*, Mem*);
void opLDA_indY(Cpu*, Mem*);

void opLDX(Cpu*, Mem*);
void opLDY(Cpu*, uint8_t);
void opSTA(Cpu*, uint8_t);
void opSTX(Cpu*, uint8_t);
void opSTY(Cpu*, uint8_t);
//****************************************
// REGISTER TRANSFERS
//****************************************
void opTAX(Cpu *cpu);
void opTAY(Cpu *cpu);
void opTXA(Cpu *cpu);
void opTYA(Cpu *cpu);
//****************************************
// STACK OPERATIONS
//****************************************
void opTSX(Cpu *cpu, Mem *mem);
void opTXS(Cpu *cpu, Mem *mem);
void opPHA(Cpu *cpu, Mem *mem);
void opPHP(Cpu *cpu, Mem *mem);
void opPLA(Cpu *cpu, Mem *mem);
void opPLP(Cpu *cpu, Mem *mem);


void opJMPdirect(Cpu *cpu, Mem *mem);
void opJMPindirect(Cpu *cpu, Mem *mem);

void step(Cpu *cpu);

uint16_t readAddr(Cpu *cpu, Mem *mem);
void opNOP(Cpu *cpu, Mem *mem);


#endif // CPU_H
