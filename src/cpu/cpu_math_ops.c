#include "cpu.h"
#include "../bus.h"
#include "../6502_types.h"
#include <stdio.h>


void opDEY(Cpu *cpu, Bus *bus) {
   (void)bus; // Unused
   cpu->Y = (cpu->Y - 1) & 0xff;
   setZN(cpu, cpu->Y);
   cpu->PC += 1;
}
void opDEX(Cpu *cpu, Bus *bus) {
   (void)bus; // Unused
   cpu->X = (cpu->X - 1) & 0xff;
   setZN(cpu, cpu->X);
   cpu->PC += 1;
}

void opINX(Cpu *cpu, Bus *bus) {
   (void)bus; // Unused
   cpu->X = (cpu->X + 1) & 0xff;
   setZN(cpu, cpu->X);
   cpu->PC += 1;
}

void opINY(Cpu *cpu, Bus *bus) {
   (void)bus; // Unused
   cpu->Y = (cpu->Y + 1) & 0xff;
   setZN(cpu, cpu->Y);
   cpu->PC += 1;
}


void opDEC_A(Cpu *cpu, Bus *bus) {
    (void)bus; // Unused
    cpu->A = (cpu->A - 1) & 0xff;
    setZN(cpu, cpu->A);
    cpu->PC += 1;
}

void opINC_A(Cpu *cpu, Bus *bus) {
    (void)bus; // Unused
    cpu->A = (cpu->A + 1) & 0xff;
    setZN(cpu, cpu->A);
    cpu->PC += 1;
}

void opINC_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC+1);
   uint16_t value = busRead(bus, addr);
   value++;
   busWrite(bus, addr, (value & 0xff));
   setZN(cpu, value);
   cpu->PC += 2;
}


void opADC_imm(Cpu *cpu, Bus *bus){
   Byte value = busRead(bus, cpu->PC + 1);
   uint16_t sum = cpu->A + value + cpu->C;
   //set carry flag
   cpu->C = (sum > 0xFF);
   //set overflow flag
   setV(cpu, cpu->A, value, sum & 0xFF);

   cpu->A = sum & 0xFF;

   setZN(cpu, cpu->A);
   cpu->PC += 2;
}

void opSBC_zp(Cpu *cpu, Bus *bus){
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, addr);
   uint16_t result = cpu->A - value - (cpu->C ? 0:1);
   //set carry flag
   cpu->C = (result < 0x100);
   //set overflow flag
   setV(cpu, cpu->A, value, result & 0xFF);

   cpu->A = result & 0xFF;

   setZN(cpu, cpu->A);
   cpu->PC += 2;
}

void opCMP_imm(Cpu *cpu, Bus *bus) {
   Byte value = busRead(bus, cpu->PC + 1);
   uint16_t result = cpu->A - value;
   //set carry flag
   cpu->C = (cpu->A >= value);
   setZN(cpu, result & 0xFF);
   cpu->PC += 2;
}

void opCMP_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, addr);
   uint16_t result = cpu->A - value;
   //set carry flag
   cpu->C = (cpu->A >= value);
   setZN(cpu, result & 0xFF);
   cpu->PC += 2;
}

void opCPY_imm(Cpu *cpu, Bus *bus) {
   Byte value = busRead(bus, cpu->PC + 1);
   uint16_t result = cpu->Y - value;
   //set carry flag
   cpu->C = (cpu->Y >= value);
   setZN(cpu, result & 0xFF);
   cpu->PC += 2;
}
void opCPY_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, addr);
   uint16_t result = cpu->Y - value;
   //set carry flag
   cpu->C = (cpu->Y >= value);
   setZN(cpu, result & 0xFF);
   cpu->PC += 2;
}

void opAND_imm(Cpu *cpu, Bus *bus) {
   Byte value = busRead(bus, cpu->PC + 1);
   cpu->A &= value;
   setZN(cpu, cpu->A);
   cpu->PC += 2;
}

void opBIT_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, addr);
   Byte result = cpu->A & value;
   cpu->Z = (result == 0 );
   cpu->N = (value & 0x80) != 0;
   cpu->V = (value & 0x40) != 0;
   cpu->PC += 2;
}

void opEOR_imm(Cpu *cpu, Bus *bus) {
   Byte value = busRead(bus, cpu->PC + 1);
   cpu->A ^= value;
   setZN(cpu, cpu->A);
   cpu->PC += 2;
}
void opROL_A(Cpu *cpu, Bus *bus) {
    (void)bus;
    bool old_carry = cpu->C;
    cpu->C = (cpu->A & 0x80) != 0;
    cpu->A = (cpu->A << 1) | (old_carry ? 1 : 0);
    setZN(cpu, cpu->A);
    cpu->PC += 1;
}

void opROL_zp(Cpu *cpu, Bus *bus) {
   // fetch a one byte address
   Byte addr = busRead(bus, cpu->PC + 1);
   // read value from zp
   Byte value = busRead(bus, addr);

   // rotate left (carry flag goes in bit 0, old bit 7 goes to carry)
   bool old_carry = cpu->C;
   cpu->C = (value & 0x80) != 0;
   Byte result = (value<<1) | (old_carry ? 1 : 0);

   //write result back to memory
   busWrite(bus, (Addr)addr, result);
   setZN(cpu, result);
   cpu->PC += 2;
}

void opASL_A(Cpu *cpu, Bus *bus) {
   (void)bus;
   Byte value = cpu->A << 1;
   cpu->C = ( (cpu->A & 0x80) != 0 );
   cpu->A = value;
   setZN(cpu, cpu->A);
   cpu->PC += 1;
}

void opLSR_A(Cpu *cpu, Bus *bus) {
   (void)bus;
   cpu->C  = (cpu->A & 0x01) != 0;
   Byte value = cpu->A >> 1;
   cpu->A = value;
   setZN(cpu, cpu->A);
   cpu->PC += 1;
}

void opORA_imm(Cpu *cpu, Bus *bus){
   Byte value = busRead(bus, cpu->PC + 1);
   cpu->A |= value;
   setZN(cpu, cpu->A);
   cpu->PC +=2;
}
