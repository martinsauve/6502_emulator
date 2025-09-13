#include <stdint.h>
#include <stdbool.h>
#include "../6502_types.h"

#ifndef CPU_BASE_H
#define CPU_BASE_H


#define CPU_FREQ         1000000
#define CPU_BATCH_SIZE   10000

#ifndef BUS_H
typedef struct Bus Bus;
#endif // BUS_H

typedef enum {
   CPU_6502,
   CPU_65C02,
} CpuType;

typedef struct Cpu {
   Addr  PC;   // Program Counter
   Byte  SP;   // Stack Pointer
   Byte  A;    // Accumulator
   Byte  X;    // Index Register
   Byte  Y;    // Index Register

   bool    C;  // Carry Flag
   bool    Z;  // Zero Flag
   bool    I;  // Interupt Disable
   bool    D;  // Decimal Mode
   bool    B;  // Break Command
   bool    V;  // Overflow Flag
   bool    N;  // Negative Flag


   bool    irq_pending; // interrupt requested
   bool    nmi_pending; // non masquable interrupt requested

   CpuType type; // CPU Type

} Cpu;

// forward declare, prototypes in cpu_addressing_modes.h

typedef struct AddrModeResult AddrModeResult;
typedef AddrModeResult (*AddrModeFunc)(Cpu*, Bus*);

typedef struct {
   void (*handler)(Cpu*, Bus*);
   void (*newHandler)(Cpu*, Bus*, AddrModeFunc addrModeFunc);
   bool usesNewHandler;
   AddrModeFunc addrModeFunc;
   uint8_t cycles;
} Opcodes;


void cpuReset(Cpu *cpu, Bus *bus);

Cpu* initCpu(CpuType type);
void freeCpu(Cpu *cpu);


void dumpCpu(Cpu *cpu);

void sleep_ms(int milliseconds);


void opUnknown(Cpu*, Bus*);
void opNOP(Cpu*, Bus*, AddrModeFunc);


Cycles step(Cpu *cpu, Bus *bus, Opcodes *table);
void stepBatch(Cpu *cpu, Bus *bus, Opcodes *table, int batch_size, float freq);

void initOpcodeTable(Opcodes[256], CpuType);

Addr readAddr(Cpu*, Bus*);
void setZN(Cpu *cpu, Byte val);
void setV(Cpu *cpu, Byte a, Byte b, uint16_t result);

void pushStack(Cpu*, Bus*, Byte value);
Byte pullStack(Cpu*, Bus*);


Byte packStatusFlags(Cpu *cpu, bool break_flag);
void unpackStatusFlags(Cpu *cpu, Byte status);

void handleIRQ(Cpu *cpu, Bus *bus);
void handleNMI(Cpu *cpu, Bus *bus);
void handleRTI(Cpu *cpu, Bus *bus);
void handleBRK(Cpu *cpu, Bus *bus);

#endif // CPU_BASE_H
