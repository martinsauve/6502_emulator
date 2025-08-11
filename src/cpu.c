#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "bus.h"
#include "6502_types.h"


Cpu* initCpu() {
   Cpu *cpu = NULL;
   cpu = malloc(sizeof *cpu);

   cpu->PC = 0;
   cpu->SP = 0;
   cpu->A  = 0;
   cpu->X  = 0;
   cpu->Y  = 0;

   cpu->C  = false;
   cpu->Z  = false;
   cpu->I  = false;
   cpu->D  = false;
   cpu->B  = false;
   cpu->V  = false;
   cpu->N  = false;
   return cpu;

}

void opUnknown(Cpu *cpu, Bus *bus){
   printf( "Unrecognized opcode 0x%02x at 0x%04x\n", bus->memory[cpu->PC], cpu->PC );
   dumpBus(bus, "dump.bin");
   printf("Rom dumped!\n");

}

// untility : set Z and N flags after loading value
static void setZN(Cpu *cpu, Byte val) {
   cpu->Z = (val == 0);
   cpu->N = ( (val & 0x80) != 0 );
}


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

//*******************************************
// STA
// *******************************************


// zero page (0x85)
void opSTA_zp(Cpu *cpu, Bus *bus) {
   Byte addr = bus->memory[cpu->PC + 1];
   bus->memory[addr] = cpu->A;
   cpu->PC += 2;
}

// zero page,X (0x95)
void opSTA_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (bus->memory[cpu->PC + 1] + cpu->X) & 0xFF;
   bus->memory[addr] = cpu->A;
   cpu->PC += 2;
}

// absolute (0x8D)
void opSTA_abs(Cpu *cpu, Bus *bus) {
   Addr addr = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   bus->memory[addr] = cpu->A;
   cpu->PC += 3;
}

// absolute,X (0x9D)
void opSTA_absX(Cpu *cpu, Bus *bus) {
   Addr base = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Addr addr = (base + cpu->X) & 0xFFFF;
   bus->memory[addr] = cpu->A;
   cpu->PC += 3;
}

// absolute,Y (0x99)
void opSTA_absY(Cpu *cpu, Bus *bus) {
   Addr base = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF;
   bus->memory[addr] = cpu->A;
   cpu->PC += 3;
}

// (indirect,X) (0x81)
void opSTA_indX(Cpu *cpu, Bus *bus) {
   Byte zp_addr = (bus->memory[cpu->PC + 1] + cpu->X) & 0xFF;
   Addr addr = bus->memory[zp_addr] | (bus->memory[(zp_addr + 1) & 0xFF] << 8);
   bus->memory[addr] = cpu->A;
   cpu->PC += 2;
}

// (indirect),Y (0x91)
void opSTA_indY(Cpu *cpu, Bus *bus) {
   Byte zp_addr = bus->memory[cpu->PC + 1];
   Addr base = bus->memory[zp_addr] | (bus->memory[(zp_addr + 1) & 0xFF] << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF;
   bus->memory[addr] = cpu->A;
   cpu->PC += 2;
}

//*******************************************
// STX (Store X Register)
//*******************************************

// zero page (0x86)
void opSTX_zp(Cpu *cpu, Bus *bus) {
   Byte addr = bus->memory[cpu->PC + 1];
   bus->memory[addr] = cpu->X;
   cpu->PC += 2;
}

// zero page,Y (0x96)
void opSTX_zpY(Cpu *cpu, Bus *bus) {
   Byte addr = (bus->memory[cpu->PC + 1] + cpu->Y) & 0xFF;
   bus->memory[addr] = cpu->X;
   cpu->PC += 2;
}

// absolute (0x8E)
void opSTX_abs(Cpu *cpu, Bus *bus) {
   Addr addr = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   bus->memory[addr] = cpu->X;
   cpu->PC += 3;
}

//*******************************************
// STY (Store Y Register)
//*******************************************

// zero page (0x84)
void opSTY_zp(Cpu *cpu, Bus *bus) {
   Byte addr = bus->memory[cpu->PC + 1];
   bus->memory[addr] = cpu->Y;
   cpu->PC += 2;
}

// zero page,X (0x94)
void opSTY_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (bus->memory[cpu->PC + 1] + cpu->X) & 0xFF;
   bus->memory[addr] = cpu->Y;
   cpu->PC += 2;
}

// absolute (0x8C)
void opSTY_abs(Cpu *cpu, Bus *bus) {
   Addr addr = bus->memory[cpu->PC + 1] | (bus->memory[cpu->PC + 2] << 8);
   bus->memory[addr] = cpu->Y;
   cpu->PC += 3;
}



// Store value in Y, set negative and zero flag
void opLDY(Cpu *cpu, Byte val) {
}

// Store value in A, don't touch the flags
void opSTA(Cpu *cpu, Byte val) {
}

// Store value in X, don't touch the flags
void opSTX(Cpu *cpu, Byte val) {
}

// Store value in Y, don't touch the flags
void opSTY(Cpu *cpu, Byte val) {
}


//*******************************************
// REGISTER TRANSFERS
//*******************************************


void opTAX(Cpu *cpu) { // Transfer accumulator to X
}


void opTAY(Cpu *cpu) { // Transfer accumulator to Y
}
void opTXA(Cpu *cpu) { // Transfer X to accumulator
}

