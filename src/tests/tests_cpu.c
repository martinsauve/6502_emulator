#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../cpu.h"
#include "../bus.h"
#include "../6502_types.h"

void print_lda_result(const char* label, Cpu *cpu, uint8_t expectedA) {
    printf("%s: A=0x%02X (expected 0x%02X), Z=%d, N=%d, PC=0x%04X\n",
           label, cpu->A, expectedA, cpu->Z, cpu->N, cpu->PC);
}

void test_lda_ops() {
    Bus *bus = initBus();
    Cpu *cpu = initCpu();

    // LDA Immediate (0xA9)
    cpu->PC = 0x0000;
    bus->memory[0x0000] = 0xA9; // opcode
    bus->memory[0x0001] = 0x42; // value
    opLDA_imm(cpu, bus);
    print_lda_result("LDA #imm", cpu, 0x42);

    // LDA Zero Page (0xA5)
    //cpu->PC = 0x0002;
    bus->memory[0x0002] = 0xA5; // opcode
    bus->memory[0x0003] = 0x10; // addr
    bus->memory[0x0010] = 0x55; // value at $10
    opLDA_zp(cpu, bus);
    print_lda_result("LDA zp", cpu, 0x55);

    // LDA Zero Page,X (0xB5)
    //cpu->PC = 0x0004;
    cpu->X = 0x05;
    bus->memory[0x0004] = 0xB5; // opcode
    bus->memory[0x0005] = 0x20; // addr
    bus->memory[0x0025] = 0x77; // value at $20 + X
    opLDA_zpX(cpu, bus);
    print_lda_result("LDA zp,X", cpu, 0x77);

    // LDA Absolute (0xAD)
    //cpu->PC = 0x0006;
    bus->memory[0x0006] = 0xAD; // opcode
    bus->memory[0x0007] = 0x34; // low byte
    bus->memory[0x0008] = 0x12; // high byte
    bus->memory[0x1234] = 0x88; // value
    opLDA_abs(cpu, bus);
    print_lda_result("LDA abs", cpu, 0x88);

    // LDA Absolute,X (0xBD)
    cpu->PC = 0x0009;
    cpu->X = 0x04;
    bus->memory[0x0009] = 0xBD; // opcode
    bus->memory[0x000A] = 0x00; // low byte
    bus->memory[0x000B] = 0x20; // high byte
    bus->memory[0x2004] = 0x99; // value at $2000 + X
    opLDA_absX(cpu, bus);
    print_lda_result("LDA abs,X", cpu, 0x99);

    // LDA Absolute,Y (0xB9)
    cpu->PC = 0x000C;
    cpu->Y = 0x02;
    bus->memory[0x000C] = 0xB9; // opcode
    bus->memory[0x000D] = 0x00; // low byte
    bus->memory[0x000E] = 0x30; // high byte
    bus->memory[0x3002] = 0xAB; // value at $3000 + Y
    opLDA_absY(cpu, bus);
    print_lda_result("LDA abs,Y", cpu, 0xAB);

    // LDA (Indirect,X) (0xA1)
    cpu->PC = 0x000F;
    cpu->X = 0x10;
    bus->memory[0x000F] = 0xA1; // opcode
    bus->memory[0x0010] = 0x80; // zp pointer
    bus->memory[0x0090] = 0x00; // low byte
    bus->memory[0x0091] = 0x40; // high byte
    bus->memory[0x4000] = 0xCD;
    opLDA_indX(cpu, bus);
    print_lda_result("LDA (ind,X)", cpu, 0xCD);

    // LDA (Indirect),Y (0xB1)
    cpu->PC = 0x0011;
    cpu->Y = 0x05;
    bus->memory[0x0011] = 0xB1; // opcode
    bus->memory[0x0012] = 0x70; // zp pointer
    bus->memory[0x0070] = 0x00; // low byte
    bus->memory[0x0071] = 0x60; // high byte
    bus->memory[0x6005] = 0xEF;
    opLDA_indY(cpu, bus);
    print_lda_result("LDA (ind),Y", cpu, 0xEF);

    dumpBus(bus, "dump.bin");
    printf("LDA opcodes tested.\n");

    free(bus);
    free(cpu);
}
