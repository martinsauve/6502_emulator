#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bus.h"


int dumpRom(Bus *bus, char *filename) {
   FILE *fp;
   fp = fopen(filename, "wb");

   if (fp==NULL) {
      perror("Error opening file");
      return 1;
   }

   fwrite(bus->rom, sizeof(bus->rom), 1, fp);
   fclose(fp);
   printf("Rom dumped to %s\n", filename);
   return 0;
}

int dumpRam(Bus *bus, char *filename) {
   FILE *fp;
   fp = fopen(filename, "wb");

   if (fp==NULL) {
      perror("Error opening file");
      return 1;
   }

   fwrite(bus->ram, sizeof(bus->ram), 1, fp);
   fclose(fp);
   printf("Ram dumped to %s\n", filename);
   return 0;
}

int loadRom(Bus *bus, const char *filename, Addr offset) {
   printf("Loading ROM from '%s' at offset 0x%04x\n", filename, offset);
   if ( offset < ROM_START ){
      fprintf(
            stderr,
            "offset 0x%04x is incorrect, ROM starts at 0x%04x",
            offset, ROM_START);
      return -1;
   }

   int offset_in_rom = offset - ROM_START;

   size_t max_size = ROM_SIZE - offset_in_rom;
   FILE *f = fopen(filename, "rb");
   if (!f) {
      perror("Failed to open file");
      return -1;
   }
   // Read up to mem_size bytes
   size_t bytes_read = fread(bus->rom + offset_in_rom, 1, max_size, f);
   int extra = fgetc(f);
   fclose(f);
   if (extra != EOF) {
      fprintf(
            stderr,
            "Error: Input file '%s' is larger than %zu bytes, cannot load at offset 0x%04x (64K bus overflow)\n",
            filename, max_size, offset);
      return -1;
   }
   return (int)bytes_read;


}

void busWrite(Bus *bus, Addr addr, Byte value) {
   #pragma GCC diagnostic push
   #pragma GCC diagnostic ignored "-Wtype-limits" // yes GCC, RAM_START is 0 in this case, but could be set to a different value
   if (addr >= RAM_START && addr <=RAM_END){
      bus->ram[addr - RAM_START] = value;
   } else if (addr >= ROM_START && addr <= ROM_END){
      fprintf(stderr, "Error: trying to write to ROM (addr 0x%04x is readonly)\n", addr);
   } else if (addr == ACIA_DATA) { // trap for printing
      aciaWriteData(bus->acia, value);
   } else {
      fprintf(stderr, "Error: (write) addr 0x%04x currently unmapped\n", addr);
   }
   #pragma GCC diagnostic pop
}

Byte busRead(Bus *bus, Addr addr) {
   #pragma GCC diagnostic push
   #pragma GCC diagnostic ignored "-Wtype-limits" // yes GCC, RAM_START is 0 in this case, but could be set to a different value
   if (addr >= RAM_START && addr <=RAM_END)
   {
      return bus->ram[addr - RAM_START];
   }
   else if (addr >= ROM_START && addr <= ROM_END ) { // idem for ROM_END, could be set to a lower value
      return bus->rom[addr - ROM_START];
   } else if (addr == ACIA_DATA) {
      return aciaReadData(bus->acia);
   } else if (addr == ACIA_STATUS) {
      return aciaReadStatus(bus->acia);
   } else {
      fprintf(stderr, "Error: (read) addr 0x%04x currently unmapped\n", addr);
      return -1;
   }
   #pragma GCC diagnostic pop
}


Bus* initBus(void) {
   Bus *bus = NULL;
   bus = malloc(sizeof *bus);
   memset(bus->ram, 0, RAM_SIZE);
   memset(bus->rom, 0, ROM_SIZE);
   bus->acia = malloc(sizeof(Acia)); // MEMORY LEAK!!(always deallocate with freeBus())
   memset(bus->acia, 0, sizeof(Acia));
   return bus;
}

void freeBus(Bus *bus) {
   if (!bus) return;
   free(bus->acia);
   free(bus);
}
