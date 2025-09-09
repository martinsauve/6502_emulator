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

void opDEC_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC+1);
   uint16_t value = busRead(bus, addr);
   value--;
   busWrite(bus, addr, (value & 0xff));
   setZN(cpu, (value & 0xff));
   cpu->PC += 2;
}
void opDEC_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF; // wrap around
   uint16_t value = busRead(bus, addr);
   value--;
   busWrite(bus, addr, (value & 0xff));
   setZN(cpu, (value & 0xff));
   cpu->PC += 2;
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
   setZN(cpu, (value & 0xff));
   cpu->PC += 2;
}
void opINC_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF; // wrap around
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

void opADC_zp(Cpu *cpu, Bus *bus){
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, addr);
   uint16_t sum = cpu->A + value + cpu->C;
   //set carry flag
   cpu->C = (sum > 0xFF);
   //set overflow flag
   setV(cpu, cpu->A, value, sum & 0xFF);

   cpu->A = sum & 0xFF;

   setZN(cpu, cpu->A);
   cpu->PC += 2;
}

void opADC_zpX(Cpu *cpu, Bus *bus){
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF; // wrap around
   Byte value = busRead(bus, addr);
   uint16_t sum = cpu->A + value + cpu->C;
   //set carry flag
   cpu->C = (sum > 0xFF);
   //set overflow flag
   setV(cpu, cpu->A, value, sum & 0xFF);

   cpu->A = sum & 0xFF;

   setZN(cpu, cpu->A);
   cpu->PC += 2;
}
void opADC_abs(Cpu *cpu, Bus *bus){
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Byte value = busRead(bus, addr);
   uint16_t sum = cpu->A + value + cpu->C;
   //set carry flag
   cpu->C = (sum > 0xFF);
   //set overflow flag
   setV(cpu, cpu->A, value, sum & 0xFF);

   cpu->A = sum & 0xFF;

   setZN(cpu, cpu->A);
   cpu->PC += 3;
}

void opADC_absX(Cpu *cpu, Bus *bus){
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   addr = (addr + cpu->X);
   Byte value = busRead(bus, addr);
   uint16_t sum = cpu->A + value + cpu->C;
   //set carry flag
   cpu->C = (sum > 0xFF);
   //set overflow flag
   setV(cpu, cpu->A, value, sum & 0xFF);

   cpu->A = sum & 0xFF;

   setZN(cpu, cpu->A);
   cpu->PC += 3;
}
void opADC_absY(Cpu *cpu, Bus *bus){
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   addr = (addr + cpu->Y);
   Byte value = busRead(bus, addr);
   uint16_t sum = cpu->A + value + cpu->C;
   //set carry flag
   cpu->C = (sum > 0xFF);
   //set overflow flag
   setV(cpu, cpu->A, value, sum & 0xFF);

   cpu->A = sum & 0xFF;

   setZN(cpu, cpu->A);
   cpu->PC += 3;
}

