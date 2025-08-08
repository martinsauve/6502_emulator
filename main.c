#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef union {
   struct {
   char   ZP[0x0100];      // 0x0000 - 0x00FF
   char   STACK[0x0100];   // 0x0100 - 0x01FF
   char   PAGES[0xFDF9];   // 0x0200 - 0xFFF9
   char   NMI[0x2];        // 0xFFFA - 0xFFFB
   char   RESET[0x2];      // 0xFFFC - 0xFFFD
   char   IRQBRK[0x2];     // 0xFFFE - 0xFFFF
   } MAP;
   char   ROM[0x10000];    // 0x0000 - 0xFFFF
} MEM;

typedef struct {
   uint16_t PC; // Program Counter
   uint8_t  SP; // Stack Pointer
   uint8_t  A;  // Accumulator
   uint8_t  X;  // Index Register
   uint8_t  Y;  // Index Register

   bool    C;  // Carry Flag
   bool    Z;  // Zero Flag
   bool    I;  // Interupt Disable
   bool    D;  // Decimal Mode
   bool    B;  // Break Command
   bool    V;  // Overflow Flag
   bool    N;  // Negative Flag

} CPU;

void initCpu(CPU *cpu) {
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

}




//*******************************************
// LOAD/STORE OPERATIONS
//*******************************************

// Store value in A, set negative and zero flag
void LDA(CPU *cpu, int8_t val) {
  cpu->A = val;
  if (val==0) cpu->Z = true; else cpu->Z = false;
  if (val<0)  cpu->N = true; else cpu->N = false;
}


// Store value in X, set negative and zero flag
void LDX(CPU *cpu, int8_t val) {
  cpu->X = val;
  if (val==0) cpu->Z = true; else cpu->Z = false;
  if (val<0)  cpu->N = true; else cpu->N = false;
}


// Store value in Y, set negative and zero flag
void LDY(CPU *cpu, int8_t val) {
  cpu->Y = val;
  if (val==0) cpu->Z = true; else cpu->Z = false;
  if (val<0)  cpu->N = true; else cpu->N = false;
}

// Store value in A, don't touch the flags
void STA(CPU *cpu, int8_t val) {
  cpu->A = val;
}

// Store value in X, don't touch the flags
void STX(CPU *cpu, int8_t val) {
  cpu->X = val;
}

// Store value in Y, don't touch the flags
void STY(CPU *cpu, int8_t val) {
  cpu->Y = val;
}


//*******************************************
// REGISTER TRANSFERS
//*******************************************


void TAX(CPU *cpu) { // Transfer accumulator to X
  cpu->X = cpu->A;
  if (cpu->A == 0) cpu->Z = true; else cpu->Z = false;
  if (cpu->A < 0)  cpu->N = true; else cpu->N = false;
}


void TAY(CPU *cpu) { // Transfer accumulator to Y
  cpu->Y = cpu->A;
  if (cpu->A == 0) cpu->Z = true; else cpu->Z = false;
  if (cpu->A < 0)  cpu->N = true; else cpu->N = false;
}
void TXA(CPU *cpu) { // Transfer X to accumulator
  cpu->A = cpu->X;
  if (cpu->A == 0) cpu->Z = true; else cpu->Z = false;
  if (cpu->A < 0)  cpu->N = true; else cpu->N = false;
}

void TYA(CPU *cpu) { // Transfer Y to accumulator
  cpu->A = cpu->Y;
  if (cpu->A == 0) cpu->Z = true; else cpu->Z = false;
  if (cpu->A < 0)  cpu->N = true; else cpu->N = false;
}


//*******************************************
// STACK OPERATIONS
//*******************************************

void TSX(CPU *cpu, MEM *mem) {
}

void TXS(CPU *cpu, MEM *mem) {
}

void PHA(CPU *cpu, MEM *mem) {
}

void PHP(CPU *cpu, MEM *mem) {
}

void PLA(CPU *cpu, MEM *mem) {
}

void PLP(CPU *cpu, MEM *mem) {
}



void regDump(CPU *cpu) {
   printf(" A | X | C | Z | N\n");
   printf(" %i | %i | %i | %i | %i\n", cpu->A,cpu->X, cpu->C, cpu->Z, cpu->N);
}


int dumpRom(MEM *mem) {
   FILE *fp;
   fp = fopen("dump.bin", "wb");

   if (fp==NULL) {
      perror("Error opening file");
      return 1;
   }

   fwrite(mem->ROM, sizeof(mem->ROM), 1, fp);
   fclose(fp);
   return 0;
}

int main(void) {
   MEM *M = NULL;
   M = malloc(sizeof *M);
   memset(M->ROM, 0, sizeof(M->ROM));                 //NOLINT
   memset(M->MAP.ZP, 'Z', sizeof(M->MAP.ZP));         //NOLINT
   memset(M->MAP.STACK, 'S', sizeof(M->MAP.STACK));   //NOLINT
   memset(M->MAP.IRQBRK, 'B', sizeof(M->MAP.IRQBRK)); //NOLINT
   printf("%i\n", M->ROM[0x0100]);
   //printf("%s", M->ROM);
   dumpRom(M);
   free(M);
   M = NULL;



   CPU cpu;
   initCpu(&cpu);
   LDA(&cpu, 2);
   regDump(&cpu);

}
