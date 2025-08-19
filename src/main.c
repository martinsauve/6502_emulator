#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>
#include <stdbool.h>
#include "cpu/cpu.h"
#include "6502_types.h"
#include "bus.h"
#include "tests/tests_cpu.h"
#include "snapshot.h"

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


   char *rom_path = NULL;
   char *snapshot_path = NULL;

   static struct option long_options[] = {
      {"rom",        required_argument, 0, 'r'},
      {"snapshot",   required_argument, 0, 's'},
      {"help",       no_argument,       0, 'h'},
      {0,            0,                 0,  0 }
   };

   int opt;
   int option_index = 0;
   while ((opt = getopt_long( argc, argv, "r:s:h", long_options, &option_index)) != -1) {
      switch (opt) {
         case 'r':
            rom_path = optarg;
            break;
         case 's':
            snapshot_path = optarg;
            break;
         case 'h':
            printf("Usage: %s [--rom <path>] [--snapshot <path>]\n", argv[0]);
            exit(0);
            break;
         default:
            printf("Unknown option. Try --help.\n");
            exit(1);

      }

   }

   if (!rom_path && !snapshot_path) {
      printf("No ROM or snapshot provided! attempting to load default ROM\n");
      rom_path = "roms/wozmon.bin";
   }

//#define DEBUG

#ifdef DEBUG
   runTests();
#else

   Cpu *cpu = initCpu(CPU_65C02);
   Bus *bus = initBus();
   Opcodes opcode_table[256];
   initOpcodeTable(opcode_table, cpu->type);

   if (snapshot_path) {
      if(loadSnapshot(snapshot_path, cpu, bus) != 0) {
         printf("Failed to load snapshot from %s\n", snapshot_path);
         exit(1);
      }
   } else if (rom_path) {
      if (loadRom(bus, rom_path, ROM_START) < 0){
         printf("Failed to load rom from %s\n", rom_path);
         exit(1);
      }
      cpuReset(cpu, bus);
   }


   bool shouldStep = true;
   enableNonBlockingInput();
   while (shouldStep){
      stepBatch(cpu, bus, opcode_table, 10000, 1000000);
      //dumpCpu(cpu);
   }
   restoreInputMode();

   freeCpu(cpu);
   freeBus(bus);
#endif

   return 0;

}
