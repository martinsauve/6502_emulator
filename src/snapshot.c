#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "cpu/cpu.h"
#include "bus.h"

#define SNAP_FILE_FORMAT_VERSION       1
#define SNAP_FILE_FORMAT_HEADER  "SNAP" /*must be 4 bytes*/

static void writeChunk(FILE *f, const char *id, const void *data, uint32_t size) {
   fwrite(id, 1, 4, f); // Write the chunk ID (4 bytes) to f
   fwrite(&size, 1, 4, f); // Write the chunk size (4 bytes so uint32) to f
   fwrite(data, 1, size, f); // Write the data to f
}

int saveSnapshot(const Cpu *cpu, const Bus *bus, const char *filename) {
   FILE *f = fopen(filename, "wb");
   if (!f) {
      perror("Failed to open file");
      return -1;
   }

   // write header
   fwrite(SNAP_FILE_FORMAT_HEADER, 1, 4, f);
   uint32_t version = SNAP_FILE_FORMAT_VERSION;
   fwrite(&version, 1, 4, f);

   //write cpu chunk
   writeChunk(f, "CPU ", cpu, sizeof(Cpu));
   //write ram chunk
   writeChunk(f, "RAM ", bus->ram, RAM_SIZE);
   //write rom chunk
   writeChunk(f, "ROM ", bus->rom, ROM_SIZE);

   fclose(f);
   printf("Snapshot saved at \'%s\'\n", filename);
   return 0;
}

int loadSnapshot(const char *filename, Cpu *cpu, Bus *bus) {
   FILE *f = fopen(filename, "rb");
   if (!f) {
      perror("Failed to open file");
      return -1;
   }

   // READ HEADER
   char header[4];
   fread(header, 1, 4, f);
   if (memcmp(header, SNAP_FILE_FORMAT_HEADER, 4) != 0) {
      // if different file format
      fclose(f);
      fprintf(stderr, "The provided file is not a valid snapshot!\n");
      return -2; // not a valid snapshot
   }

   uint32_t version;
   uint32_t expected_version = SNAP_FILE_FORMAT_VERSION;
   fread(&version, 1, 4, f);
   if (memcmp(&version, &expected_version, 4) != 0) {
      // if different file version
      fclose(f);
      fprintf(stderr, "This snapshot is incompatible with this version of the emulator\n");
      return -3; // version doesnt match
   }

   while (!feof(f)) {
      char chunk_id[4];
      uint32_t chunk_size;
      size_t read = fread(chunk_id, 1, 4, f);
      if (read < 4) break; //if cant read a full chunk id

      fread(&chunk_size, 1, 4, f);

      if (memcmp(chunk_id, "CPU ", 4) == 0) {
         // load cpu struct
         if ( sizeof(Cpu) != chunk_size ) {
            fclose(f);
            fprintf(stderr, "Mismatched chunk size in the provided file !\n");
            return -4; //mismatched chunk size!
            }
         fread(cpu, 1, sizeof(Cpu), f);
      } else if (memcmp(chunk_id, "RAM ", 4) == 0){
         if ( RAM_SIZE != chunk_size ) {
            fclose(f);
            fprintf(stderr, "Mismatched chunk size in the provided file !\n");
            return -4;
         }
         fread(bus->ram, 1, chunk_size, f);
      } else if (memcmp(chunk_id, "ROM ", 4) == 0){
         if ( ROM_SIZE != chunk_size ){
            fclose(f);
            fprintf(stderr, "Mismatched chunk size in the provided file !\n");
            return -4;
         }
         fread(bus->rom, 1, chunk_size, f);
      } else {
         // unknown chunk: skip
         fseek(f, chunk_size, SEEK_CUR);
      }

   }

   fclose(f);
   return 0;
}
