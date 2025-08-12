#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bus.h"


int dumpBus(Bus *bus, char *filename) {
   FILE *fp;
   fp = fopen(filename, "wb");

   if (fp==NULL) {
      perror("Error opening file");
      return 1;
   }

   fwrite(bus->memory, sizeof(bus->memory), 1, fp);
   fclose(fp);
   printf("BUS dumped to %s\n", filename);
   return 0;
}


int loadBus(Bus *bus, const char *filename) {
   size_t mem_size = sizeof(bus->memory);
   FILE *f = fopen(filename, "rb");
   if (!f) {
      perror("Failed to open file");
      return 0;
   }
   // Read up to mem_size bytes
   size_t bytes_read = fread(bus->memory, 1, mem_size, f);

   // Check for overflow: try reading one more byte
   int extra = fgetc(f);
   fclose(f);

   if (extra != EOF) {
      fprintf(stderr, "Error: Input file '%s' is larger than %zu bytes (64K bus overflow)\n", filename, mem_size);
      return 0;
   }
   if (bytes_read != mem_size) {
      fprintf(stderr, "Warning: Input file '%s' is smaller than %zu bytes, loaded %zu bytes\n", filename, mem_size, bytes_read);
   }
   return (int)bytes_read;
}


Bus* initBus(void) {
   Bus *bus = NULL;
   bus = malloc(sizeof *bus);
   //memset(bus->memory, 0, sizeof(bus->memory));           //NOLINT
   //memset(bus->MAP.ZP, 'Z', sizeof(bus->MAP.ZP));         //NOLINT
   //memset(bus->MAP.STACK, 'S', sizeof(bus->MAP.STACK));   //NOLINT
   //memset(bus->MAP.IRQBRK, 'B', sizeof(bus->MAP.IRQBRK)); //NOLINT
   return bus;
}
