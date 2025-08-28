#include <stdint.h>
#include "rl_io.h"
#include "6502_types.h"

#ifndef ACIA_H
#define ACIA_H

#define ACIA_DATA    0x5000
#define ACIA_STATUS  0x5001
#define ACIA_CMD     0x5002
#define ACIA_CTRL    0x5003

typedef struct Acia Acia;
typedef char (*AciaGetCharFunc)(Acia *acia);
typedef void (*AciaPutCharFunc)(Acia *acia, char c);

typedef struct Acia{
   Byte input_buffer;
   bool input_ready;
   AciaGetCharFunc getChar;
   AciaPutCharFunc putChar;
   RlTextBuffer rlTextBuf;
} Acia;

#ifndef CPU_H
typedef struct Cpu Cpu;
#endif
#ifndef BUS_H
typedef struct Bus Bus;
#endif


void pollAciaInput(Bus *bus, Cpu *cpu);

void aciaWriteData(Acia *acia, Byte value);
Byte aciaReadData(Acia *acia);
Byte aciaReadStatus(Acia *acia);

#endif // ACIA_H
