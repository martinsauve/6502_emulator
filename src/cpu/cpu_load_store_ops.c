#include "cpu.h"
#include "../bus.h"
#include "../6502_types.h"

//*******************************************
// LOAD/STORE OPERATIONS
//*******************************************

//*******************************************
// LDA
//*******************************************



// immediate addressing mode (0xA9)
void opLDA_imm(Cpu *cpu, Bus *bus) {
   Byte val = busRead(bus, cpu->PC + 1);
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}


// zero page (0xA5)
void opLDA_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte val = busRead(bus, addr);
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// zero page with X offset (0xB5)
void opLDA_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF; // wrap around
   Byte val = busRead(bus, addr);
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// absolute addressing mode (0xAD)
void opLDA_abs(Cpu *cpu, Bus *bus) {
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Byte val = busRead(bus, addr);
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// absolute with X offset (0xBD)
void opLDA_absX(Cpu *cpu, Bus *bus) {
   Addr base = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Addr addr = (base + cpu->X) & 0xFFFF; // wrap around
   Byte val = busRead(bus, addr);
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// absolute with Y offset (0xB9)
void opLDA_absY(Cpu *cpu, Bus *bus) {
   Addr base = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF; // wrap around
   Byte val = busRead(bus, addr);
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// indirect with X offset to the pointer(0xA1)
void opLDA_indX(Cpu *cpu, Bus *bus) {
   Byte zp_addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF;
   Addr addr = busRead(bus, zp_addr) | (busRead(bus, (zp_addr + 1) & 0xFF) << 8);
   Byte val = busRead(bus, addr);
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// indirect with Y offset to the address(0xB1)
void opLDA_indY(Cpu *cpu, Bus *bus) {
   Byte zp_addr = busRead(bus, cpu->PC + 1);
   Addr base = busRead(bus, zp_addr) | (busRead(bus, (zp_addr + 1) & 0xFF) << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF;
   Byte val = busRead(bus, addr);
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}


//*******************************************
// LDX
// *******************************************

// immediate addressing mode (0xA2)
void opLDX_imm(Cpu *cpu, Bus *bus) {
   Byte val = busRead(bus, cpu->PC + 1);
   cpu->X = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// zero page (0xA6)
void opLDX_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte val = busRead(bus, addr);
   cpu->X = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// zero page with Y offset (0xB6)
void opLDX_zpY(Cpu *cpu, Bus *bus) {
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->Y) & 0xFF; // wrap around
   Byte val = busRead(bus, addr);
   cpu->X = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// absolute addressing mode (0xAE)
void opLDX_abs(Cpu *cpu, Bus *bus) {
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Byte val = busRead(bus, addr);
   cpu->X = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// absolute with Y offset (0xBE)
void opLDX_absY(Cpu *cpu, Bus *bus) {
   Addr base = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF; // wrap around
   Byte val = busRead(bus, addr);
   cpu->X = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

//*******************************************
// LDY
// *******************************************

// immediate addressing mode (0xA0)
void opLDY_imm(Cpu *cpu, Bus *bus) {
   Byte val = busRead(bus, cpu->PC + 1);
   cpu->Y = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// zero page (0xA4)
void opLDY_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte val = busRead(bus, addr);
   cpu->Y = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// zero page with X offset (0xB4)
void opLDY_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF; // wrap around
   Byte val = busRead(bus, addr);
   cpu->Y = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// absolute addressing mode (0xAC)
void opLDY_abs(Cpu *cpu, Bus *bus) {
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Byte val = busRead(bus, addr);
   cpu->Y = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// absolute with X offset (0xBC)
void opLDY_absX(Cpu *cpu, Bus *bus) {
   Addr base = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Addr addr = (base + cpu->X) & 0xFFFF; // wrap around
   Byte val = busRead(bus, addr);
   cpu->Y = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// STORE

//*******************************************
// STA
// *******************************************


// zero page (0x85)
void opSTA_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC + 1);
   busWrite(bus, addr, cpu->A);
   cpu->PC += 2;
}

// zero page,X (0x95)
void opSTA_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF;
   busWrite(bus, addr, cpu->A);
   cpu->PC += 2;
}

// absolute (0x8D)
void opSTA_abs(Cpu *cpu, Bus *bus) {
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   busWrite(bus, addr, cpu->A);
   cpu->PC += 3;
}

// absolute,X (0x9D)
void opSTA_absX(Cpu *cpu, Bus *bus) {
   Addr base = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Addr addr = (base + cpu->X) & 0xFFFF;
   busWrite(bus, addr, cpu->A);
   cpu->PC += 3;
}

// absolute,Y (0x99)
void opSTA_absY(Cpu *cpu, Bus *bus) {
   Addr base = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF;
   busWrite(bus, addr, cpu->A);
   cpu->PC += 3;
}

// (indirect,X) (0x81)
void opSTA_indX(Cpu *cpu, Bus *bus) {
   Byte zp_addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF;
   Addr addr = busRead(bus, zp_addr) | (busRead(bus, (zp_addr + 1) & 0xFF) << 8);
   busWrite(bus, addr, cpu->A);
   cpu->PC += 2;
}

// (indirect),Y (0x91)
void opSTA_indY(Cpu *cpu, Bus *bus) {
   Byte zp_addr = busRead(bus, cpu->PC + 1);
   Addr base = busRead(bus, zp_addr) | (busRead(bus, (zp_addr + 1) & 0xFF) << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF;
   busWrite(bus, addr, cpu->A);
   cpu->PC += 2;
}

//*******************************************
// STX (Store X Register)
//*******************************************

// zero page (0x86)
void opSTX_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC + 1);
   busWrite(bus, addr, cpu->X);
   cpu->PC += 2;
}

// zero page,Y (0x96)
void opSTX_zpY(Cpu *cpu, Bus *bus) {
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->Y) & 0xFF;
   busWrite(bus, addr, cpu->X);
   cpu->PC += 2;
}

// absolute (0x8E)
void opSTX_abs(Cpu *cpu, Bus *bus) {
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   busWrite(bus, addr, cpu->X);
   cpu->PC += 3;
}

//*******************************************
// STY (Store Y Register)
//*******************************************

// zero page (0x84)
void opSTY_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC + 1);
   busWrite(bus, addr, cpu->Y);
   cpu->PC += 2;
}

// zero page,X (0x94)
void opSTY_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF;
   busWrite(bus, addr, cpu->Y);
   cpu->PC += 2;
}

// absolute (0x8C)
void opSTY_abs(Cpu *cpu, Bus *bus) {
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   busWrite(bus, addr, cpu->Y);
   cpu->PC += 3;
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
