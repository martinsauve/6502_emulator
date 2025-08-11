#include <stdint.h>
#include <stdbool.h>
#include "cpu.h"
#include "6502_types.h"
#include "bus.h"
#include "tests/tests_cpu.h"

int main(void) {

   test_lda_ops();
   //Bus *bus = initBus();
   //Cpu *cpu = initCpu();

   //bus->memory[0xfffc] = 0x00;
   //bus->memory[0xfffd] = 0x01;

   //opJMPdirect(cpu, bus);
   //printf("%04x\n", cpu->PC);
   //opJMPindirect(cpu, bus);
   //cpu->PC = 0x01f0;
   //printf("pc %04x\n", cpu->PC);
   //printf("bus %02x\n", bus->memory[cpu->PC]);
   //opUnknown(cpu, bus);

//free:
   //free(bus);
   //bus = NULL;
   //free(cpu);
   //cpu = NULL;
}
