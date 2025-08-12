#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpu.h"
#include "bus.h"
#include "6502_types.h"


#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

void sleep_ms(int milliseconds){ // cross-platform sleep function
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    if (milliseconds >= 1000)
      sleep(milliseconds / 1000);
    usleep((milliseconds % 1000) * 1000);
#endif
}


Cpu* initCpu() {
   Cpu *cpu = NULL;
   cpu = malloc(sizeof *cpu);

   cpu->PC = 0;
   cpu->SP = 0;
   cpu->A  = 0;
   cpu->X  = 0;
   cpu->Y  = 0;

   cpu->C  = false;
   cpu->Z  = false;
   cpu->I  = false;
   cpu->D  = false;
   cpu->B  = false;
   cpu->V  = false;
   cpu->N  = false;
   return cpu;

}

void opUnknown(Cpu *cpu, Bus *bus){
   printf( "Unrecognized opcode 0x%02x at 0x%04x\n", bus->memory[cpu->PC], cpu->PC );
   dumpBus(bus, "dump.bin");

}

Addr readAddr(Cpu *cpu, Bus *bus) {
   return (Byte)bus->memory[cpu->PC] | ((Byte)bus->memory[cpu->PC + 1] << 8);
}

void opNOP(Cpu *cpu, Bus *bus) {
   printf("NOP at 0x%04x\n", cpu->PC);
   (void)bus; // Unused parameter
   cpu->PC += 1;
}


void dumpReg(Cpu *cpu) {
   printf(" A | X | C | Z | N\n");
   printf(" %i | %i | %i | %i | %i\n", cpu->A,cpu->X, cpu->C, cpu->Z, cpu->N);
}


void step(Cpu *cpu, Bus *bus, float freq, Opcodes *table){
   Byte op = bus->memory[cpu->PC];
   table[op].handler(cpu, bus);
   sleep_ms(table[op].cycles / freq);
}



 void initOpcodeTable(Opcodes opcode_table[256]) {
   for (int i = 0; i < 256; i++) {
      opcode_table[i].handler = opUnknown;
      opcode_table[i].cycles = 2;
   }

   opcode_table[0xEA].handler = opNOP;       opcode_table[0xEA].cycles = 2;

   // *******************************************
   // LOAD/STORE OPERATIONS
   // *******************************************

   // LDA
   opcode_table[0xA9].handler = opLDA_imm;   opcode_table[0xA9].cycles = 2;
   opcode_table[0xA5].handler = opLDA_zp;    opcode_table[0xA5].cycles = 3;
   opcode_table[0xB5].handler = opLDA_zpX;   opcode_table[0xB5].cycles = 4;
   opcode_table[0xAD].handler = opLDA_abs;   opcode_table[0xAD].cycles = 4;
   opcode_table[0xBD].handler = opLDA_absX;  opcode_table[0xBD].cycles = 4;
   // TODO: or 5 if page boundary crossed
   opcode_table[0xB9].handler = opLDA_absY;  opcode_table[0xB9].cycles = 4;
   // TODO: or 5 if page boundary crossed
   opcode_table[0xA1].handler = opLDA_indX;  opcode_table[0xA1].cycles = 6;
   opcode_table[0xB1].handler = opLDA_indY;  opcode_table[0xB1].cycles = 5;
   // TODO: or 6 if page boundary crossed }

   // LDX
   opcode_table[0xA2].handler = opLDX_imm;  opcode_table[0xA2].cycles = 2;
   opcode_table[0xA6].handler = opLDX_zp;   opcode_table[0xA6].cycles = 3;
   opcode_table[0xB6].handler = opLDX_zpY;  opcode_table[0xB6].cycles = 4;
   opcode_table[0xAE].handler = opLDX_abs;  opcode_table[0xAE].cycles = 4;
   opcode_table[0xBE].handler = opLDX_absY; opcode_table[0xBE].cycles = 4;
   // +1 if page crossed

   // LDY
   opcode_table[0xA0].handler = opLDY_imm;  opcode_table[0xA0].cycles = 2;
   opcode_table[0xA4].handler = opLDY_zp;   opcode_table[0xA4].cycles = 3;
   opcode_table[0xB4].handler = opLDY_zpX;  opcode_table[0xB4].cycles = 4;
   opcode_table[0xAC].handler = opLDY_abs;  opcode_table[0xAC].cycles = 4;
   opcode_table[0xBC].handler = opLDY_absX; opcode_table[0xBC].cycles = 4;
   // +1 if page crossed

   // STA
   opcode_table[0x85].handler = opSTA_zp;   opcode_table[0x85].cycles = 3;
   opcode_table[0x95].handler = opSTA_zpX;  opcode_table[0x95].cycles = 4;
   opcode_table[0x8D].handler = opSTA_abs;  opcode_table[0x8D].cycles = 4;
   opcode_table[0x9D].handler = opSTA_absX; opcode_table[0x9D].cycles = 5;
   opcode_table[0x99].handler = opSTA_absY; opcode_table[0x99].cycles = 5;
   opcode_table[0x81].handler = opSTA_indX; opcode_table[0x81].cycles = 6;
   opcode_table[0x91].handler = opSTA_indY; opcode_table[0x91].cycles = 6;

   // STX
   opcode_table[0x86].handler = opSTX_zp;   opcode_table[0x86].cycles = 3;
   opcode_table[0x96].handler = opSTX_zpY;  opcode_table[0x96].cycles = 4;
   opcode_table[0x8E].handler = opSTX_abs;  opcode_table[0x8E].cycles = 4;

   // STY
   opcode_table[0x84].handler = opSTY_zp;   opcode_table[0x84].cycles = 3;
   opcode_table[0x94].handler = opSTY_zpX;  opcode_table[0x94].cycles = 4;
   opcode_table[0x8C].handler = opSTY_abs;  opcode_table[0x8C].cycles = 4;
}