void opSBC_imm(Cpu *cpu, Bus *bus){
   Byte value = busRead(bus, cpu->PC + 1);
   uint16_t result = cpu->A - value - (cpu->C ? 0:1);
   //set carry flag
   cpu->C = (result < 0x100);
   //set overflow flag
   setV(cpu, cpu->A, value, result & 0xFF);

   cpu->A = result & 0xFF;

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
void opSBC_abs(Cpu *cpu, Bus *bus){
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Byte value = busRead(bus, addr);
   uint16_t result = cpu->A - value - (cpu->C ? 0:1);
   //set carry flag
   cpu->C = (result < 0x100);
   //set overflow flag
   setV(cpu, cpu->A, value, result & 0xFF);

   cpu->A = result & 0xFF;

   setZN(cpu, cpu->A);
   cpu->PC += 3;
}

void opSBC_zpX(Cpu *cpu, Bus *bus){
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF; // wrap around
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

void opSBC_absX(Cpu *cpu, Bus *bus){
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   addr = (addr + cpu->X);
   Byte value = busRead(bus, addr);
   uint16_t result = cpu->A - value - (cpu->C ? 0:1);
   //set carry flag
   cpu->C = (result < 0x100);
   //set overflow flag
   setV(cpu, cpu->A, value, result & 0xFF);

   cpu->A = result & 0xFF;

   setZN(cpu, cpu->A);
   cpu->PC += 3;
}

void opSBC_absY(Cpu *cpu, Bus *bus){
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   addr = (addr + cpu->Y);
   Byte value = busRead(bus, addr);
   uint16_t result = cpu->A - value - (cpu->C ? 0:1);
   //set carry flag
   cpu->C = (result < 0x100);
   //set overflow flag
   setV(cpu, cpu->A, value, result & 0xFF);

   cpu->A = result & 0xFF;

   setZN(cpu, cpu->A);
   cpu->PC += 3;
}

void opSBC_indY(Cpu *cpu, Bus *bus){
   Byte zp_addr = busRead(bus, cpu->PC + 1);
   Addr base = busRead(bus, zp_addr) | (busRead(bus, (zp_addr + 1) & 0xFF) << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF;
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

void opCMP_absX(Cpu *cpu, Bus *bus) {
    Addr base = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
    Addr addr = (base + cpu->X) & 0xFFFF;
    Byte value = busRead(bus, addr);
    uint16_t result = cpu->A - value;
    cpu->C = (cpu->A >= value);
    setZN(cpu, result & 0xFF);
    cpu->PC += 3;
}

void opCMP_indY(Cpu *cpu, Bus *bus) {
   Byte zp_addr = busRead(bus, cpu->PC + 1);
   Addr base = busRead(bus, zp_addr) | (busRead(bus, (zp_addr + 1) & 0xFF) << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF;
   Byte value = busRead(bus, addr);
   uint16_t result = cpu->A - value;
   //set carry flag
   cpu->C = (cpu->A >= value);
   setZN(cpu, result & 0xFF);
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
void opCPX_imm(Cpu *cpu, Bus *bus) {
   Byte value = busRead(bus, cpu->PC + 1);
   uint16_t result = cpu->X - value;
   //set carry flag
   cpu->C = (cpu->X >= value);
   setZN(cpu, result & 0xFF);
   cpu->PC += 2;
}
void opCPX_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, addr);
   uint16_t result = cpu->X - value;
   //set carry flag
   cpu->C = (cpu->X >= value);
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

void opAND_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, addr);
   cpu->A &= value;
   setZN(cpu, cpu->A);
   cpu->PC += 2;
}

void opAND_abs(Cpu *cpu, Bus *bus) {
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Byte value = busRead(bus, addr);
   cpu->A &= value;
   setZN(cpu, cpu->A);
   cpu->PC += 3;
}
void opAND_absX(Cpu *cpu, Bus *bus) {
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   addr = (addr + cpu->X);
   Byte value = busRead(bus, addr);
   cpu->A &= value;
   setZN(cpu, cpu->A);
   cpu->PC += 3;
}

void opBIT_abs(Cpu *cpu, Bus *bus) {
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Byte value = busRead(bus, addr);
   Byte result = cpu->A & value;
   cpu->Z = (result == 0 );
   cpu->N = (value & 0x80) != 0;
   cpu->V = (value & 0x40) != 0;
   cpu->PC += 3;
}

void opBIT_absX(Cpu *cpu, Bus *bus) {
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   addr = (addr + cpu->X);
   Byte value = busRead(bus, addr);
   Byte result = cpu->A & value;
   cpu->Z = (result == 0 );
   cpu->N = (value & 0x80) != 0;
   cpu->V = (value & 0x40) != 0;
   cpu->PC += 3;
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

void opEOR_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, addr);
   cpu->A ^= value;
   setZN(cpu, cpu->A);
   cpu->PC += 2;
}

void opEOR_imm(Cpu *cpu, Bus *bus) {
   Byte value = busRead(bus, cpu->PC + 1);
   cpu->A ^= value;
   setZN(cpu, cpu->A);
   cpu->PC += 2;
}

void opROR_A(Cpu *cpu, Bus *bus) {
    (void)bus;
    bool old_carry = cpu->C;
    cpu->C = (cpu->A & 0x01) != 0;
    cpu->A = (cpu->A >> 1) | (old_carry ? 0x80 : 0);
    setZN(cpu, cpu->A);
    cpu->PC += 1;
}
void opROR_zp(Cpu *cpu, Bus *bus) {
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, addr);
   bool old_carry = cpu->C;
   cpu->C = (value & 0x01) != 0;
   Byte result = (value >> 1) | (old_carry ? 0x80 : 0);
   busWrite(bus, (Addr)addr, result);
   setZN(cpu, result);
   cpu->PC += 2;
}
void opROR_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF; // wrap around
   Byte value = busRead(bus, addr);
   bool old_carry = cpu->C;
   cpu->C = (value & 0x01) != 0;
   Byte result = (value >> 1) | (old_carry ? 0x80 : 0);
   busWrite(bus, (Addr)addr, result);
   setZN(cpu, result);
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
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, addr);
   bool old_carry = cpu->C;
   cpu->C = (value & 0x80) != 0;
   Byte result = (value<<1) | (old_carry ? 1 : 0);
   busWrite(bus, (Addr)addr, result);
   setZN(cpu, result);
   cpu->PC += 2;
}

