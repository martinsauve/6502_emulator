#include <stdint.h>
#include "6502_types.h"

#ifndef BUS_H
#define BUS_H

typedef union {
   struct {
   Byte   ZP[0x0100];      // 0x0000 - 0x00FF
   Byte   STACK[0x0100];   // 0x0100 - 0x01FF
   Byte   PAGES[0xFDF9];   // 0x0200 - 0xFFF9
   Byte   NMI[0x2];        // 0xFFFA - 0xFFFB
   Byte   RESET[0x2];      // 0xFFFC - 0xFFFD
   Byte   IRQBRK[0x2];     // 0xFFFE - 0xFFFF
   } map;
   Byte   memory[0x10000];    // 0x0000 - 0xFFFF
} Bus;


int dumpBus(Bus *mem, char *filename);
int loadBus(Bus *bus, const char *filename);
int loadRom(Bus *bus, const char *filename, Addr offset);
void busWrite(Bus *bus, Addr addr, Byte value);

Bus* initBus(void);

#endif // BUS_H
