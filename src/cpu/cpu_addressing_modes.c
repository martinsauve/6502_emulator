#include "cpu.h"
#include "cpu_addressing_modes.h"
#include "../bus.h"

AddrModeResult addressingNone(Cpu *cpu, Bus *bus)
{
   (void)cpu;
   (void)bus;

   return (AddrModeResult){
         .has_value = false,
         .has_addr = false
   };
}

AddrModeResult addressingImmediate(Cpu *cpu, Bus *bus)
{
   Byte value = busRead(bus, cpu->PC);
   cpu->PC ++;

   return (AddrModeResult){
         .value = value,
         .has_value = true,
         .has_addr = false
   };
}

AddrModeResult addressingZeroPage(Cpu *cpu, Bus *bus)
{
   Addr addr = busRead(bus, cpu->PC);
   cpu->PC ++;

   Byte value = busRead(bus, addr);

   return (AddrModeResult) {
      .value = value,
      .has_value = true,
      .addr = addr,
      .has_addr = true,
   };
}


AddrModeResult addressingZeroPageX(Cpu *cpu, Bus *bus)
{
   Addr addr = (busRead(bus, cpu->PC) + cpu->X) & 0xFF;
   cpu->PC ++;

   Byte value = busRead(bus, addr);

   return (AddrModeResult) {
      .value = value,
      .has_value = true,
      .addr = addr,
      .has_addr = true,
   };
}


AddrModeResult addressingZeroPageY(Cpu *cpu, Bus *bus)
{
   Addr addr = (busRead(bus, cpu->PC) + cpu->Y) & 0xFF;
   cpu->PC ++;

   Byte value = busRead(bus, addr);

   return (AddrModeResult) {
      .value = value,
      .has_value = true,
      .addr = addr,
      .has_addr = true,
   };
}

AddrModeResult addressingAbsolute(Cpu *cpu, Bus *bus)
{
   Addr addr = busRead(bus, cpu->PC);            // LSB
   cpu->PC ++;
   addr |= ((Addr)busRead(bus, cpu->PC)) << 8;   // MSB
   cpu->PC ++;

   Byte value = busRead(bus, addr);

   return (AddrModeResult) {
      .value = value,
      .has_value = true,
      .addr = addr,
      .has_addr = true
   };
}


AddrModeResult addressingAbsoluteX(Cpu *cpu, Bus *bus)
{
   Addr addr = busRead(bus, cpu->PC);            // LSB
   cpu->PC ++;
   addr |= ((Addr)busRead(bus, cpu->PC)) << 8;   // MSB
   cpu->PC ++;
   addr = (addr + cpu->X) & 0xFFFF;

   Byte value = busRead(bus, addr);

   return (AddrModeResult) {
      .value = value,
      .has_value = true,
      .addr = addr,
      .has_addr = true
   };
}


AddrModeResult addressingAbsoluteY(Cpu *cpu, Bus *bus)
{
   Addr addr = busRead(bus, cpu->PC);            // LSB
   cpu->PC ++;
   addr |= ((Addr)busRead(bus, cpu->PC)) << 8;   // MSB
   cpu->PC ++;
   addr = (addr + cpu->Y) & 0xFFFF;

   Byte value = busRead(bus, addr);

   return (AddrModeResult) {
      .value = value,
      .has_value = true,
      .addr = addr,
      .has_addr = true
   };
}


// indirect with X offset to the pointer
AddrModeResult addressingIndirectX(Cpu *cpu, Bus *bus)
{
   Byte zp_addr = (busRead(bus, cpu->PC) + cpu->X) & 0xFF;
   cpu->PC ++;
   Addr addr = busRead(bus, zp_addr) | (busRead(bus, (zp_addr + 1) & 0xFF) << 8);

   Byte value = busRead(bus, addr);

   return (AddrModeResult) {
      .value = value,
      .has_value = true,
      .addr = addr,
      .has_addr = true
   };
}

// indirect with Y offset to the address
AddrModeResult addressingIndirectY(Cpu *cpu, Bus *bus)
{
   Byte zp_addr = busRead(bus, cpu->PC);
   cpu->PC ++;
   Addr addr = busRead(bus, zp_addr) | (busRead(bus, (zp_addr + 1) & 0xFF) << 8);
   addr = (addr + cpu->Y) & 0xFFFF;

   Byte value = busRead(bus, addr);

   return (AddrModeResult) {
      .value = value,
      .has_value = true,
      .addr = addr,
      .has_addr = true
   };
}

AddrModeResult addressingAccumulator(Cpu *cpu, Bus *bus)
{
   (void)bus;

   return (AddrModeResult) {
         .value = cpu->A,
         .has_value = true,
         .has_addr = false
   };
}
