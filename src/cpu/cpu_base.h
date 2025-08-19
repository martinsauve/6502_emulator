#include <stdint.h>
#include <stdbool.h>
#include "../6502_types.h"

#ifndef CPU_BASE_H
#define CPU_BASE_H

#ifndef BUS_H
typedef struct Bus Bus;
#endif // BUS_H

typedef enum {
   CPU_6502,
   CPU_65C02,
} CpuType;

typedef struct Cpu {
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
   CpuType type; // CPU Type

} Cpu;

typedef struct {
   void (*handler)(Cpu*, Bus*);
   uint8_t cycles;
} Opcodes;


void cpuReset(Cpu *cpu, Bus *bus);

Cpu* initCpu(CpuType type);
void freeCpu(Cpu *cpu);


void dumpCpu(Cpu *cpu);

void sleep_ms(int milliseconds);


void opUnknown(Cpu*, Bus*);
void opNOP(Cpu*, Bus*);
Cycles step(Cpu *cpu, Bus *bus, Opcodes *table);
void stepBatch(Cpu *cpu, Bus *bus, Opcodes *table, int batch_size, float freq);

void initOpcodeTable(Opcodes[256], CpuType);

Addr readAddr(Cpu*, Bus*);
void setZN(Cpu *cpu, Byte val);
void setV(Cpu *cpu, Byte a, Byte b, uint16_t result);

void pushStack(Cpu*, Bus*, Byte value);
Byte pullStack(Cpu*, Bus*);

#endif // CPU_BASE_H
