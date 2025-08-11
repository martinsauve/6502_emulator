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
//*******************************************
//LDA
void opLDA_imm(Cpu*, Bus*);
void opLDA_zp(Cpu*, Bus*);
void opLDA_zpX(Cpu*, Bus*);
void opLDA_abs(Cpu*, Bus*);
void opLDA_absX(Cpu*, Bus*);
void opLDA_absY(Cpu*, Bus*);
void opLDA_indX(Cpu*, Bus*);
void opLDA_indY(Cpu*, Bus*);

// LDX
void opLDX_imm(Cpu*, Bus*);
void opLDX_zp(Cpu*, Bus*);
void opLDX_zpY(Cpu*, Bus*);
void opLDX_abs(Cpu*, Bus*);
void opLDX_absY(Cpu*, Bus*);

// LDY
void opLDY_imm(Cpu*, Bus*);
void opLDY_zp(Cpu*, Bus*);
void opLDY_zpX(Cpu*, Bus*);
void opLDY_abs(Cpu*, Bus*);
void opLDY_absX(Cpu*, Bus*);

// STA
void opSTA_zp(Cpu*, Bus*);
void opSTA_zpX(Cpu*, Bus*);
void opSTA_abs(Cpu*, Bus*);
void opSTA_absX(Cpu*, Bus*);
void opSTA_absY(Cpu*, Bus*);
void opSTA_indX(Cpu*, Bus*);
void opSTA_indY(Cpu*, Bus*);

// STX
void opSTX_zp(Cpu*, Bus*);
void opSTX_zpY(Cpu*, Bus*);
void opSTX_abs(Cpu*, Bus*);

// STY
void opSTY_zp(Cpu*, Bus*);
void opSTY_zpX(Cpu*, Bus*);
void opSTY_abs(Cpu*, Bus*);
void step(Cpu *cpu);

uint16_t readAddr(Cpu*, Bus*);
void opNOP(Cpu*, Bus*);


#endif // CPU_H
