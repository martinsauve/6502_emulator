#include "bus.h"
#include "cpu/cpu_base.h"
#include "snapshot.h"
#include <stdio.h>


Byte aciaReadData(Acia *acia) {
   if (acia->input_ready) {
      acia->input_ready = false;
      char value = acia->input_buffer;
      switch (value) {
         case '\n':  // CR
            return 0x0D;
         case '\r':
            return 0x0D;
         default:
            return value;
      }
   } else {
      fprintf(stderr, "Error: ACIA read attempted with no data available\n");
      return 0; // or some error code
   }

}

Byte aciaReadStatus(Acia *acia) {
   // Bit # (0x08) set = input ready
   return acia->input_ready ? 0x08 : 0x00;
}

void aciaWriteData(Acia *acia, Byte value) {
   switch (value) {
      case 0x0D:
         putchar('\n');
         break;
      default:
         putchar(value);
         break;
   }
   fflush(stdout);
}

void pollAciaInput(Bus *bus, Cpu *cpu) {
   if (!bus->acia.input_ready) {
      int ch = getchar();
      if (ch == 's') {// s, save snapshot
         saveSnapshot(cpu, bus, "ctrlS.snap");
         fflush(stdout);
         return;
      }
      if (ch != EOF) {
         bus->acia.input_buffer = (Byte)ch;
         bus->acia.input_ready = true;
      }
   }
}
