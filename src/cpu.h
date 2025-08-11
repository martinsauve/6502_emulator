#include <stdint.h>
#include <stdbool.h>
#include "bus.h"
#include "6502_types.h"

#ifndef CPU_H
#define CPU_H

typedef struct {
   Addr PC; // Program Counter
   Byte  SP; // Stack Pointer
   Byte  A;  // Accumulator
   Byte  X;  // Index Register
   Byte  Y;  // Index Register

   bool    C;  // Carry Flag
   bool    Z;  // Zero Flag
   bool    I;  // Interupt Disable
   bool    D;  // Decimal Mode
   bool    B;  // Break Command
   bool    V;  // Overflow Flag
   bool    N;  // Negative Flag

} Cpu;



Cpu* initCpu();

void opUnknown(Cpu *cpu, Bus *mem);
//*******************************************
// LOAD/STORE OPERATIONS
// //*******************************************
void opLDA_imm(Cpu*, Bus*);
void opLDA_zp(Cpu*, Bus*);
void opLDA_zpX(Cpu*, Bus*);
void opLDA_abs(Cpu*, Bus*);
void opLDA_absX(Cpu*, Bus*);
void opLDA_absY(Cpu*, Bus*);
void opLDA_indX(Cpu*, Bus*);
void opLDA_indY(Cpu*, Bus*);

void opLDX(Cpu*, Bus*);
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
void opTSX(Cpu *cpu, Bus *mem);
void opTXS(Cpu *cpu, Bus *mem);
void opPHA(Cpu *cpu, Bus *mem);
void opPHP(Cpu *cpu, Bus *mem);
void opPLA(Cpu *cpu, Bus *mem);
void opPLP(Cpu *cpu, Bus *mem);


void opJMPdirect(Cpu *cpu, Bus *mem);
void opJMPindirect(Cpu *cpu, Bus *mem);

void step(Cpu *cpu);

uint16_t readAddr(Cpu *cpu, Bus *mem);
void opNOP(Cpu *cpu, Bus *mem);


#endif // CPU_H
