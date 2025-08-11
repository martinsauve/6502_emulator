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


// Utility function for LDX/LDY results
void print_ldx_result(const char* label, Cpu *cpu, uint8_t expectedX) {
    printf("%s: X=0x%02X (expected 0x%02X), Z=%d, N=%d, PC=0x%04X\n",
           label, cpu->X, expectedX, cpu->Z, cpu->N, cpu->PC);
}
void print_ldy_result(const char* label, Cpu *cpu, uint8_t expectedY) {
    printf("%s: Y=0x%02X (expected 0x%02X), Z=%d, N=%d, PC=0x%04X\n",
           label, cpu->Y, expectedY, cpu->Z, cpu->N, cpu->PC);
}

// Utility function for store ops
void print_store_result(const char* label, uint8_t actual, uint8_t expected, uint16_t addr) {
    printf("%s: memory[0x%04X]=0x%02X (expected 0x%02X)\n", label, addr, actual, expected);
}

// Test LDX operations
void test_ldx_ops() {
    printf("\nTesting LDX...\n");
    Bus *bus = initBus();
    Cpu *cpu = initCpu();

    // LDX Immediate (0xA2)
    cpu->PC = 0x0200;
    bus->memory[0x0200] = 0xA2;
    bus->memory[0x0201] = 0x11;
    opLDX_imm(cpu, bus);
    print_ldx_result("LDX #imm", cpu, 0x11);

    // LDX Zero Page (0xA6)
    bus->memory[0x0202] = 0xA6;
    bus->memory[0x0203] = 0x20;
    bus->memory[0x0020] = 0x22;
    opLDX_zp(cpu, bus);
    print_ldx_result("LDX zp", cpu, 0x22);

    // LDX Zero Page,Y (0xB6)
    cpu->Y = 0x04;
    bus->memory[0x0204] = 0xB6;
    bus->memory[0x0205] = 0x30;
    bus->memory[0x0034] = 0x33; // 0x30 + 0x04
    opLDX_zpY(cpu, bus);
    print_ldx_result("LDX zp,Y", cpu, 0x33);

    // LDX Absolute (0xAE)
    bus->memory[0x0206] = 0xAE;
    bus->memory[0x0207] = 0x00;
    bus->memory[0x0208] = 0x40;
    bus->memory[0x4000] = 0x44;
    opLDX_abs(cpu, bus);
    print_ldx_result("LDX abs", cpu, 0x44);

    // LDX Absolute,Y (0xBE)
    cpu->Y = 0x01;
    bus->memory[0x0209] = 0xBE;
    bus->memory[0x020A] = 0x00;
    bus->memory[0x020B] = 0x50;
    bus->memory[0x5001] = 0x55;
    opLDX_absY(cpu, bus);
    print_ldx_result("LDX abs,Y", cpu, 0x55);

    free(bus);
    free(cpu);
}

// Test LDY operations
void test_ldy_ops() {
    printf("\nTesting LDY...\n");
    Bus *bus = initBus();
    Cpu *cpu = initCpu();

    // LDY Immediate (0xA0)
    cpu->PC = 0x0300;
    bus->memory[0x0300] = 0xA0;
    bus->memory[0x0301] = 0x12;
    opLDY_imm(cpu, bus);
    print_ldy_result("LDY #imm", cpu, 0x12);

    // LDY Zero Page (0xA4)
    bus->memory[0x0302] = 0xA4;
    bus->memory[0x0303] = 0x21;
    bus->memory[0x0021] = 0x23;
    opLDY_zp(cpu, bus);
    print_ldy_result("LDY zp", cpu, 0x23);

    // LDY Zero Page,X (0xB4)
    cpu->X = 0x03;
    bus->memory[0x0304] = 0xB4;
    bus->memory[0x0305] = 0x31;
    bus->memory[0x0034] = 0x34;
    opLDY_zpX(cpu, bus);
    print_ldy_result("LDY zp,X", cpu, 0x34);

    // LDY Absolute (0xAC)
    bus->memory[0x0306] = 0xAC;
    bus->memory[0x0307] = 0x00;
    bus->memory[0x0308] = 0x60;
    bus->memory[0x6000] = 0x66;
    opLDY_abs(cpu, bus);
    print_ldy_result("LDY abs", cpu, 0x66);

    // LDY Absolute,X (0xBC)
    cpu->X = 0x02;
    bus->memory[0x0309] = 0xBC;
    bus->memory[0x030A] = 0x00;
    bus->memory[0x030B] = 0x70;
    bus->memory[0x7002] = 0x77;
    opLDY_absX(cpu, bus);
    print_ldy_result("LDY abs,X", cpu, 0x77);

    free(bus);
    free(cpu);
}

