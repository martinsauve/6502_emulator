#include <stdint.h>
#include "6502_types.h"

#ifndef BUS_H
#define BUS_H

#define BUS_SIZE     0x10000
#define RAM_SIZE     0x4000 // 16K of RAM
#define RAM_START    0x0000
#define RAM_END      (RAM_START + RAM_SIZE - 1)


#define ROM_SIZE    0x8000 // 32K of ROM
#define ROM_START   0x8000
#define ROM_END     (ROM_START + ROM_SIZE - 1)

#define ACIA_DATA    0x5000
#define ACIA_STATUS  0x5001
#define ACIA_CMD     0x5002
#define ACIA_CTRL    0x5003

#define STACK_SIZE   0x0100 // 256 bytes of stack
#define STACK_START  0x0100
#define STACK_END    (STACK_START + STACK_SIZE - 1)

#define ZERO_PAGE_SIZE 0x0100 // 256 bytes of zero page
#define ZERO_PAGE_START 0x0000
#define ZERO_PAGE_END (ZERO_PAGE_START + ZERO_PAGE_SIZE - 1)

#define NMI_VECTOR_ADDR 0xFFFA
#define RESET_VECTOR_ADDR 0xFFFC
#define IRQBRK_VECTOR_ADDR 0xFFFE


typedef struct {
   Byte input_buffer;
   bool input_ready;
} Acia;

typedef struct {
   Byte ram[RAM_SIZE];
   Byte rom[ROM_SIZE];
   Acia acia;
} Bus;


int dumpRom(Bus *mem, char *filename);
int dumpRam(Bus *mem, char *filename);
int loadRom(Bus *bus, const char *filename, Addr offset);
void busWrite(Bus *bus, Addr addr, Byte value);
Byte busRead(Bus *bus, Addr addr);
void pollAciaInput(Bus *bus);

Bus* initBus(void);

#endif // BUS_H
