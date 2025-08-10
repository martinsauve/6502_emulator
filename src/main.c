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

   opJMPdirect(cpu, 0xfffc);
   printf("%04x\n", cpu->PC);
   opJMPindirect(cpu, mem);
   printf("%04x\n", cpu->PC);

free:
   free(mem);
   mem = NULL;
   free(cpu);
   cpu = NULL;
}