// Test STA operations
void test_sta_ops() {
    printf("\nTesting STA...\n");
    Bus *bus = initBus();
    Cpu *cpu = initCpu();

    cpu->A = 0xAA;

    // STA Zero Page (0x85)
    cpu->PC = 0x0400;
    bus->memory[0x0400] = 0x85;
    bus->memory[0x0401] = 0x10;
    opSTA_zp(cpu, bus);
    print_store_result("STA zp", bus->memory[0x0010], 0xAA, 0x0010);

    // STA Zero Page,X (0x95)
    cpu->X = 0x01;
    bus->memory[0x0402] = 0x95;
    bus->memory[0x0403] = 0x20;
    opSTA_zpX(cpu, bus);
    print_store_result("STA zp,X", bus->memory[0x0021], 0xAA, 0x0021);

    // STA Absolute (0x8D)
    bus->memory[0x0404] = 0x8D;
    bus->memory[0x0405] = 0x00;
    bus->memory[0x0406] = 0x50;
    opSTA_abs(cpu, bus);
    print_store_result("STA abs", bus->memory[0x5000], 0xAA, 0x5000);

    // STA Absolute,X (0x9D)
    cpu->X = 0x02;
    bus->memory[0x0407] = 0x9D;
    bus->memory[0x0408] = 0x00;
    bus->memory[0x0409] = 0x60;
    opSTA_absX(cpu, bus);
    print_store_result("STA abs,X", bus->memory[0x6002], 0xAA, 0x6002);

    // STA Absolute,Y (0x99)
    cpu->Y = 0x01;
    bus->memory[0x040A] = 0x99;
    bus->memory[0x040B] = 0x00;
    bus->memory[0x040C] = 0x70;
    opSTA_absY(cpu, bus);
    print_store_result("STA abs,Y", bus->memory[0x7001], 0xAA, 0x7001);

    // STA (indirect,X) (0x81)
    cpu->X = 0x04;
    bus->memory[0x040D] = 0x81;
    bus->memory[0x040E] = 0x20;
    bus->memory[0x0024] = 0x00;
    bus->memory[0x0025] = 0x80; // pointer to 0x8000
    opSTA_indX(cpu, bus);
    print_store_result("STA (ind,X)", bus->memory[0x8000], 0xAA, 0x8000);

    // STA (indirect),Y (0x91)
    cpu->Y = 0x03;
    bus->memory[0x040F] = 0x91;
    bus->memory[0x0410] = 0x30;
    bus->memory[0x0030] = 0x00;
    bus->memory[0x0031] = 0x90; // pointer to 0x9000
    opSTA_indY(cpu, bus);
    print_store_result("STA (ind),Y", bus->memory[0x9003], 0xAA, 0x9003);

    free(bus);
    free(cpu);
}

// Test STX operations
void test_stx_ops() {
    printf("\nTesting STX...\n");
    Bus *bus = initBus();
    Cpu *cpu = initCpu();

    cpu->X = 0xBB;

    // STX Zero Page (0x86)
    cpu->PC = 0x0500;
    bus->memory[0x0500] = 0x86;
    bus->memory[0x0501] = 0x11;
    opSTX_zp(cpu, bus);
    print_store_result("STX zp", bus->memory[0x0011], 0xBB, 0x0011);

    // STX Zero Page,Y (0x96)
    cpu->Y = 0x05;
    bus->memory[0x0502] = 0x96;
    bus->memory[0x0503] = 0x22;
    opSTX_zpY(cpu, bus);
    print_store_result("STX zp,Y", bus->memory[0x0027], 0xBB, 0x0027);

    // STX Absolute (0x8E)
    bus->memory[0x0504] = 0x8E;
    bus->memory[0x0505] = 0x00;
    bus->memory[0x0506] = 0xA0;
    opSTX_abs(cpu, bus);
    print_store_result("STX abs", bus->memory[0xA000], 0xBB, 0xA000);

    free(bus);
    free(cpu);
}

// Test STY operations
void test_sty_ops() {
    printf("\nTesting STY...\n");
    Bus *bus = initBus();
    Cpu *cpu = initCpu();

    cpu->Y = 0xCC;

    // STY Zero Page (0x84)
    cpu->PC = 0x0600;
    bus->memory[0x0600] = 0x84;
    bus->memory[0x0601] = 0x12;
    opSTY_zp(cpu, bus);
    print_store_result("STY zp", bus->memory[0x0012], 0xCC, 0x0012);

    // STY Zero Page,X (0x94)
    cpu->X = 0x06;
    bus->memory[0x0602] = 0x94;
    bus->memory[0x0603] = 0x23;
    opSTY_zpX(cpu, bus);
    print_store_result("STY zp,X", bus->memory[0x0029], 0xCC, 0x0029);

    // STY Absolute (0x8C)
    bus->memory[0x0604] = 0x8C;
    bus->memory[0x0605] = 0x00;
    bus->memory[0x0606] = 0xB0;
    opSTY_abs(cpu, bus);
    print_store_result("STY abs", bus->memory[0xB000], 0xCC, 0xB000);

    free(bus);
    free(cpu);
}
