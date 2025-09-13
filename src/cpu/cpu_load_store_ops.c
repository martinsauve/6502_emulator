#include "cpu.h"
#include "../bus.h"
#include "../6502_types.h"

//*******************************************
// LOAD/STORE OPERATIONS
//*******************************************

void opLDA(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc)
{
   AddrModeResult res = addrModeFunc(cpu, bus);
   cpu->A = res.value;
   setZN(cpu, cpu->A);
}

void opLDX(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc)
{
   AddrModeResult res = addrModeFunc(cpu, bus);
   cpu->X = res.value;
   setZN(cpu, cpu->X);
}

void opLDY(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc)
{
   AddrModeResult res = addrModeFunc(cpu, bus);
   cpu->Y = res.value;
   setZN(cpu, cpu->Y);
}

void  opSTA(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc)
{
   AddrModeResult res = addrModeFunc(cpu, bus);
   busWrite(bus, res.addr, cpu->A);
}

void  opSTX(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc)
{
   AddrModeResult res = addrModeFunc(cpu, bus);
   busWrite(bus, res.addr, cpu->X);
}

void  opSTY(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc)
{
   AddrModeResult res = addrModeFunc(cpu, bus);
   busWrite(bus, res.addr, cpu->Y);
}



// transfer a to x
void opTAX(Cpu *cpu, Bus *bus) {
   (void)bus;
   cpu->X = cpu->A;
   setZN(cpu, cpu->X);
   cpu->PC += 1;
}

// transfer a to y
void opTAY(Cpu *cpu, Bus *bus) {
   (void)bus;
   cpu->Y = cpu->A;
   setZN(cpu, cpu->Y);
   cpu->PC += 1;
}

// transfer x to a
void opTXA(Cpu *cpu, Bus *bus) {
   (void)bus;
   cpu->A = cpu->X;
   setZN(cpu, cpu->A);
   cpu->PC += 1;
}

// transfer y to a
void opTYA(Cpu *cpu, Bus *bus) {
   (void)bus;
   cpu->A = cpu->Y;
   setZN(cpu, cpu->A);
   cpu->PC += 1;
}

// push A onto stack
void opPHA(Cpu *cpu, Bus *bus) {
   pushStack(cpu, bus, cpu->A);
   cpu->PC += 1;
}

// pull A from stack
void opPLA(Cpu *cpu, Bus *bus) {
   cpu->A = pullStack(cpu, bus);
   setZN(cpu, cpu->A);
   cpu->PC += 1;
}


// push X onto stack
void opPHX(Cpu *cpu, Bus *bus) {
   pushStack(cpu, bus, cpu->X);
   cpu->PC += 1;
}


// pull X from stack
void opPLX(Cpu *cpu, Bus *bus) {
   cpu->X = pullStack(cpu, bus);
   setZN(cpu, cpu->X);
   cpu->PC += 1;
}
