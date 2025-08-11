#include <stdint.h>
#include "6502_types.h"
#ifndef MEMORY_H
#define MEMORY_H


typedef union {
   struct {
   Byte   ZP[0x0100];      // 0x0000 - 0x00FF
   Byte   STACK[0x0100];   // 0x0100 - 0x01FF
   Byte   PAGES[0xFDF9];   // 0x0200 - 0xFFF9
   Byte   NMI[0x2];        // 0xFFFA - 0xFFFB
   Byte   RESET[0x2];      // 0xFFFC - 0xFFFD
   Byte   IRQBRK[0x2];     // 0xFFFE - 0xFFFF
   } MAP;
   Byte   ROM[0x10000];    // 0x0000 - 0xFFFF
} Mem;


int dumpRom(Mem *mem, char *filename);
int dumpRom(Mem *mem, char *filename);

Mem* initMemory(void);

#endif // MEMORY_H
