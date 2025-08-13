#include <stdint.h>
#include <stdbool.h>
#include "bus.h"
#include "6502_types.h"

#ifndef CPU_BASE_H
#define CPU_BASE_H

typedef struct {
   Addr  PC; // Program Counter
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

typedef struct {
   void (*handler)(Cpu*, Bus*);
   uint8_t cycles;
} Opcodes;


Cpu* initCpu();
void dumpReg(Cpu *cpu);

void sleep_ms(int milliseconds);


void opUnknown(Cpu*, Bus*);
void opNOP(Cpu*, Bus*);
void step(Cpu *cpu, Bus *bus, float freq, Opcodes *table);
void initOpcodeTable(Opcodes[256]);

Addr readAddr(Cpu*, Bus*);
void setZN(Cpu *cpu, Byte val);

#endif // CPU_BASE_H
