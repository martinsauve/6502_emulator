#include <stdint.h>
#include <stdbool.h>
#include "cpu.h"
#include "6502_types.h"
#include "bus.h"
#include "tests/tests_cpu.h"

void runTests() {
   test_flags_ops();
   //test_lda_ops();
   //test_ldx_ops();
   //test_ldy_ops();
   //test_sta_ops();
   //test_stx_ops();
   //test_sty_ops();
}

#define DEBUG



int main(void) {
#ifdef DEBUG
   runTests();
#else
   Cpu *cpu = initCpu();
   Bus *bus = initBus();
   loadRom(bus, "jump.bin", 0x0600);
   Opcodes opcode_table[256];
   initOpcodeTable(opcode_table);
   cpu->PC = 0x0600; // Set the program counter to the start of the loaded ROM
   for (int i = 0; i < 500; i++){
      step(cpu, bus, 1, opcode_table);
      //dumpReg(cpu);
   }

   free(cpu);
   free(bus);
#endif

   return 0;

}
