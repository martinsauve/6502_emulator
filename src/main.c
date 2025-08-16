#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>
#include <stdbool.h>
#include "cpu/cpu.h"
#include "6502_types.h"
#include "bus.h"
#include "tests/tests_cpu.h"

void runTests() {
   //test_jmp_indirect_wraparound();
   //test_flags_ops();
   //test_lda_ops();
   //test_ldx_ops();
   //test_ldy_ops();
   test_sta_ops();
   //test_stx_ops();
   //test_sty_ops();
}


void enableNonBlockingInput() {
    // Set terminal to raw mode (no buffering, no echo)
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    // Set file descriptor to non-blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}
void restoreInputMode() {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag |= ICANON | ECHO; // Re-enable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}

int main(int argc, char *argv[]) {


   char *rom_path;
   if (argc == 1) { // no args
      rom_path = "roms/wozmon.bin";
      //rom_path = "roms/jump.bin";
   } else {
      rom_path = argv[1];
   }

   // If DEBUG is defined, run tests; otherwise, run the CPU simulation
//#define DEBUG

#ifdef DEBUG
   runTests();
#else

   Cpu *cpu = initCpu(CPU_65C02);
   Bus *bus = initBus();
   loadRom(bus, rom_path, 0xFF00);
   Opcodes opcode_table[256];
   initOpcodeTable(opcode_table, cpu->type);
   cpu->PC = 0xFF00;
   bool shouldStep = true;
   enableNonBlockingInput();
   while (shouldStep){
      step(cpu, bus, 1000, opcode_table);
      //dumpCpu(cpu);
   }
   restoreInputMode();

   free(cpu);
   free(bus);
#endif

   return 0;

}
