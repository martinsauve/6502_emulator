#ifndef MEMORY_H
#define MEMORY_H

typedef union {
   struct {
   char   ZP[0x0100];      // 0x0000 - 0x00FF
   char   STACK[0x0100];   // 0x0100 - 0x01FF
   char   PAGES[0xFDF9];   // 0x0200 - 0xFFF9
   char   NMI[0x2];        // 0xFFFA - 0xFFFB
   char   RESET[0x2];      // 0xFFFC - 0xFFFD
   char   IRQBRK[0x2];     // 0xFFFE - 0xFFFF
   } MAP;
   char   ROM[0x10000];    // 0x0000 - 0xFFFF
} Mem;


int dumpRom(Mem *mem, char *filename);
int dumpRom(Mem *mem, char *filename);

Mem* initMemory(void);

#endif // MEMORY_H
