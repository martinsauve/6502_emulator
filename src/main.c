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
   loadBus(bus, "in.bin");
   Opcodes opcode_table[256];
   initOpcodeTable(opcode_table);
   cpu->Z = 0;
   opBEQ(cpu, bus);
   cpu->Z = 1;
   opBEQ(cpu, bus);

   free(cpu);
   free(bus);

   return 0;
}