void opTYA(Cpu *cpu) { // Transfer Y to accumulator
}


//*******************************************
// STACK OPERATIONS
//*******************************************

void opTSX(Cpu *cpu, Bus *bus) {
   opUnknown(cpu, bus);
}

void opTXS(Cpu *cpu, Bus *bus) {
   opUnknown(cpu, bus);
}

void opPHA(Cpu *cpu, Bus *bus) {
   opUnknown(cpu, bus);
}

void opPHP(Cpu *cpu, Bus *bus) {
   opUnknown(cpu, bus);
}

void opPLA(Cpu *cpu, Bus *bus) {
   opUnknown(cpu, bus);
}

void opPLP(Cpu *cpu, Bus *bus) {
   opUnknown(cpu, bus);
}

Addr readAddr(Cpu *cpu, Bus *bus) {
   return (Byte)bus->memory[cpu->PC] | ((Byte)bus->memory[cpu->PC + 1] << 8);
}

void opNOP(Cpu *cpu, Bus *bus) {
   cpu->PC += 1;
}


void dumpReg(Cpu *cpu) {
   printf(" A | X | C | Z | N\n");
   printf(" %i | %i | %i | %i | %i\n", cpu->A,cpu->X, cpu->C, cpu->Z, cpu->N);
}

typedef struct {
   void (*handler)(Cpu *cpu, Bus *bus);
   unsigned cycles;
} Opcodes;

Opcodes opcode_table[256];

void initOpcodeTable() {
   for (int i = 0; i < 256; i++) {
      opcode_table[i].handler = opUnknown;
      opcode_table[i].cycles = 2;
   }

   opcode_table[0xEA].handler = opNOP;       opcode_table[0xEA].cycles = 2;

   // Load Accumulator
   opcode_table[0xA9].handler = opLDA_imm;   opcode_table[0xA9].cycles = 2;
   opcode_table[0xA5].handler = opLDA_zp;    opcode_table[0xA5].cycles = 3;
   opcode_table[0xB5].handler = opLDA_zpX;   opcode_table[0xB5].cycles = 4;
   opcode_table[0xAD].handler = opLDA_abs;   opcode_table[0xAD].cycles = 4;
   opcode_table[0xBD].handler = opLDA_absX;  opcode_table[0xBD].cycles = 4;
   // TODO: or 5 if page boundary crossed
   opcode_table[0xB9].handler = opLDA_absY;  opcode_table[0xB9].cycles = 4;
   // TODO: or 5 if page boundary crossed
   opcode_table[0xA1].handler = opLDA_indX;  opcode_table[0xA1].cycles = 6;
   opcode_table[0xB1].handler = opLDA_indY;  opcode_table[0xB1].cycles = 5;
   // TODO: or 6 if page boundary crossed }
   // LDX
   opcode_table[0xA2].handler = opLDX_imm;  opcode_table[0xA2].cycles = 2;
   opcode_table[0xA6].handler = opLDX_zp;   opcode_table[0xA6].cycles = 3;
   opcode_table[0xB6].handler = opLDX_zpY;  opcode_table[0xB6].cycles = 4;
   opcode_table[0xAE].handler = opLDX_abs;  opcode_table[0xAE].cycles = 4;
   opcode_table[0xBE].handler = opLDX_absY; opcode_table[0xBE].cycles = 4;
   // +1 if page crossed

   // LDY
   opcode_table[0xA0].handler = opLDY_imm;  opcode_table[0xA0].cycles = 2;
   opcode_table[0xA4].handler = opLDY_zp;   opcode_table[0xA4].cycles = 3;
   opcode_table[0xB4].handler = opLDY_zpX;  opcode_table[0xB4].cycles = 4;
   opcode_table[0xAC].handler = opLDY_abs;  opcode_table[0xAC].cycles = 4;
   opcode_table[0xBC].handler = opLDY_absX; opcode_table[0xBC].cycles = 4;
   // +1 if page crossed

   // STA
   opcode_table[0x85].handler = opSTA_zp;   opcode_table[0x85].cycles = 3;
   opcode_table[0x95].handler = opSTA_zpX;  opcode_table[0x95].cycles = 4;
   opcode_table[0x8D].handler = opSTA_abs;  opcode_table[0x8D].cycles = 4;
   opcode_table[0x9D].handler = opSTA_absX; opcode_table[0x9D].cycles = 5;
   opcode_table[0x99].handler = opSTA_absY; opcode_table[0x99].cycles = 5;
   opcode_table[0x81].handler = opSTA_indX; opcode_table[0x81].cycles = 6;
   opcode_table[0x91].handler = opSTA_indY; opcode_table[0x91].cycles = 6;

   // STX
   opcode_table[0x86].handler = opSTX_zp;   opcode_table[0x86].cycles = 3;
   opcode_table[0x96].handler = opSTX_zpY;  opcode_table[0x96].cycles = 4;
   opcode_table[0x8E].handler = opSTX_abs;  opcode_table[0x8E].cycles = 4;

   // STY
   opcode_table[0x84].handler = opSTY_zp;   opcode_table[0x84].cycles = 3;
   opcode_table[0x94].handler = opSTY_zpX;  opcode_table[0x94].cycles = 4;
   opcode_table[0x8C].handler = opSTY_abs;  opcode_table[0x8C].cycles = 4;
}
