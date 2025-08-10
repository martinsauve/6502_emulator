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

//*******************************************
// LOAD/STORE OPERATIONS
// //*******************************************
void opLDA(Cpu *cpu, Mem *mem);
void opLDX(Cpu *cpu, uint8_t val);
void opLDY(Cpu *cpu, uint8_t val);
void opSTA(Cpu *cpu, uint8_t val);
void opSTX(Cpu *cpu, uint8_t val);
void opSTY(Cpu *cpu, uint8_t val);
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


void opJMPdirect(Cpu *cpu, uint16_t val);
void opJMPindirect(Cpu *cpu, Mem *mem);

void step(Cpu *cpu);

uint16_t readAddr(Cpu *cpu, Mem *mem);
void opNOP(Cpu *cpu, Mem *mem);


#endif // CPU_H
