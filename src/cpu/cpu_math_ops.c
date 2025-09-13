#include "cpu.h"
#include "../bus.h"
#include "../6502_types.h"
#include <stdint.h>
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

static inline void adc_binary(Cpu *cpu, Byte value) {
   //printf("adc binary");
   uint16_t sum = cpu->A + value + (cpu->C ? 1 : 0);
   cpu->C = (sum > 0xFF);

   setV(cpu, cpu->A, value, sum);
   cpu->A = sum & 0xFF;
   setZN(cpu, cpu->A);
}

static inline void sbc_binary(Cpu *cpu, Byte value) {
   //printf("sbc binary");
   uint16_t diff = cpu->A - value - (cpu->C ? 0 : 1);
   cpu->C = (diff < 0x100);
   setV(cpu, cpu->A, value, diff);
   cpu->A = diff & 0xFF;
   setZN(cpu, cpu->A);
}

static inline void adc_decimal(Cpu *cpu, Byte value) {
   printf("adc decimal \n");
   Byte a = cpu->A;
   Byte c = cpu->C ? 1 : 0;

   Byte lsn = (a & 0x0F) + (value & 0x0F) + c;
   Byte msn = (a >> 4) + (value >> 4);

   if (lsn > 9) {
      lsn += 6;
      msn += 1;
   }
   if (msn > 9) {
      msn += 6;
   }
   Byte result = (msn << 4) | (lsn & 0x0F);

   cpu->C = (msn > 15);
   cpu->A = result;
   setZN(cpu, result);
}

static inline void sbc_decimal(Cpu *cpu, Byte value) {
   printf("sbc decimal\n ");
   Byte a = cpu->A;
   Byte c = cpu->C ? 1 : 0;

   int8_t lsn = (a & 0x0F) - (value & 0x0F) - (1 - c);
   int8_t msn = (a >> 4) - (value >> 4);

   if (lsn < 0) {
      lsn -= 6;
      msn -= 1;
   }
   if (msn < 0) {
      msn -= 6;
   }

   Byte result = ((msn << 4) & 0xF0) | (lsn & 0x0F);

   cpu->C = ((int)a - (int)value - (1 - c)) >= 0;
   cpu->A = result;
   setZN(cpu, result);
}

void opADC_imm(Cpu *cpu, Bus *bus){
   Byte value = busRead(bus, cpu->PC + 1);
   if (cpu->D) {
      adc_decimal(cpu, value);
   } else {
      adc_binary(cpu, value);
   }
   cpu->PC += 2;
}

void opADC_zp(Cpu *cpu, Bus *bus){
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, addr);
   if (cpu->D) {
      adc_decimal(cpu, value);
   } else {
      adc_binary(cpu, value);
   }
   cpu->PC += 2;
}

void opADC_zpX(Cpu *cpu, Bus *bus){
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF; // wrap around
   Byte value = busRead(bus, addr);
   if (cpu->D) {
      adc_decimal(cpu, value);
   } else {
      adc_binary(cpu, value);
   }
   cpu->PC += 2;
}
void opADC_abs(Cpu *cpu, Bus *bus){
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Byte value = busRead(bus, addr);
   if (cpu->D) {
      adc_decimal(cpu, value);
   } else {
      adc_binary(cpu, value);
   }
   cpu->PC += 3;
}

void opADC_absX(Cpu *cpu, Bus *bus){
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   addr = (addr + cpu->X);
   Byte value = busRead(bus, addr);
   if (cpu->D) {
      adc_decimal(cpu, value);
   } else {
      adc_binary(cpu, value);
   }
   cpu->PC += 3;
}
void opADC_absY(Cpu *cpu, Bus *bus){
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   addr = (addr + cpu->Y);
   Byte value = busRead(bus, addr);
   if (cpu->D) {
      adc_decimal(cpu, value);
   } else {
      adc_binary(cpu, value);
   }
   cpu->PC += 3;
}

void opSBC_imm(Cpu *cpu, Bus *bus){
   Byte value = busRead(bus, cpu->PC + 1);
   if (cpu->D) {
      sbc_decimal(cpu, value);
   } else {
      sbc_binary(cpu, value);
   }
   cpu->PC += 2;
}

void opSBC_zp(Cpu *cpu, Bus *bus){
   Byte addr = busRead(bus, cpu->PC + 1);
   Byte value = busRead(bus, addr);
   if (cpu->D) {
      sbc_decimal(cpu, value);
   } else {
      sbc_binary(cpu, value);
   }
   cpu->PC += 2;
}
void opSBC_abs(Cpu *cpu, Bus *bus){
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   Byte value = busRead(bus, addr);
   if (cpu->D) {
      sbc_decimal(cpu, value);
   } else {
      sbc_binary(cpu, value);
   }
   cpu->PC += 3;
}

void opSBC_zpX(Cpu *cpu, Bus *bus){
   Byte addr = (busRead(bus, cpu->PC + 1) + cpu->X) & 0xFF; // wrap around
   Byte value = busRead(bus, addr);
   if (cpu->D) {
      sbc_decimal(cpu, value);
   } else {
      sbc_binary(cpu, value);
   }
   cpu->PC += 2;
}

void opSBC_absX(Cpu *cpu, Bus *bus){
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   addr = (addr + cpu->X);
   Byte value = busRead(bus, addr);
   if (cpu->D) {
      sbc_decimal(cpu, value);
   } else {
      sbc_binary(cpu, value);
   }
   cpu->PC += 3;
}

void opSBC_absY(Cpu *cpu, Bus *bus){
   Addr addr = busRead(bus, cpu->PC + 1) | (busRead(bus, cpu->PC + 2) << 8);
   addr = (addr + cpu->Y);
   Byte value = busRead(bus, addr);
   if (cpu->D) {
      sbc_decimal(cpu, value);
   } else {
      sbc_binary(cpu, value);
   }
   cpu->PC += 3;
}

void opSBC_indY(Cpu *cpu, Bus *bus){
   Byte zp_addr = busRead(bus, cpu->PC + 1);
   Addr base = busRead(bus, zp_addr) | (busRead(bus, (zp_addr + 1) & 0xFF) << 8);
   Addr addr = (base + cpu->Y) & 0xFFFF;
   Byte value = busRead(bus, addr);
   if (cpu->D) {
      sbc_decimal(cpu, value);
   } else {
      sbc_binary(cpu, value);
   }
   cpu->PC += 2;
}

// NEW
void opCMP(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc) {
   AddrModeResult res = addrModeFunc(cpu, bus);

   uint16_t result = cpu->A - res.value;
   cpu->C = (cpu->A >= res.value);
   setZN(cpu, result & 0xFF);
}

void opCPX(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc) {
   AddrModeResult res = addrModeFunc(cpu, bus);

   uint16_t result = cpu->X - res.value;
   cpu->C = (cpu->X >= res.value);
   setZN(cpu, result & 0xFF);
}

void opCPY(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc) {
   AddrModeResult res = addrModeFunc(cpu, bus);

   uint16_t result = cpu->Y - res.value;
   cpu->C = (cpu->Y >= res.value);
   setZN(cpu, result & 0xFF);
}

void opAND(Cpu *cpu, Bus *bus, AddrModeFunc addrModeFunc) {
   AddrModeResult res = addrModeFunc(cpu, bus);

   cpu->A &= res.value;
   setZN(cpu, cpu->A);
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
