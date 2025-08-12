#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "cpu.h"
#include "6502_types.h"
#include "bus.h"
#include "tests/tests_cpu.h"

void runTests() {
   test_lda_ops();
   test_ldx_ops();
   test_ldy_ops();
   test_sta_ops();
   test_stx_ops();
   test_sty_ops();
}

//#define DEBUG



int main(void) {
#ifdef DEBUG
   runTests();
#endif
   Cpu *cpu = initCpu();
   Bus *bus = initBus();
   Opcodes opcode_table[256];
   initOpcodeTable(opcode_table);
   for(int i=1; i<256;i++) {
      //printf("%lu\n", (uint64_t)opcode_table[i].handler);
   }
   bus->memory[0x00] = 0xA9;
   bus->memory[0x01] = 69;
   bus->memory[0x02] = 0xEA;
   bus->memory[0x03] = 0xA9;
   bus->memory[0x04] = 32;

   step(cpu, bus, 0.01, opcode_table);
   dumpReg(cpu);
   step(cpu, bus, 0.01, opcode_table);
   dumpReg(cpu);
   step(cpu, bus, 0.01, opcode_table);
   dumpReg(cpu);
   step(cpu, bus, 0.01, opcode_table);
   dumpReg(cpu);

   free(cpu);
   free(bus);

   return 0;
}