void opROL_zpX(Cpu *cpu, Bus *bus) {
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF; // wrap around
   Byte value = busRead(bus, addr);
   bool old_carry = cpu->C;
   cpu->C = (value & 0x80) != 0;
   Byte result = (value<<1) | (old_carry ? 1 : 0);
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
void opASL_zp(Cpu *cpu, Bus *bus) {
   Byte zp_addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, zp_addr)<<1;
   cpu->C = ( (cpu->A & 0x80) != 0 );
   cpu->A = value;
   setZN(cpu, cpu->A);
   cpu->PC += 2;
}

void opASL_zpX(Cpu *cpu, Bus *bus) {
   Byte zp_addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF; // wrap around
   Byte value = busRead(bus, zp_addr)<<1;
   cpu->C = ( (cpu->A & 0x80) != 0 );
   cpu->A = value;
   setZN(cpu, cpu->A);
   cpu->PC += 2;
}

void opLSR_zpX(Cpu *cpu, Bus *bus) {
   Byte zp_addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF; // wrap around
   Byte old = busRead(bus, zp_addr);
   cpu->C  = (old & 0x01) != 0;
   Byte value = old >> 1;
   busWrite(bus, zp_addr, value);
   setZN(cpu, value);
   cpu->PC += 2;
}

void opLSR_zp(Cpu *cpu, Bus *bus) {
   Byte zp_addr = busRead(bus, cpu->PC + 1);
   Byte old = busRead(bus, zp_addr);
   cpu->C  = (old & 0x01) != 0;
   Byte value = old >> 1;
   busWrite(bus, zp_addr, value);
   setZN(cpu, value);
   cpu->PC += 2;
}

void opLSR_A(Cpu *cpu, Bus *bus) {
   (void)bus;
   cpu->C  = (cpu->A & 0x01) != 0;
   Byte value = cpu->A >> 1;
   cpu->A = value;
   setZN(cpu, cpu->A);
   cpu->PC += 1;
}

void opORA_indX(Cpu *cpu, Bus *bus){
   Byte zp_addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF;
   Byte value = busRead(bus, zp_addr);
   cpu->A |= value;
   setZN(cpu, cpu->A);
   cpu->PC +=2;
}
void opORA_zp(Cpu *cpu, Bus *bus){
   Byte zp_addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, zp_addr);
   cpu->A |= value;
   setZN(cpu, cpu->A);
   cpu->PC +=2;
}

void opORA_imm(Cpu *cpu, Bus *bus){
   Byte value = busRead(bus, cpu->PC + 1);
   cpu->A |= value;
   setZN(cpu, cpu->A);
   cpu->PC +=2;
}
void opORA_abs(Cpu *cpu, Bus *bus){
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Byte value = busRead(bus, addr);
   cpu->A |= value;
   setZN(cpu, cpu->A);
   cpu->PC +=2;
}

void opTSB_zp(Cpu *cpu, Bus *bus){
   Byte addr = busRead(bus, cpu->PC +1);
   Byte mem = busRead(bus, addr);
   cpu->Z = ((cpu->A & mem) == 0);
   busWrite(bus, addr, (mem|cpu->A));
   cpu->PC += 2;
}

void opTSB_abs(Cpu *cpu, Bus *bus){
   Byte addr = busRead(bus, cpu->PC +1) | (busRead(bus, cpu->PC+2) << 8);
   Byte mem = busRead(bus, addr);
   cpu->Z = ((cpu->A & mem) == 0);
   busWrite(bus, addr, (mem|cpu->A));
   cpu->PC += 3;
}
