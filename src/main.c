#include <stdint.h>
#include <stdbool.h>
#include "cpu.h"
#include "6502_types.h"
#include "memory.h"
#include "tests/tests_cpu.h"

int main(void) {

   test_lda_ops();
   //Mem *mem = initMemory();
   //Cpu *cpu = initCpu();

   //mem->ROM[0xfffc] = 0x00;
   //mem->ROM[0xfffd] = 0x01;

   //opJMPdirect(cpu, mem);
   //printf("%04x\n", cpu->PC);
   //opJMPindirect(cpu, mem);
   //cpu->PC = 0x01f0;
   //printf("pc %04x\n", cpu->PC);
   //printf("mem %02x\n", mem->ROM[cpu->PC]);
   //opUnknown(cpu, mem);

//free:
   //free(mem);
   //mem = NULL;
   //free(cpu);
   //cpu = NULL;
}
