#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"


int main(void) {

   Mem *mem = initMemory();
   Cpu *cpu = initCpu();

   mem->ROM[0xfffc] = 0x00;
   mem->ROM[0xfffd] = 0x01;

   opJMPdirect(cpu, mem);
   printf("%04x\n", cpu->PC);
   opJMPindirect(cpu, mem);
   cpu->PC = 0x01f0;
   printf("pc %04x\n", cpu->PC);
   printf("mem %02x\n", mem->ROM[cpu->PC]);
   opUnknown(cpu, mem);

free:
   free(mem);
   mem = NULL;
   free(cpu);
   cpu = NULL;
}
