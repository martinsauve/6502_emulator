#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bus.h"


int dumpBus(Bus *mem, char *filename) {
   FILE *fp;
   fp = fopen(filename, "wb");

   if (fp==NULL) {
      perror("Error opening file");
      return 1;
   }

   fwrite(mem->memory, sizeof(mem->memory), 1, fp);
   fclose(fp);
   printf("BUS dumped to %s\n", filename);
   return 0;
}

Bus* initBus(void) {
   Bus *bus = NULL;
   bus = malloc(sizeof *bus);
   memset(bus->memory, 0, sizeof(bus->memory));            //NOLINT
   memset(bus->MAP.ZP, 'Z', sizeof(bus->MAP.ZP));         //NOLINT
   memset(bus->MAP.STACK, 'S', sizeof(bus->MAP.STACK));   //NOLINT
   memset(bus->MAP.IRQBRK, 'B', sizeof(bus->MAP.IRQBRK)); //NOLINT
   return bus;
}
