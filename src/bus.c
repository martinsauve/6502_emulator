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

int loadRom(Bus *bus, const char *filename, Addr offset) {
   size_t max_size = sizeof(bus->memory) - offset;
   FILE *f = fopen(filename, "rb");
   if (!f) {
      perror("Failed to open file");
      return -1;
   }
   // Read up to mem_size bytes
   size_t bytes_read = fread(bus->memory + offset, 1, max_size, f);
   int extra = fgetc(f);
   fclose(f);
   if (extra != EOF) {
      fprintf( //NOLINT
            stderr,
            "Error: Input file '%s' is larger than %zu bytes, cannot load at offset 0x%04x (64K bus overflow)\n",
            filename, max_size, offset);
      return -1;
   }
   return (int)bytes_read;


}

int loadBus(Bus *bus, const char *filename) {
   size_t mem_size = sizeof(bus->memory);
   FILE *f = fopen(filename, "rb");
   if (!f) {
      perror("Failed to open file");
      return -1;
   }
   // Read up to mem_size bytes
   size_t bytes_read = fread(bus->memory, 1, mem_size, f);

   // Check for overflow: try reading one more byte
   int extra = fgetc(f);
   fclose(f);

   if (extra != EOF) {
      fprintf( //NOLINT
            stderr,
            "Error: Input file '%s' is larger than %zu bytes (64K bus overflow)\n",
            filename, mem_size);
      return -1;
   }
   if (bytes_read != mem_size) {
      fprintf( //NOLINT
            stderr,
            "Warning: Input file '%s' is smaller than %zu bytes, loaded %zu bytes\n",
            filename, mem_size, bytes_read);
   }
   return (int)bytes_read;
}

void busWrite(Bus *bus, Addr addr, Byte value) {
   bus->memory[addr] = value;
   if (addr == 0xF001) { // trap for printing
      putchar(value);
      fflush(stdout);
   }
}


Bus* initBus(void) {
   Bus *bus = NULL;
   bus = malloc(sizeof *bus);
   memset(bus->memory, 0, sizeof(bus->memory));           //NOLINT
   //memset(bus->MAP.ZP, 'Z', sizeof(bus->MAP.ZP));         //NOLINT
   //memset(bus->MAP.STACK, 'S', sizeof(bus->MAP.STACK));   //NOLINT
   //memset(bus->MAP.IRQBRK, 'B', sizeof(bus->MAP.IRQBRK)); //NOLINT
   return bus;
}
