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
   Byte val = bus->memory[cpu->PC + 1];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}


// zero page (0xA5)
void opLDA_zp(Cpu *cpu, Bus *bus) {
   Byte addr = bus->memory[cpu->PC + 1];
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// zero page with X offset (0xB5)
void opLDA_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (bus->memory[cpu->PC + 1] + cpu->X) & 0xFF; // wrap around
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// absolute addressing mode (0xAD)
void opLDA_abs(Cpu *cpu, Bus *bus) {
   Addr addr = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// absolute with X offset (0xBD)
void opLDA_absX(Cpu *cpu, Bus *bus) {
   Addr base = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Addr addr = (base + cpu->X) & 0xFFFF; // wrap around
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// absolute with Y offset (0xB9)
void opLDA_absY(Cpu *cpu, Bus *bus) {
   Addr base = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF; // wrap around
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// indirect with X offset to the pointer(0xA1)
void opLDA_indX(Cpu *cpu, Bus *bus) {
   Byte zp_addr = (bus->memory[cpu->PC + 1] + cpu->X) & 0xFF;
   Addr addr = bus->memory[zp_addr] | (bus->memory[(zp_addr + 1) & 0xFF] << 8);
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// indirect with Y offset to the addressk(0xA1)
void opLDA_indY(Cpu *cpu, Bus *bus) {
   Byte zp_addr = bus->memory[cpu->PC + 1];
   Addr base = bus->memory[zp_addr] | (bus->memory[(zp_addr + 1) & 0xFF] << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF;
   Byte val = bus->memory[addr];
   cpu->A = val;
   setZN(cpu, val);
   cpu->PC += 2;
}


//*******************************************
// LDX
// *******************************************

// immediate addressing mode (0xA2)
void opLDX_imm(Cpu *cpu, Bus *bus) {
   Byte val = bus->memory[cpu->PC + 1];
   cpu->X = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// zero page (0xA6)
void opLDX_zp(Cpu *cpu, Bus *bus) {
   Byte addr = bus->memory[cpu->PC + 1];
   Byte val = bus->memory[addr];
   cpu->X = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// zero page with Y offset (0xB6)
void opLDX_zpY(Cpu *cpu, Bus *bus) {
   Byte addr = (bus->memory[cpu->PC + 1] + cpu->Y) & 0xFF; // wrap around
   Byte val = bus->memory[addr];
   cpu->X = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// absolute addressing mode (0xAE)
void opLDX_abs(Cpu *cpu, Bus *bus) {
   Addr addr = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Byte val = bus->memory[addr];
   cpu->X = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// absolute with Y offset (0xBE)
void opLDX_absY(Cpu *cpu, Bus *bus) {
   Addr base = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF; // wrap around
   Byte val = bus->memory[addr];
   cpu->X = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

//*******************************************
// LDY
// *******************************************

// immediate addressing mode (0xA0)
void opLDY_imm(Cpu *cpu, Bus *bus) {
   Byte val = bus->memory[cpu->PC + 1];
   cpu->Y = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// zero page (0xA4)
void opLDY_zp(Cpu *cpu, Bus *bus) {
   Byte addr = bus->memory[cpu->PC + 1];
   Byte val = bus->memory[addr];
   cpu->Y = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// zero page with X offset (0xB4)
void opLDY_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (bus->memory[cpu->PC + 1] + cpu->X) & 0xFF; // wrap around
   Byte val = bus->memory[addr];
   cpu->Y = val;
   setZN(cpu, val);
   cpu->PC += 2;
}

// absolute addressing mode (0xAC)
void opLDY_abs(Cpu *cpu, Bus *bus) {
   Addr addr = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Byte val = bus->memory[addr];
   cpu->Y = val;
   setZN(cpu, val);
   cpu->PC += 3;
}

// absolute with X offset (0xBC)
void opLDY_absX(Cpu *cpu, Bus *bus) {
   Addr base = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Addr addr = (base + cpu->X) & 0xFFFF; // wrap around
   Byte val = bus->memory[addr];
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
   Byte addr = bus->memory[cpu->PC + 1];
   busWrite(bus, addr, cpu->A);
   cpu->PC += 2;
}

// zero page,X (0x95)
void opSTA_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (bus->memory[cpu->PC + 1] + cpu->X) & 0xFF;
   busWrite(bus, addr, cpu->A);
   cpu->PC += 2;
}

// absolute (0x8D)
void opSTA_abs(Cpu *cpu, Bus *bus) {
   Addr addr = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   busWrite(bus, addr, cpu->A);
   cpu->PC += 3;
}

// absolute,X (0x9D)
void opSTA_absX(Cpu *cpu, Bus *bus) {
   Addr base = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Addr addr = (base + cpu->X) & 0xFFFF;
   busWrite(bus, addr, cpu->A);
   cpu->PC += 3;
}

// absolute,Y (0x99)
void opSTA_absY(Cpu *cpu, Bus *bus) {
   Addr base = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF;
   busWrite(bus, addr, cpu->A);
   cpu->PC += 3;
}

// (indirect,X) (0x81)
void opSTA_indX(Cpu *cpu, Bus *bus) {
   Byte zp_addr = (bus->memory[cpu->PC + 1] + cpu->X) & 0xFF;
   Addr addr = bus->memory[zp_addr] | (bus->memory[(zp_addr + 1) & 0xFF] << 8);
   busWrite(bus, addr, cpu->A);
   cpu->PC += 2;
}

// (indirect),Y (0x91)
void opSTA_indY(Cpu *cpu, Bus *bus) {
   Byte zp_addr = bus->memory[cpu->PC + 1];
   Addr base = bus->memory[zp_addr] | (bus->memory[(zp_addr + 1) & 0xFF] << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF;
   busWrite(bus, addr, cpu->A);
   cpu->PC += 2;
}

//*******************************************
// STX (Store X Register)
//*******************************************

// zero page (0x86)
void opSTX_zp(Cpu *cpu, Bus *bus) {
   Byte addr = bus->memory[cpu->PC + 1];
   busWrite(bus, addr, cpu->X);
   cpu->PC += 2;
}

// zero page,Y (0x96)
void opSTX_zpY(Cpu *cpu, Bus *bus) {
   Byte addr = (bus->memory[cpu->PC + 1] + cpu->Y) & 0xFF;
   busWrite(bus, addr, cpu->X);
   cpu->PC += 2;
}

// absolute (0x8E)
void opSTX_abs(Cpu *cpu, Bus *bus) {
   Addr addr = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   busWrite(bus, addr, cpu->X);
   cpu->PC += 3;
}

//*******************************************
// STY (Store Y Register)
//*******************************************

// zero page (0x84)
void opSTY_zp(Cpu *cpu, Bus *bus) {
   Byte addr = bus->memory[cpu->PC + 1];
   busWrite(bus, addr, cpu->Y);
   cpu->PC += 2;
}

// zero page,X (0x94)
void opSTY_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (bus->memory[cpu->PC + 1] + cpu->X) & 0xFF;
   busWrite(bus, addr, cpu->Y);
   cpu->PC += 2;
}

// absolute (0x8C)
void opSTY_abs(Cpu *cpu, Bus *bus) {
   Addr addr = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   busWrite(bus, addr, cpu->Y);
   cpu->PC += 3;
}
