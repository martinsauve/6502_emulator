#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "memory.h"


int dumpRom(Mem *mem, char *filename) {
   FILE *fp;
   fp = fopen(filename, "wb");

   if (fp==NULL) {
      perror("Error opening file");
      return 1;
   }

   fwrite(mem->ROM, sizeof(mem->ROM), 1, fp);
   fclose(fp);
   return 0;
}

Mem* initMemory(void) {
   Mem *M = NULL;
   M = malloc(sizeof *M);
   memset(M->ROM, 0, sizeof(M->ROM));                 //NOLINT
   memset(M->MAP.ZP, 'Z', sizeof(M->MAP.ZP));         //NOLINT
   memset(M->MAP.STACK, 'S', sizeof(M->MAP.STACK));   //NOLINT
   memset(M->MAP.IRQBRK, 'B', sizeof(M->MAP.IRQBRK)); //NOLINT
   return M;
}
